#include <iostream>
#include <string>
using namespace std;

// Function to convert a character to an 8-bit binary string
string charToBinary(char c) {
    string binary = "";
    for (int i = 7; i >= 0; i--) {
        binary += ((c >> i) & 1) ? '1' : '0';
    }
    return binary;
}

// Function to convert an 8-letter word into a 64-bit binary string
string wordToBinary(const string& word) {
    string result = "";
    for (char c : word) {
        result += charToBinary(c); 
    }
    return result;
}

// Parity Drop to convert 64-bit key to 56-bit
string parityDrop(string key) {
    string par = "";
    for (int i = 0; i < 64; i++) {
        if (i != 0 && i % 7 == 0) {
            continue;
        } else {
            par += key[i];
        }
    }
    return par;
}

// Function to perform circular left shifts
string leftShift(string s, int shifts) {
    return s.substr(shifts) + s.substr(0, shifts);
}

// Function to expand 32-bit half to 48-bit using Expansion P-Box
string PBox(string r) {
    int expTable[48] = {
        32, 1, 2, 3, 4, 5, 4, 5,
        6, 7, 8, 9, 8, 9, 10, 11,
        12, 13, 12, 13, 14, 15, 16, 17,
        16, 17, 18, 19, 20, 21, 20, 21,
        22, 23, 24, 25, 24, 25, 26, 27,
        28, 29, 28, 29, 30, 31, 32, 1
    };
    string e = "";
    for (int i = 0; i < 48; i++) {
        e += r[expTable[i] - 1];
    }
    return e;
}

// XOR operation
string XOR(string a, string b) {
    string result = "";
    for (int i = 0; i < a.size(); i++) {
        result += (a[i] == b[i]) ? '0' : '1';
    }
    return result;
}

// S-Box Compression from 48-bit to 32-bit
string SBox(string input) {
    int sBox[4][16] = {
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
    };
    
    string output = "";
    for (int i = 0; i < 48; i += 6) {
        int row = (input[i] - '0') * 2 + (input[i + 5] - '0');
        int col = (input[i + 1] - '0') * 8 + (input[i + 2] - '0') * 4 + (input[i + 3] - '0') * 2 + (input[i + 4] - '0');
        int val = sBox[row][col];
        for (int j = 3; j >= 0; j--) {
            output += ((val >> j) & 1) ? '1' : '0';
        }
    }
    return output;
}

// DES Round Function
string DES_Round(string left, string right, string roundKey) {
    string eRight = PBox(right);
    cout << "Expanded Right Half: " << eRight << endl;
    
    string xorResult = XOR(eRight, roundKey);
    cout << "XOR with Round Key: " << xorResult << endl;
    
    string compressed = SBox(xorResult);
    cout << "S-Box Compressed: " << compressed << endl;
    
    string newRight = XOR(left, compressed);
    cout << "XOR with Left Half: " << newRight << endl;
    
    return newRight;
}

// DES Encryption Process
void DES_Encrypt(string plaintext, string key) {
    string key56 = parityDrop(key);
    cout << "56-bit Key: " << key56 << endl;

    string leftKey = key56.substr(0, 28);
    string rightKey = key56.substr(28, 28);

    cout << "Round 1 - 28-bit Keys before Shifting:\nLeft: " << leftKey << "\nRight: " << rightKey << endl;

    // Split plaintext into two 32-bit halves
    string left = plaintext.substr(0, 32);
    string right = plaintext.substr(32, 32);

    cout << "Initial Plaintext Halves:\nLeft: " << left << "\nRight: " << right << endl;

    // Round 1
    leftKey = leftShift(leftKey, 1);
    rightKey = leftShift(rightKey, 1);
    cout << "Round 1 - After Shifting:\nLeft: " << leftKey << "\nRight: " << rightKey << endl;
    string round1Key = PBox(leftKey + rightKey);
    cout << "Round 1 - 48-bit Key: " << round1Key << endl;

    string newRight1 = DES_Round(left, right, round1Key);
    string newLeft1 = right;

    // Round 2
    leftKey = leftShift(leftKey, 2);
    rightKey = leftShift(rightKey, 2);
    cout << "Round 2 - After Shifting:\nLeft: " << leftKey << "\nRight: " << rightKey << endl;
    string round2Key = PBox(leftKey + rightKey);
    cout << "Round 2 - 48-bit Key: " << round2Key << endl;

    string newRight2 = DES_Round(newLeft1, newRight1, round2Key);
    string newLeft2 = newRight1;

    string ciphertext = newLeft2 + newRight2;
    cout << "Ciphertext after 2 rounds: " << ciphertext << endl;
}

int main() {
    string plaintext, key;
    cout << "Enter an 8-letter plaintext: ";
    cin >> plaintext;
    cout << "Enter an 8-letter key: ";
    cin >> key;

    string binaryPlaintext = wordToBinary(plaintext);
    string binaryKey = wordToBinary(key);
    
    cout << "64-bit Plaintext: " << binaryPlaintext << endl;
    cout << "64-bit Key: " << binaryKey << endl;
    
    DES_Encrypt(binaryPlaintext, binaryKey);

    return 0;
}
