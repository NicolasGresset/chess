CC=g++
CFLAGS = -Wall -g -Wextra
INCLUDE_PATH = ./include

TARGET   =echecs

SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin


SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(INCLUDE_PATH)/*.hpp)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)


$(BINDIR)/$(TARGET): $(OBJECTS)
	mkdir -p $(BINDIR)
	$(CC) -o $@ $^ $(CFLAGS) $(LDLIBS)
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	mkdir -p $(OBJDIR)
	g++ -o $@ -c $< $(CFLAGS)

doc:
	doxygen Doxyfile

.PHONY: clean
clean:
	rm -rf obj/*.o
	rm -f bin/echecs