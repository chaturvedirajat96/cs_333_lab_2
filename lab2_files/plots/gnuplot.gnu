set term postscript eps color

#set output 'random_one.eps'
#set output 'random_zero.eps'
set output 'fixed_zero.eps'


#used to make the fonts appear larger;  makes the figure smaller but keeps the fonts same size
set size 0.6, 0.6
set key right bottom
set grid ytics lc rgb "#bbbbbb" lw 1 lt 0
set grid xtics lc rgb "#bbbbbb" lw 1 lt 0
set xlabel "Number of Clients"


set ylabel "Average Response Time(sec/res)"

#plot 'random_one.txt' using 1:2 title "Average Response Time" with linespoints ls 1
#set ylabel "Throughput(in req/sec)"
#plot 'random_one.txt' using 1:3 title "Throughput" with linespoints ls 1

#plot 'random_zero.txt' using 1:2 title "Average Response Time" with linespoints ls 1
#set ylabel "Throughput(in req/sec)"
#plot 'random_zero.txt' using 1:3 title "Throughput" with linespoints ls 1

plot 'fixed_zero.txt' using 1:2 title "Average Response Time" with linespoints ls 1
set ylabel "Throughput(in req/sec)"
plot 'fixed_zero.txt' using 1:3 title "Throughput" with linespoints ls 1

