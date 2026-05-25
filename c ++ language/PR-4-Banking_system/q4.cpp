#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
using namespace std;

// ============================================================
// TRANSACTION STRUCT - for history
// ============================================================
struct Transaction {
    string type;
    double amount;
    double balanceAfter;
    string timestamp;

    Transaction(string t, double a, double b) : type(t), amount(a), balanceAfter(b) {
        time_t now = time(0);
        char buf[20];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
        timestamp = string(buf);
    }
};

// ============================================================
// BASE CLASS: BankAccount  (Encapsulation + Polymorphism base)
// ============================================================
class BankAccount {
private:
    string accountNumber;
    string accountHolderName;
    double balance;

protected:
    vector<Transaction> history;

public:
    // Constructor
    BankAccount(string accNum, string name, double initialBalance)
        : accountNumber(accNum), accountHolderName(name), balance(initialBalance) {}

    // Virtual Destructor
    virtual ~BankAccount() {}

    // Getters (Encapsulation)
    string getAccountNumber() const { return accountNumber; }
    string getAccountHolderName() const { return accountHolderName; }
    double getBalance() const { return balance; }

    // Setters
    void setAccountHolderName(string name) { accountHolderName = name; }

    // Deposit
    virtual bool deposit(double amount) {
        if (amount <= 0) {
            cout << "  [ERROR] Deposit amount must be positive!" << endl;
            return false;
        }
        balance += amount;
        history.push_back(Transaction("DEPOSIT", amount, balance));
        cout << fixed << setprecision(2);
        cout << "  [SUCCESS] Deposited Rs." << amount << " | New Balance: Rs." << balance << endl;
        return true;
    }

    // Withdraw (virtual for polymorphism)
    virtual bool withdraw(double amount) {
        if (amount <= 0) {
            cout << "  [ERROR] Withdrawal amount must be positive!" << endl;
            return false;
        }
        if (amount > balance) {
            cout << "  [ERROR] Insufficient balance!" << endl;
            return false;
        }
        balance -= amount;
        history.push_back(Transaction("WITHDRAWAL", amount, balance));
        cout << fixed << setprecision(2);
        cout << "  [SUCCESS] Withdrawn Rs." << amount << " | New Balance: Rs." << balance << endl;
        return true;
    }

    // Virtual display (Polymorphism)
    virtual void displayAccountInfo() const {
        cout << fixed << setprecision(2);
        cout << "  Account Number : " << accountNumber << endl;
        cout << "  Holder Name    : " << accountHolderName << endl;
        cout << "  Balance        : Rs." << balance << endl;
    }

    // Virtual method for polymorphism
    virtual double calculateInterest() { return 0.0; }
    virtual string getAccountType() const { return "Bank Account"; }

    void displayTransactionHistory() const {
        if (history.empty()) {
            cout << "  No transactions yet." << endl;
            return;
        }
        cout << fixed << setprecision(2);
        cout << "  " << left << setw(20) << "Timestamp"
             << setw(15) << "Type"
             << setw(15) << "Amount"
             << setw(15) << "Balance" << endl;
        cout << "  " << string(62, '-') << endl;
        for (const auto& t : history) {
            cout << "  " << left << setw(20) << t.timestamp
                 << setw(15) << t.type
                 << setw(15) << t.amount
                 << setw(15) << t.balanceAfter << endl;
        }
    }

    // Update balance (used by subclasses internally)
    void addToBalance(double amount) { balance += amount; }
};


// ============================================================
// DERIVED CLASS 1: SavingsAccount  (Inheritance + Polymorphism)
// ============================================================
class SavingsAccount : public BankAccount {
private:
    double interestRate; // annual rate in %

public:
    SavingsAccount(string accNum, string name, double balance, double rate)
        : BankAccount(accNum, name, balance), interestRate(rate) {}

    ~SavingsAccount() {}

    double getInterestRate() const { return interestRate; }
    void setInterestRate(double rate) { interestRate = rate; }

