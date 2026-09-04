#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <stdexcept>
#include <limits>
using namespace std;

class Transaction {
private:
    string type;
    double amount;
    string description;
    string timestamp;

public:
    Transaction(string t, double amt, string desc) {
        type = t;
        amount = amt;
        description = desc;
        timestamp = getCurrentTime();
    }

    string getCurrentTime() {
        time_t now = time(0);
        string t = ctime(&now);
        t.pop_back();
        return t;
    }

    void display() const {
        cout << "[" << timestamp << "] " << type << ": Rs. " << amount;
        if (!description.empty()) {
            cout << " (" << description << ")";
        }
        cout << "\n";
    }
};

class Account {
private:
    string accountNumber;
    double balance;
    vector<Transaction> history;

public:
    Account(string accNum, double initialBalance) {
        accountNumber = accNum;
        balance = initialBalance;
        history.push_back(Transaction("Account Opened", initialBalance, ""));
    }

    string getAccountNumber() const {
        return accountNumber;
    }

    double getBalance() const {
        return balance;
    }

    void deposit(double amount) {
        if (amount <= 0) {
            throw invalid_argument("Deposit amount must be greater than 0.");
        }
        balance += amount;
        history.push_back(Transaction("Deposit", amount, ""));
    }

    void withdraw(double amount) {
        if (amount <= 0) {
            throw invalid_argument("Withdrawal amount must be greater than 0.");
        }
        if (amount > balance) {
            throw runtime_error("Insufficient balance.");
        }
        balance -= amount;
        history.push_back(Transaction("Withdrawal", amount, ""));
    }

    void recordTransfer(double amount, const string& direction, const string& otherAccount) {
        history.push_back(Transaction("Transfer " + direction, amount, "Account " + otherAccount));
    }

    void displayInfo() const {
        cout << "\nAccount Number: " << accountNumber << "\n";
        cout << "Balance: Rs. " << balance << "\n";
    }

    void displayRecentTransactions(int count) const {
        cout << "\nRecent Transactions (Account " << accountNumber << "):\n";
        int start = (history.size() > (size_t)count) ? history.size() - count : 0;
        for (size_t i = start; i < history.size(); i++) {
            history[i].display();
        }
    }
};

class Customer {
private:
    string customerId;
    string name;
    vector<Account> accounts;

public:
    Customer(string id, string customerName) {
        customerId = id;
        name = customerName;
    }

    string getCustomerId() const {
        return customerId;
    }

    string getName() const {
        return name;
    }

    void addAccount(const Account& acc) {
        accounts.push_back(acc);
    }

    Account* findAccount(const string& accNum) {
        for (size_t i = 0; i < accounts.size(); i++) {
            if (accounts[i].getAccountNumber() == accNum) {
                return &accounts[i];
            }
        }
        return nullptr;
    }

    void displayAllAccounts() const {
        cout << "\nCustomer: " << name << " (ID: " << customerId << ")\n";
        for (size_t i = 0; i < accounts.size(); i++) {
            accounts[i].displayInfo();
        }
    }
};

class Bank {
private:
    vector<Customer> customers;
    int nextCustomerId;
    int nextAccountNumber;

public:
    Bank() {
        nextCustomerId = 1;
        nextAccountNumber = 1001;
    }

    Customer* findCustomer(const string& id) {
        for (size_t i = 0; i < customers.size(); i++) {
            if (customers[i].getCustomerId() == id) {
                return &customers[i];
            }
        }
        return nullptr;
    }

    Account* findAccountGlobally(const string& accNum) {
        for (size_t i = 0; i < customers.size(); i++) {
            Account* acc = customers[i].findAccount(accNum);
            if (acc != nullptr) {
                return acc;
            }
        }
        return nullptr;
    }

    string createCustomer(const string& name) {
        string id = "C" + to_string(nextCustomerId++);
        customers.push_back(Customer(id, name));
        return id;
    }

    string createAccount(const string& customerId, double initialDeposit) {
        Customer* cust = findCustomer(customerId);
        if (cust == nullptr) {
            throw invalid_argument("Customer ID not found.");
        }
        if (initialDeposit < 0) {
            throw invalid_argument("Initial deposit cannot be negative.");
        }

        string accNum = "A" + to_string(nextAccountNumber++);
        cust->addAccount(Account(accNum, initialDeposit));
        return accNum;
    }

