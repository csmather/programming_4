//
// Created by Cole on 11/14/2018.
//

#include <fstream>

#include "llrb_multimap.h"

class Scheduler {
public:
    // Reads input file and fills multimap of waiting tasks
    void ReadInput(std::ifstream& input);

private:
    unsigned int min_vruntime;
    unsigned int tick;

    class Program {
    public:

    private:
        char identifier;
        unsigned int start_time;
        unsigned int duration;
        unsigned int runtime;
        unsigned int vruntime;
    };
    // Contains programs which are currently running, sorted by vruntime
    LLRB_multimap<unsigned int, std::unique_ptr<Program>> running;
    // Contains programs which are waiting to run, sorted by start_time
    LLRB_multimap<unsigned int, std::unique_ptr<Program>> waiting;
};

void ReadInput(std::ifstream& input) {
    char identifier;
    unsigned int start_time;
    unsigned int duration;
    while (!input.eof()) {
        input >> identifier;
        input >> start_time;
        input >> duration;

    }
}

int main(int argc, char* argv[]) {
    // Check correct # of args
    if (argc != 2) {
        std::cerr << "Usage: ./" << argv[0]
                  << "<taskfile.dat>" << std::endl;
        exit(1);
    }
    std::ifstream inputFile(argv[1]);
    // Check input file opens
    if (!inputFile.good()) {
        std::cerr << "Error: cannot open file " << argv[1] << std::endl;
        exit(1);
    }

    Scheduler CFS;
    CFS.ReadInput(inputFile);
}