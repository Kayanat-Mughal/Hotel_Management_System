#ifndef HOTEL_H
#define HOTEL_H

#include "Database.h"
#include <string>

class Hotel {
private:
    Database database;
    string hotelName;
    string hotelAddress;
    string hotelContact;
    string hotelEmail;
    
    // Current logged in employee
    Employee* currentEmployee;
    
    // Main menu functions
    void showMainMenu();
    void showDashboard();
    
    // Management modules
    void manageRooms();
    void manageCustomers();
    void manageReservations();
    void manageEmployees();
    void manageBilling();
    void showReports();
    void systemSettings();
    
    // Quick operations
    void quickCheckIn();
    void quickCheckOut();
    void quickPayment();
    
    // Helper functions for each module
    void addNewRoom();
    void searchRoom();
    void updateRoomStatus();
    void modifyRoomDetails();
    
    void addNewCustomer();
    void searchCustomer();
    void updateCustomerInfo();
    void viewCustomerHistory();
    
    void makeNewReservation();
    void viewReservation();
    void modifyReservation();
    void cancelReservation();
    void processCheckIn();
    void processCheckOut();
    
    void addNewEmployee();
    void searchEmployee();
    void updateEmployeeInfo();
    void changeEmployeePassword();
    
    void createNewBill();
    void viewBill();
    void addBillItems();
    void processBillPayment();
    
    void generateRevenueReport();
    void generateOccupancyReport();
    void generateCustomerReport();
    
    // Utility functions
    void login();
    void logout();
    bool isAdmin() const;
    bool isManager() const;
    void saveAndExit();
    
public:
    Hotel();
    ~Hotel();
    
    void run();
    
    // Hotel information
    string getName() const;
    string getAddress() const;
    string getContact() const;
    string getEmail() const;
    void displayHotelInfo() const;
    
    // Current user
    string getCurrentUser() const;
    bool isLoggedIn() const;
};

#endif // HOTEL_H