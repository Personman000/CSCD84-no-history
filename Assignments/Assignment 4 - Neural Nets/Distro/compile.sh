g++ -fPIC -O3 -c -g NeuralNets.c -no-pie
g++ -fPIC -O3 -g *.o -lm -o NeuralNets -no-pie

