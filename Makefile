program_NAME := portScanner
program_SRCS := $(wildcard *.cpp)
program_OBJS := ${program_SRCS:.cpp=.o}

CPPFLAGS+=-lpcap -Wall -Wextra -O0 -pedantic -std=c++11 -DDEBUG -lpthread

.PHONY: all clean

all: $(program_NAME)

$(program_NAME): $(program_OBJS)
	$(LINK.cc) $(program_OBJS) -o $(program_NAME)

clean:
	@- $(RM) $(program_NAME)
	@- $(RM) $(program_OBJS)
	@- $(RM) *~
