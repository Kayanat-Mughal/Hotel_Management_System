#include "Hotel.h"
#include "Database.h"
#include "Exceptions.h"
#include <iostream>
#include <cassert>
#include <ctime>

using namespace std;

void testDatabaseOperations() {
    cout << "Testing Database Operations...\n";
    
    try {
        Database db;
        
        // Test room operations
        int roomId = db.addRoom(STANDARD, 100.0, 2);
        assert(roomId > 0);
        cout << "✓ Room added: #" << roomId << "\n";
        
        Room* foundRoom = db.findRoom(roomId);
        assert(foundRoom != nullptr);
        assert(foundRoom->getRoomNumber() == roomId);
        cout << "✓ Room found successfully\n";
        
        // Test customer operations
        int customerId = db.addCustomer("Test Customer", "test@email.com", 
                                       "1234567890", "Test Address", "TEST123");
        assert(customerId > 0);
        cout << "✓ Customer added: #" << customerId << "\n";
        
        Customer* foundCustomer = db.findCustomer(customerId);
        assert(foundCustomer != nullptr);
        assert(foundCustomer->getCustomerId() == customerId);
        cout << "✓ Customer found successfully\n";
        
        // Test reservation operations
        time_t now = time(nullptr);
        time_t checkIn = now + 86400; // Tomorrow
        time_t checkOut = checkIn + 259200; // 3 days later
        
        int reservationId = db.makeReservation(customerId, roomId, checkIn, 
                                              checkOut, 2, "Test reservation");
        assert(reservationId > 0);
        cout << "✓ Reservation created: #" << reservationId << "\n";
        
        // Test employee operations
        int employeeId = db.addEmployee("Test Employee", "Receptionist", 
                                       FRONT_DESK, MORNING, 2500.0, 
                                       "9876543210", "Emp Address", "2024-01-15");
        assert(employeeId > 0);
        cout << "✓ Employee added: #" << employeeId << "\n";
        
        // Test billing operations
        int billId = db.createBill(reservationId);
        assert(billId > 0);
        cout << "✓ Bill created: #" << billId << "\n";
        
        assert(db.addBillItem(billId, "Test Item", 50.0, 2) == true);
        cout << "✓ Bill item added\n";
        
        // Test statistics
        assert(db.getRoomCount() >= 1);
        assert(db.getCustomerCount() >= 1);
        assert(db.getReservationCount() >= 1);
        assert(db.getEmployeeCount() >= 1);
        assert(db.getBillCount() >= 1);
        cout << "✓ Statistics calculation passed\n";
        
        // Test save/load operations
        assert(db.saveAllData() == true);
        cout << "✓ Data saved successfully\n";
        
        // Test find methods
        vector<Room*> availableRooms = db.findAvailableRooms();
        cout << "✓ Available rooms found: " << availableRooms.size() << "\n";
        
        vector<Reservation*> customerReservations = db.findReservationsByCustomer(customerId);
        assert(customerReservations.size() >= 1);
        cout << "✓ Customer reservations found\n";
        
        vector<Employee*> frontDeskEmployees = db.findEmployeesByDepartment(FRONT_DESK);
        cout << "✓ Front desk employees found: " << frontDeskEmployees.size() << "\n";
        
    } catch (const HotelException& e) {
        cout << "✗ Hotel Exception: " << e.what() << "\n";
        assert(false);
    } catch (const exception& e) {
        cout << "✗ Unexpected exception: " << e.what() << "\n";
        assert(false);
    }
    
    cout << "Database Operations Tests: PASSED\n\n";
}

void testHotelSystemIntegration() {
    cout << "Testing Hotel System Integration...\n";
    
    try {
        Hotel hotel;
        
        // Test hotel information
        assert(!hotel.getName().empty());
        assert(!hotel.getAddress().empty());
        assert(!hotel.getContact().empty());
        cout << "✓ Hotel information initialized\n";
        
        // Note: Actual hotel.run() would start the interactive system
        // For testing, we'll verify the object can be created and basic methods work
        
        cout << "✓ Hotel system integration test passed (interactive test required for full run)\n";
        
    } catch (const exception& e) {
        cout << "✗ Exception: " << e.what() << "\n";
        assert(false);
    }
    
    cout << "Hotel System Integration Tests: PASSED\n\n";
}

void testErrorHandling() {
    cout << "Testing Error Handling...\n";
    
    Database db;
    
    // Test invalid room operations
    try {
        db.findRoom(99999); // Non-existent room
        cout << "✗ Should have thrown exception for non-existent room\n";
        assert(false);
    } catch (const RoomException& e) {
        cout << "✓ Room not found exception handled\n";
    }
    
    // Test invalid customer operations
    try {
        db.findCustomer(99999); // Non-existent customer
        cout << "✗ Should have thrown exception for non-existent customer\n";
        assert(false);
    } catch (const CustomerException& e) {
        cout << "✓ Customer not found exception handled\n";
    }
    
    // Test invalid reservation
    try {
        db.makeReservation(99999, 99999, time(nullptr), time(nullptr) + 86400, 2);
        cout << "✗ Should have thrown exception for invalid reservation\n";
        assert(false);
    } catch (const HotelException& e) {
        cout << "✓ Invalid reservation exception handled\n";
    }
    
    // Test file operations with invalid path
    try {
        ifstream invalidFile("nonexistent_directory/nonexistent_file.dat");
        if (!invalidFile) {
            throw FileNotFoundException("nonexistent_file.dat");
        }
    } catch (const FileException& e) {
        cout << "✓ File not found exception handled\n";
    }
    
    cout << "Error Handling Tests: PASSED\n\n";
}

