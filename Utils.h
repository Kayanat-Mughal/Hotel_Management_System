#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include "Exceptions.h"

using namespace std;

// ==================== ENUMS ====================
enum RoomType {
    STANDARD = 1,
    DELUXE = 2,
    SUITE = 3,
    PRESIDENTIAL = 4
};

enum Shift {
    MORNING = 1,
    AFTERNOON = 2,
    NIGHT = 3
};

enum RoomStatus {
    AVAILABLE = 1,
    OCCUPIED = 2,
    RESERVED = 3,
    MAINTENANCE = 4
};

enum ReservationStatus {
    CONFIRMED = 1,
    CHECKED_IN = 2,
    CHECKED_OUT = 3,
    CANCELLED = 4
};

enum PaymentStatus {
    PENDING = 1,
    PAID = 2,
    PARTIAL = 3
};

enum Department {
    FRONT_DESK = 1,
    HOUSEKEEPING = 2,
    KITCHEN = 3,
    MANAGEMENT = 4
};

// ==================== UTILITY FUNCTIONS ====================
class Utils {
public:
    // Input validation
    static int getIntegerInput(const string& prompt, int min = -2147483647, int max = 2147483647);
    static double getDoubleInput(const string& prompt, double min = 0.0);
    static string getStringInput(const string& prompt, bool allowEmpty = false);
    static bool getYesNoInput(const string& prompt);
    
    // Date and time
    static string getCurrentDate();
    static string getCurrentDateTime();
    static time_t parseDate(const string& dateStr);
    static string formatDate(time_t time);
    static bool isValidDate(const string& dateStr);
    static int daysBetween(time_t start, time_t end);
    
    // String operations
    static string toUpper(const string& str);
    static string toLower(const string& str);
    static string trim(const string& str);
    
    // Validation
    static bool isValidEmail(const string& email);
    static bool isValidPhone(const string& phone);
    
    // File operations
    static bool fileExists(const string& filename);
    static void createDirectory(const string& dirname);
    
    // Display
    static void clearScreen();
    static void displayHeader(const string& title);
    static void displayMenu(const vector<string>& options);
    static void showMessage(const string& message, char type = 'i');
    
    // Formatting
    static string formatCurrency(double amount);
    static string centerText(const string& text, int width);
    
    // Wait for user
    static void pressEnterToContinue();
    
    // Random
    static int generateId();
    
    // Conversion
    static string roomTypeToString(RoomType type);
    static string roomStatusToString(RoomStatus status);
    static RoomType intToRoomType(int value);
};

#endif // UTILS_H