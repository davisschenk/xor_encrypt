#include <iostream>
#include <fstream>

using namespace std;

void xor_data(char buffer[16], char key[16]) {
    for (int i = 0 ; i < 16 ; i++)
        buffer[i] = buffer[i] ^ key[i];
}

void swap_bytes(char buffer[16], char key[16]) {
    int start = 0;
    int end = 15;

    while (start != end) {
        for (int i = 0 ; i < 15 ; i++) {
            if (int(key[i]) % 2 == 0) {
                start++;
            } else {
                char t = buffer[start];
                buffer[start] = buffer[end];
                buffer[end] = t;

                start++;
                end--;
            }

            if (start == end)
                break;
        }
    }
}

int main(int argc, char* argv[]) {
    char cipher_type;
    char mode;
    fstream input;
    fstream output;
    fstream keyfile;

    if (argc != 6) {
        cerr << "Not enough arguments provided, got " << argc - 1 << " expected 5" << endl;
        return 1;
    }

    cipher_type = argv[1][0];
    if (!(cipher_type == 'B' || cipher_type == 'S')) {
        cerr << "Cipher Type " << cipher_type << " not recognized" << endl;
        return 1;
    }

    input.open(argv[2], ios::in | ios::binary);
    if (!input) {
        cerr << "Input file " << argv[2] << " could not be found" << endl;
        return 1;
    }

    output.open(argv[3], ios::out | ios::binary);
    if (!output) {
        cerr << "Output file " << argv[3] << " could not be opened" << endl;
        return 1;
    }

    keyfile.open(argv[4], ios::in);
    if (!keyfile) {
        cerr << "Keyfile " << argv[4] << " could not be found" << endl;
        return 1;
    }

    mode = argv[5][0];
    if (!(mode == 'E' || mode == 'D')) {
        cerr << "Mode " << mode << " not recognized" << endl;
        return 1;
    }

    if (cipher_type == 'B') {
        // Block cipher
        if (mode == 'E') {
            char* key = new char[16];
            keyfile.read(key, 16);

            char* buffer = new char[16];
            while (input.readsome(buffer, 16)) {
                for (int i = input.gcount() ; i < 16 ; i++)
                    buffer[i] = static_cast<char>(0x81);

                xor_data(buffer, key);
                swap_bytes(buffer, key);

                output.write(buffer, 16);
            }
        } else if (mode == 'D') {
            char* key = new char[16];
            keyfile.read(key, 16);

            char* buffer = new char[16];
            while (input.readsome(buffer, 16)) {
                swap_bytes(buffer, key);
                xor_data(buffer, key);

                int i;
                for (i = 0 ; i < 16 ; i++)
                    if (buffer[i] == static_cast<char>(0x81))
                        break;

                output.write(buffer, i);
            }
        }
    } else if (cipher_type == 'S') {
        char key;
        char inp;

        while (input.get(inp)) {
            if(!keyfile.get(key)) {
                keyfile.seekg(0);
                keyfile.get(key);
            }

            output.put(key ^ inp);

        }
    }

}