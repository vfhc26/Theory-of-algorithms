#include <iostream>
#include <fstream>
#include <cstring>

const int MAX_TAPE_SIZE = 30;
const int MAX_STATES = 20;
const int MAX_SYMBOLS = 256;
const int MAX_STEPS = 10000;

struct Rule{
    int next_state;
    char write;
    char move;
};


class TuringMachine
{
private:
    char tape[MAX_TAPE_SIZE];
    int head;
    int state;
    Rule rules[MAX_STATES][MAX_SYMBOLS];
public:
    TuringMachine(){
        for (int i = 0; i < MAX_TAPE_SIZE; ++i) tape[i] = '#';
        head = 0;
        state = 0;
        for (int i = 0; i < MAX_STATES; ++i)
            for (int j = 0; j < MAX_SYMBOLS; ++j)
                rules[i][j].next_state = -1;
    }
    void add_rule(int s, char read, int ns, char wr, char mv){
        rules[s][static_cast<unsigned char>(read)] = {ns, wr, mv};
    }
    void load_rules(const char* filename){
        std::ifstream file(filename);
        int s, ns;
        char read, wr, mv;
        while (file >> s >> read >> ns >> wr >> mv) {
            add_rule(s, read, ns, wr, mv);
        }
        file.close();
    }
    void set_tape(const char* input){
        for (int i = 0; i < MAX_TAPE_SIZE; ++i) tape[i] = '#';
        int len = strlen(input);
        int offset = (MAX_TAPE_SIZE - len) / 2;
        for (int i = 0; i < len; ++i) {
            tape[offset + 1 + i] = input[i];
        }
        head = offset + 1;
        state = 1;
    }
    void get_tape(char* buffer, int buffer_size) const {
        int start = 0;
        int end = MAX_TAPE_SIZE - 1;
        while (start < MAX_TAPE_SIZE && tape[start] == '#') start++;
        while (end >= 0 && tape[end] == '#') end--;
        if (start > end) {
            buffer[0] = '\0';
            return;
        }
        int len = end - start + 1;
        if (len >= buffer_size) len = buffer_size - 1;
        for (int i = 0; i < len; ++i) {
            buffer[i] = tape[start + i];
        }
        buffer[len] = '\0';
    }
    void print_tape(){
        for (int i = 0; i < MAX_TAPE_SIZE; i++){
            if (i == head) std::cout << "(" << tape[i] << ")";
            else std::cout << tape[i];
        }
        std::cout << std::endl;
    }
    void run(bool debug = false)
    {
        int steps = 0;
        while (state != 0 && steps < MAX_STEPS){
            char current = tape[head];
            Rule r = rules[state][static_cast<unsigned char>(current)];
            if (r.next_state == -1) {
                std::cerr << "No rule for state q" << state << " and symbol '" << current << "'" << std::endl;
                break;
            }
            if (debug) {
                std::cout << "State q" << state << ", Current tape: ";
                print_tape();
            }
            if (head < 0 || head >= MAX_TAPE_SIZE) {
                std::cerr << "Tape overflow\n";
                break;
            }
            tape[head] = r.write;
            if (r.move == 'R') head++;
            else if (r.move == 'L') head--;
            
            state = r.next_state;
            steps++;
        }
    }
};

void testing(TuringMachine* machines){
    const char* TEST1_INPUTS[] = {"abc", "babab", "babca", "abca", "", "b"};           // Шифр Цезаря, сдвиг на 1
    const char* TEST2_INPUTS[] = {"11000011", "00110100", "010101001", "", "0", "1"};              // n -> 2n
    const char* TEST3_INPUTS[] = {"123", "9999", "89", "", "0", "19293"};                 // x+1

    const char** all_test[] = {TEST1_INPUTS, TEST2_INPUTS, TEST3_INPUTS};
    int sizes[] = {6, 6, 6};
    for (int machine = 0; machine < 3; ++machine){
        std::cout << "Testing Turing Machine " << (machine + 1) << ":\n";
        for (int test = 0; test < sizes[machine]; ++test){
            machines[machine].set_tape(all_test[machine][test]);
            machines[machine].run();
            char output[MAX_TAPE_SIZE];
            machines[machine].get_tape(output, sizeof(output));
            std::cout << "input: " << all_test[machine][test] << " -> output: " << output << std::endl;
        }
        std::cout << std::endl;
    }
}
int main()
{
    const char* FILES[] = {"tm1.txt", "tm2.txt", "tm3.txt"};
    TuringMachine machines[3];
    for (int i = 0; i < 3; ++i) {
        machines[i].load_rules(FILES[i]);
    }
    int choice;
    std::cout << "Choose mode:\n1. Run Turing Machine\n2. Run Tests\n0. Exit\n";

    while (std::cin >> choice && choice != 0){
        if (choice == 1){
            std::cout << "Available Turing Machines:\n1. a*b*c*\n2. Even\n3. Increment Number (x+1)\n";
            int machine_num;
            std::cout << "Enter Turing Machine number (1-3): ";
            std::cin >> machine_num;
            if (machine_num < 1 || machine_num > 3) {
                std::cout << "Invalid machine number. Try again.\n";
                continue;
            }
            char input[MAX_TAPE_SIZE];
            std::cout << "Enter input string: ";
            std::cin >> input;
            machines[machine_num - 1].set_tape(input);
            machines[machine_num - 1].run(true);
            char output[MAX_TAPE_SIZE];
            machines[machine_num - 1].get_tape(output, sizeof(output));
            std::cout << "Final tape: " << output << std::endl;
            std::cout << std::endl;
        }else if (choice == 2) testing(machines);
        else std::cout << "Invalid choice. Try again.\n";
        std::cout << "Choose mode:\n1. Run Turing Machine\n2. Run Tests\n0. Exit\n";
    }
    return 0;
}