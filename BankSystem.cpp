#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <ostream>
#include <sstream>
#include <cassert>
#include <ctime>
#include <limits>  // Added for input validation

using namespace std;

// Utility Functions

// Read lines from a file into a vector
vector<string> ReadFile(const string& path) {
    vector<string> lines;
    ifstream file(path);
    if (!file.is_open()) {
        throw runtime_error("ERROR: Can't open the file");
    }
    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    file.close();
    return lines;
}

// Write lines to a file, with an option to append
void WriteFile(const string& path, const vector<string>& lines, bool append = true) {
    ios::openmode mode = ios::out | ios::trunc;
    if (append) {
        mode = ios::out | ios::app;
    }
    ofstream file(path, mode);
    if (!file.is_open()) {
        throw runtime_error("ERROR: Can't open the file");
    }
    for (const string& line : lines) {
        file << line << "\n";
    }
    file.close();
}

// Splitting the string that has been stored in the database
vector<string> SplitString(string line, string delimiter = ",") {
    vector<string> strs;

    int pos = 0;
    while((pos = (int) line.find(delimiter)) != -1){
        
        strs.push_back(line.substr(0,pos));
        line.erase(0, pos + delimiter.length());
    }
    strs.push_back(line);
    return strs;
}

// Reading a number from the user
int ReadInt(int low, int high) {
    int value;
    while (true) {
        cout << "\nEnter a number in the range " << low << " - " << high << ": ";
        if (cin >> value) {
            if (value >= low && value <= high) {
                return value;
            } else {
                cout << "ERROR: Number out of range. Try again." << endl;
            }
        } else {
            cout << "ERROR: Invalid input. Please enter a valid number." << endl;
            cin.clear();            // Clear the error state
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignore any remaining input up to a newline
        }
    }
}

// Print the choices to choose from
int ShowMenu(vector<string> options){
    cout<<"\nMenu:\n";
    for(int i =0 ; i < options.size(); i++){
        cout<<"\t"<< 1+i << ") "<< options[i] << endl;
    }
    return ReadInt(1,options.size());
}

// Integer Conversion
int ToInt(string str) {
    int id;
    istringstream iss(str);
    if (!(iss >> id)) {
        cout << "ERROR: Invalid input for integer conversion.\n";
        return -1; // Return a default value or handle the error as needed
    }
    return id;
}

// Double Conversion
double ToDouble(string str) {
    double balance;
    istringstream iss(str);
    if (!(iss >> balance)) {
        cout << "ERROR: Invalid input for double conversion.\n";
        return -1.0; // Return a default value or handle the error as needed
    }
    return balance;
}

bool ValidatePassword(const string& password) {
    // Password must be at least 8 characters long
    if (password.length() < 8) {
        return false;
    }

    bool hasDigit = false;
    bool hasCharacter = false;
    bool hasSpecialCharacter = false;
    bool hasUppercaseLetter = false;

    for (char ch : password) {
        if (isdigit(ch)) {
            hasDigit = true;
        } else if (isalpha(ch)) {
            hasCharacter = true;
            if (isupper(ch)) {
                hasUppercaseLetter = true;
            }
        } else {
            hasSpecialCharacter = true;
        }
    }

    // Password must contain at least one digit, one character, one special character, and one uppercase letter
    return hasDigit && hasCharacter && hasSpecialCharacter && hasUppercaseLetter;
}


//// Classes  ////

class TransactionHistory {
private:
    string type;
    string date;
    string message;
    double balance;
    double amount;
    int accID;

public:
    // Default constructor
    TransactionHistory()
        : type(""), date(""), message(""), amount(0.0), accID(-1), balance(0.0) {}

    // Parameterized constructor
    TransactionHistory(const string& type_, const string& message_,
                       const double amount_, const string& date_,
                       const int id, const double balance_)
        : type(type_), message(message_), amount(amount_), date(date_), accID(id), balance(balance_) {}

    // Constructor to create a TransactionHistory object from a formatted string
    TransactionHistory(string line) {
        vector<string> content = SplitString(line);
        assert(content.size() == 6);
        accID = ToInt(content[0]);
        type = content[1];
        amount = ToDouble(content[2]);
        message = content[3];
        balance = ToDouble(content[4]);
        date = content[5];
    }

    // Convert a TransactionHistory object to a formatted string
    string ToString() {
        ostringstream oss;
        oss << accID << "," << type << "," << amount << "," << message << "," << balance << "," << date;
        return oss.str();
    }

