PROG:=main
SRCS:=main.cc

CXX:=g++ -std=c++20 -Wall -Wfatal-errors

OBJS:=$(SRCS:.cc=.o)
DEPS:=$(SRCS:.cc=.d)

all: main

main: $(OBJS)
	$(CXX) -MMD -o $@ $^

%.o: %.cc
	$(CXX) -MMD -c $<

.PHONY: edit

edit:
	@geany -s -i $(SRCS) *.h &

clean:
	@rm -f $(PROG) *.o *.d core

-include $(DEPS)
