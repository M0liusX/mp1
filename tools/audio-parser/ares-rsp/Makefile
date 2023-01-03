SRC_DIR := .
OBJ_DIR := ./obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
# LDFLAGS :=
CPPFLAGS := -g -fpermissive
CXXFLAGS := -MMD

-include $(OBJ_FILES:.o=.d)

GeMidiToMidi.exe: $(OBJ_FILES)
	g++ -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ $(CPPFLAGS) -c -o $@ $<