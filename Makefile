AR			=	$(CROSS)ar
CXX			=	$(CROSS)g++

ARFLAGS		=	rcv
CPPFLAGS	=	-Wall 
LDSHFLAGS	=	-rdynamic -shared  

SOURCES		=	tinyxml2.cpp xmlParser.cpp	
OBJECTS		=	$(SOURCES:.cpp=.o)

TARGETS		=	test libxmlparser.a libxmlparser.so

LDSHFLAGS	=	-rdynamic -shared  

.PHONY:all clean 

all:$(TARGETS)

clean:
	rm -rf *.o $(TARGETS) a.out depend.d 

test:	test.o $(OBJECTS)
	$(CXX) $(CPPFLAGS) -o $@ $^

libxmlparser.a:$(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

libxmlparser.so:$(OBJECTS)
	$(CXX) $(LDSHFLAGS) -o $@ $^

depend.d:$(wildcard *.cpp) $(wildcard *.h)
	$(CXX) -M $(CPPFLAGS) $^ > $@

-include depend.d

