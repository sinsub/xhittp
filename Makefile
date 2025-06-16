CC = gcc
CFLAGS = -Wall -Wextra

SRC_DIR = src
BUILD_DIR = build

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))

TARGET = xhittp

all: ${BUILD_DIR} ${TARGET}

${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}

${TARGET}: ${OBJ}
	$(CC) $(CFLAGS) -o ${BUILD_DIR}/${TARGET} $(OBJ)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)