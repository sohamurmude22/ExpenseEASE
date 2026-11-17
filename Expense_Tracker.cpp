
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <limits>
#include <map>

using namespace std;

// Structure representing a single expense record
struct Expense {
    int id;
    double amount;
    string category;
    string date;        // Format: YYYY-MM-DD
    string description;
};

// File persistence constants
const string EXPENSE_FILE = "expenses.csv";
const string BUDGET_FILE = "budget.txt";

// ============================================================================
//                              HELPER FUNCTIONS
// ============================================================================

// Clears the input buffer to prevent stream errors and infinite loops
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Converts a string to lowercase for case-insensitive comparisons
string toLowerCase(string str) {
    for (char &c : str) {
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    }
    return str;
}

// Returns the current system date as a string in YYYY-MM-DD format
string getCurrentDate() {
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);
    char buffer[16];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", localTime);
    return string(buffer);
}

// Returns the current system year and month as a string in YYYY-MM format
string getCurrentMonth() {
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);
    char buffer[16];
    strftime(buffer, sizeof(buffer), "%Y-%m", localTime);
    return string(buffer);
}

// Validates whether a string matches the YYYY-MM-DD format and has valid ranges
bool isValidDate(const string& date) {
    if (date.length() != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;

    for (int i = 0; i < 10; ++i) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(static_cast<unsigned char>(date[i]))) return false;
    }

    try {
        int year = stoi(date.substr(0, 4));
        int month = stoi(date.substr(5, 2));
        int day = stoi(date.substr(8, 2));

        if (year < 2000 || year > 2100) return false;
        if (month < 1 || month > 12) return false;
        if (day < 1 || day > 31) return false;
    } catch (...) {
        return false;
    }

    return true;
}

// Validates whether a string matches the YYYY-MM format and has valid ranges
bool isValidMonth(const string& month) {
    if (month.length() != 7) return false;
    if (month[4] != '-') return false;

    for (int i = 0; i < 7; ++i) {
        if (i == 4) continue;
        if (!isdigit(static_cast<unsigned char>(month[i]))) return false;
    }

    try {
        int year = stoi(month.substr(0, 4));
        int m = stoi(month.substr(5, 2));

        if (year < 2000 || year > 2100) return false;
        if (m < 1 || m > 12) return false;
    } catch (...) {
        return false;
    }

    return true;
}

// Reads an integer within [minVal, maxVal] with complete input validation
int readInt(const string& prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        if (!prompt.empty()) {
            cout << prompt;
        }
        if (cin >> value) {
            clearInputBuffer();
            if (value >= minVal && value <= maxVal) {
                return value;
            }
            cout << "Please enter a number between " << minVal << " and " << maxVal << ": ";
        } else {
            cout << "Invalid input. Please enter an integer: ";
            clearInputBuffer();
        }
    }
}

// Reads a positive floating-point amount (> 0)
double readPositiveDouble(const string& prompt) {
    double value;
    while (true) {
        if (!prompt.empty()) {
            cout << prompt;
        }
        if (cin >> value) {
            clearInputBuffer();
            if (value > 0.0) {
                return value;
            }
            cout << "Amount must be greater than 0. Please enter a positive amount: ";
        } else {
            cout << "Invalid amount. Please enter a valid number: ";
            clearInputBuffer();
        }
    }
}

// Reads a line of text cleanly from the console
string readLine(const string& prompt) {
    if (!prompt.empty()) {
        cout << prompt;
    }
    string line;
    getline(cin, line);
    return line;
}

// Reads a date in YYYY-MM-DD format with optional default value on pressing Enter
string readDate(const string& prompt, const string& defaultDate = "") {
    while (true) {
        cout << prompt;
        string input;
        getline(cin, input);

        size_t start = input.find_first_not_of(" \t\r\n");
        if (start == string::npos) {
            if (!defaultDate.empty()) {
                return defaultDate;
            }
            cout << "Date cannot be empty. Please use YYYY-MM-DD format: ";
            continue;
        }
        size_t end = input.find_last_not_of(" \t\r\n");
        input = input.substr(start, end - start + 1);

        if (isValidDate(input)) {
            return input;
        }
        cout << "Invalid date. Please enter in YYYY-MM-DD format (e.g. 2026-11-17): ";
    }
}

