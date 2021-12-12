OUT	= panda
CC	 = g++

HEADER	= -I ./include/
SOURCE	= $(wildcard src/*.cpp)
OBJS	= $(SOURCE:.cpp=.o)

TRASH	= $(wildcard src/*~) $(wildcard include/*~)

FLAGS	 = -g -W -Wall -Wextra -O0 -fPIC -std=gnu++17 -c
LFLAGS	 = -llapin -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -lstdc++ -lm -ldl -lpthread -lgcc_s -lavcall

all: bin

bin: $(OBJS)
	@$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)
	@echo "[OUT]" $(OUT)

.cpp.o:
	@$(CC) $(FLAGS) $< -o $@ $(HEADER)
	@echo "[G++]" $<

clean:
	@rm -f $(OBJS)
	@rm -f $(TRASH)

fclean: clean
	@rm -f $(OUT)


re: fclean all

.PHONY: all clean

SUFFIXES: .c .cpp .o
