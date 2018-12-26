You need to have cachesim.conf like this

clock_rate(Ghz): 2
mem_access_latency(cycles): 250
L1_size(Bytes): 4096
L1_line_size(Bytes): 16
set_associativity: 2

The execute program name is cachesim.
First the program read the config file. The cache's write mode is write back.
The MainMemory is allocated 64MB and initialized by '0'.

If you enter 'make', it automatically compile and make the execute file.


EX)
Average memory access latency without cache: 111.11 ns

* L1 Cache Contents

0:  7f454c46 01010100  00fff000 000cdcd0
1:  02000300 01000000  30840408 34000000
2:  ec220000 00fe0000  34002000 08002800
3:  84300804 00340000 22ec0000 0000cc00

...

62:  7f454c46 01010100  00fff000 000cdcd0
63:  02000300 01000000  30840408 34000000

Total program run time: 35.5 seconds
L1 total access: 389025
L1 total miss count: 12138
L1 miss rate: 3.12%
Average memory access latency: 2.43 ns