    // Print transaction details
    void Print() {
        cout << "\n ---------------------\n\n";
        cout << type << " $" << amount << " - Balance: $" << balance << endl << message << date;
        cout << "\n ---------------------\n";
    }

    // Get the account ID associated with this transaction
    const int GetAccountID() {
        return accID;
    }
};

class Account {
private:
    int accountID;
    double balance;
    vector<TransactionHistory> transactionHistory;

public:
    // Default constructor
    Account() : accountID(-1), balance(0.0) {}

    // Constructor to initialize an Account object from a line of text
    Account(string line) {
        vector<string> content = SplitString(line);
        assert(content.size() == 2);
        accountID = ToInt(content[0]);
        balance = ToDouble(content[1]);
    }

    // Add a transaction to the account's transaction history
    void AddTransaction(const TransactionHistory& transaction) {
        transactionHistory.push_back(transaction);
    }

    // Get the transaction history as a vector of strings
    vector<string> GetTransactionHistory() {
        vector<string> transactionLines;
        for (auto& transaction : transactionHistory) {
            transactionLines.push_back(transaction.ToString());
        }
        return transactionLines;
    }

    // Print account information
    void PrintInfo() {
        cout << "\t-> Account Details <-\n";
        cout << "-> Account ID: " << accountID << "\n";
        cout << "-> Account Balance: $" << balance << "\n\n";
    }

    // Print transaction history
    void PrintTransactionHistory() {
        if (transactionHistory.empty()) {
            cout << "\n\t->-> Transaction history is empty! <-<-\n";
            return;
        }
        cout << "\n\t->-> Transaction History <-<-\n";

        for (auto& transaction : transactionHistory) {
            transaction.Print();
        }
    }

    // Convert Account object to a string for storage
    string ToString() const {
        ostringstream oss;
        oss << accountID << "," << balance;
        return oss.str();
    }

    // Update the account balance
    void UpdateBalance(double newBalance) {
        balance += newBalance;
    }

    // Setter for balance
    void SetBalance(double balance_) {
        balance = balance_;
    }

    // Setter for account ID
    void SetAccountID(int id_) {
        accountID = id_;
    }

    // Getter for account ID
    const int& GetAccountID() const {
        return accountID;
    }

    // Getter for balance
    double& GetBalance() {
        return balance;
    }

};

class User {
private:
    string firstName;
    string lastName;
    string email;
    string userName;
    string password;
    int accID;

public:
    // Default constructor
    User() : firstName(""), lastName(""), email(""), userName(""), password(""), accID(-1) {}

    // Constructor to initialize a User object from a line of text
    User(string line) {
        vector<string> content = SplitString(line);
        assert(content.size() == 6);
        firstName = content[0];
        lastName = content[1];
        email = content[2];
        userName = content[3];
        password = content[4];
        accID = ToInt(content[5]);
    }

    // Read user data and initialize a User object
    void ReadData(const string& newUserName, int newAccountID) {
        accID = newAccountID;
        userName = newUserName;
        // Input and validate the user's password
        while (true) {
            cout << "\nEnter your password \n(at least 8 characters with numbers, characters,\n special characters, and an uppercase letter): ";
            cin >> password;

            if (ValidatePassword(password)) {
                string confirmPassword;
                cout << "\nConfirm your password: ";
                cin >> confirmPassword;

                if (password == confirmPassword) {
                    break;
                } else {
                    cout << "Passwords do not match. Please try again." << endl;
                }
            } else {
                cout << "\n->-> Invalid password format. Please try again. <-<-\n" << endl;
            }
        }

        cout << "Enter First Name: ";
        cin >> firstName;
        cout << "Enter Last Name: ";
        cin >> lastName;
        cout << "Enter Email: ";
        cin >> email;

    } 

    
    // Getters for user attributes
    const string &GetUserName() const {
        return userName;
    }
    
    int GetAccountID(){
        return accID;
    }

    const string &GetPassword() const {
        return password;
    }

    // Setters for user attributes
    void ChangeFirstName(string fname) {
        firstName = fname;
    }

    void ChangeLastName(string lname) {
        lastName = lname;
    }

    void ChangeEmail(string email_) {
        email = email_;
    }

    void ChangeUserName(string user_name) {
        userName = user_name;
    }

    void ChangePassword(string pass) {
        password = pass;
    }

