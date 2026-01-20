#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "Utils.h"

class Employee {
private:
    int employeeId;
    string name;
    string position;
    Department department;
    Shift shift;
    double salary;
    string contactNumber;
    string address;
    string joinDate;
    string email;
    string password;  // In real app, store hashed password
    
public:
    // Constructors
    Employee();
    Employee(int id, const string& n, const string& pos, Department dept,
             Shift sh, double sal, const string& contact, 
             const string& addr, const string& joinDt);
    
    // Getters
    int getEmployeeId() const;
    string getName() const;
    string getPosition() const;
    Department getDepartment() const;
    Shift getShift() const;
    double getSalary() const;
    string getContactNumber() const;
    string getAddress() const;
    string getJoinDate() const;
    string getEmail() const;
    
    // Department and Shift strings
    string getDepartmentString() const;
    string getShiftString() const;
    
    // Setters
    void setName(const string& newName);
    void setPosition(const string& newPosition);
    void setDepartment(Department newDept);
    void setShift(Shift newShift);
    void setSalary(double newSalary);
    void setContactNumber(const string& newContact);
    void setAddress(const string& newAddress);
    void setEmail(const string& newEmail);
    bool setPassword(const string& newPassword, const string& confirmPassword);
    
    // Authentication
    bool authenticate(const string& inputPassword) const;
    
    // Operations
    double calculateMonthlySalary() const;
    bool isManager() const;
    
    // Display
    void display() const;
    void displayBrief() const;
    
    // File operations
    void saveToFile(ofstream& file) const;
    void loadFromFile(ifstream& file);
    
    // Validation
    bool isValid() const;
};

#endif // EMPLOYEE_H