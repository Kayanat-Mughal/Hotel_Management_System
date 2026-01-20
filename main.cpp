#include "Hotel.h"
#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>
#include <memory>
#include "Constants.h"
#include "Exceptions.h"
#include "Database.h"
#include "Hotel.h"
#include "Room.h"
#include "Customer.h"
#include "Reservation.h"
#include "Bill.h"
#include "Employee.h"

using namespace std;
namespace fs = std::filesystem;

// Function prototypes
void showWelcomeScreen();
void runSystemDiagnostics();
void createRequiredDirectories();
void checkSystemRequirements();
void handleCriticalError(const HotelException& e);

int main() {
    try {
        // Display welcome screen
        showWelcomeScreen();
        
        // Run system diagnostics
        runSystemDiagnostics();
        
        // Create required directories
        createRequiredDirectories();
        
        // Check system requirements
        checkSystemRequirements();
        
        // Initialize and run the hotel system
        std::cout << "\nInitializing Hotel Management System...\n";
        std::cout << "========================================\n";
        
        Hotel hotelSystem;
        
        std::cout << "\n✅ System initialized successfully!\n";
        std::cout << "📅 System Date: " << Utils::getCurrentDateTime() << "\n";
        std::cout << "💾 Data Directory: " << HotelConstants::DATA_DIR << "\n";
        std::cout << "📊 Log Directory: " << HotelConstants::LOG_DIR << "\n";
        
        Utils::pressEnterToContinue();
        
        // Run the main application
        hotelSystem.run();
        
        std::cout << "\n\n========================================\n";
        std::cout << "  Hotel Management System - Session Ended\n";
        std::cout << "========================================\n";
        std::cout << "Thank you for using " << HotelConstants::HOTEL_NAME << " System!\n";
        std::cout << "Session ended at: " << Utils::getCurrentDateTime() << "\n\n";
        
        return 0;
        
    } catch (const HotelException& e) {
        handleCriticalError(e);
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "\n⛔ UNEXPECTED ERROR: " << e.what() << std::endl;
        std::cerr << "Program terminated abnormally.\n";
        return 2;
    } catch (...) {
        std::cerr << "\n⛔ UNKNOWN FATAL ERROR!" << std::endl;
        std::cerr << "Program terminated abnormally.\n";
        return 3;
    }
}

void showWelcomeScreen() {
    Utils::clearScreen();
    
    std::cout << R"(
╔═══════════════════════════════════════════════════════════════╗
║                                                               ║
║      ██╗  ██╗ ██████╗ ████████╗███████╗██╗                  ║
║      ██║  ██║██╔═══██╗╚══██╔══╝██╔════╝██║                  ║
║      ███████║██║   ██║   ██║   █████╗  ██║                  ║
║      ██╔══██║██║   ██║   ██║   ██╔══╝  ██║                  ║
║      ██║  ██║╚██████╔╝   ██║   ███████╗███████╗             ║
║      ╚═╝  ╚═╝ ╚═════╝    ╚═╝   ╚══════╝╚══════╝             ║
║                                                               ║
║             MANAGEMENT SYSTEM v2.0                            ║
║                                                               ║
╚═══════════════════════════════════════════════════════════════╝
    )" << std::endl;
    
    std::cout << "\n" << HotelConstants::HOTEL_NAME << "\n";
    std::cout << HotelConstants::HOTEL_ADDRESS << "\n";
    std::cout << "Contact: " << HotelConstants::HOTEL_CONTACT << " | ";
    std::cout << "Email: " << HotelConstants::HOTEL_EMAIL << "\n";
    std::cout << "Website: " << HotelConstants::HOTEL_WEBSITE << "\n\n";
    
    std::cout << "─────────────────────────────────────────────────────\n";
    std::cout << "  A Comprehensive Hotel Management Solution\n";
    std::cout << "  Developed in C++ with Complete Error Handling\n";
    std::cout << "  Version: 2.0 | Build: " << __DATE__ << " " << __TIME__ << "\n";
    std::cout << "─────────────────────────────────────────────────────\n\n";
    
    // System check indicator
    std::cout << "Performing system checks...";
    std::cout.flush();
}

