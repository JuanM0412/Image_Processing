CXX = g++

CXXFLAGS = -Wall -Wextra -std=c++17

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

TARGET = $(BIN_DIR)/image_processing

SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/arg_parser.cpp $(SRC_DIR)/image.cpp $(SRC_DIR)/stb_wrapper.cpp $(SRC_DIR)/buddy_image_memory_manager.cpp $(SRC_DIR)/buddy_allocator.cpp $(SRC_DIR)/vector_image_memory_manager.cpp

OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

ifeq ($(OS),Windows_NT)
    MKDIR = mkdir
    RM = rmdir /s /q
else
    MKDIR = mkdir -p
    RM = rm -rf
endif

all: $(TARGET)

$(TARGET): $(BIN_DIR) $(OBJ_DIR) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	$(MKDIR) $@

clean:
	$(RM) $(OBJ_DIR)/*.o $(TARGET)

dist-clean: clean
	$(RM) $(OBJ_DIR) $(BIN_DIR)