    // Polymorphic method
    double calculateInterest() override {
        double interest = (getBalance() * interestRate) / 100.0;
        addToBalance(interest);
        history.push_back(Transaction("INTEREST", interest, getBalance()));
        cout << fixed << setprecision(2);
        cout << "  [INTEREST] Rs." << interest << " added | New Balance: Rs." << getBalance() << endl;
        return interest;
    }

    void displayAccountInfo() const override {
        cout << "  Account Type   : " << getAccountType() << endl;
        BankAccount::displayAccountInfo();
        cout << fixed << setprecision(2);
        cout << "  Interest Rate  : " << interestRate << "% p.a." << endl;
    }

    string getAccountType() const override { return "Savings Account"; }
};


// ============================================================
// DERIVED CLASS 2: CheckingAccount  (Inheritance + Polymorphism)
// ============================================================
class CheckingAccount : public BankAccount {
private:
    double overdraftLimit;

public:
    CheckingAccount(string accNum, string name, double balance, double overdraft)
        : BankAccount(accNum, name, balance), overdraftLimit(overdraft) {}

    ~CheckingAccount() {}

    double getOverdraftLimit() const { return overdraftLimit; }
    void setOverdraftLimit(double limit) { overdraftLimit = limit; }

    bool checkOverdraft(double amount) const {
        return (getBalance() - amount) >= -overdraftLimit;
    }

    // Override withdraw to allow overdraft
    bool withdraw(double amount) override {
        if (amount <= 0) {
            cout << "  [ERROR] Withdrawal amount must be positive!" << endl;
            return false;
        }
        if (!checkOverdraft(amount)) {
            cout << fixed << setprecision(2);
            cout << "  [ERROR] Exceeds overdraft limit of Rs." << overdraftLimit << "!" << endl;
            return false;
        }
        // Directly modify (use base deposit trick: negative withdrawal)
        double newBal = getBalance() - amount;
        // Use base withdraw if enough balance, else overdraft
        if (amount <= getBalance()) {
            BankAccount::withdraw(amount);
        } else {
            double overdraftUsed = amount - getBalance();
            history.push_back(Transaction("WITHDRAWAL(OD)", amount, newBal));
            addToBalance(-getBalance()); // zero out
            addToBalance(-(overdraftUsed)); // go negative
            cout << fixed << setprecision(2);
            cout << "  [SUCCESS] Withdrawn Rs." << amount
                 << " (Overdraft used: Rs." << overdraftUsed
                 << ") | New Balance: Rs." << getBalance() << endl;
        }
        return true;
    }

    void displayAccountInfo() const override {
        cout << "  Account Type   : " << getAccountType() << endl;
        BankAccount::displayAccountInfo();
        cout << fixed << setprecision(2);
        cout << "  Overdraft Limit: Rs." << overdraftLimit << endl;
    }

    string getAccountType() const override { return "Checking Account"; }
};


// ============================================================
// DERIVED CLASS 3: FixedDepositAccount (Inheritance + Polymorphism)
// ============================================================
class FixedDepositAccount : public BankAccount {
private:
    int term;           // duration in months
    double interestRate; // annual rate in %
    bool matured;

public:
    FixedDepositAccount(string accNum, string name, double balance, int months, double rate)
        : BankAccount(accNum, name, balance), term(months), interestRate(rate), matured(false) {}

    ~FixedDepositAccount() {}

    int getTerm() const { return term; }
    double getInterestRate() const { return interestRate; }

    // FD interest = Principal * Rate * Term(years)
    double calculateInterest() override {
        double principal = getBalance();
        double years = term / 12.0;
        double interest = principal * (interestRate / 100.0) * years;
        addToBalance(interest);
        matured = true;
        history.push_back(Transaction("FD MATURITY", interest, getBalance()));
        cout << fixed << setprecision(2);
        cout << "  [FD MATURITY] Interest earned: Rs." << interest
             << " | Maturity Amount: Rs." << getBalance() << endl;
        return interest;
    }

    // FD cannot be withdrawn before maturity (simplified rule)
    bool withdraw(double amount) override {
        if (!matured) {
            cout << "  [ERROR] Fixed Deposit has not matured yet! Term: " << term << " months." << endl;
            return false;
        }
        return BankAccount::withdraw(amount);
    }

