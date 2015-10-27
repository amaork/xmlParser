CC			=	g++
CPPFLAGS	=	-Wall -g  
TARGETS		=	test
SOURCES		=	test.cpp


all:$(TARGETS)
clean:
	rm -rf *.o $(TARGETS) a.out depend.d 

test:	tinyxml2.o test.o xmlParser.o

depend.d:$(SOURCES)
	$(CC) -M $(CPPFLAGS) $^ > $@

-include depend.d

