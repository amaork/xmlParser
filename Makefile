-include ../.config
-include ../config.mk

AR			=	$(CROSS)ar
CXX			=	$(CROSS)g++

ARFLAGS		=	rcv
CPPFLAGS	=	-Wall -fPIC
LDSHFLAGS	=	-rdynamic -shared

SOURCES		=	tinyxml2.cpp xmlParser.cpp
OBJECTS		=	$(SOURCES:.cpp=.o)

TARGETS		=	test libxmlparser.a libxmlparser.so

LDSHFLAGS	=	-rdynamic -shared

.PHONY:all clean install style

all:$(TARGETS)

clean:
	rm -rf *.o $(TARGETS) a.out depend.d

test:	test.o $(OBJECTS)
	$(CXX) $(CPPFLAGS) -o $@ $^

style:
	@find -regex '.*/.*\.\(c\|cpp\|h\)$$' | xargs astyle --align-pointer=name --align-reference=name --suffix=none --break-blocks --pad-oper --pad-header --break-blocks --keep-one-line-blocks --indent-switches --indent=tab=4

libxmlparser.a:$(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

libxmlparser.so:$(OBJECTS)
	$(CXX) $(LDSHFLAGS) -o $@ $^

install:libxmlparser.so
	@echo "Install xmlParser dynamic lib..."
	@mkdir -p $(LOCAL_PACK_PATH)/$(LIB_PATH)
	$(if ${LOCAL_PACK_PATH},@cp $^ $(LOCAL_PACK_PATH)/$(LIB_PATH),@cp $^ /usr/local/lib)


depend.d:$(wildcard *.cpp) $(wildcard *.h)
	$(CXX) -MM $(CPPFLAGS) $^ > $@

-include depend.d

