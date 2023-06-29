SRCS = dict.c list.c graph.c program.c program2graph.c simulation.c test_simul.c
OBJS = dict.o list.o graph.o program.o program2graph.o simulation.o test_simul.o

CC = gcc
PROG = test_simul
CFLAGS = -Wall -Wextra -Wmissing-prototypes --pedantic -std=gnu99 -g
LDFLAGS = -lm

.PHONY: all clean run

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

clean:
	$(RM) $(OBJS) $(PROG)

run: $(PROG)
	./$(PROG) program_1000_200.txt 0.8 0.0 0.2 0.3 25 500

dict.o: dict.c dict.h
graph.o: graph.c graph.h
list.o: list.c list.h
program.o: program.c program.h list.h dict.h
program2graph.o: program2graph.c program.h list.h graph.h
simulation.o: simulation.c program.h list.h simulation.h graph.h
test_simul.o: test_simul.c program.h list.h program2graph.h graph.h \
  simulation.h

