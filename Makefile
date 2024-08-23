# COMPILER AND FLAGS
CC := gcc
CFLAGS := -Wno-pointer-sign -Wno-incompatible-pointer-types -g

# COLLECT SOURCE FILES AND CORRESPONDING OBJECT FILES
SRCS := $(wildcard ./src/*.c)
OBJS := $(SRCS:.c=.o)

EXEC := build

# PHONY TARGETS
.PHONY: all build run clean

# RULE TO BUILD AND RUN THE PROGRAM
all: clean build run

# RULE TO BUILD THE PROGRAM
build: $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $@

# RULE RUN THE PROGRAM
run: 
	@./$(EXEC)

# RULE TO COMPILE .c FILES TO .o FILES
%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

# RULE TO CLEAN UP THE FILES
clean:
	@rm -rf src/*.o
	@rm -rf build



