#**************************************************************************
#   Makefile                                                              #
#                                                                         #
#   VeeVee                                                                #
#                                                                         #
#   This program is free software; you can redistribute it and/or modify  #
#   it under the terms of the GNU General Public License as published by  #
#   the Free Software Foundation, version 3                               #
#                                                                         #
#   This program is distributed in the hope that it will be useful, but   #
#   WITHOUT ANY WARRANTY; without even the implied warranty of            #
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     #
#   General Public License for more details.                              #
#                                                                         #
#   You should have received a copy of the GNU General Public License     #
#   along with this program; if not, write to the Free Software           #
#   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA             #
#   02110-1301, USA.                                                      #
#                                                                         #
# **************************************************************************/

# set compiler and compile options
EXEC = v2c
# use the GNU C++ compiler
CXX = g++
# use some optimization, report all warnings and enable debugging
OPTS = -O3 -Wall -Wno-write-strings
# add compile flags
CFLAGS = $(OPTS) -std=c++0x
# specify link flags here
LDFLAGS = -lpcrecpp -lpcre

# set a list of directories
INCDIR  = ./include
OBJDIR  = ./obj
BINDIR  = ./bin
SRCDIR  = ./src

# set the include folder where the .h files reside
CFLAGS += -I$(INCDIR) -I$(SRCDIR)

# add here the source files for the compilation
SOURCES = v2c.cpp vaspreader.cpp atom.cpp state.cpp lexical_casts.cpp

# create the obj variable by substituting the extension of the sources
# and adding a path
_OBJ = $(SOURCES:.cpp=.o)
OBJ = $(patsubst %,$(OBJDIR)/%,$(_OBJ))

all: $(BINDIR)/$(EXEC)

$(BINDIR)/$(EXEC): $(OBJ)
	$(CXX) -o $(BINDIR)/$(EXEC) $(OBJ) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c -o $@ $< $(CFLAGS)

$(TESTDIR)/%.test: $(TESTDIR)/%_test.cpp
	$(CXX) -o $@ $< $(SRCDIR)/$*.cpp -I$(INCDIR)

clean:
	rm -vf $(BINDIR)/$(EXEC) $(OBJ) $(TESTS_EXEC)