    void deposit(const string& accNum, double amount) {
        Account* acc = findAccountGlobally(accNum);
        if (acc == nullptr) {
            throw invalid_argument("Account not found.");
        }
        acc->deposit(amount);
    }

    void withdraw(const string& accNum, double amount) {
        Account* acc = findAccountGlobally(accNum);
        if (acc == nullptr) {
            throw invalid_argument("Account not found.");
        }
        acc->withdraw(amount);
    }

    void transfer(const string& fromAcc, const string& toAcc, double amount) {
        Account* from = findAccountGlobally(fromAcc);
        Account* to = findAccountGlobally(toAcc);

        if (from == nullptr || to == nullptr) {
            throw invalid_argument("One or both accounts not found.");
        }
        if (fromAcc == toAcc) {
            throw invalid_argument("Cannot transfer to the same account.");
        }

        from->withdraw(amount);
        to->deposit(amount);

        from->recordTransfer(amount, "Sent", toAcc);
        to->recordTransfer(amount, "Received", fromAcc);
    }

    void displayCustomerInfo(const string& customerId) {
        Customer* cust = findCustomer(customerId);
        if (cust == nullptr) {
            throw invalid_argument("Customer ID not found.");
        }
        cust->displayAllAccounts();
    }

    void displayAccountTransactions(const string& accNum, int count) {
        Account* acc = findAccountGlobally(accNum);
        if (acc == nullptr) {
            throw invalid_argument("Account not found.");
        }
        acc->displayRecentTransactions(count);
    }
};

void showMenu() {
    cout << "\n===== Banking System =====\n";
    cout << "1. Create Customer\n";
    cout << "2. Create Account\n";
    cout << "3. Deposit\n";
    cout << "4. Withdraw\n";
    cout << "5. Transfer Funds\n";
    cout << "6. View Customer Accounts\n";
    cout << "7. View Recent Transactions\n";
    cout << "8. Exit\n";
    cout << "Enter choice: ";
}

int main() {
    Bank bank;
    int choice;

    do {
        showMenu();
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        try {
            if (choice == 1) {
                string name;
                cout << "Enter customer name: ";
                getline(cin, name);
                string id = bank.createCustomer(name);
                cout << "Customer created. ID: " << id << "\n";

            } else if (choice == 2) {
    			string custId;
    			cout << "Enter customer ID: ";
    			cin >> custId;

    			if (bank.findCustomer(custId) == nullptr) {
        			throw invalid_argument("Customer ID not found.");
   				 }

    			double initialDeposit;
    			cout << "Enter initial deposit: ";
    			cin >> initialDeposit;

    			string accNum = bank.createAccount(custId, initialDeposit);
    			cout << "Account created. Account Number: " << accNum << "\n";
			
			} else if (choice == 3) {
                string accNum;
                double amount;
                cout << "Enter account number: ";
                cin >> accNum;
                cout << "Enter deposit amount: ";
                cin >> amount;
                bank.deposit(accNum, amount);
                cout << "Deposit successful.\n";

            } else if (choice == 4) {
                string accNum;
                double amount;
                cout << "Enter account number: ";
                cin >> accNum;
                cout << "Enter withdrawal amount: ";
                cin >> amount;
                bank.withdraw(accNum, amount);
                cout << "Withdrawal successful.\n";

            } else if (choice == 5) {
                string fromAcc, toAcc;
                double amount;
                cout << "Enter sender account number: ";
                cin >> fromAcc;
                cout << "Enter receiver account number: ";
                cin >> toAcc;
                cout << "Enter amount: ";
                cin >> amount;
                bank.transfer(fromAcc, toAcc, amount);
                cout << "Transfer successful.\n";

            } else if (choice == 6) {
                string custId;
                cout << "Enter customer ID: ";
                cin >> custId;
                bank.displayCustomerInfo(custId);

            } else if (choice == 7) {
                string accNum;
                int count;
                cout << "Enter account number: ";
                cin >> accNum;
                cout << "How many recent transactions to view: ";
                cin >> count;
                bank.displayAccountTransactions(accNum, count);

            } else if (choice == 8) {
                cout << "Exiting. Goodbye.\n";

            } else {
                cout << "Invalid choice.\n";
            }

        } catch (const exception& e) {
            cout << "Error: " << e.what() << "\n";
        }

    } while (choice != 8);

    return 0;
}
