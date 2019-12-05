function [ ] = PhaseShift( freq, width, height, vertical)

% ͼƬ�ĳ�ʼ��
% width = 1024;
% height = 768;
% width = 1280;
% height = 720;
% freq = 32;
% vertical = true;

% ���÷ֿ����C�洢4��ͼ��4����λ�ƣ�
C = cell(1, 4);

for j=1:4
    C{1,j} = zeros(height,width);
end

% �������Һ�������4��ͼ�ĻҶ�
% ͼ������ɣ�������λ
for  j = 1:4 % ��Ӧ������λ
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

% �Ҷȹ�һ������
for j = 1:4
    C{j} = mat2gray(C{j});
end

% ��ʾ����ͼ��
for j = 1:4
    % n = j;
	% h = figure(n);
    % imshow(C{j}); % ��ʾ
    if(true == vertical)
        filename = ['./pattern/vPhase',num2str(j-1),'.bmp'];
    else
        filename = ['./pattern/hPhase',num2str(j-1),'.bmp'];
    end
	imwrite(C{j}, filename); % ����
end
