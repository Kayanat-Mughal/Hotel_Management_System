#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "Utils.h"

class Customer {
private:
    int customerId;
    string name;
    string email;
    string phone;
    string address;
    string idProof;
    time_t registrationDate;
    int totalVisits;
    double totalSpent;
    
public:
    // Constructors
    Customer();
    Customer(int id, const string& n, const string& e, const string& p, 
             const string& addr, const string& proof);
    
    // Getters
    int getCustomerId() const;
    string getName() const;
    string getEmail() const;
    string getPhone() const;
    string getAddress() const;
    string getIdProof() const;
    string getRegistrationDate() const;
    int getTotalVisits() const;
    double getTotalSpent() const;
    
    // Setters
    void setName(const string& newName);
    void setEmail(const string& newEmail);
    void setPhone(const string& newPhone);
    void setAddress(const string& newAddress);
    void setIdProof(const string& newProof);
    
    // Operations
    void addVisit(double amount);
    void updateInfo(const string& newPhone, const string& newEmail, 
                    const string& newAddress);
    
    // Display
    void display() const;
    void displayBrief() const;
    
    // File operations
    void saveToFile(ofstream& file) const;
    void loadFromFile(ifstream& file);
    
    // Validation
    bool isValid() const;
};

#endif // CUSTOMER_H