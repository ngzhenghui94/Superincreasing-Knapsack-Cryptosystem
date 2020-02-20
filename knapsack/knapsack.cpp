#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>
#include <stdio.h>
#include <bitset>
#include <sstream>
#include <numeric>


using namespace std;
vector<int> privateKey;
vector<int> publicKey;
vector<int> messageBinary;
vector<int> ciphertext;
vector<int> decryptionnumber;
vector<int> buffer;
int keySize, userInput, mod, sum, multiplier;
int sumOfKnapSack;
string message;

// A naive method to find modulor multiplicative inverse of
// 'a' under modulo 'm'
int modInverse(int a, int b) {
    a = a % b;
    for (int x = 1; x < b; x++) {
        if ((a * x) % b == 1)
            return x;
    }
}

int main() {
    cout << "Please key in the size of the super-increasing knapsack: ";
    cin >> keySize;
    privateKey.resize(keySize);
    privateKey.clear();
    cout << "Please enter the values of the " << keySize << " private keys: " << endl;
    sumOfKnapSack = 0;
    for (int i = 0; i < keySize; i++) {
        sumOfKnapSack = accumulate(privateKey.begin(), privateKey.end(), 0);
        cout << "Current knapsack : " << sumOfKnapSack << ". Enter input key #" << i + 1 << ":";
        cin >> userInput;
        //check if it's super-increasing.

        while (userInput < sumOfKnapSack) {
            cout << "Invalid! Sequence is not super-increasing. Current value: " << sumOfKnapSack
                 << " Please re-input a valid key for #" << i + 1 << ":";
            cin >> userInput;
        }
        privateKey.push_back(userInput);
    }

    int a = 1;
    cout << "Your private keys are shown below:" << endl;
    for (vector<int>::const_iterator i = privateKey.begin(); i != privateKey.end(); ++i) {
        cout << "#" << a << ":" << " " << *i << "| ";
        a++;
    }

    /*Calculation of sum of knapsack*/
    sumOfKnapSack = accumulate(privateKey.begin(), privateKey.end(), 0);
    cout << endl;
    cout << "Sum of your knapsack is " << sumOfKnapSack << endl;
    cout << "Enter your mod value (>" << sumOfKnapSack << "):";
    cin >> mod;

    /*This while loops ensures user can only choose a mod value > sum of the knapsack*/
    while (mod < sum) {
        cout << "Mod value needs to be: (>" << sumOfKnapSack << "). Please re-enter your mod value: ";
        cin >> mod;
    }
    cout << "Enter a multiplier that is co-prime to the mod value of " << mod << " : ";
    cin >> multiplier;

    while (__gcd(multiplier, mod) != 1) {
        cout << "Invalid! Please re-enter a multiplier value that is co-prime with " << mod << ": ";
        cin >> multiplier;
    }
    for (int i = 0; i < keySize; i++) {
        publicKey.push_back(privateKey.at(i) * multiplier % mod); //create public key
    }
    cout << "Your public key is:" << endl;
    for (vector<int>::const_iterator i = publicKey.begin(); i != publicKey.end(); ++i) {
        cout << *i << "|"; //print out public key
    }
    cout << endl;
    cin.ignore();
    cout << "Please enter message to be encrypted: ";
    cin >> message;

    for (size_t i = 0; i < message.size(); ++i) {
        bitset<8> test(message.c_str()[i]);
        string s = test.to_string<char, string::traits_type, string::allocator_type>();

        for (int j = 0; j < s.size(); j++) {
            int t = 0;
            stringstream ss;
            ss << s.at(j);
            while (ss >> t) {
                messageBinary.push_back(t);
            }
        }
    }

    cout << "Binary form of the plaintext: " << endl;
    for (int i = 0; i < messageBinary.size(); i++) {
        cout << messageBinary[i];
    }
    cout << endl;

    while (messageBinary.size() % keySize != 0) {
        messageBinary.insert(messageBinary.begin(), 0);
    }

    int count = 0;
    while (count != messageBinary.size()) {
        int x = 0;
        for (int i = 0; i < keySize; i++) {
            x += publicKey[i] * messageBinary[count];
            count++;
        }
        ciphertext.push_back(x);
    }
    for (vector<int>::const_iterator i = ciphertext.begin(); i != ciphertext.end(); ++i)
        cout << "T: " << *i << endl; //Print our encrypted text


    /*DECRYPTION*/
    for (int i = 0; i < ciphertext.size(); i++) {
        decryptionnumber.push_back(ciphertext.at(i) * modInverse(multiplier, mod) % mod);
    }
    cout << endl;
    cout << "DECRYPTION IN PROGRESS..." << endl;
    for (vector<int>::const_iterator i = decryptionnumber.begin(); i != decryptionnumber.end(); ++i) {
        cout << *i << endl;
    }
    cout << endl;


    int R = 0;
    for (int i = 0; i < ciphertext.size(); i++) {
        R = modInverse(multiplier, mod) * ciphertext.at(i) % mod;
        for (int j = keySize - 1; j > -1; j--) {
            R = R - privateKey[j];
            if (R >= 0) {
                buffer.push_back(1);
            } else if (R < 0) {
                buffer.push_back(0);
                R = R + privateKey[j];
            }
        }
    }

    reverse(buffer.begin(), buffer.end());

    cout << "Reversed Message in Binary Format:" << endl;
    for (vector<int>::const_iterator i = buffer.begin(); i != buffer.end(); ++i) {
        cout << *i;
    }
    cout << endl;

    string tempBinary;
    ostringstream oss;

    if (!buffer.empty()) {
        copy(buffer.begin(), buffer.end() - 1, ostream_iterator<int>(oss, ""));
        oss << buffer.back();
    }

    tempBinary = oss.str(); //tempBinary now contains the plaintext in Binary (REVERSED)

    stringstream sstream(tempBinary);
    string output;
    char c;
    /*The variable "output" will contains the plaintext, but in reversed.*/
    while (sstream.good()) {
        bitset<8> bits;
        sstream >> bits;
        c = char(bits.to_ulong());
        output += c;
    }
    reverse(output.begin(), output.end());
    cout << "Decrypted plaintext = " << output << endl;
}

