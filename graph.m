x=[0:365]
load eot.csv
y=[eot]
plot(x,y)
grid on
grid minor on
axis([0,365])
hold on
cur=gmtime(time()).yday
plot(cur, y(cur+1), 'x')
set(gca, "xaxislocation", "origin")