    // FD does not allow deposit after creation
    bool deposit(double amount) override {
        cout << "  [ERROR] Cannot deposit into a Fixed Deposit Account after creation." << endl;
        return false;
    }

    void displayAccountInfo() const override {
        cout << "  Account Type   : " << getAccountType() << endl;
        BankAccount::displayAccountInfo();
        cout << fixed << setprecision(2);
        cout << "  Interest Rate  : " << interestRate << "% p.a." << endl;
        cout << "  Term           : " << term << " months" << endl;
        cout << "  Status         : " << (matured ? "Matured" : "Active") << endl;
        double projInterest = getBalance() * (interestRate/100.0) * (term/12.0);
        cout << "  Projected Int. : Rs." << projInterest << endl;
    }

    string getAccountType() const override { return "Fixed Deposit Account"; }
};


// ============================================================
// BANK SYSTEM: Manages accounts dynamically
// ============================================================
class BankSystem {
private:
    vector<BankAccount*> accounts;
    int nextAccNum;

    string generateAccNum() {
        return "ACC" + to_string(1000 + nextAccNum++);
    }

    BankAccount* findAccount(const string& accNum) {
        for (auto& acc : accounts) {
            if (acc->getAccountNumber() == accNum) return acc;
        }
        return nullptr;
    }

public:
    BankSystem() : nextAccNum(1) {}

    ~BankSystem() {
        for (auto& acc : accounts) delete acc;
        accounts.clear();
    }

    void createSavingsAccount() {
        string name;
        double balance, rate;
        cout << "  Enter Account Holder Name : ";
        cin.ignore(); getline(cin, name);
        cout << "  Enter Initial Balance (Rs.): ";
        cin >> balance;
        cout << "  Enter Annual Interest Rate (%): ";
        cin >> rate;
        string accNum = generateAccNum();
        accounts.push_back(new SavingsAccount(accNum, name, balance, rate));
        cout << "\n  [CREATED] Savings Account | Acc No: " << accNum << endl;
    }

    void createCheckingAccount() {
        string name;
        double balance, overdraft;
        cout << "  Enter Account Holder Name : ";
        cin.ignore(); getline(cin, name);
        cout << "  Enter Initial Balance (Rs.): ";
        cin >> balance;
        cout << "  Enter Overdraft Limit (Rs.): ";
        cin >> overdraft;
        string accNum = generateAccNum();
        accounts.push_back(new CheckingAccount(accNum, name, balance, overdraft));
        cout << "\n  [CREATED] Checking Account | Acc No: " << accNum << endl;
    }

    void createFixedDepositAccount() {
        string name;
        double balance, rate;
        int term;
        cout << "  Enter Account Holder Name : ";
        cin.ignore(); getline(cin, name);
        cout << "  Enter Deposit Amount (Rs.) : ";
        cin >> balance;
        cout << "  Enter Term (months)        : ";
        cin >> term;
        cout << "  Enter Annual Interest Rate (%): ";
        cin >> rate;
        string accNum = generateAccNum();
        accounts.push_back(new FixedDepositAccount(accNum, name, balance, term, rate));
        cout << "\n  [CREATED] Fixed Deposit Account | Acc No: " << accNum << endl;
    }

    void depositToAccount() {
        string accNum;
        double amount;
        cout << "  Enter Account Number: "; cin >> accNum;
        BankAccount* acc = findAccount(accNum);
        if (!acc) { cout << "  [ERROR] Account not found!" << endl; return; }
        cout << "  Enter Deposit Amount: Rs."; cin >> amount;
        acc->deposit(amount);
    }

    void withdrawFromAccount() {
        string accNum;
        double amount;
        cout << "  Enter Account Number: "; cin >> accNum;
        BankAccount* acc = findAccount(accNum);
        if (!acc) { cout << "  [ERROR] Account not found!" << endl; return; }
        cout << "  Enter Withdrawal Amount: Rs."; cin >> amount;
        acc->withdraw(amount);
    }