    // Convert User object to a string for storage
    string ToString() const {
        ostringstream oss;
        oss << firstName << "," << lastName << "," << email << "," << userName << "," << password << "," << accID;
        return oss.str();
    }

    // Print user information
    void PrintInfo() const {
        cout << "\t->-> Personal Details <-<-\n";
        cout << "Mr/s: " << firstName << " " << lastName << endl;
        cout << "Email: " << email << "\nUser Name: " << userName << endl;
        cout << "Account Number: " << accID << endl;
    }

    // Get the account ID of the user
    const int GetAccID() const {
        return accID;
    }

};

class BankSystem {
private:
    User currentUser;
    Account currentAccount;
    map<string, User> userMap; // username to user object
    map<int, Account> accountMap; // account id to account object
    int lastAccountID;

public:
    BankSystem() : currentUser(), currentAccount(), lastAccountID(0) {}

    void UpdateDatabase() {
        // Update user data
        vector<string> userLines;
        for (const auto& userPair : userMap) {
            userLines.push_back(userPair.second.ToString());
        }
        WriteFile("users.txt", userLines, false);

        // Update account data
        vector<string> accountLines;
        for (const auto& accountPair : accountMap) {
            accountLines.push_back(accountPair.second.ToString());
        }
        WriteFile("accounts.txt", accountLines, false);

        // Update transaction history data
        vector<string> historyLines;
        for (auto& accountPair : accountMap) {
            vector<string> transactionLines = accountPair.second.GetTransactionHistory();
            historyLines.insert(historyLines.end(), transactionLines.begin(), transactionLines.end());
        }
        WriteFile("history.txt", historyLines, false);

        LoadDatabase(); // Reload data after updating
    }

    void LoadDatabase() {
        userMap.clear();
        accountMap.clear();

        // Load user data
        vector<string> userLines = ReadFile("users.txt");
        for (const string& userLine : userLines) {
            User user(userLine);
            userMap[user.GetUserName()] = user;
        }

        // Load account data
        vector<string> accountLines = ReadFile("accounts.txt");
        for (const string& accountLine : accountLines) {
            Account account(accountLine);
            accountMap[account.GetAccountID()] = account;
            lastAccountID = max(lastAccountID, account.GetAccountID());
        }

        // Load transaction history data
        vector<string> historyLines = ReadFile("history.txt");
        for (const string& historyLine : historyLines) {
            TransactionHistory transaction(historyLine);
            accountMap[transaction.GetAccountID()].AddTransaction(transaction);
        }
    }

    void Access() {
        LoadDatabase();
        int choice = ShowMenu({ "Login", "Sign Up" });
        if (choice == 1)
            Login();
        else
            SignUp();
    }

    void Run() {
        Access();

        while (true) {
            vector<string> menuOptions;
            menuOptions.push_back("Account Information");
            menuOptions.push_back("Personal Information");
            menuOptions.push_back("Edit Personal Information");
            menuOptions.push_back("Transaction History");
            menuOptions.push_back("Transfer Money");
            menuOptions.push_back("Deposit Money");
            menuOptions.push_back("Withdraw Money");
            menuOptions.push_back("Log Out");


            int choice = ShowMenu(menuOptions);

            switch (choice) {
                case 1:
                    currentAccount.PrintInfo();
                    break;
                case 2:
                    currentUser.PrintInfo();
                    break;
                case 3:
                    EditPersonalInfo();
                    break;
                case 4:
                    currentAccount.PrintTransactionHistory();
                    break;
                case 5:
                    TransferMoney();
                    break;
                case 6:
                    DepositMoney();
                    break;
                case 7:
                    WithdrawMoney();
                    break;
                case 8:
                    Logout();
                    return;
                default:
                    break;
            }
                vector<string> exitOptions;
                exitOptions.push_back("Return to Main Menu");
                exitOptions.push_back("Exit Program");

                int exitChoice = ShowMenu(exitOptions);

                if (exitChoice == 2) {
                    exit(0);  // Exit the program
                }
        }
    }

