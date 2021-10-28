GCC = g++ -std=c++17
EXEC = main

main : test.o order.o 
	$(GCC) -o $(EXEC) test.o order.o 

test.o : test.cpp
	$(GCC) -c test.cpp

order.o : order.cpp
	$(GCC) -c order.cpp

clean : 
	rm -f *.o $(EXEC) 

debug : test.cpp order.cpp 
	$(GCC) -g -o $(EXEC) test.cpp order.cpp 
