% ʹ�÷������޸ķֱ���resolution�������м���

% resolution = [1024, 768];
resolution = [1024, 720];
  
graySeq = GrayCodePattern4ProjectorCalibration(resolution);
% graySeq = GrayCodePattern(resolution);

s = size(graySeq);

for i = 1:s(3)
    imwrite(graySeq(:,:,i), sprintf('./output/%d.jpg', i));
end

