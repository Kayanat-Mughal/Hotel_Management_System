#ifndef DATABASE_H
#define DATABASE_H

#include "Room.h"
#include "Customer.h"
#include "Reservation.h"
#include "Employee.h"
#include "Bill.h"
#include <vector>
#include <map>
#include <fstream>
#include <memory>

class Database {
private:
    vector<Room> rooms;
    vector<Customer> customers;
    vector<Reservation> reservations;
    vector<Employee> employees;
    vector<Bill> bills;
    
    // ID counters
    int nextRoomNumber;
    int nextCustomerId;
    int nextReservationId;
    int nextEmployeeId;
    int nextBillId;
    
    // File names
    const string ROOMS_FILE = "data/rooms.dat";
    const string CUSTOMERS_FILE = "data/customers.dat";
    const string RESERVATIONS_FILE = "data/reservations.dat";
    const string EMPLOYEES_FILE = "data/employees.dat";
    const string BILLS_FILE = "data/bills.dat";
    
    // Helper methods
    void initializeSampleData();
    void createDataDirectory();
    
public:
    Database();
    ~Database();
    
    // ==================== ROOM OPERATIONS ====================
    int addRoom(RoomType type, double price, int capacity, const vector<string>& features = {});
    Room* findRoom(int roomNumber);
    std::vector<Room*> findAvailableRooms(RoomType type, int capacity);
    bool updateRoomStatus(int roomNumber, RoomStatus status);
    bool modifyRoom(int roomNumber, double newPrice, const vector<string>& newFeatures);
    void displayAllRooms() const;
    void displayAvailableRooms() const;
    int getRoomCount() const;
    int getAvailableRoomCount() const;
    
    // ==================== CUSTOMER OPERATIONS ====================
    int addCustomer(const string& name, const string& email, const string& phone,
                   const string& address, const string& idProof);
    Customer* findCustomer(int customerId);
    Customer* findCustomerByName(const string& name);
    vector<Customer*> findCustomersByPhone(const string& phone);
    void displayAllCustomers() const;
    bool updateCustomerInfo(int customerId, const string& phone,
                           const string& email, const string& address);
    int getCustomerCount() const;
    
    // ==================== RESERVATION OPERATIONS ====================
    int makeReservation(int customerId, int roomNumber, time_t checkIn,
                       time_t checkOut, int guests, const string& requests = "");
    Reservation* findReservation(int reservationId);
    vector<Reservation*> findReservationsByCustomer(int customerId);
    vector<Reservation*> findActiveReservations() const;
    vector<Reservation*> findTodayCheckIns() const;
    vector<Reservation*> findTodayCheckOuts() const;
    bool cancelReservation(int reservationId);
    bool checkIn(int reservationId);
    bool checkOut(int reservationId);
    int getReservationCount() const;
    int getActiveReservationCount() const;
    
    // ==================== EMPLOYEE OPERATIONS ====================
    int addEmployee(const string& name, const string& position, Department dept,
                   Shift shift, double salary, const string& contact,
                   const string& address, const string& joinDate);
    Employee* findEmployee(int employeeId);
    Employee* authenticateEmployee(const string& email, const string& password);
    vector<Employee*> findEmployeesByDepartment(Department dept);
    void displayAllEmployees() const;
    bool updateEmployeeInfo(int employeeId, const string& position,
                           Department dept, Shift shift, double salary);
    int getEmployeeCount() const;
    
    // ==================== BILLING OPERATIONS ====================
    int createBill(int reservationId, double taxRate = 0.10, double discount = 0.0);
    Bill* findBill(int billId);
    Bill* findBillByReservation(int reservationId);
    bool addBillItem(int billId, const string& description, double amount, int quantity = 1);
    bool processPayment(int billId, const string& paymentMethod);
    void displayAllBills() const;
    void displayUnpaidBills() const;
    void displayAllReservations() const;
    double calculateTotalRevenue() const;
    double calculateTodayRevenue() const;
    int getBillCount() const;
    
    // ==================== FILE OPERATIONS ====================
    bool saveAllData();
    bool loadAllData();
    bool backupData() const;
    bool restoreData(const string& backupDir);
    
    // ==================== STATISTICS ====================
    void showStatistics() const;
    map<string, double> getDailyRevenue(int days = 7) const;
    map<string, int> getOccupancyRate() const;
    vector<pair<string, int>> getPopularRooms() const;
    
    // ==================== SEARCH OPERATIONS ====================
    vector<Room*> searchRooms(double maxPrice, int minCapacity, RoomType type = STANDARD);
    vector<Customer*> searchCustomers(const string& keyword);
    vector<Reservation*> searchReservationsByDateRange(time_t start, time_t end);
    
private:
    // Internal file operations
    bool saveRooms();
    bool saveCustomers();
    bool saveReservations();
    bool saveEmployees();
    bool saveBills();
    
    bool loadRooms();
    bool loadCustomers();
    bool loadReservations();
    bool loadEmployees();
    bool loadBills();
};

#endif // DATABASE_H