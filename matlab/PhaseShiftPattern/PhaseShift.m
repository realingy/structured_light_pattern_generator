function [ ] = PhaseShift( freq, width, height, vertical)

% 图片的初始化
% width = 1024;
% height = 768;
% width = 1280;
% height = 720;
% freq = 32;
% vertical = true;

% 利用分块矩阵C存储4张图（4步相位移）
C = cell(1, 4);

for j=1:4
    C{1,j} = zeros(height,width);
end

% 利用余弦函数计算4张图的灰度
% 图像的生成，四组相位
for  j = 1:4 % 对应四种相位
    if(true == vertical)
        for k = 1 : width
            C{j}(:,k) = 128 + 127 * sin(2*pi*k*freq/width + j*pi/2);
        end
    else
        for k = 1 : height
            C{j}(k,:) = 128 + 127 * sin(2*pi*k*freq/height + j*pi/2);
        end
    end
end

% 灰度归一化处理
for j = 1:4
    C{j} = mat2gray(C{j});
end

% 显示四张图像
for j = 1:4
    % n = j;
	% h = figure(n);
    % imshow(C{j}); % 显示
    if(true == vertical)
        filename = ['./pattern/vPhase',num2str(j-1),'.bmp'];
    else
        filename = ['./pattern/hPhase',num2str(j-1),'.bmp'];
    end
	imwrite(C{j}, filename); % 保存
end
