% ��Ƶ��֮��Ƶ�������
% ����ʼ
clc;
close all;
clear;

% ͼƬ�ĳ�ʼ��
width = 1024;
heigth = 768;

% ��Ƶ��
freq = [70 64 59];  %���ص�λΪ���������Կ���Ƶ�ʣ����Һ���Ϊ���ں���

% ���÷ֿ����C�洢3�鹲��12��ͼ
% ����Ƶ�ʣ�������λ
C = cell(3,4);

for i=1:3
    for j=1:4
        C{i,j} = zeros(heigth,width);
    end
end

% �������Һ�������12��ͼ�ĻҶ�ֵ
% ͼ�������
% ����Ƶ�ʣ�������λ
for i = 1:3 % ��Ӧ���ֲ�ͬ��Ƶ��
    for  j = 1:4 % ��Ӧ������λ
        for k = 1:width
            C{i,j}(:,k) = 128+127*sin(2*pi*k*freq(i)/width+j*pi/2);
        end
    end
end

% �ԻҶ�ֵ���й�һ������
for i = 1:3
    for j = 1:4
        C{i,j} = mat2gray(C{i,j});
    end
end

% ��ʾ����12��ͼ
for i = 1:3
    for j = 1:4
        % n = 4*(i-1)+j;
        % h = figure(n);
        % imshow(C{i,j});
    filename = ['./pattern/vPhase_',num2str(i-1),'_',num2str(j-1),'.bmp'];
	imwrite(C{j}, filename); % ����
    end
end

% ��ʼ�����鴦����ͼƬ�ҶȾ���
% phiҲ�Ƿֿ����
% �洢��λ��ֵͼ��
phi = cell(3,1);
for i = 1:3
    phi{i,1} = zeros(heigth,width);
end

% ��ȡ��λ��
% ����ÿ��Ƶ�ʶ�Ӧ����λ��ֵ
% �������Ƶ�ʵ���λ��ֵ������������
for i = 1:3 % ����3���е�ÿһ��ͼƬ��ÿһ����ͬƵ�ʵ�������ͼƬ
     I1 = C{i,1};
     I2 = C{i,2};
     I3 = C{i,3};
     I4 = C{i,4};
     for g = 1:heigth
        for k = 1:width          
            if I4(g,k)==I2(g,k)&&I1(g,k)>I3(g,k) %�ĸ�����λ��
                phi{i,1}(g,k)=0;
            elseif I4(g,k)==I2(g,k)&&I1(g,k)<I3(g,k) %�ĸ�����λ��
                phi{i,1}(g,k)=pi; 
            elseif I1(g,k)==I3(g,k)&&I4(g,k)>I2(g,k) %�ĸ�����λ��
                phi{i,1}(g,k)=pi/2;
            elseif I1(g,k)==I3(g,k)&&I4(g,k)<I2(g,k) %�ĸ�����λ��
                phi{i,1}(g,k)=3*pi/2;
            elseif I1(g,k)<I3(g,k) %��������
                phi{i,1}(g,k)=atan((I4(g,k)-I2(g,k))./(I1(g,k)-I3(g,k)))+pi;
            elseif I1(g,k)>I3(g,k)&&I4(g,k)>I2(g,k) %��һ����
                phi{i,1}(g,k)=atan((I4(g,k)-I2(g,k))./(I1(g,k)-I3(g,k)));
            elseif I1(g,k)>I3(g,k)&&I4(g,k)<I2(g,k) %��������
                phi{i,1}(g,k)=atan((I4(g,k)-I2(g,k))./(I1(g,k)-I3(g,k)))+2*pi;  
            end
        end
     end
end

% �������
% ����������ڶ�Ƶ���ļ���
PH1 = phi{1,1};   %Ƶ��1
PH2 = phi{2,1};   %Ƶ��2
PH3 = phi{3,1};   %Ƶ��3

% ��ʼ��������
% ��Ƶ���
PH12 = zeros(heigth,width);
PH23 = zeros(heigth,width);
PH123 = zeros(heigth,width);

% �������
% ������
% ����
for g = 1:heigth
	for k = 1:width
		% �����һ��͵ڶ�������
		if PH1(g,k)>PH2(g,k)
			PH12(g,k) = PH1(g,k)-PH2(g,k);
		else
			PH12(g,k) = PH1(g,k)+2*pi-PH2(g,k);
		end
		% ����ڶ���͵���������
		if PH2(g,k)>PH3(g,k)
			PH23(g,k) = PH2(g,k)-PH3(g,k);
		else
			PH23(g,k) = PH2(g,k)+2*pi-PH3(g,k);
		end
	   % plot(1,k);
	end
end

% �����������
% ���ͼ��
% ��λ�����  ��λչ��
for g = 1:heigth
	for k = 1:width 
		if PH12(g,k)>PH23(g,k)
			PH123(g,k) = PH12(g,k)-PH23(g,k);
		else
			PH123(g,k) = PH12(g,k)+2*pi-PH23(g,k);
		end
	end
end

% ��ʾ
% figure,imshow(mat2gray(PH12));title('1,2���');
imwrite(mat2gray(PH12), './output/12���.bmp');
% figure,imshow(mat2gray(PH23));title('2,3���');
imwrite(mat2gray(PH23), './output/23���.bmp');
% figure,imshow(mat2gray(PH123));title('1,2,3���');
imwrite(mat2gray(PH123), './output/123���.bmp');
% figure,imshow(mat2gray(PH1));title('1��λ��ֵ');
imwrite(mat2gray(PH1), './output/1��λ��ֵ.bmp');
% figure,imshow(mat2gray(PH2));title('2��λ��ֵ');
imwrite(mat2gray(PH2), './output/2��λ��ֵ.bmp');
% figure,imshow(mat2gray(PH3));title('3��λ��ֵ');
imwrite(mat2gray(PH3), './output/3��λ��ֵ.bmp');