    void calculateInterestForAccount() {
        string accNum;
        cout << "  Enter Account Number: "; cin >> accNum;
        BankAccount* acc = findAccount(accNum);
        if (!acc) { cout << "  [ERROR] Account not found!" << endl; return; }
        // Polymorphism: base class pointer calls derived method
        double interest = acc->calculateInterest();
        if (interest == 0.0) {
            cout << "  [INFO] This account type does not earn interest this way." << endl;
        }
    }

    void displayAccount() {
        string accNum;
        cout << "  Enter Account Number: "; cin >> accNum;
        BankAccount* acc = findAccount(accNum);
        if (!acc) { cout << "  [ERROR] Account not found!" << endl; return; }
        cout << "\n  ===== Account Details =====" << endl;
        acc->displayAccountInfo();
    }

    void displayAllAccounts() {
        if (accounts.empty()) { cout << "\n  No accounts found." << endl; return; }
        cout << "\n  ===== All Accounts =====" << endl;
        cout << fixed << setprecision(2);
        cout << "  " << left << setw(12) << "Acc No"
             << setw(22) << "Holder Name"
             << setw(25) << "Type"
             << setw(15) << "Balance" << endl;
        cout << "  " << string(72, '-') << endl;
        for (auto& acc : accounts) {
            cout << "  " << left << setw(12) << acc->getAccountNumber()
                 << setw(22) << acc->getAccountHolderName()
                 << setw(25) << acc->getAccountType()
                 << "Rs." << acc->getBalance() << endl;
        }
        cout << "\n  Total Accounts: " << accounts.size() << endl;
    }

    void showTransactionHistory() {
        string accNum;
        cout << "  Enter Account Number: "; cin >> accNum;
        BankAccount* acc = findAccount(accNum);
        if (!acc) { cout << "  [ERROR] Account not found!" << endl; return; }
        cout << "\n  ===== Transaction History: " << accNum << " =====" << endl;
        acc->displayTransactionHistory();
    }

    void demonstratePolymorphism() {
        if (accounts.empty()) { cout << "\n  No accounts to demonstrate polymorphism." << endl; return; }
        cout << "\n  ===== Polymorphism Demo: calculateInterest() via Base Pointer =====" << endl;
        for (BankAccount* acc : accounts) {  // base class pointer
            cout << "\n  [" << acc->getAccountType() << "] Acc: " << acc->getAccountNumber() << endl;
            acc->calculateInterest();         // runtime polymorphism
        }
    }
};


// ============================================================
// MAIN FUNCTION - Menu Driven
// ============================================================
int main() {
    BankSystem bank;
    int choice;

    cout << "============================================" << endl;
    cout << "        BANKING SYSTEM (C++)                " << endl;
    cout << "  Demonstrating OOP: Polymorphism,          " << endl;
    cout << "  Inheritance & Encapsulation               " << endl;
    cout << "============================================" << endl;

    do {
        cout << "\n  ========== MAIN MENU ==========" << endl;
        cout << "  1.  Create Savings Account" << endl;
        cout << "  2.  Create Checking Account" << endl;
        cout << "  3.  Create Fixed Deposit Account" << endl;
        cout << "  4.  Deposit" << endl;
        cout << "  5.  Withdraw" << endl;
        cout << "  6.  Calculate Interest" << endl;
        cout << "  7.  Display Account Info" << endl;
        cout << "  8.  Display All Accounts" << endl;
        cout << "  9.  Transaction History" << endl;
        cout << "  10. Polymorphism Demo (All Accounts)" << endl;
        cout << "  11. Exit" << endl;
        cout << "  Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:  bank.createSavingsAccount(); break;
            case 2:  bank.createCheckingAccount(); break;
            case 3:  bank.createFixedDepositAccount(); break;
            case 4:  bank.depositToAccount(); break;
            case 5:  bank.withdrawFromAccount(); break;
            case 6:  bank.calculateInterestForAccount(); break;
            case 7:  bank.displayAccount(); break;
            case 8:  bank.displayAllAccounts(); break;
            case 9:  bank.showTransactionHistory(); break;
            case 10: bank.demonstratePolymorphism(); break;
            case 11: cout << "\n  Thank you for using Banking System. Goodbye!" << endl; break;
            default: cout << "\n  [ERROR] Invalid choice!" << endl;
        }
    } while (choice != 11);

    return 0;
}