CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET = expense_tracker
SRC = Expense\ Tracker.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) "Expense Tracker.cpp" -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
