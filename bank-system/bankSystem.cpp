// banking system 

#include <iostream>
#include <string>

class BankAccount {
private: 
    std::string name; 
    long long accountnumber; 
    long long balance; 
    inline static int TotalAccounts; 
    inline static int AccountNumber; 

    long long generateAccountNumber() {
        ++TotalAccounts;  
        return ++AccountNumber;   
    }

public: 
    BankAccount(std::string name, long long initialbalance) {
        if (initialbalance < 0) {
            throw std::invalid_argument("account balance cannot be negative\n"); 
        }
        this->name = name; 
        this->accountnumber = generateAccountNumber();    
        this->balance = initialbalance;
    }
    
    static int getTotalAccounts() {
        return TotalAccounts;  
    }

    void deposit(int amount) {
        if (amount < 0) {
            throw std::invalid_argument("invalid amount! amount cannot be negative\n"); 
        }
        this->balance += amount;
        std::cout << "money deposited successfully.\n";
    }

    void withdraw (int amount) {
        if (amount > this->balance) {
            throw std::invalid_argument("insufficient balance.\n");  
        }
        if (amount < 0) {
            throw std::invalid_argument(""); 
        }
        this->balance -= amount; 
        std::cout << amount << " withdrawn successfully.\n";  
    }
    
    void transfermoney(BankAccount* b, int amount) {
        if (amount < 0) {
            throw std::invalid_argument("invalid amount, amount cannot be negative\n"); 
        }
        if (amount > this->balance) {
            throw std::invalid_argument("you do not have suffient balance.\n"); 
        }
 
        b->balance += amount; 
        this->balance -= amount;  
        std::cout << "money transfered successfully\n";
    }

    void accountinfo() {
        std::cout << "---------------account info---------------\n"; 
        std::cout << "name: " << this->name << "\n"; 
        std::cout << "account number: " << this->accountnumber << "\n"; 
        std::cout << "balance: " << this->balance << "\n"; 
        std::cout << "-------------------------------------------\n";  
    }
}; 

int main() {
    BankAccount* a = new BankAccount("arceus", 10000); 
    BankAccount* b = new BankAccount("zelda", 5000);  
    a->transfermoney(b, 1000); 
    a->accountinfo(); 
    b->accountinfo(); 
    
    int accounts = BankAccount::getTotalAccounts(); 
    std::cout << "Total Number of accounts in our bank is: " << accounts << "\n";  
    
    a->withdraw(1000);
    a->accountinfo(); 
    
    delete a; 
    delete b; 
    return 0;
}
