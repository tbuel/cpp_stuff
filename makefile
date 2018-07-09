# Flags
CC = g++
CFLAGS = -static-libgcc -static-libstdc++

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp
	
clean:
	rm