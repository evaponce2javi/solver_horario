# ⫘⫘⫘ Makefile — compila todo el proyecto con un solo comando ⫘⫘⫘
# Uso:
#   make          -> compila el binario ./horario
#   make run      -> compila y ejecuta
#   make clean    -> borra objetos, dependencias y binario

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -MMD -MP
TARGET   := horario

SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)
DEPS := $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)

-include $(DEPS)

.PHONY: all run clean
