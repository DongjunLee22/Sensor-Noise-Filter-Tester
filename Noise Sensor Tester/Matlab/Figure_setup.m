%% Figure Properties Setup
set(groot,'defaultLineLineWidth',1)
set(groot,'defaultFigureColor','w')
set(groot,'defaultAxesfontweight','bold')
set(groot,'defaultAxesfontsize',20)
set(groot,'defaultAxesfontname','Times New Roman')
set(groot,'defaultAxesxgrid','on')
set(groot,'defaultAxesygrid','on')
set(groot,'defaultAxeszgrid','on')
set(groot,'defaultAxesLineStyleOrder',{'-','--',':','-.'})
ColorOrder = [  0,      0,      0
    1,      0,      0
    0,      0,      1
    0.8     0       0.8
    0.5     0.5     1
    0       1       0];
set(groot,'defaultAxesColorOrder',ColorOrder)


%% Figure initialization
global FontSize FontName;
Tstring =  'Times (h)'; 
Fstring = 'Frequency (Hz)';
Astring =  'RMS Values';
FontSize = 9;   FontName = 'Times New Roman';
MarkerSize = 4;  LineWidth = 1;