// Reads a month in YYYY-MM format with optional default value on pressing Enter
string readMonth(const string& prompt, const string& defaultMonth = "") {
    while (true) {
        cout << prompt;
        string input;
        getline(cin, input);

        size_t start = input.find_first_not_of(" \t\r\n");
        if (start == string::npos) {
            if (!defaultMonth.empty()) {
                return defaultMonth;
            }
            cout << "Month cannot be empty. Please use YYYY-MM format: ";
            continue;
        }
        size_t end = input.find_last_not_of(" \t\r\n");
        input = input.substr(start, end - start + 1);

        if (isValidMonth(input)) {
            return input;
        }
        cout << "Invalid month. Please enter in YYYY-MM format (e.g. 2026-11): ";
    }
}

// Pauses the console and waits for the user to press Enter
void pausePrompt() {
    cout << "\nPress Enter to continue...";
    string dummy;
    getline(cin, dummy);
}

// Calculates next unique Expense ID based on existing expenses
int getNextId(const vector<Expense>& expenses) {
    int maxId = 0;
    for (const auto& exp : expenses) {
        if (exp.id > maxId) {
            maxId = exp.id;
        }
    }
    return maxId + 1;
}

// Calculates total spending for a specific month (YYYY-MM)
double calculateMonthlySpending(const vector<Expense>& expenses, const string& month) {
    double total = 0.0;
    for (const auto& exp : expenses) {
        if (exp.date.length() >= 7 && exp.date.substr(0, 7) == month) {
            total += exp.amount;
        }
    }
    return total;
}

// ============================================================================
//                             FILE HANDLING
// ============================================================================

// Loads expenses from a CSV file into the vector
void loadExpenses(vector<Expense>& expenses, const string& filename) {
    expenses.clear();
    ifstream file(filename);
    if (!file.is_open()) {
        // File does not exist yet (normal on first launch)
        return;
    }

    string line;
    bool isFirstLine = true;
    while (getline(file, line)) {
        // Strip trailing carriage return if coming from Windows CRLF
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (line.empty()) continue;

        // Skip CSV header line if present
        if (isFirstLine && (line.find("id") != string::npos || line.find("ID") != string::npos)) {
            isFirstLine = false;
            continue;
        }
        isFirstLine = false;

        stringstream ss(line);
        string idStr, amountStr, category, date, description;

        if (getline(ss, idStr, ',') &&
            getline(ss, amountStr, ',') &&
            getline(ss, category, ',') &&
            getline(ss, date, ',')) {
            getline(ss, description); // The rest of the line is the description

            try {
                Expense exp;
                exp.id = stoi(idStr);
                exp.amount = stod(amountStr);
                exp.category = category;
                exp.date = date;
                exp.description = description.empty() ? "No description" : description;
                expenses.push_back(exp);
            } catch (...) {
                // Safely skip any corrupted or malformed lines
            }
        }
    }
    file.close();
}

// Saves all expenses from the vector into a CSV file
bool saveExpenses(const vector<Expense>& expenses, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Unable to open file " << filename << " for saving.\n";
        return false;
    }

    file << "id,amount,category,date,description\n";
    for (const auto& exp : expenses) {
        file << exp.id << ","
             << fixed << setprecision(2) << exp.amount << ","
             << exp.category << ","
             << exp.date << ","
             << exp.description << "\n";
    }
    file.close();
    return true;
}

// Loads monthly budget from text file
double loadBudget(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return 0.0;
    }
    double budget = 0.0;
    file >> budget;
    file.close();
    return (budget > 0.0) ? budget : 0.0;
}

// Saves monthly budget to text file
bool saveBudget(double budget, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Unable to save budget to " << filename << ".\n";
        return false;
    }
    file << fixed << setprecision(2) << budget << "\n";
    file.close();
    return true;
}

// ============================================================================
//                             DISPLAY HELPERS
// ============================================================================

// Formats and displays a table of expenses
void printExpenseTable(const vector<Expense>& expenses) {
    cout << left << setw(6)  << "ID"
         << left << setw(13) << "Date"
         << left << setw(20) << "Category"
         << right << setw(12) << "Amount ($)"
         << "   "
         << left << "Description" << "\n";
    cout << string(75, '-') << "\n";

    double total = 0.0;
    for (const auto& exp : expenses) {
        cout << left << setw(6)  << exp.id
             << left << setw(13) << exp.date
             << left << setw(20) << exp.category
             << right << setw(12) << fixed << setprecision(2) << exp.amount
             << "   "
             << left << exp.description << "\n";
        total += exp.amount;
    }
    cout << string(75, '-') << "\n";
    cout << "Total Records: " << expenses.size()
         << " | Total Amount: $" << fixed << setprecision(2) << total << "\n";
}

