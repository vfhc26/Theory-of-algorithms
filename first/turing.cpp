#include <iostream>
#include <string>
#include <vector>

class TuringMachine{
private:
    enum State{   
        qstart,               // начальное состояние
        qa,                   // символ а 
        qb,                   // символ b
        qc,                   // символ c
        qcompare_a,           // сравнение с символом а
        qcompare_b,           // сравнение с символом b
        qcompare_c,           // сравнение с символом c
        qleft,                // движение влево
        qaccept,              // палиндром
        qreject               // не палиндром
    };
    std::string tape;
    int head;
    State state;
public:
    TuringMachine(std::string input){
        tape = '#';
        for (auto ch : input) {
            if (ch != 'a' && ch != 'b' && ch != 'c') {
                throw std::invalid_argument(input + ": Turing machine only accepts characters 'a', 'b', and 'c'.");
            }
            tape += ch;
        }
        tape += '#';
        head = 1;
        state = qstart;
    }
    bool run(bool debug = false){
        while(state != qreject && state != qaccept){
            char current = tape[head];
            if (debug) {
                std::cout << "Current tape: " << tape << ", Head position: " << head << ", State: " << stateName(state) << std::endl;
            }

            if (state == qstart){
                if (current == '#'){
                    state = qaccept;
                } else if (current == 'a' || current == 'b' || current == 'c'){
                    tape[head] = '#';
                    state = (current == 'a') ? qa : (current == 'b') ? qb : qc;
                    head++;
                } else {
                    state = qreject;
                }
            }else if (state == qa || state == qb || state == qc){
                if (current == 'a' || current == 'b' || current == 'c'){
                    head++;
                } else {
                    state = (state == qa) ? qcompare_a : (state == qb) ? qcompare_b : qcompare_c;
                    head--;
                }
            }else if (state == qcompare_a){
                if (current == 'a'){
                    tape[head] = '#';
                    state = qleft;
                    head--;
                } else if (current == '#') {
                    state = qaccept;
                } else {
                    state = qreject;
                }
            }else if (state == qcompare_b){
                if (current == 'b'){
                    tape[head] = '#';
                    state = qleft;
                    head--;
                } else if (current == '#') {
                    state = qaccept;
                } else {
                    state = qreject;
                }
            }else if (state == qcompare_c){
                if (current == 'c'){
                    tape[head] = '#';
                    state = qleft;
                    head--;
                } else if (current == '#') {
                    state = qaccept;
                } else {
                    state = qreject;
                }
            }else if (state == qleft){
                if (current == '#'){
                    state = qstart;
                    head++;
                } else {
                    head--;
                }
            }
        }
        return state == qaccept;
    }
    std::string stateName(State s){
        switch(s){
            case qstart: return "qstart";
            case qa: return "qa";
            case qb: return "qb";
            case qc: return "qc";
            case qcompare_a: return "qcompare_a";
            case qcompare_b: return "qcompare_b";
            case qcompare_c: return "qcompare_c";
            case qleft: return "qleft";
            case qaccept: return "qaccept";
            case qreject: return "qreject";
            default: return "unknown";
        }
    }
};

int main(){    
    std::vector<std::pair<std::string, bool>> test_cases = {
            {"abccbab", false},
            {"abcba", true},
            {"abccba", true},
            {"abbccbba", true},
            {"abcd", false},
            {"a", true},
            {"aa", true},
            {"abc", false},
            {"#", true},
            {"2", false},
            {"", true}
    };
    for (const auto& test : test_cases) {
        try{
            TuringMachine tm(test.first);
            bool is_palindrome = tm.run();
            std::cout << "Input: " << test.first << ", Is palindrome: " << (is_palindrome ? "Yes" : "No") << " " << test.second << std::endl;
        }catch(const std::invalid_argument& e){
            std::cout << "Error: " << e.what() << std::endl;
        }
    }
    std::string input;
    std::cout << "Enter input: ";
    std::cin >> input;
    try{
        TuringMachine tm(input);
        bool is_palindrome = tm.run(true);
        std::cout << "Is palindrome: " << (is_palindrome ? "Yes" : "No") << std::endl;
    }catch(const std::invalid_argument& e){
        std::cout << "Error: " << e.what() << std::endl;
    }
    return 0;
}
