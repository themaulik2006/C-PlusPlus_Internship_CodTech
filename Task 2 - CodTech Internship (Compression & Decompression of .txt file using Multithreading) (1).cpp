#include <iostream>
#include <fstream>
#include <thread>
#include <string>
using namespace std;

// Global strings to store compressed parts from two threads
string part1 = "", part2 = "";

// RLE Compression Function
void compress(const string& input, string& output) {
    int count;
    for (int i = 0; i < input.length(); i++) {
        count = 1;
        while (i < input.length() - 1 && input[i] == input[i + 1]) {
            count++;
            i++;
        }
        output += to_string(count) + input[i];
    }
}

// Decompression Function
void decompressFile(const string& inputFile, const string& outputFile) {
    ifstream inFile(inputFile);
    ofstream outFile(outputFile);

    if (!inFile.is_open()) {
        cerr << "Error: Unable to open input file: " << inputFile << endl;
        return;
    }

    if (!outFile.is_open()) {
        cerr << "Error: Unable to create output file: " << outputFile << endl;
        inFile.close();
        return;
    }

    char ch;
    string number = "";

    while (inFile.get(ch)) {
        if (isdigit(ch)) {
            number += ch;
        } else {
            try {
                int count = stoi(number);
                outFile << string(count, ch);  // Repeat the character 'count' times
                number.clear();
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

// Multithreaded Compression Function
void compressFile(const string& inputFile, const string& outputFile) {
    ifstream inFile(inputFile, ios::in | ios::binary);
    if (!inFile.is_open()) {
        cerr << "Error: Could not open input file: " << inputFile << endl;
        return;
    }

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

    int mid = data.length() / 2;
    while (mid < data.length() && data[mid] == data[mid - 1]) {
        mid++;  // Prevent splitting repeated sequence
    }

    string firstHalf = data.substr(0, mid);
    string secondHalf = data.substr(mid);

    thread t1(compress, cref(firstHalf), ref(part1));
    thread t2(compress, cref(secondHalf), ref(part2));

    t1.join();
    t2.join();

    ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        cerr << "Error: Could not create output file: " << outputFile << endl;
        return;
    }

    outFile << part1 << part2;
    outFile.close();

    cout << "Compression complete using multithreading!" << endl;
}

int main() {
    int choice;
    string inputFile, outputFile;

    do {
        cout << "\n=== FILE COMPRESSION MENU ===\n";
        cout << "1. Compress File\n";
        cout << "2. Decompress File\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();  // Clear leftover newline

        switch (choice) {
            case 1:
                cout << "Enter input filename (to compress): ";
                getline(cin, inputFile);
                cout << "Enter output filename (compressed file): ";
                getline(cin, outputFile);

                part1.clear();
                part2.clear();

                compressFile(inputFile, outputFile);
                break;

            case 2:
                cout << "Enter compressed filename: ";
                getline(cin, inputFile);
                cout << "Enter output filename (after decompression): ";
                getline(cin, outputFile);

                decompressFile(inputFile, outputFile);
                break;

            case 3:
                cout << "Exiting program. Goodbye!" << endl;
                break;

            default:
                cout << "Invalid choice. Please enter 1, 2, or 3.\n";
        }

    } while (choice != 3);

    return 0;
}