// Prompts user to select from predefined categories or enter custom
string selectCategory() {
    cout << "\nChoose Category:\n";
    cout << "1. Food\n";
    cout << "2. Transportation\n";
    cout << "3. Shopping\n";
    cout << "4. Bills & Utilities\n";
    cout << "5. Entertainment\n";
    cout << "6. Custom Category\n";

    int choice = readInt("Select category (1-6): ", 1, 6);
    switch (choice) {
        case 1: return "Food";
        case 2: return "Transportation";
        case 3: return "Shopping";
        case 4: return "Bills & Utilities";
        case 5: return "Entertainment";
        case 6: {
            while (true) {
                string custom = readLine("Enter custom category name: ");
                size_t start = custom.find_first_not_of(" \t\r\n");
                if (start != string::npos) {
                    size_t end = custom.find_last_not_of(" \t\r\n");
                    custom = custom.substr(start, end - start + 1);
                    replace(custom.begin(), custom.end(), ',', ' ');
                    return custom;
                }
                cout << "Category name cannot be empty. Please enter a valid name: ";
            }
        }
        default: return "Other";
    }
}

// ============================================================================
//                             CORE FEATURES
// ============================================================================

// 1. Add Expense
void addExpense(vector<Expense>& expenses, double budget) {
    cout << "\n---------------- Add New Expense ----------------\n";
    Expense exp;
    exp.id = getNextId(expenses);
    cout << "Assigned Expense ID: " << exp.id << "\n";

    exp.amount = readPositiveDouble("Enter amount ($): ");
    exp.category = selectCategory();

    string today = getCurrentDate();
    string datePrompt = "Enter date (YYYY-MM-DD) [Press Enter for today: " + today + "]: ";
    exp.date = readDate(datePrompt, today);

    exp.description = readLine("Enter description: ");
    if (exp.description.empty()) {
        exp.description = "No description";
    }
    // Replace commas to maintain CSV column structure
    replace(exp.description.begin(), exp.description.end(), ',', ';');

    expenses.push_back(exp);
    if (saveExpenses(expenses, EXPENSE_FILE)) {
        cout << "\nExpense added successfully with ID " << exp.id << "!\n";
    }

    // Check budget alert / warning if monthly budget is set
    if (budget > 0.0) {
        string expMonth = exp.date.substr(0, 7);
        double monthSpent = calculateMonthlySpending(expenses, expMonth);
        double pct = (monthSpent / budget) * 100.0;

        if (monthSpent > budget) {
            cout << "\n[!] ALERT: Your spending for " << expMonth
                 << " ($" << fixed << setprecision(2) << monthSpent
                 << ") has EXCEEDED your monthly budget ($"
                 << fixed << setprecision(2) << budget << ") by $"
                 << fixed << setprecision(2) << (monthSpent - budget) << "!\n";
        } else if (pct >= 80.0) {
            cout << "\n[!] WARNING: Your spending for " << expMonth
                 << " has reached " << fixed << setprecision(1) << pct
                 << "% of your monthly budget (80% threshold reached)!\n";
        }
    }
}

// 2. View Expenses
void viewExpenses(const vector<Expense>& expenses) {
    cout << "\n---------------- All Expenses ----------------\n";
    if (expenses.empty()) {
        cout << "No expenses recorded yet. Use Option 1 to add an expense.\n";
        return;
    }
    printExpenseTable(expenses);
}