    void Login() {
        while (true) {
            string userName, password;
            cout << "\nEnter User Name: ";
            cin >> userName;
            cout << "Enter Password: ";
            cin >> password;

            if (!userMap.count(userName)) {
                cout << "\nInvalid username or password. Try again.\n";
                continue;
            }

            currentUser = userMap[userName];

            if (currentUser.GetPassword() != password) {
                cout << "\nInvalid username or password. Try again.\n";
                currentUser = User(); // Reset currentUser to default state
                currentAccount = Account(); // Reset currentAccount to default state
                continue;
            }

            currentAccount = accountMap[currentUser.GetAccountID()];
            cout << "\n\t->->-> Welcome Back!! <-<-<-\n\n";
            break;
        }
    }

    void SignUp() {
        string userName;
        while (true) {
            cout << "\nEnter User Name: ";
            cin >> userName;
            if (userMap.count(userName)) {
                cout << "\n-> Username already in use. Try again <-\n\n";
                continue;
            }
            break;
        }

        currentUser = User();
        currentAccount = Account();
        lastAccountID++;

        currentUser.ReadData(userName, lastAccountID);

        const double initialBalanceRequirement = 100.0; // Minimum balance requirement
        cout << "\n\tTo open an account, you need to deposit at least $" << initialBalanceRequirement << endl;

        double initialDeposit;
        while (true) {
            cout << "\n\tEnter the initial deposit amount: $";
            cin >> initialDeposit;
            if (initialDeposit < initialBalanceRequirement) {
                cout << "\n\tThe initial deposit amount is less than the required amount. Try again.\n";
            } else {
                break;
            }
        }

        currentAccount.SetAccountID(lastAccountID);
        currentAccount.SetBalance(initialDeposit);

        accountMap[currentUser.GetAccountID()] = currentAccount;
        userMap[userName] = currentUser;

        UpdateDatabase();

        cout << "\n\t->->-> Welcome!! <-<-<-\n\n";
    }

    char* GetTime() {
        time_t now = time(0);
        char* dt = ctime(&now);
        return dt;
    }

    void EditPersonalInfo() {
        vector<string> choices;
        choices.push_back("First Name");
        choices.push_back("Last Name");
        choices.push_back("Email");
        choices.push_back("User Name");
        choices.push_back("Password");

        int choice = ShowMenu(choices);

        switch (choice) {
            case 1:
                ChangeFirstName();
                break;
            case 2:
                ChangeLastName();
                break;
            case 3:
                ChangeEmail();
                break;
            case 4:
                ChangeUserName();
                break;
            case 5:
                ChangePassword();
                break;
            default:
                break;
        }

        cout << "\nDo any other changes? (Y/N): ";
        char input;
        cin >> input;
        if (input == 'y' || input == 'Y') {
            EditPersonalInfo();
        }

        UpdateDatabase();
    }

    void ChangeFirstName() {
        string firstName;
        cout << "\nEnter your new First Name: ";
        cin >> firstName;
        currentUser.ChangeFirstName(firstName);
        cout << "\n\t->-> Done! <-<-\n";

        userMap[currentUser.GetUserName()].ChangeFirstName(firstName);
    }

    void ChangeLastName() {
        string lastName;
        cout << "\nEnter your Last Name: ";
        cin >> lastName;
        currentUser.ChangeLastName(lastName);
        cout << "\n\t->-> Done! <-<-\n";

        userMap[currentUser.GetUserName()].ChangeLastName(lastName);
    }

    void ChangeEmail() {
        string email;
        cout << "\nEnter your Email: ";
        cin >> email;
        currentUser.ChangeEmail(email);
        cout << "\n\t->-> Done! <-<-\n";

        userMap[currentUser.GetUserName()].ChangeEmail(email);
    }

    void ChangeUserName() {
        string newUserName;
        while (true) {
            cout << "\nEnter your new User Name: ";
            cin >> newUserName;

            if (userMap.count(newUserName)) {
                cout << "\n-> Username already in use. Please choose a different username.\n\n";
            } else if (currentUser.GetUserName() == newUserName) {
                cout << "\n-> This is your current username. Please choose a different username.\n\n";
            } else {
                break;
            }
        }

        currentUser.ChangeUserName(newUserName);
        cout << "\n\t->-> Username updated successfully! <-<-\n";

        userMap[currentUser.GetUserName()] = currentUser;
    }

