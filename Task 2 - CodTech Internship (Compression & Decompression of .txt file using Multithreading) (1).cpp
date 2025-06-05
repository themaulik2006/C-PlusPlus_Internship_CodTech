#include <iostream>
#include <fstream>
#include <thread>
#include <string>
using namespace std;

// Global strings to store compressed parts from two threads
string part1 = "", part2 = "";

// -------------------------
// RLE Compression Function
// -------------------------
// This function compresses a string using Run Length Encoding (RLE).
// Repeated characters are stored as <count><character> (e.g., AAA -> 3A)
void compress(const string& input, string& output) {
    int count;
    for (int i = 0; i < input.length(); i++) {
        count = 1;
        // Count the number of times the same character repeats
        while (i < input.length() - 1 && input[i] == input[i + 1]) {
            count++;
            i++;
        }
        // Append the count and character to the output string
        output += to_string(count) + input[i];
    }
}

// ---------------------------
// RLE Decompression Function
// ---------------------------
// This function reads a compressed file and reconstructs the original text
void decompressFile(const string& inputFile, const string& outputFile) {
    ifstream inFile(inputFile);
    ofstream outFile(outputFile);

    // Check if input file can be opened
    if (!inFile.is_open()) {
        cerr << "Error: Unable to open input file: " << inputFile << endl;
        return;
    }

    // Check if output file can be created
    if (!outFile.is_open()) {
        cerr << "Error: Unable to create output file: " << outputFile << endl;
        inFile.close();
        return;
    }

    char ch;
    string number = "";

    // Read characters one by one
    while (inFile.get(ch)) {
        if (isdigit(ch)) {
            // Build the full number (multi-digit count)
            number += ch;
        } else {
            try {
                // Convert number string to integer and repeat character
                int count = stoi(number);
                outFile << string(count, ch);  // Write character 'count' times
                number.clear();  // Reset number string for next sequence
            } catch (...) {
                cerr << "Error: Invalid format in compressed file.\n";
                break;
            }
        }
    }

    inFile.close();
    outFile.close();
    cout << "Decompression complete!" << endl;
}

// ----------------------------------------
// Compression Function using Multithreading
// ----------------------------------------
// This function reads the input file, splits it into 2 parts,
// compresses them in parallel using threads, and stores output.
void compressFile(const string& inputFile, const string& outputFile) {
    ifstream inFile(inputFile, ios::in | ios::binary);
    if (!inFile.is_open()) {
        cerr << "Error: Could not open input file: " << inputFile << endl;
        return;
    }

    // Read the entire file content into 'data' string
    string data;
    char ch;
    while (inFile.get(ch)) {
        data += ch;
    }
    inFile.close();

    if (data.empty()) {
        cerr << "Error: Input file is empty.\n";
        return;
    }

    // Determine midpoint for dividing the string
    int mid = data.length() / 2;

    // Adjust split point to avoid breaking repeated character sequence
    while (mid < data.length() && data[mid] == data[mid - 1]) {
        mid++;
    }

    // Split the data into two parts
    string firstHalf = data.substr(0, mid);
    string secondHalf = data.substr(mid);

    // Launch two threads to compress both parts
    thread t1(compress, cref(firstHalf), ref(part1));
    thread t2(compress, cref(secondHalf), ref(part2));

    // Wait for both threads to finish
    t1.join();
    t2.join();

    // Combine both compressed parts and write to output file
    ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        cerr << "Error: Could not create output file: " << outputFile << endl;
        return;
    }

    outFile << part1 << part2;
    outFile.close();

    cout << "Compression complete using multithreading!" << endl;
}

// ----------------
// Main Menu Driver
// ----------------
int main() {
    int choice;
    string inputFile, outputFile;

    do {
        // Display menu
        cout << "\n=== FILE COMPRESSION MENU ===\n";
        cout << "1. Compress File\n";
        cout << "2. Decompress File\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();  // Ignore leftover newline from input buffer

        switch (choice) {
            case 1:
                // Compression option
                cout << "Enter input filename (to compress): ";
                getline(cin, inputFile);
                cout << "Enter output filename (compressed file): ";
                getline(cin, outputFile);

                // Clear previous results
                part1.clear();
                part2.clear();

                // Perform compression
                compressFile(inputFile, outputFile);
                break;

            case 2:
                // Decompression option
                cout << "Enter compressed filename: ";
                getline(cin, inputFile);
                cout << "Enter output filename (after decompression): ";
                getline(cin, outputFile);

                // Perform decompression
                decompressFile(inputFile, outputFile);
                break;

            case 3:
                // Exit option
                cout << "Exiting program. Goodbye!" << endl;
                break;

            default:
                cout << "Invalid choice. Please enter 1, 2, or 3.\n";
        }

    } while (choice != 3);  // Repeat until user chooses to exit

    return 0;
}
