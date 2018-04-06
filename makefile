CXX      = g++
CXXFLAGS = -Wall
LD       = g++
PROG     = fm

tagsCandidate = *.cpp *.h

.PHONY: default
default: debug

$(PROG): main.o parser.o FMPartiter.o
	$(LD) -o $@ $^

main.o: main.cpp parser.h data.h
parser.o: parser.cpp parser.h data.h
FMPartiter.o: FMPartiter.cpp FMPartiter.h data.h parser.h

.PHONY: release
release: CXXFLAGS += -DNDEBUG
release: $(PROG)

.PHONY: debug
debug: CXXFLAGS += -g
debug: $(PROG)

.PHONY: tags
tags:
	ctags $(tagsCandidate)

.PHONY: clean
clean:
	rm $(PROG) *.o