// 3. Update Expense
void updateExpense(vector<Expense>& expenses) {
    cout << "\n---------------- Update Expense ----------------\n";
    if (expenses.empty()) {
        cout << "No expenses recorded yet.\n";
        return;
    }

    int targetId = readInt("Enter Expense ID to update: ", 1, 1000000);
    int targetIndex = -1;
    for (size_t i = 0; i < expenses.size(); ++i) {
        if (expenses[i].id == targetId) {
            targetIndex = static_cast<int>(i);
            break;
        }
    }

    if (targetIndex == -1) {
        cout << "Expense with ID " << targetId << " not found.\n";
        return;
    }

    Expense& exp = expenses[targetIndex];
    cout << "\nCurrent details for ID " << exp.id << ":\n";
    cout << "  Amount:      $" << fixed << setprecision(2) << exp.amount << "\n";
    cout << "  Category:    " << exp.category << "\n";
    cout << "  Date:        " << exp.date << "\n";
    cout << "  Description: " << exp.description << "\n";
    cout << "------------------------------------------------\n";

    // 1. Amount
    cout << "Enter new amount ($) [Enter 0 to keep current $" << fixed << setprecision(2) << exp.amount << "]: ";
    while (true) {
        double newAmt;
        if (cin >> newAmt) {
            clearInputBuffer();
            if (newAmt > 0.0) {
                exp.amount = newAmt;
                break;
            } else if (newAmt == 0.0) {
                break; // Keep current
            } else {
                cout << "Amount cannot be negative. Try again: ";
            }
        } else {
            cout << "Invalid number. Try again: ";
            clearInputBuffer();
        }
    }

    // 2. Category
    cout << "\nUpdate Category:\n";
    cout << "0. Keep current (" << exp.category << ")\n";
    cout << "1. Food\n";
    cout << "2. Transportation\n";
    cout << "3. Shopping\n";
    cout << "4. Bills & Utilities\n";
    cout << "5. Entertainment\n";
    cout << "6. Custom Category\n";
    int catChoice = readInt("Select choice (0-6): ", 0, 6);
    if (catChoice == 1) exp.category = "Food";
    else if (catChoice == 2) exp.category = "Transportation";
    else if (catChoice == 3) exp.category = "Shopping";
    else if (catChoice == 4) exp.category = "Bills & Utilities";
    else if (catChoice == 5) exp.category = "Entertainment";
    else if (catChoice == 6) {
        string custom = readLine("Enter custom category name: ");
        if (!custom.empty()) {
            replace(custom.begin(), custom.end(), ',', ' ');
            exp.category = custom;
        }
    }

    // 3. Date
    string datePrompt = "Enter new date (YYYY-MM-DD) [Press Enter to keep " + exp.date + "]: ";
    exp.date = readDate(datePrompt, exp.date);

    // 4. Description
    cout << "Enter new description [Press Enter to keep \"" << exp.description << "\"]: ";
    string newDesc;
    getline(cin, newDesc);
    if (!newDesc.empty()) {
        replace(newDesc.begin(), newDesc.end(), ',', ';');
        exp.description = newDesc;
    }

    if (saveExpenses(expenses, EXPENSE_FILE)) {
        cout << "\nExpense with ID " << exp.id << " updated and saved successfully!\n";
    }
}

// 4. Delete Expense
void deleteExpense(vector<Expense>& expenses) {
    cout << "\n---------------- Delete Expense ----------------\n";
    if (expenses.empty()) {
        cout << "No expenses recorded yet.\n";
        return;
    }

    int targetId = readInt("Enter Expense ID to delete: ", 1, 1000000);
    int targetIndex = -1;
    for (size_t i = 0; i < expenses.size(); ++i) {
        if (expenses[i].id == targetId) {
            targetIndex = static_cast<int>(i);
            break;
        }
    }

    if (targetIndex == -1) {
        cout << "Expense with ID " << targetId << " not found.\n";
        return;
    }

    const Expense& exp = expenses[targetIndex];
    cout << "\nSelected Expense:\n";
    cout << "  ID: " << exp.id << " | Date: " << exp.date
         << " | Category: " << exp.category
         << " | Amount: $" << fixed << setprecision(2) << exp.amount
         << " | Description: " << exp.description << "\n";

    cout << "Are you sure you want to delete this expense? (y/n): ";
    string confirm;
    getline(cin, confirm);
    if (!confirm.empty() && (confirm[0] == 'y' || confirm[0] == 'Y')) {
        expenses.erase(expenses.begin() + targetIndex);
        if (saveExpenses(expenses, EXPENSE_FILE)) {
            cout << "Expense with ID " << targetId << " deleted successfully!\n";
        }
    } else {
        cout << "Deletion cancelled. No changes made.\n";
    }
}

