CXX      = g++
CXXFLAGS = -Wall
LD       = g++
PROG     = fm

tagsCandidate = *.cpp *.h

.PHONY: default
default: debug

$(PROG): main.o
	$(LD) -o $@ $^

main.o: main.cpp 
	$(CXX) $(CXXFLAGS) -c -o $@ $<

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
