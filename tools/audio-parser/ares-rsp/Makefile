SRC_DIR := .
OBJ_DIR := .
SRC_FILES := $(SRC_DIR)/rsp.cpp
OBJ_FILES := $(OBJ_DIR)/rsp.o
# LDFLAGS :=
CPPFLAGS := -g -fpermissive
CXXFLAGS := -I . -MMD

-include $(OBJ_FILES:.o=.d)

GeMidiToMidi.exe: $(OBJ_FILES)
	g++ -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<