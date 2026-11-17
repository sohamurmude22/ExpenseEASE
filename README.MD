# 💰 SpendWise – Personal Expense Tracker

SpendWise is a beginner-friendly command-line application developed using **C++17** to manage personal expenses and monthly budgets.

It allows users to record daily expenses, manage spending limits, search transactions and view basic financial reports. The application uses file handling to store data permanently, so saved records remain available after restarting the program.

## 🚀 Features

### 1. Expense Management
- Add new expenses with amount, category, date and description.
- View all recorded expenses.
- Update expenses using their unique ID.
- Delete expenses using their ID.
- Automatically generate unique IDs for transactions.

### 2. File Handling
- Store expense records in a CSV file.
- Automatically load saved expenses when the application starts.
- Save changes after adding, updating or deleting expenses.
- Maintain records between application sessions.

### 3. Monthly Budget Management
- Set a monthly spending budget.
- Calculate total expenses for a selected month.
- Display the remaining budget.
- Show a warning when spending reaches 80% of the budget.
- Display an alert when spending exceeds the budget.
- Save budget information in a text file.

### 4. Search and Filter
- Search expenses using their ID.
- Filter expenses by category.
- Filter expenses by month.
- Display total spending for filtered results.

### 5. Financial Reports
- Display total expenses.
- Display the number of transactions.
- Identify the highest expense.
- Show category-wise spending and percentage.

### 6. Input Validation
- Validate menu choices and expense IDs.
- Prevent negative or zero expense amounts.
- Validate date input.
- Handle invalid inputs properly.

---

## 🛠️ Technologies Used

- **Language:** C++17
- **Structures:** Store expense details.
- **Vectors:** Manage expense records.
- **File Handling:** Store and retrieve data.
- **Maps:** Calculate category-wise spending.
- **Standard Template Library (STL):** Perform basic operations.

**Development Tools:** GCC / G++ and Visual Studio Code.

---

## 📂 Project Structure

```text
SpendWise/
│
├── Expense Tracker.cpp
├── expenses.csv
├── budget.txt
├── Makefile
└── README.md
```

---

## 💾 Data Storage

SpendWise uses local files to maintain records.

**expenses.csv**

Stores expense details such as ID, amount, category, date and description.

**budget.txt**

Stores the monthly budget amount.

Data is automatically loaded when the application starts and saved whenever changes are made.

---

## 💻 How to Compile and Run

### Linux / Ubuntu

```bash
g++ -std=c++17 "Expense Tracker.cpp" -o expense_tracker
./expense_tracker
```

### Windows

```bash
g++ -std=c++17 -Wall -Wextra "Expense Tracker.cpp" -o spendwise.exe
spendwise.exe
```

---

## 📋 Application Menu

```text
=========================================
          SPENDWISE EXPENSE TRACKER
=========================================
1. Add Expense
2. View Expenses
3. Update Expense
4. Delete Expense
5. Search Expense
6. Filter Expenses
7. Set Monthly Budget
8. View Budget Status
9. View Reports
10. Exit
=========================================
```

---

## 🎯 Project Objective

The main objective of SpendWise is to provide a simple way to track personal expenses and understand spending habits while demonstrating practical C++ programming concepts such as structures, vectors, functions, file handling and data management.