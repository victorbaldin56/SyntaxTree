# Compiler
CC = g++
# Flags
FLAGS = -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ \
	-Waggressive-loop-optimizations \
	-Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual \
	-Wchar-subscripts \
	-Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body \
	-Wfloat-equal \
	-Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 \
	-Winline -Wlogical-op \
	-Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked \
	-Winit-self \
	-Wredundant-decls \
	-Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel \
	-Wstrict-overflow=2 \
	-Wsuggest-attribute=noreturn -Wsuggest-final-methods \
	-Wsuggest-final-types \
	-Wsuggest-override \
	-Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code \
	-Wunused -Wuseless-cast \
	-Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers \
	-Wno-narrowing \
	-Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new \
	-fsized-deallocation \
	-fstack-protector -fstrict-overflow -flto-odr-type-merging \
	-fno-omit-frame-pointer \
	-Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla \
	-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,$\
	float-divide-by-zero,$\
	integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,$\
	returns-nonnull-attribute,$\
	shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

BUILD_DIR = build
SRC_DIRS  = src
EXAMPLES  = examples
SRC       = $(wildcard $(SRC_DIRS)/*.cpp)
OBJS      = $(SRC:%.cpp=$(BUILD_DIR)/%.cpp.o)
INCLUDE   = include

LIB = tree.a

EXECUTABLE = test
EX_DIR     = examples
EXAMPLE    = example.cpp

$(BUILD_DIR)/$(LIB): $(OBJS)
	@ar rcs $@ $(OBJS)

$(BUILD_DIR)/%.cpp.o: %.cpp
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/$(SRC_DIRS)
	@$(CC) $(FLAGS) -c $< -o $@ -I $(INCLUDE)

test:
	@mkdir -p $(BUILD_DIR)/$(EX_DIR)
	@$(CC) $(FLAGS) -c $(EX_DIR)/$(EXAMPLE) -o \
	$(BUILD_DIR)/$(EX_DIR)/$(EXAMPLE).o -I $(INCLUDE)
	@$(CC) $(FLAGS) $(BUILD_DIR)/$(EX_DIR)/$(EXAMPLE).o \
	$(BUILD_DIR)/$(LIB) -o $(BUILD_DIR)/$(EXECUTABLE) \
	-I $(INCLUDE)

.PHONY: clean

clean:
	@rm -rf $(BUILD_DIR)

run:
	@./$(BUILD_DIR)/$(EXECUTABLE)
