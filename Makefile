# makefile thankfully stolen from https://github.com/ajuvercr/aoc-2021/blob/master/Makefile

CC := c++
CXX_STANDARD := c++20
CXX_FLAGS := -std=$(CXX_STANDARD) -Wall -Werror -fconcepts
DAY ?= $(shell date '+%d')
TARGET_EXEC ?= $(DAY).out

BUILD_DIR ?= ./build
LIB_DIR ?= ./lib
SRC_DIR ?= ./days

EXEC := $(SRC_DIR)/day$(DAY)/main.cpp
LIBS := $(shell find $(LIB_DIR) -type f -name "*.cpp")
OBJS := $(EXEC:./%.cpp=$(BUILD_DIR)/%.o) $(LIBS:./%.cpp=$(BUILD_DIR)/%.o)
DEPS := $(LIBS:%.cpp=%.h)

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) $(CXX_FLAGS) -o $@ $(LDFLAGS)

# cpp source

$(BUILD_DIR)/%.o: %.cpp $(DEPS)
	$(MKDIR_P) $(dir $@)
	$(CC) $(CXX_FLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean run

run: $(BUILD_DIR)/$(TARGET_EXEC)
	@echo "-------------------------------------------------------------"
	$(BUILD_DIR)/$(TARGET_EXEC)

clean:
	$(RM) -r $(BUILD_DIR)

MKDIR_P ?= mkdir -p