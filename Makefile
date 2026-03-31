# 1. Compiler and Flags
CXX = g++
CXXFLAGS = -std=c++17 -I./imgui -I./imgui/backends -g -O2
LIBS = -lglfw -lGL -ldl -lpthread

# 2. Source Files
# Your project files
SRCS = main.cpp tracer.cpp

# ImGui Core files
IMGUI_SRCS = imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp \
             imgui/imgui_widgets.cpp imgui/imgui_demo.cpp

# ImGui Backend files (GLFW + OpenGL3)
BACKEND_SRCS = imgui/backends/imgui_impl_glfw.cpp \
               imgui/backends/imgui_impl_opengl3.cpp

# 3. Object Files (Automatic mapping of .cpp to .o)
OBJS = $(SRCS:.cpp=.o) $(IMGUI_SRCS:.cpp=.o) $(BACKEND_SRCS:.cpp=.o)

# 4. Target Executable
EXE = vdbug

# 5. Build Rules
all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

# Pattern rule to compile .cpp to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(EXE) $(OBJS)
