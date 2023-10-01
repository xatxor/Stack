all: program.exe
program.exe: stack_reallocs.o main.o stack_dumps.o stack_funcs.o
	g++ $^ -o $@
main.o: main.cpp
	g++ -c $< -o $@
stack_reallocs.o: stack_reallocs.cpp
	g++ -c $< -o $@
stack_dumps.o: stack_dumps.cpp
	g++ -c $< -o $@
stack_funcs.o: stack_funcs.cpp
	g++ -c $< -o $@