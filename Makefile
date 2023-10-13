CFLAGS=-Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE
all: program.exe
program.exe: hash.o stack_reallocs.o main.o stack_dumps.o stack_funcs.o 
	@g++ $^ -o $@ $(CFLAGS)
	@echo g++ $^ -o $@
main.o: main.cpp
	@g++ -c $< -o $@ $(CFLAGS)
	@echo g++ $^ -o $@
stack_reallocs.o: stack_reallocs.cpp
	@g++ -c $< -o $@ $(CFLAGS)
	@echo g++ $^ -o $@
stack_dumps.o: stack_dumps.cpp
	@g++ -c $< -o $@ $(CFLAGS)
	@echo g++ $^ -o $@
stack_funcs.o: stack_funcs.cpp
	@g++ -c $< -o $@ $(CFLAGS)
	@echo g++ $^ -o $@
hash.o: hash.cpp
	@g++ -c $< -o $@ $(CFLAGS)
	@echo g++ $^ -o $@