    void ChangePassword() {
        string currentPassword;
        int remainingAttempts = 3; // Number of password change attempts allowed

        while (remainingAttempts > 0) {
            cout << "\nEnter your current password: ";
            cin >> currentPassword;

            if (currentPassword != currentUser.GetPassword()) {
                cout << "\nIncorrect current password. ";
                cout << "Remaining attempts: " << remainingAttempts - 1 << endl;
                remainingAttempts--;
                continue;
            }
            string newPassword;
            
            while (true) {
                cout << "\nEnter your password \n(at least 8 characters with numbers, characters, \nspecial characters, and an uppercase letter): ";
                cin >> newPassword;

                if (ValidatePassword(newPassword)) {
                    string confirmPassword;
                    cout << "Confirm your password: ";
                    cin >> confirmPassword;

                    if (newPassword == confirmPassword) {
                        break;
                    } else {
                        cout << "Passwords do not match. Please try again." << endl;
                    }
                } else {
                    cout << "Invalid password format. Please try again." << endl;
                }
            }
            currentUser.ChangePassword(newPassword);
            cout << "\nPassword updated successfully.\n";

            userMap[currentUser.GetUserName()] = currentUser;

            return;
        }

        cout << "\nExceeded maximum password change attempts. ";
        cout << "Please try again later.\n";
    }

    void DepositMoney() {
        double amount;
        while (true) {
            cout << "\nEnter the amount to deposit: $";
            cin >> amount;
            if (amount > 1000000.0) {
                cout << "\n->-> You can't deposit more than a million dollars at a time. Try again <-<-\n";
                continue;
            }
            break;
        }

        currentAccount.UpdateBalance(amount);
        string transactionDate = GetTime();
        TransactionHistory transaction("Deposit", "", amount, transactionDate, currentUser.GetAccountID(), currentAccount.GetBalance());
        currentAccount.AddTransaction(transaction);
        accountMap[currentAccount.GetAccountID()] = currentAccount;
        UpdateDatabase();

        cout << "\n\t->-> $" << amount << " has been added to your account successfully! <-<-\n";
    }

    void WithdrawMoney() {
        double amount;
        while (true) {
            cout << "\nEnter the amount to withdraw: $";
            cin >> amount;
            if (currentAccount.GetBalance() < amount) {
                cout << "\n->-> The amount you entered is greater than your balance. Try again <-<-\n";
                continue;
            }
            break;
        }

        currentAccount.UpdateBalance(-amount);
        string transactionDate = GetTime();
        TransactionHistory transaction("Withdraw", "", amount, transactionDate, currentUser.GetAccountID(), currentAccount.GetBalance());
        currentAccount.AddTransaction(transaction);
        accountMap[currentAccount.GetAccountID()] = currentAccount;
        UpdateDatabase();

        cout << "\n\t->-> $" << amount << " has been withdrawn successfully! <-<-\n";
    }

    void TransferMoney() {
        double amount;
        while (true) {
            cout << "\nEnter the amount to transfer: $";
            cin >> amount;
            if (currentAccount.GetBalance() < amount) {
                cout << "\n->-> The amount you entered is greater than your balance. Try again <-<-\n";
                continue;
            }
            break;
        }

        string receiver;

        while (true) {
            cout << "\nTo user: ";
            cin >> receiver;
            if (!userMap.count(receiver)) {
                cout << "\n->-> User does not exist. Try again\n";
                continue;
            }
            break;
        }

        currentAccount.UpdateBalance(-amount);
        string transactionDate = GetTime();
        string transactionMessage = " to (" + receiver + ") ";
        TransactionHistory senderTransaction("Transfer", transactionMessage, amount, transactionDate, currentUser.GetAccountID(), currentAccount.GetBalance());
        currentAccount.AddTransaction(senderTransaction);
        accountMap[currentAccount.GetAccountID()] = currentAccount;

        int receiverAccountID = userMap[receiver].GetAccountID();
        accountMap[receiverAccountID].UpdateBalance(amount);
        double receiverBalance = accountMap[receiverAccountID].GetBalance();
        string receiverTransactionMessage = " from (" + currentUser.GetUserName() + ") ";
        TransactionHistory receiverTransaction("Receive", receiverTransactionMessage, amount, transactionDate, receiverAccountID, receiverBalance);
        accountMap[receiverAccountID].AddTransaction(receiverTransaction);

        UpdateDatabase();

        cout << "\n\t->$" << amount << " has been sent to " << receiver << " successfully! <-\n";
    }

    void Logout() {
        currentUser = User();
        currentAccount = Account();
        cout << "\n\t->->-> You have been successfully logged out. <-<-<-\n\n";
    }

};


int main() {
    BankSystem system;
    system.Run();
    return 0;
}
