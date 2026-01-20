#include "Room.h"
#include "Exceptions.h"
#include <iostream>
#include <cassert>
#include <vector>

using namespace std;

void testRoomCreation() {
    cout << "Testing Room Creation...\n";
    
    try {
        // Test valid room creation
        Room room1(101, STANDARD, 100.0, 2);
        assert(room1.getRoomNumber() == 101);
        assert(room1.getType() == STANDARD);
        assert(room1.getStatus() == AVAILABLE);
        assert(room1.getPricePerNight() == 100.0);
        assert(room1.getCapacity() == 2);
        cout << "✓ Basic room creation passed\n";
        
        // Test with features
        vector<string> features = {"WiFi", "TV", "AC"};
        Room room2(102, DELUXE, 200.0, 3, features);
        assert(room2.getFeatures().size() == 3);
        cout << "✓ Room with features passed\n";
        
        // Test invalid room number
        try {
            Room room3(-101, STANDARD, 100.0, 2);
            cout << "✗ Should have thrown exception for negative room number\n";
            assert(false);
        } catch (const ValidationException& e) {
            cout << "✓ Negative room number exception caught\n";
        }
        
        // Test invalid price
        try {
            Room room4(103, STANDARD, -50.0, 2);
            cout << "✗ Should have thrown exception for negative price\n";
            assert(false);
        } catch (const ValidationException& e) {
            cout << "✓ Negative price exception caught\n";
        }
        
        // Test invalid capacity
        try {
            Room room5(104, STANDARD, 100.0, 0);
            cout << "✗ Should have thrown exception for zero capacity\n";
            assert(false);
        } catch (const ValidationException& e) {
            cout << "✓ Zero capacity exception caught\n";
        }
        
    } catch (const exception& e) {
        cout << "✗ Unexpected exception: " << e.what() << "\n";
        assert(false);
    }
    
    cout << "Room Creation Tests: PASSED\n\n";
}

void testRoomOperations() {
    cout << "Testing Room Operations...\n";
    
    Room room(201, DELUXE, 200.0, 3);
    
    // Test availability
    assert(room.isAvailable() == true);
    cout << "✓ Initial availability check passed\n";
    
    // Test status change
    room.setStatus(OCCUPIED);
    assert(room.getStatus() == OCCUPIED);
    assert(room.isAvailable() == false);
    cout << "✓ Status change passed\n";
    
    // Test capacity check
    assert(room.canAccommodate(2) == true);
    assert(room.canAccommodate(3) == true);
    assert(room.canAccommodate(4) == false);
    cout << "✓ Capacity check passed\n";
    
    // Test price calculation
    room.setPrice(250.0);
    assert(room.getPricePerNight() == 250.0);
    assert(room.calculateStayCost(3) == 750.0);
    cout << "✓ Price calculation passed\n";
    
    // Test features
    room.addFeature("Mini-bar");
    room.addFeature("Jacuzzi");
    assert(room.getFeatures().size() == 2);
    cout << "✓ Features management passed\n";
    
    // Test display methods
    cout << "\nRoom Display Test:\n";
    room.displayBrief();
    cout << "✓ Display methods passed\n";
    
    cout << "Room Operations Tests: PASSED\n\n";
}

