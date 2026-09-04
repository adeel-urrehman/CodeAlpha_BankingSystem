# Banking System — CodeAlpha Internship Task

A console-based Banking Management System implemented in C++ using Object-Oriented Programming (OOP) concepts. The system manages customer records, bank accounts, fund transfers, and timestamped transaction histories.

### Features
* **Customer & Account Management:** Register new customers and link multiple bank accounts with unique auto-generated IDs.
* **Core Banking Operations:** Deposit and withdraw funds with input validation and balance checks.
* **Direct Fund Transfers:** Transfer funds between accounts with sender/receiver transaction tracking.
* **Transaction History:** Records every activity with real-time timestamps (`ctime`), with an option to view the most recent logs.
* **OOP Architecture:** Organized into modular classes (`Transaction`, `Account`, `Customer`, `Bank`) with clean encapsulation and exception handling (`std::invalid_argument`, `std::runtime_error`).

### How to Run
Compile using any standard C++ compiler and run the executable:

```bash
g++ CodeAlpha_BankingSystem.cpp -o banking_app
./banking_app