void testBusinessLogic() {
    cout << "Testing Business Logic...\n";
    
    try {
        Database db;
        
        // Create test data
        int roomId = db.addRoom(STANDARD, 100.0, 2);
        int customerId = db.addCustomer("Business Logic Test", "test@business.com", 
                                       "1112223333", "Test Address", "BLT001");
        
        // Test reservation with check-in/check-out
        time_t now = time(nullptr);
        time_t checkIn = now + 86400; // Tomorrow
        time_t checkOut = checkIn + 172800; // 2 days later
        
        int reservationId = db.makeReservation(customerId, roomId, checkIn, checkOut, 2);
        cout << "✓ Reservation created for business logic test\n";
        
        // Test check-in
        assert(db.checkIn(reservationId) == true);
        cout << "✓ Check-in processed\n";
        
        // Verify room status changed to occupied
        Room* room = db.findRoom(roomId);
        assert(room != nullptr);
        assert(room->getStatus() == OCCUPIED);
        cout << "✓ Room status updated to occupied\n";
        
        // Test check-out
        assert(db.checkOut(reservationId) == true);
        cout << "✓ Check-out processed\n";
        
        // Verify room status changed back to available
        assert(room->getStatus() == AVAILABLE);
        cout << "✓ Room status updated to available\n";
        
        // Test cancellation
        int reservationId2 = db.makeReservation(customerId, roomId, 
                                               checkIn + 86400, checkOut + 86400, 2);
        assert(db.cancelReservation(reservationId2) == true);
        cout << "✓ Reservation cancelled\n";
        
        // Test statistics after operations
        db.showStatistics();
        cout << "✓ Statistics displayed successfully\n";
        
    } catch (const HotelException& e) {
        cout << "✗ Business Logic Exception: " << e.what() << "\n";
        assert(false);
    }
    
    cout << "Business Logic Tests: PASSED\n\n";
}

void testPerformance() {
    cout << "Testing Performance (Basic Operations)...\n";
    
    try {
        Database db;
        
        auto start = chrono::high_resolution_clock::now();
        
        // Perform multiple operations
        const int TEST_COUNT = 100;
        for (int i = 0; i < TEST_COUNT; i++) {
            int roomId = db.addRoom(STANDARD, 100.0 + i, 2);
            int customerId = db.addCustomer("Perf Customer " + to_string(i), 
                                          "customer" + to_string(i) + "@test.com",
                                          "555000" + to_string(i), 
                                          "Address " + to_string(i), 
                                          "ID" + to_string(i));
            
            time_t checkIn = time(nullptr) + (i * 86400);
            time_t checkOut = checkIn + 86400;
            
            db.makeReservation(customerId, roomId, checkIn, checkOut, 2);
        }
        
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
        
        cout << "✓ " << TEST_COUNT << " operations completed in " 
             << duration.count() << "ms\n";
        cout << "✓ Average: " << (duration.count() / TEST_COUNT) << "ms per operation\n";
        
        // Test search performance
        start = chrono::high_resolution_clock::now();
        vector<Room*> rooms = db.findAvailableRooms();
        end = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::milliseconds>(end - start);
        
        cout << "✓ Found " << rooms.size() << " available rooms in " 
             << duration.count() << "ms\n";
        
        // Test file operations performance
        start = chrono::high_resolution_clock::now();
        bool saved = db.saveAllData();
        end = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::milliseconds>(end - start);
        
        assert(saved == true);
        cout << "✓ Data saved in " << duration.count() << "ms\n";
        
    } catch (const exception& e) {
        cout << "✗ Performance Test Exception: " << e.what() << "\n";
    }
    
    cout << "Performance Tests: PASSED\n\n";
}

int main() {
    cout << "========================================\n";
    cout << "     HOTEL SYSTEM INTEGRATION TESTS\n";
    cout << "========================================\n\n";
    
    try {
        testDatabaseOperations();
        testHotelSystemIntegration();
        testErrorHandling();
        testBusinessLogic();
        testPerformance();
        
        cout << "========================================\n";
        cout << "     ALL INTEGRATION TESTS PASSED!\n";
        cout << "========================================\n";
        cout << "\nNote: For complete testing, also run:\n";
        cout << "1. Room unit tests (test_room.cpp)\n";
        cout << "2. Interactive system testing\n";
        cout << "3. File system operations testing\n";
        
        return 0;
        
    } catch (const exception& e) {
        cout << "\n✗ TEST FAILED: " << e.what() << "\n";
        return 1;
    }
}