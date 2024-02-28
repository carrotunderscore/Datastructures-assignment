#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <chrono>

using namespace std;

class BankAccount {
    string accountNumber;
    float balance;
public:
    BankAccount() {
    }

    BankAccount(string accountNumber, float balance = 0)
        :accountNumber(accountNumber), balance(balance)
    {
    }

    string getAccountNumber()
    {
        return this->accountNumber;
    }
};

//INTERFACE - gränssnitt "standard"
class IAccountStorage {
public:
    virtual void addAccount(BankAccount account) = 0;
    virtual BankAccount* findAccount(string accountNumber) = 0;
};

class Bank
{
private:
    IAccountStorage* accountStorage;
public:
    Bank(IAccountStorage* storage) :accountStorage(storage) {

    }
    bool addAccount(string accountNumber) {
        //validate
        //if something (accountNumber) return false
        accountStorage->addAccount(accountNumber);
        return true;
    }
    BankAccount* getAccount(string accountNumber) {
        return accountStorage->findAccount(accountNumber);
    }
};

class MapAccountStorage : public IAccountStorage {
    map<string, BankAccount> accounts;
public:
    void addAccount(BankAccount account) override {
        accounts[account.getAccountNumber()] = account;
    }
    BankAccount* findAccount(string accountNumber) {
        return &(accounts[accountNumber]);
    }

};

class DistributedVectorAccountStorage : public IAccountStorage {
    vector<BankAccount> accountsStartingWith0;
    vector<BankAccount> accountsStartingWith1;
    vector<BankAccount> accountsStartingWith2;
    vector<BankAccount> accountsStartingWith3;
    vector<BankAccount> accountsStartingWith4;
    vector<BankAccount> accountsStartingWith5;
    vector<BankAccount> accountsStartingWith6;
    vector<BankAccount> accountsStartingWith7;
    vector<BankAccount> accountsStartingWith8;
    vector<BankAccount> accountsStartingWith9;
    vector<BankAccount> empty;

    vector<BankAccount>& getRef(char firstChar) {

        switch (firstChar) {
        case '0':
            return accountsStartingWith0;
        case '1':
            return  accountsStartingWith1;
        case '2':
            return accountsStartingWith2;
        case '3':
            return accountsStartingWith3;
        case '4':
            return accountsStartingWith4;
        case '5':
            return  accountsStartingWith5;
        case '6':
            return accountsStartingWith6;
        case '7':
            return accountsStartingWith7;
        case '8':
            return accountsStartingWith8;
        case '9':
            return  accountsStartingWith9;
        }
        return empty;
    }   


public:

    void addAccount(BankAccount account) override {
        vector<BankAccount>& ref = getRef(account.getAccountNumber()[0]);
        ref.push_back(account);
    }
    BankAccount* findAccount(string accountNumber) {
        vector<BankAccount>& ref = getRef(accountNumber[0]);
        BankAccount* ret = nullptr;
        for (BankAccount& account : ref) {
            if (account.getAccountNumber() == accountNumber) {
                return &account;
            }
        }
        return ret;
    }
};

class VectorAccountStorage : public IAccountStorage {
    vector<BankAccount> accounts;
public:
    void addAccount(BankAccount account) override {
        accounts.push_back(account);
    }

    BankAccount* findAccount(string accountNumber) {
        BankAccount* ret = nullptr;
        for (BankAccount& account : accounts) {
            if (account.getAccountNumber() == accountNumber) {
                return &account;
            }
        }
        return ret;
    }


};

int main(int, char**) {
    //VectorAccountStorage storage;
    DistributedVectorAccountStorage storage;
    //MapAccountStorage storage;
    Bank bank(&storage);

    const int numberOfAccounts = 1000000;

    string sFirst = "";
    string sLast = "";
    string sNotFound = "notfound";


    cout << "INITIALIZE: " << endl;
    auto startTime = chrono::high_resolution_clock::now();
    for (int i = 0; i < numberOfAccounts; i++) {

        string accountNumber = "0000000000";
        auto startAccountNumberString = accountNumber.end() - to_string(i).size();
        auto endAccountNumberString = accountNumber.end();
        accountNumber.replace(startAccountNumberString, endAccountNumberString, to_string(i));

        if (i == 0) {
            sFirst = accountNumber;
        }
        if (i == numberOfAccounts - 1) {
            sLast = accountNumber;
        }
        bank.addAccount(accountNumber);
    }

    auto endTime = chrono::high_resolution_clock::now();
    cout << "INIT Took: " << chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count() << " milliseconds" << endl;

    startTime = chrono::high_resolution_clock::now();
    BankAccount* p = bank.getAccount(sFirst);
    endTime = chrono::high_resolution_clock::now();
    cout << p->getAccountNumber() << " took: " << chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count() << " milliseconds" << endl;

    startTime = chrono::high_resolution_clock::now();
    p = bank.getAccount(sLast);
    endTime = chrono::high_resolution_clock::now();
    cout << p->getAccountNumber() << " took: " << chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count() << " milliseconds" << endl;

    startTime = chrono::high_resolution_clock::now();
    p = bank.getAccount(sNotFound);
    endTime = chrono::high_resolution_clock::now();
    cout << "NOT FOUND" << " took: " << chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count() << " milliseconds" << endl;
}
