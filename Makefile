output:MyGraph.o
	gcc MyGraph.o -o output

MyGraph.o: MyGraph.c
	gcc -c MyGraph.c

clean:
	rm *.o output