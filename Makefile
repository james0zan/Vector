all: fixed_vector.h fixed_vector_test.cpp
	g++ -std=c++11 -I/usr/local/include -L/usr/local/lib fixed_vector_test.cpp -o a