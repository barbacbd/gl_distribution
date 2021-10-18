objects := distribution.o
CXXFLAGS := -std=c++11

dgl: CC := $(CXX)

dgl: LDLIBS := -lglut -lGL -lGLU -lglut -lm
# MAC OSX build
# NOTE: works to build but currently segfaults
#dgl: LDLIBS := -Wall -Wno-deprecated-declarations -framework GLUT -framework Opengl


dgl: $(objects)
	$(LINK.o) $^ $(LDLIBS) -o $@

distribution.o: distribution.h

.PHONY: clean
clean:
	$(RM) $(objects)
	$(RM) dgl
