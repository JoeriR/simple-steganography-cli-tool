#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>

unsigned long calculateFileSizeInBytes(std::ifstream &file) {
    std::streampos begin,end;

    begin = file.tellg();
    file.seekg (0, std::ios::end);
    end = file.tellg();

    unsigned long fileSize = end - begin;
    return fileSize;
}

int main(int argc, char* argv[]) {

    std::string hiddenText;

    if (argc < 2) {
        std::cout << "No path given, please restart this tool and pass a valid path into it";
        exit(1);
    }
    if (argc < 3) {
        std::cout << "Please enter the text you want to hide in the bitmap file: ";
        std::getline(std::cin, hiddenText);
    }
    else {
        hiddenText = argv[2];
    }

    std::string inputFilePath = std::string(argv[1]);

    std::vector<bool> hiddenTextBits = std::vector<bool>();

    std::ifstream inputFile  = std::ifstream(inputFilePath, std::ios::binary);

    unsigned long fileSize = calculateFileSizeInBytes(inputFile);

    inputFile.seekg(0, std::ios::beg);
    unsigned char bytes[fileSize];

    inputFile.read((char*)bytes, fileSize);
    inputFile.close();

    // Create hiddenTextBits
    for (char c : hiddenText) {
        std::bitset<8> charBits(c);

        for (int i = charBits.size() - 1; i >= 0; --i) {
            if (charBits.test(i))
                hiddenTextBits.push_back(1);
            else
                hiddenTextBits.push_back(0);
        }
    }

    for (int bit : hiddenTextBits) {
        std::cout << bit;
    }

    for (int i = 0; i < hiddenTextBits.size(); ++i) {
        
        unsigned char* bytePtr = bytes + 64 + i;

        std::cout << "byte before change: ";
        for (int i = 0; i < 8; i++) {
            printf("%d", !!((*bytePtr << i) & 0x80));
        }

        bool hiddenCharBit = hiddenTextBits[i];

        if (hiddenCharBit == 0)
            *bytePtr &= ~(1UL << 0);    // set the bit to 0
        else
            *bytePtr |= 1UL << 0;       // set the bit to 1

        std::cout << "    byte after change: ";
        for (int i = 0; i < 8; i++) {
            printf("%d", !!((*bytePtr << i) & 0x80));
        }
        std::cout << "\n";
    }

    unsigned char* bytePtr;

    // Modify another 8 least significant bits to the value 0
    // This writes the end-marker of the hidden message
    for (int i = 0; i < 8; ++i) {
        bytePtr = bytes + 64 + hiddenTextBits.size();
        bytePtr += i;
        *bytePtr &= ~(1UL << 0);    // set the bit to 0
    }

    // print bytes for testing
    /*
    for (unsigned long i = 0; i < fileSize; ++i) {
        std::cout << bytes[i];
    }
    */

    // Prepare generated filename for the output file
    std::string outputFileName = inputFilePath;

    // Remove directory if present.     source: https://stackoverflow.com/a/8520815
    // Do this before extension removal incase directory has a period character.
    /*
    const size_t last_slash_idx = outputFileName.find_last_of("\\/");
    if (std::string::npos != last_slash_idx)
    {
        outputFileName.erase(0, last_slash_idx + 1);
    }
    */

    // Remove extension if present.     source: https://stackoverflow.com/a/8520815
    const size_t period_idx = outputFileName.rfind('.');
    if (std::string::npos != period_idx)
    {
        outputFileName.erase(period_idx);
    }

    outputFileName.append("_hidden.bmp");

    // Write output to a file
    std::ofstream outputFile = std::ofstream(outputFileName);
    outputFile.write((const char*)bytes, fileSize);

    std::cout << "\n\ndone" << std::endl;
}