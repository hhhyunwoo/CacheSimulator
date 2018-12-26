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