// 5. Search Expense
void searchExpense(const vector<Expense>& expenses) {
    cout << "\n---------------- Search Expense ----------------\n";
    if (expenses.empty()) {
        cout << "No expenses recorded yet.\n";
        return;
    }

    int targetId = readInt("Enter Expense ID to search: ", 1, 1000000);
    bool found = false;

    for (const auto& exp : expenses) {
        if (exp.id == targetId) {
            cout << "\n========== Expense Details ==========\n";
            cout << "ID:          " << exp.id << "\n";
            cout << "Date:        " << exp.date << "\n";
            cout << "Category:    " << exp.category << "\n";
            cout << "Amount:      $" << fixed << setprecision(2) << exp.amount << "\n";
            cout << "Description: " << exp.description << "\n";
            cout << "=====================================\n";
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "No expense found with ID " << targetId << ".\n";
    }
}

// 6. Filter Expenses
void filterExpenses(const vector<Expense>& expenses) {
    cout << "\n---------------- Filter Expenses ----------------\n";
    if (expenses.empty()) {
        cout << "No expenses recorded yet.\n";
        return;
    }

    cout << "1. Filter by Category\n";
    cout << "2. Filter by Month (YYYY-MM)\n";
    int filterChoice = readInt("Select filter option (1-2): ", 1, 2);

    if (filterChoice == 1) {
        string targetCat = readLine("Enter category name to filter: ");
        string targetCatLower = toLowerCase(targetCat);

        vector<Expense> matches;
        for (const auto& exp : expenses) {
            if (toLowerCase(exp.category) == targetCatLower) {
                matches.push_back(exp);
            }
        }

        if (matches.empty()) {
            cout << "\nNo expenses found under category \"" << targetCat << "\".\n";
        } else {
            cout << "\n--- Filtered Results for Category: " << targetCat << " ---\n";
            printExpenseTable(matches);
        }
    } else {
        string currentMonth = getCurrentMonth();
        string prompt = "Enter month (YYYY-MM) [Press Enter for current month: " + currentMonth + "]: ";
        string targetMonth = readMonth(prompt, currentMonth);

        vector<Expense> matches;
        for (const auto& exp : expenses) {
            if (exp.date.length() >= 7 && exp.date.substr(0, 7) == targetMonth) {
                matches.push_back(exp);
            }
        }

        if (matches.empty()) {
            cout << "\nNo expenses found for month " << targetMonth << ".\n";
        } else {
            cout << "\n--- Filtered Results for Month: " << targetMonth << " ---\n";
            printExpenseTable(matches);
        }
    }
}

// 7. Set Monthly Budget
void setMonthlyBudget(double& budget) {
    cout << "\n---------------- Set Monthly Budget ----------------\n";
    if (budget > 0.0) {
        cout << "Current Monthly Budget: $" << fixed << setprecision(2) << budget << "\n";
    }
    double newBudget = readPositiveDouble("Enter new monthly budget ($): ");
    budget = newBudget;
    if (saveBudget(budget, BUDGET_FILE)) {
        cout << "Monthly budget of $" << fixed << setprecision(2) << budget << " saved successfully!\n";
    }
}

// 8. View Budget Status
void viewBudgetStatus(const vector<Expense>& expenses, double budget) {
    cout << "\n===========================================================\n";
    cout << "                    MONTHLY BUDGET STATUS                  \n";
    cout << "===========================================================\n";

    if (budget <= 0.0) {
        cout << "Notice: No monthly budget is set yet.\n";
        cout << "Please use Option 7 to set your monthly budget.\n\n";
    } else {
        cout << "Monthly Budget:    $" << fixed << setprecision(2) << budget << "\n";
    }

    string currentMonth = getCurrentMonth();
    string prompt = "Enter month (YYYY-MM) [Press Enter for current month: " + currentMonth + "]: ";
    string targetMonth = readMonth(prompt, currentMonth);

    double monthSpending = calculateMonthlySpending(expenses, targetMonth);
    cout << "\nStatus for Month:  " << targetMonth << "\n";
    cout << "Total Spent:       $" << fixed << setprecision(2) << monthSpending << "\n";

    if (budget > 0.0) {
        double remaining = budget - monthSpending;
        double pct = (monthSpending / budget) * 100.0;

        cout << "Remaining Budget:  $" << fixed << setprecision(2) << remaining << "\n";
        cout << "Budget Utilized:   " << fixed << setprecision(1) << pct << "%\n";
        cout << "-----------------------------------------------------------\n";

        if (monthSpending > budget) {
            cout << "[!] ALERT: You have EXCEEDED your monthly budget by $"
                 << fixed << setprecision(2) << (monthSpending - budget) << "!\n";
        } else if (pct >= 80.0) {
            cout << "[!] WARNING: You have reached " << fixed << setprecision(1) << pct
                 << "% of your monthly budget (80% or more utilized)!\n";
        } else {
            cout << "[i] STATUS: Spending is within your budget limits.\n";
        }
    }
    cout << "===========================================================\n";
}

// 9. View Reports
void viewReports(const vector<Expense>& expenses) {
    cout << "\n===========================================================\n";
    cout << "                     FINANCIAL REPORTS                     \n";
    cout << "===========================================================\n";

    if (expenses.empty()) {
        cout << "No expenses recorded yet. Cannot generate reports.\n";
        cout << "===========================================================\n";
        return;
    }

    double totalAmount = 0.0;
    const Expense* highestExpense = &expenses[0];
    map<string, double> categoryTotals;

    for (const auto& exp : expenses) {
        totalAmount += exp.amount;
        if (exp.amount > highestExpense->amount) {
            highestExpense = &exp;
        }
        categoryTotals[exp.category] += exp.amount;
    }

    cout << "Total Transactions:  " << expenses.size() << "\n";
    cout << "Total Spending:      $" << fixed << setprecision(2) << totalAmount << "\n";
    cout << "Average Expense:     $" << fixed << setprecision(2) << (totalAmount / expenses.size()) << "\n";
    cout << "-----------------------------------------------------------\n";
    cout << "Highest Expense:\n";
    cout << "  ID:          " << highestExpense->id << "\n";
    cout << "  Amount:      $" << fixed << setprecision(2) << highestExpense->amount << "\n";
    cout << "  Category:    " << highestExpense->category << "\n";
    cout << "  Date:        " << highestExpense->date << "\n";
    cout << "  Description: " << highestExpense->description << "\n";
    cout << "-----------------------------------------------------------\n";
    cout << "Category-wise Spending Breakdown:\n";
    cout << left << setw(22) << "Category"
         << right << setw(14) << "Total ($)"
         << right << setw(14) << "Percentage" << "\n";
    cout << string(52, '-') << "\n";

    for (const auto& [category, catTotal] : categoryTotals) {
        double pct = (totalAmount > 0.0) ? (catTotal / totalAmount) * 100.0 : 0.0;
        cout << left << setw(22) << category
             << right << setw(14) << fixed << setprecision(2) << catTotal
             << right << setw(13) << fixed << setprecision(1) << pct << "%\n";
    }
    cout << "===========================================================\n";
}

// ============================================================================
//                               MENU & MAIN
// ============================================================================

void displayMenu() {
    cout << "\n=========================================\n";
    cout << "        SPENDWISE EXPENSE TRACKER        \n";
    cout << "=========================================\n";
    cout << "1. Add Expense\n";
    cout << "2. View Expenses\n";
    cout << "3. Update Expense\n";
    cout << "4. Delete Expense\n";
    cout << "5. Search Expense\n";
    cout << "6. Filter Expenses\n";
    cout << "7. Set Monthly Budget\n";
    cout << "8. View Budget Status\n";
    cout << "9. View Reports\n";
    cout << "10. Exit\n";
    cout << "=========================================\n";
}

int main() {
    vector<Expense> expenses;
    double budget = 0.0;

    // Load saved data on startup
    loadExpenses(expenses, EXPENSE_FILE);
    budget = loadBudget(BUDGET_FILE);

    cout << "===========================================================\n";
    cout << "       Welcome to SpendWise - C++ Expense Tracker          \n";
    cout << "===========================================================\n";
    cout << "Loaded " << expenses.size() << " saved expense(s) from " << EXPENSE_FILE << ".\n";
    if (budget > 0.0) {
        cout << "Loaded monthly budget: $" << fixed << setprecision(2) << budget << " from " << BUDGET_FILE << ".\n";
    }

    bool running = true;
    while (running) {
        displayMenu();
        cout << "Enter your choice (1-10): ";
        int choice;
        if (!(cin >> choice)) {
            // Reached EOF or stream closed
            break;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
                addExpense(expenses, budget);
                pausePrompt();
                break;
            case 2:
                viewExpenses(expenses);
                pausePrompt();
                break;
            case 3:
                updateExpense(expenses);
                pausePrompt();
                break;
            case 4:
                deleteExpense(expenses);
                pausePrompt();
                break;
            case 5:
                searchExpense(expenses);
                pausePrompt();
                break;
            case 6:
                filterExpenses(expenses);
                pausePrompt();
                break;
            case 7:
                setMonthlyBudget(budget);
                pausePrompt();
                break;
            case 8:
                viewBudgetStatus(expenses, budget);
                pausePrompt();
                break;
            case 9:
                viewReports(expenses);
                pausePrompt();
                break;
            case 10:
                cout << "\nThank you for using SpendWise Expense Tracker. Goodbye!\n";
                running = false;
                break;
            default:
                cout << "\nInvalid choice. Please enter a number between 1 and 10.\n";
                pausePrompt();
                break;
        }
    }

    return 0;
}