void runSystemDiagnostics() {
    // Simulate system diagnostics
    std::cout << " ✓\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    
    std::cout << "Checking file system...";
    std::cout.flush();
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    std::cout << " ✓\n";
    
    std::cout << "Loading configuration...";
    std::cout.flush();
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    std::cout << " ✓\n";
    
    std::cout << "Initializing database...";
    std::cout.flush();
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    std::cout << " ✓\n";
    
    std::cout << "Setting up security...";
    std::cout.flush();
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    std::cout << " ✓\n";
    
    std::cout << "\n✅ All systems operational!\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void createRequiredDirectories() {
    try {
        // Create data directory
        Utils::createDirectory(HotelConstants::DATA_DIR);
        
        // Create backup directory
        if (!Utils::fileExists(HotelConstants::BACKUP_DIR)) {
            Utils::createDirectory(HotelConstants::BACKUP_DIR);
        }
        
        // Create reports directory
        if (!Utils::fileExists(HotelConstants::REPORT_DIR)) {
            Utils::createDirectory(HotelConstants::REPORT_DIR);
        }
        
        // Create logs directory
        if (!Utils::fileExists(HotelConstants::LOG_DIR)) {
            Utils::createDirectory(HotelConstants::LOG_DIR);
        }
        
        // Create config directory
        if (!Utils::fileExists("config/")) {
            Utils::createDirectory("config/");
        }
        
    } catch (const std::exception& e) {
        // Log and handle
        FileException fe("Failed to create required directories");
        ExceptionHandler::handle(fe);
        // Or rethrow if needed
        throw;
    }
}                          

void checkSystemRequirements() {
    // Check C++ version (more lenient for MSVC which doesn't always report __cplusplus correctly)
    #if __cplusplus < 201703L && !defined(_MSC_VER)
        throw SystemException("Compiler", "C++17 or higher required");
    #endif
    
    // Check for sufficient disk space (simplified)
    std::cout << "\nSystem Requirements Check:\n";
    std::cout << "──────────────────────────\n";
    
    // Display system info
    #ifdef _WIN32
        std::cout << "OS: Windows\n";
    #elif __linux__
        std::cout << "OS: Linux\n";
    #elif __APPLE__
        std::cout << "OS: macOS\n";
    #else
        std::cout << "OS: Unknown\n";
    #endif
    
    std::cout << "C++ Version: " << __cplusplus << "\n";
    std::cout << "Compiler: ";
    
    #ifdef __GNUC__
        std::cout << "GCC " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__ << "\n";
    #elif _MSC_VER
        std::cout << "MSVC " << _MSC_VER << "\n";
    #elif __clang__
        std::cout << "Clang " << __clang_major__ << "." << __clang_minor__ << "." << __clang_patchlevel__ << "\n";
    #else
        std::cout << "Unknown\n";
    #endif
    
    std::cout << "Architecture: ";
    
    #ifdef _WIN64
        std::cout << "64-bit\n";
    #elif _WIN32
        std::cout << "32-bit\n";
    #elif __x86_64__
        std::cout << "64-bit\n";
    #elif __i386__
        std::cout << "32-bit\n";
    #else
        std::cout << "Unknown\n";
    #endif
    
    std::cout << "✅ All requirements met!\n";
}

void handleCriticalError(const HotelException& e) {
    Utils::clearScreen();
    
    std::cout << R"(
╔═══════════════════════════════════════════════════════════════╗
║                                                               ║
║                     ⚠️  CRITICAL ERROR                       ║
║                                                               ║
╚═══════════════════════════════════════════════════════════════╝
    )" << std::endl;
    
    string errorCode = e.getErrorCode();
    string timestamp = e.getTimestamp();
    cout << "Error [" << errorCode << "] at " << timestamp << ": " << e.what() << endl;
}