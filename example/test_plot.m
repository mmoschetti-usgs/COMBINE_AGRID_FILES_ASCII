function []=test_plot()

%
close all

% 
f1='agrd_CEUS_adaptSm_2zone_2018.txt';
f2='agrd_WUS_adaptSm_2018.txt';
f3='agrid_comb_adaptSm_2018.txt';
% 
%f1='agrd_CEUS_adaptSm_2zone_2014.txt'
%f2='agrd_WUS_adaptSm_2014.txt'
%f3='agrid_comb_adaptSm_2014.txt'
% 
%f1='agrd_CEUS_fixSm_2zone_2014.txt'
%f2='agrd_WUS_fixSm_2014.txt'
%f3='agrid_comb_fixSm_2014.txt'
% 
%f1='agrd_CEUS_fixSm_2zone_2018.txt'
%f2='agrd_WUS_fixSm_2018.txt'
%f3='agrid_comb_fixSm_2018.txt'

%figure
%read_plot('agrd_WUScmp_adSm_floors.txt')

figure
subplot(3,1,1)
read_plot(f1)
subplot(3,1,2)
read_plot(f2)
subplot(3,1,3)
read_plot(f3)


end
%---------------------------------------------------------------


%---------------------------------------------------------------
function []=read_plot(fileIn)

%
ff=load(fileIn);
lon=ff(:,1);
lat=ff(:,2);
vals=ff(:,3);

%
sval=25;
scatter(lon,lat,sval,log(vals),'filled')
%axis([-126.5 -120 40 44])
colorbar
caxis([log(1e-5) log(20)])

end
%---------------------------------------------------------------
