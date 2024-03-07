x=[0:365]
load eot.csv
y=[eot]
plot(x,y)
grid on
grid minor on
axis([0,365])
set (gca, "xaxislocation", "origin")
