debug:
	gcc -g -std=c99 -pedantic -Wvla -Wall -Wshadow *.c -o pa4

pa4:
	gcc -std=c99 -pedantic -Wvla -Wall -Wshadow -O3 *.c -o pa4

testInvalid:
	./pa4 -e examples/invalidtree0.b
	./pa4 -e examples/invalidtree1.b
	./pa4 -e examples/invalidtree2.b
	./pa4 -e examples/tree0.b
	./pa4 -e examples/tree1.b
	./pa4 -e examples/tree2.b
	./pa4 -e examples/tree3.b
clean:
	-rm pa4
	-rm *.b