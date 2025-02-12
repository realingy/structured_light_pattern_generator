#ifndef _RXMANAGER_H_
#define _RXMANAGER_H_

#include <QGuiApplication>
#include <QJsonObject>
#include <QQmlApplicationEngine>
#include <QSet>
#include <map>
#include <mutex>

#include "glog/logging.h"

namespace scv {

typedef void* (*onLambdaEvent)(void* aParam,
                               std::function<void(void*)> aLambda);

#define SINGLENTON(aName)                          \
  static aName* instance() {                       \
    static std::mutex aName##_mutex;               \
    std::lock_guard<std::mutex> lg(aName##_mutex); \
    static aName ret;                              \
    return &ret;                                   \
  }

class renderTrigger : public QObject {
  Q_OBJECT
 public:
  explicit renderTrigger(QObject* aParent = nullptr) : QObject(aParent) {}
  ~renderTrigger();
  void trig(QJsonObject aParam = QJsonObject());
  void trig2(QJsonObject aParam = QJsonObject());
  void trigCommand(QJsonObject aParam = QJsonObject());
  void trigDrawResult(QJsonObject aParam = QJsonObject());
  void trigDrawImage(QJsonObject aParam = QJsonObject());
 signals:
  void callQmlRefresh(QJsonObject aParam);
  void callQmlRefresh2(QJsonObject aParam);
  void callQmlSetCommand(QJsonObject aParam);
  void callQmlDrawResult(QJsonObject aParam);
  void callQmlDrawImage(QJsonObject aParam);

 public:
  static renderTrigger* initance();
};

// rxmanager 类的工作机制
// 作用是实现一个事件管理和订阅发布机制。它允许在C++中注册、订阅事件，并使用回调机制处理这些事件。
// 这种设计通常用于实现观察者模式，其中一个事件可以由多个观察者（即订阅者）响应。
//  addObservable2() 用于将事件与回调函数关联，类似于事件的注册。
//  subscribe2() 用于订阅已注册的事件，并执行相应的回调。
class rxmanager {
 public:
  // the front-end js only support the object from the same engine
  QQmlApplicationEngine* engine = nullptr;
  QGuiApplication* app = nullptr;  // for translating dynamically

 public:
  void addObservable2(const std::string& aName, onLambdaEvent aEvent);
  void* subscribe2(const std::string& aName, void* aParam,
                   std::function<void(void*)> aEvent = nullptr);

 private:
  std::map<std::string, onLambdaEvent> m_observables2;
  // std::map<std::string, rxcpp::observable<>> m_observables;
 protected:
  rxmanager();

 public:
  static rxmanager* initance();
};

class streamData {
 public:
  streamData(std::function<void(void*)> aCallback = nullptr) {
    m_callback = aCallback;
  }

  virtual ~streamData() {}

  void callback(void* aParam) {
    if (m_callback != nullptr) {
      m_callback(aParam);
    }
  }

  virtual std::shared_ptr<streamData> clone() {
    return std::make_shared<streamData>(m_callback);
  }

 protected:
  std::function<void(void*)> m_callback;
};

class streamJson : public streamData {
 public:
  streamJson(const QJsonObject& aData,
             std::function<void(void*)> aCallback = nullptr)
      : streamData(aCallback) {
    m_data = QJsonObject(aData);
  }
  QJsonObject* getData() { return &m_data; }
  std::shared_ptr<streamData> clone() override {
    return std::make_shared<streamJson>(m_data, m_callback);
  }

 protected:
  QJsonObject m_data;
};

class streamEvent : public QEvent {
 public:
  static const Type type = static_cast<Type>(QEvent::User + 1);

 public:
  streamEvent(const QString& aName, std::shared_ptr<streamData> aStream)
      : QEvent(type) {
    m_name = aName;
    m_stream = aStream;
  }
  QString getName() { return m_name; }
  std::shared_ptr<streamData> getStream() { return m_stream; }

 private:
  QString m_name;
  std::shared_ptr<streamData> m_stream = nullptr;
};

class streamManager {
 public:
  SINGLENTON(streamManager)

 public:
  QQmlApplicationEngine* engine = nullptr;

 public:
 public:
  streamManager();
  ~streamManager();
  void registerEvent(
      const QString& aSignal, const QString& aEventName,
      std::function<std::shared_ptr<streamData>(std::shared_ptr<streamData>)>
          aFunc,
      const QString& aPreviouses = "", const QString& aNexts = "",
      int aThreadNo = 0);
  void registerJSEvent(const QString& aSignal, const QString& aEventName,
                       QJSValue aFunc, const QString& aPreviouses = "",
                       const QString& aNexts = "");
  void emitSignal(const QString& aSignal,
                  std::shared_ptr<streamData> aInput = nullptr);
  static void testSequence();
  static void testDynamicRegister();

 private:
  class eventPack : public QObject {
   private:
    friend streamManager;

   public:
    bool event(QEvent* e) override;
    void execute(std::shared_ptr<scv::streamData> aStream);

   public:
    QString name;
    bool jsevent;
    QThread* thread;
    std::function<std::shared_ptr<streamData>(std::shared_ptr<streamData>)>
        function_;
    QJSValue function2;
    QSet<QString> previous;
    QMap<QString, bool> nexts;  // qset is unordered, but qmap is ordered
    QMap<QString, std::shared_ptr<scv::streamManager::eventPack>>* events_map;
  };

 public:
  QMap<QString, std::shared_ptr<scv::streamManager::eventPack>>*
  unregisterEvent(const QString& aSignal, const QString& aEventName);

 private:
  QMap<QString, std::shared_ptr<eventPack>>* doRegisterEvent(
      const QString& aSignal, const QString& aEventName,
      std::shared_ptr<eventPack> aEvent);
  QThread* moveToThread(int aNo, std::shared_ptr<eventPack> aEvent);
  QMap<QString, QMap<QString, std::shared_ptr<eventPack>>>
      m_events;  // <signal, <eventname, eventpack>>
  QMap<int, std::shared_ptr<QThread>>
      m_threads;  // <signal, <threadno, thread>>
};

// 用于注册一个管道事件（signal-event）并为其配置处理逻辑，包括指定事件的名称、回调函数、以及在哪个线程上处理这个事件。
class regPipe {
 public:
  regPipe(
      const char* aSignal, const char* aEventName,
      std::function<std::shared_ptr<streamData>(std::shared_ptr<streamData>)>
          aEvent,
      int aThreadNo) {
    streamManager::instance()->registerEvent(aSignal, aEventName, aEvent, "",
                                             "", aThreadNo);
  }
};

}  // namespace scv

// 将一个事件（aName）和对应的回调（aEvent）通过rxmanager类进行注册。
// 会调用rxmanager::initance()->addObservable2(aName, aEvent)，将事件名称 aName
// 和回调函数 aEvent 注册到 rxmanager 的观察者机制中。
class regRX {
 public:
  regRX(const char* aName, scv::onLambdaEvent aEvent) {
    scv::rxmanager::initance()->addObservable2(aName, aEvent);
  }
};

// 将事件aName和对应的回调函数aObservable注册到rxmanager中。
#define REGISTEROBSERVABLE2(aName, aObservable) \
  static regRX g_Register2##aName(              \
      #aName, reinterpret_cast<scv::onLambdaEvent>(aObservable));
//namespace RegMethod { \
//static std::string Reg_##aName = #aName; \
//};

// 用于注册一个管道事件（aSignal）和对应的事件名称（aEventName）以及事件的回调（aEvent），并指定该事件处理的线程号aThreadNo
#define REGISTERPipe(aSignal, aEventName, aEvent, aThreadNo)                 \
  static scv::regPipe g_Register##aSignal##aEventName(#aSignal, #aEventName, \
                                                      aEvent, aThreadNo);

#endif
