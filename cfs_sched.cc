//
// Created by Cole on 11/14/2018.
//

#include <fstream>

#include "llrb_multimap.h"

class Scheduler {
public:
    // Reads input file and fills multimap of waiting tasks
    void ReadInput(std::ifstream& input);
    void RunPrograms();
    void PrintStatus();

private:
    unsigned int min_vruntime = 0;
    unsigned int tick= 0;

    struct Program {
        char identifier;
        unsigned int start_time;
        unsigned int duration;
        unsigned int runtime;
        unsigned int vruntime;
        };
    struct ProgramCompare {
        bool operator()(Program *lhs, Program *rhs) {
            return lhs->identifier < rhs->identifier;
        }
    };

    Program* current_program = nullptr;

    // Contains programs which are currently running, sorted by vruntime
    LLRB_multimap<unsigned int, Program*> running;
    // Contains programs which are waiting to run, sorted by start_time
    LLRB_multimap<unsigned int, Program*> waiting;
};

void Scheduler::PrintStatus() {
    if (current_program) {
            std::cout << tick << " [" << running.Size() + 1 << "]: "
                      << current_program->identifier;
        if (current_program->runtime == current_program->duration) {
            std::cout << "*";
        }
    } else {
        std::cout << tick << " [0]: _";
    }
    std::cout << std::endl;
}

void Scheduler::RunPrograms() {
    // There are still programs which are running or waiting to be run
    while (current_program ||waiting.Size() || running.Size()) {

        // If there are tasks that should be launched at the current tick value,
        // add them to the timeline of tasks
        // The programs will be sorted by identifier before they are inserted
        std::vector<Program*> programs;
        while (waiting.Contains(tick)) {
            Program* program = waiting.Get(tick);
            // Initialize their virtual runtime to the current global min_vruntime
            program->vruntime = min_vruntime;
            programs.push_back(program);
            waiting.Remove(tick);
        }
        std::sort(programs.begin(), programs.end(), ProgramCompare());
        for (unsigned int i = 0; i < programs.size(); i++) {
            running.Insert(min_vruntime, programs.at(i));
        }
        programs.clear();

        // If there are no tasks continue to next tick
        if (!current_program && !running.Size()) {
            PrintStatus();
            tick++;
            continue;
        }

        // Check if the currently running task should let the processing resource to the next task
        // If it does, it should be added back to the timeline
        if (current_program && current_program->vruntime > min_vruntime) {
            running.Insert(current_program->vruntime, current_program);
            current_program = nullptr;
        }

        // If there is no more current task, get the next schedulable task from the timeline.
        if (!current_program && running.Size()){
            current_program = running.Get(running.Min());
            running.Remove(running.Min());
        }

        // The global min_vruntime should be set to the next task’s virtual runtime
        if (running.Size()) {
            min_vruntime = running.Min();
        }

        // The current task runs for one tick
        current_program->runtime += 1;
        current_program->vruntime += 1;

        // The scheduler reports the scheduling status on the screen
        PrintStatus();

        // If the current task has completed, it should be purged from the system
        if (current_program->runtime == current_program->duration) {
            delete current_program;
            current_program = nullptr;
        }

        // The tick value is incremented by one, and the loop can continue from the beginning
        tick++;
    }
}

void Scheduler::ReadInput(std::ifstream& input) {
    char identifier;
    unsigned int start_time;
    unsigned int duration;
    while (!input.eof()) {
        input >> identifier;
        if (input.bad()) {
            std::cerr << "Error: input file is invalid" << std::endl;
            exit(1);
        }
        input >> start_time;
        if (input.bad()) {
            std::cerr << "Error: input file is invalid" << std::endl;
            exit(1);
        }
        input >> duration;
        if (input.bad()) {
            std::cerr << "Error: input file is invalid" << std::endl;
            exit(1);
        }
        // This line needs to be inserted on Linux, because of differences in eof signals
        // if (input.eof()) return;
        waiting.Insert(start_time, new Program{identifier, start_time, duration, 0, 0});
    }
}

int main(int argc, char* argv[]) {
    // Check correct # of args
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0]
                  << " <task_file.dat>" << std::endl;
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
    CFS.RunPrograms();
    return 0;
}