void testRoomFileOperations() {
    cout << "Testing Room File Operations...\n";
    
    try {
        // Create test room
        vector<string> features = {"WiFi", "TV", "AC", "Mini-fridge"};
        Room originalRoom(301, SUITE, 350.0, 4, features);
        
        // Save to file
        ofstream outFile("test_room.dat");
        originalRoom.saveToFile(outFile);
        outFile.close();
        cout << "✓ Room saved to file\n";
        
        // Load from file
        ifstream inFile("test_room.dat");
        Room loadedRoom;
        loadedRoom.loadFromFile(inFile);
        inFile.close();
        
        // Verify loaded data
        assert(loadedRoom.getRoomNumber() == 301);
        assert(loadedRoom.getType() == SUITE);
        assert(loadedRoom.getPricePerNight() == 350.0);
        assert(loadedRoom.getCapacity() == 4);
        assert(loadedRoom.getFeatures().size() == 4);
        cout << "✓ Room loaded from file\n";
        
        // Test invalid file
        try {
            ifstream badFile("nonexistent.dat");
            Room badRoom;
            badRoom.loadFromFile(badFile);
            cout << "✗ Should have thrown exception for non-existent file\n";
            assert(false);
        } catch (const FileException& e) {
            cout << "✓ File exception caught\n";
        }
        
        // Cleanup
        remove("test_room.dat");
        cout << "✓ Test file cleaned up\n";
        
    } catch (const exception& e) {
        cout << "✗ Unexpected exception: " << e.what() << "\n";
        assert(false);
    }
    
    cout << "Room File Operations Tests: PASSED\n\n";
}

void testRoomValidation() {
    cout << "Testing Room Validation...\n";
    
    // Test valid room
    Room validRoom(401, PRESIDENTIAL, 800.0, 2);
    assert(validRoom.isValid() == true);
    cout << "✓ Valid room validation passed\n";
    
    // Test various invalid scenarios
    try {
        // Invalid room number
        Room room1(0, STANDARD, 100.0, 2);
        assert(room1.isValid() == false);
    } catch (const ValidationException&) {
        cout << "✓ Zero room number validation caught\n";
    }
    
    try {
        // Invalid price
        Room room2(402, STANDARD, 0.0, 2);
        assert(room2.isValid() == false);
    } catch (const ValidationException&) {
        cout << "✓ Zero price validation caught\n";
    }
    
    try {
        // Invalid capacity
        Room room3(403, STANDARD, 100.0, 0);
        assert(room3.isValid() == false);
    } catch (const ValidationException&) {
        cout << "✓ Zero capacity validation caught\n";
    }
    
    cout << "Room Validation Tests: PASSED\n\n";
}

void testRoomTypeConversions() {
    cout << "Testing Room Type Conversions...\n";
    
    // Test string conversion
    Room standardRoom(101, STANDARD, 100.0, 2);
    assert(standardRoom.getTypeString() == "Standard");
    cout << "✓ Standard type conversion passed\n";
    
    Room deluxeRoom(201, DELUXE, 200.0, 3);
    assert(deluxeRoom.getTypeString() == "Deluxe");
    cout << "✓ Deluxe type conversion passed\n";
    
    Room suiteRoom(301, SUITE, 350.0, 4);
    assert(suiteRoom.getTypeString() == "Suite");
    cout << "✓ Suite type conversion passed\n";
    
    Room presidentialRoom(401, PRESIDENTIAL, 800.0, 2);
    assert(presidentialRoom.getTypeString() == "Presidential");
    cout << "✓ Presidential type conversion passed\n";
    
    // Test status conversion
    assert(standardRoom.getStatusString() == "Available");
    standardRoom.setStatus(OCCUPIED);
    assert(standardRoom.getStatusString() == "Occupied");
    standardRoom.setStatus(RESERVED);
    assert(standardRoom.getStatusString() == "Reserved");
    standardRoom.setStatus(MAINTENANCE);
    assert(standardRoom.getStatusString() == "Maintenance");
    cout << "✓ Status conversions passed\n";
    
    cout << "Room Type Conversion Tests: PASSED\n\n";
}

int main() {
    cout << "========================================\n";
    cout << "     ROOM CLASS UNIT TESTS\n";
    cout << "========================================\n\n";
    
    try {
        testRoomCreation();
        testRoomOperations();
        testRoomFileOperations();
        testRoomValidation();
        testRoomTypeConversions();
        
        cout << "========================================\n";
        cout << "     ALL TESTS PASSED SUCCESSFULLY!\n";
        cout << "========================================\n";
        return 0;
        
    } catch (const exception& e) {
        cout << "\n✗ TEST FAILED: " << e.what() << "\n";
        return 1;
    }
}