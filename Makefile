# 1. Compiler and Flags
CXX = g++
CC = gcc
CXXFLAGS = -std=c++17 -I./imgui -I./imgui/backends -I./dwarf/inc -g -O2
CFLAGS = -I./dwarf/inc -g
LIBS = -lglfw -lGL -ldl -lpthread -ldw -lelf

# 2. Source Files
SRCS = main.cpp tracer.cpp
# Add your C source here
C_SRCS = dwarf/dl_parser.c 

IMGUI_SRCS = imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp \
             imgui/imgui_widgets.cpp imgui/imgui_demo.cpp

BACKEND_SRCS = imgui/backends/imgui_impl_glfw.cpp \
               imgui/backends/imgui_impl_opengl3.cpp

# 3. Object Files
# Map both .cpp and .c files to .o
OBJS = $(SRCS:.cpp=.o) $(IMGUI_SRCS:.cpp=.o) $(BACKEND_SRCS:.cpp=.o) $(C_SRCS:.c=.o)

# 4. Target Executable
EXE = vdbug

# 5. Build Rules
all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

# Rule for C++ files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Rule for C files (The DWARF parser)
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(EXE) $(OBJS)
