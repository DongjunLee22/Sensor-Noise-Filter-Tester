%% Initialization
clear;
close all;
clc;

addpath(genpath(fileparts(mfilename('fullpath'))));
rng('default')
rng(1)

%% Figure setup
Figure_setup;
FlagFigureAutoSave = 1;
currentFolder = pwd;

%% Data Load
data_load = readtable("data_rec_Ccode.txt");
% data_load = readtable("data_rec_Ccode_MAF.txt");
% data_load = readtable("data_rec_Ccode_low_pass.txt");
% data_load = readtable("data_rec_Ccode_Kalman.txt");
data = table2array(data_load);

%% Data Classification
pre_t = data(:,1);
t = (pre_t - pre_t(1))*0.001;
for it_hz = 1:length(t)-1
    A_dt(it_hz) = t(it_hz+1) - t(it_hz);
end
dt = mean(A_dt);
Hz = 1/dt;

sprintf("데이터 수집 주기 : %1.f Hz",Hz)

original = data(:,2);
filtered = data(:,3);

signal_power = var(original);
nosie_power = var(filtered);

MEAN = mean(filtered);
SNR = 10*log10(signal_power./nosie_power);
RMSE = sqrt(mean((original - filtered).^2));

sprintf("필터링 값의 평균 : %.4f [N]\n" + ...
        "필터링 값의 SNR : %.4f\n" + ...
        "필터링 값의 RMSE : %.4f", ...
        MEAN,SNR,RMSE)

%% Plot
figure;
plot(t,original)
hold on;
plot(t,filtered)
hold off;
ylim([-6 6])
title("Low-Pass Filter",'FontSize',30)
xlabel("Time [s]")
ylabel("Force [N]")
legend("Original","Filtered","Orientation","horizontal","Location","northeast")
legend box off;

% save figure
% fun_SaveFigure('Low-Pass Filter',FlagFigureAutoSave,currentFolder)