#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

// Function to read the content of a file into a string
std::string readFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Function to insert license content into a PHP file
void insertLicense(const std::string& phpFilePath, const std::string& licenseContent) {
    std::ifstream phpFile(phpFilePath);
    if (!phpFile.is_open()) {
        std::cerr << "Failed to open PHP file: " << phpFilePath << std::endl;
        return;
    }

    std::stringstream fileBuffer;
    std::string line;
    bool phpTagFound = false;
    bool licenseAlreadyExists = false;

    // Read the file and detect where to insert the license
    while (std::getline(phpFile, line)) {
        // Check if the license content already exists
        if (line.find(licenseContent) != std::string::npos) {
            licenseAlreadyExists = true;
            break;
        }

        fileBuffer << line << "\n";

        // Check for the first occurrence of "<?php"
        if (!phpTagFound && line.find("<?php") != std::string::npos) {
            phpTagFound = true;
            // Insert the license content with spaces above and below
            fileBuffer << "\n" << licenseContent << "\n";  // Insert license and add a new line for spacing
        }
    }

    phpFile.close();

    // If the license already exists, do not overwrite the file
    if (licenseAlreadyExists) {
        std::cout << "License already exists in file: " << phpFilePath << std::endl;
        return;
    }

    // Rewrite the file with the inserted license
    std::ofstream outFile(phpFilePath);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file for writing: " << phpFilePath << std::endl;
        return;
    }
    outFile << fileBuffer.str();
    outFile.close();
}

// Function to process license content (removes <?php if present)
std::string processLicenseContent(const std::string& licenseContent) {
    std::string processedContent = licenseContent;
    // Remove <?php from the beginning of the licenseContent if it exists
    if (processedContent.find("<?php") == 0) {
        processedContent.erase(0, 5); // Erase the first 5 characters
    }
    return processedContent;
}

// Function to recursively process all PHP files in the given directory
void processDirectory(const fs::path& directory, const std::string& licenseContent) {
    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".php") {
            std::cout << "Processing file: " << entry.path() << std::endl;
            insertLicense(entry.path().string(), licenseContent);
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <directory_path> <license_file_path>" << std::endl;
        return 1;
    }

    std::string directoryPath = argv[1];
    std::string licenseFilePath = argv[2];

    // Read the license content
    std::string licenseContent = readFile(licenseFilePath);
    if (licenseContent.empty()) {
        return 1;
    }

    // Process the license content to remove <?php
    licenseContent = processLicenseContent(licenseContent);

    // Remove any trailing newlines from license content to avoid double-spacing
    if (!licenseContent.empty() && licenseContent.back() == '\n') {
        licenseContent.pop_back();
    }

    // Ensure the license content has proper formatting (single blank line before and after)
    licenseContent = "\n" + licenseContent + "\n";

    // Process the directory
    processDirectory(directoryPath, licenseContent);

    std::cout << "License insertion completed." << std::endl;
    return 0;
}
