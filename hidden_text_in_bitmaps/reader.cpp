#include <iostream>
#include <fstream>

unsigned long calculateFileSizeInBytes(std::ifstream &file) {
    std::streampos begin,end;

    begin = file.tellg();
    file.seekg (0, std::ios::end);
    end = file.tellg();

    unsigned long fileSize = end - begin;
    return fileSize;
}

int main(int argc, char* argv[]) {
    
    std::cout << "Made by Joeri btw";
    
    if (argc < 2) {
        std::cout << "No path given, please restart this tool and pass a valid path into it";
        exit(1);
    }

    std::string inputFilePath = std::string(argv[1]);

    std::ifstream inputFile  = std::ifstream(inputFilePath, std::ios::binary);

    unsigned long fileSize = calculateFileSizeInBytes(inputFile);

    inputFile.seekg(0, std::ios::beg);
    unsigned char bytes[fileSize];

    inputFile.read((char*)bytes, fileSize);
    inputFile.close();

    std::cout << "\n\nReading hidden string from bitmap ... \nHidden string: ";

    unsigned char* bytePtr = bytes + 64;    // Start position

        char currentChar = 'z'; // this initialized value is irrelevant
        
        while (true) {
            
            // Retrieve 8 bits of 1 hidden character by reading the least significant bit from 8 bytes.
            int bit1 = !!((*(bytePtr + 0) << 7) & 0x80);
            int bit2 = !!((*(bytePtr + 1) << 7) & 0x80);
            int bit3 = !!((*(bytePtr + 2) << 7) & 0x80);
            int bit4 = !!((*(bytePtr + 3) << 7) & 0x80);
            int bit5 = !!((*(bytePtr + 4) << 7) & 0x80);
            int bit6 = !!((*(bytePtr + 5) << 7) & 0x80);
            int bit7 = !!((*(bytePtr + 6) << 7) & 0x80);
            int bit8 = !!((*(bytePtr + 7) << 7) & 0x80);

            // Modify currentChar by writing the bits from above into the byte
            currentChar ^= (-bit1 ^ currentChar) & (1UL << 7);
            currentChar ^= (-bit2 ^ currentChar) & (1UL << 6);
            currentChar ^= (-bit3 ^ currentChar) & (1UL << 5);
            currentChar ^= (-bit4 ^ currentChar) & (1UL << 4);
            currentChar ^= (-bit5 ^ currentChar) & (1UL << 3);
            currentChar ^= (-bit6 ^ currentChar) & (1UL << 2);
            currentChar ^= (-bit7 ^ currentChar) & (1UL << 1);
            currentChar ^= (-bit8 ^ currentChar) & (1UL << 0);

            if (currentChar == '\0') {
                break;
            }

            std::cout << currentChar;

            bytePtr += 8;
        }
}