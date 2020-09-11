Installation:
 apt-get install libopenmpi-dev
 apt-get install openmpi-bin

Compilation:
 mpicc main.c -o main

Running:
 mpirun -np 2 ./main

