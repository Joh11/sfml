LIBS = -lsfml-graphics -lsfml-window -lsfml-system
CXX := g++

CXXFLAGS = -std=c++11

EXEC_NAME = app

OBJ = main.o
OBJDIR = build/

OBJ_WITH_DIR = $(addprefix $(OBJDIR), $(OBJ))

all: mainBuild

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $(OBJDIR)$@

mainBuild: $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ_WITH_DIR) -o $(EXEC_NAME) $(LIBS)

clean:
	rm -f $(EXEC_NAME) $(OBJDIR)*.o
