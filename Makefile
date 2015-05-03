PROGNAME := animate.out
FILES := animation.cpp sprite.cpp termfuncs.cpp
OBJS := $(FILES:.cpp=.o)
DEPENDENCIES := $(FILES:.cpp=.d)

CXX := g++
CFLAGS := -Wall -Wextra -g -c
LDFLAGS := -Wall -Wextra -g
LIBS :=

all: $(PROGNAME) $(DEPENDENCIES)

ifneq ($(MAKECMDGOALS), clean)
-include $(DEPENDENCIES)
endif

$(PROGNAME): $(OBJS)
	$(CXX) $(LDFLAGS) $(LIBS) $(OBJS) -o $(PROGNAME)

%.o: %.cpp %.d
	$(CXX) $(CFLAGS) $<

# makes dependencies you can use, so you know to recompile when a header changes
%.d: %.cpp
	$(CXX) $(CFLAGS) -MM $*.cpp > $*.d

clean:
	rm -f *.o *.d *~ core.* $(PROGNAME)

