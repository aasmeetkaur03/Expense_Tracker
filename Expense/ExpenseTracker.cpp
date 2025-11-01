#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;

// Structure to store individual expenses
struct Expense {
    string dateTime;
    string category;
    string description;
    double amount;
};

// Function prototypes
void addExpense(vector<Expense>&);
void viewExpenses(const vector<Expense>&);
void showTotalExpenses(const vector<Expense>&);
void saveExpensesToFile(const vector<Expense>&);
void loadExpensesFromFile(vector<Expense>&);
void clearExpenses(vector<Expense>&);
void exportToCSV(const vector<Expense>&);
string getCurrentDateTime();

const string FILENAME = "expenses.txt";
const string CSVFILE = "expenses.csv";

int main() {
    vector<Expense> expenses;
    int choice;

    // Load saved data from file at start
    loadExpensesFromFile(expenses);

    do {
        cout << "\n============================\n";
        cout << "       EXPENSE TRACKER     \n";
        cout << "============================\n";
        cout << "1. Add Expense\n";
        cout << "2. View All Expenses\n";
        cout << "3. Show Total Expenses\n";
        cout << "4. Clear All Records\n";
        cout << "5. Export to CSV (Excel)\n";
        cout << "6. Exit\n";
        cout << "----------------------------\n";
        cout << "Enter Your Choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            addExpense(expenses);
            break;
        case 2:
            viewExpenses(expenses);
            break;
        case 3:
            showTotalExpenses(expenses);
            break;
        case 4:
            clearExpenses(expenses);
            break;
        case 5:
            exportToCSV(expenses);
            break;
        case 6:
            cout << "\nSaving and Exiting :> \n";
            saveExpensesToFile(expenses);
            break;
        default:
            cout << "\nInvalid Choice! Please Try Again.\n";
        }
    } while (choice != 6);

    return 0;
}

// Function to get current date and time
string getCurrentDateTime() {
    time_t now = time(0);
    tm* localTime = localtime(&now);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);
    return string(buffer);
}

// Function to add an expense
void addExpense(vector<Expense>& expenses) {
    Expense newExpense;
    newExpense.dateTime = getCurrentDateTime();
    cout << "\nEnter the Category (Eg : Food, Travel ): ";
    cin >> newExpense.category;
    cout << "Enter the Description: ";
    cin.ignore();
    getline(cin, newExpense.description);
    cout << "Enter the Amount ($): ";
    cin >> newExpense.amount;

    expenses.push_back(newExpense);
    saveExpensesToFile(expenses);
    cout << "\nExpense Added Successfully at " << newExpense.dateTime << "!\n";
}

// Function to display all expenses
void viewExpenses(const vector<Expense>& expenses) {
    if (expenses.empty()) {
        cout << "\nNo Expenses Recorded Yet :( \n";
        return;
    }

    cout << "\n-------------------------------------------------------------------------------------------\n";
    cout << left << setw(20) << "Date & Time"
         << setw(15) << "Category"
         << setw(30) << "Description"
         << right << setw(10) << "Amount ($)" << endl;
    cout << "-------------------------------------------------------------------------------------------\n";

    for (const auto& exp : expenses) {
        cout << left << setw(20) << exp.dateTime
             << setw(15) << exp.category
             << setw(30) << exp.description
             << right << setw(10) << fixed << setprecision(2) << exp.amount << endl;
    }

    cout << "-------------------------------------------------------------------------------------------\n";
}

// Function to show total of all expenses
void showTotalExpenses(const vector<Expense>& expenses) {
    if (expenses.empty()) {
        cout << "\nNo Expenses Recorded Yet :( \n";
        return;
    }

    double total = 0.0;
    for (const auto& exp : expenses) {
        total += exp.amount;
    }

    cout << "\nTotal Expenses : $" << fixed << setprecision(2) << total << endl;
}

// Save expenses to file
void saveExpensesToFile(const vector<Expense>& expenses) {
    ofstream file(FILENAME, ios::trunc);
    if (!file) {
        cerr << "\nError Saving to File!\n";
        return;
    }

    for (const auto& exp : expenses) {
        file << exp.dateTime << "|" << exp.category << "|" << exp.description << "|" << exp.amount << "\n";
    }
    file.close();
}

// Load expenses from file
void loadExpensesFromFile(vector<Expense>& expenses) {
    ifstream file(FILENAME);
    if (!file) return;

    expenses.clear();
    Expense exp;
    string line;
    while (getline(file, line)) {
        size_t pos1 = line.find('|');
        size_t pos2 = line.find('|', pos1 + 1);
        size_t pos3 = line.find('|', pos2 + 1);

        if (pos1 != string::npos && pos2 != string::npos && pos3 != string::npos) {
            exp.dateTime = line.substr(0, pos1);
            exp.category = line.substr(pos1 + 1, pos2 - pos1 - 1);
            exp.description = line.substr(pos2 + 1, pos3 - pos2 - 1);
            exp.amount = stod(line.substr(pos3 + 1));
            expenses.push_back(exp);
        }
    }
    file.close();
}

// Clear all expense records
void clearExpenses(vector<Expense>& expenses) {
    char confirm;
    cout << "\nAre You Sure you Want to Delete all Expense Records? (y/n): ";
    cin >> confirm;

    if (tolower(confirm) == 'y') {
        expenses.clear();
        ofstream file(FILENAME, ios::trunc);
        file.close();
        cout << "\nAll Expense Records Deleted Successfully!\n";
    } else {
        cout << "\nAction Cancelled\n";
    }
}

// Export expenses to CSV with total summary
void exportToCSV(const vector<Expense>& expenses) {
    if (expenses.empty()) {
        cout << "\nNo Expenses to Export!\n";
        return;
    }

    ofstream csv(CSVFILE, ios::trunc);
    if (!csv) {
        cerr << "\nError Creating CSV File!\n";
        return;
    }

    // CSV header
    csv << "Date & Time,Category,Description,Amount ($)\n";

    double total = 0.0;

    // Write all expenses
    for (const auto& exp : expenses) {
        csv << "\"" << exp.dateTime << "\","
            << "\"" << exp.category << "\","
            << "\"" << exp.description << "\","
            << fixed << setprecision(2) << exp.amount << "\n";
        total += exp.amount;
    }

    // Write total row
    csv << "\n,,Total Expenses ($)," << fixed << setprecision(2) << total << "\n";
    csv.close();

    cout << "\nData Successfully Exported to '" << CSVFILE 
         << "' with Total: $" << fixed << setprecision(2) << total << endl;
}
