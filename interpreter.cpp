#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>

using namespace std;

class Interpreter {
private:
  ifstream &ifs;
  vector<char> mem;
  stack<unsigned int> loop_head_stack;
  unsigned int pointer;

public:
  Interpreter(ifstream &ifs) : ifs(ifs) {
    pointer = 0;
  }

  void interpret() {
    char c;

    ifs.seekg(0);

    while (ifs.get(c)) {
      adjust_mem_size();

      switch (c) {
        case '+': increment(); break;
        case '-': decrement(); break;
        case '>': move_next(); break;
        case '<': move_prev(); break;
        case '[': loop_begin(); break;
        case ']': loop_end(); break;
        case '.': output(); break;
        case ',': input(); break;
      }

      // cout << c << " ";
      // print_mem();
    }
  }

private:

  void increment() {
    mem[pointer]++;
  }

  void decrement() {
    mem[pointer]--;
  }

  void move_next() {
    pointer++;
  }

  void move_prev() {
    pointer--;
  }

  void loop_begin() {
    if (mem[pointer] == 0) {
      break_loop(); // jump to end of this loop
    } else {
      const int current_pos = static_cast<int>(ifs.tellg()) - 1;
      loop_head_stack.push(current_pos); // store the position
      // and go into this loop
    }
  }

  void loop_end() {
    const int loop_head = loop_head_stack.top();
    ifs.seekg(loop_head); // back to the start of this loop
    loop_head_stack.pop(); // remove the stored position
  }

  void output() {
    cout << mem[pointer];
  }

  void input() {
    mem[pointer] = cin.get();
  }

  void break_loop() {
    unsigned int loop_level = 1;

    do {
      switch (ifs.get()) {
        case '[':
          loop_level++;
          break;
        case ']':
          loop_level--;
          break;
      }
    } while (loop_level > 0);
  }

  void adjust_mem_size() {
    if (mem.size() <= pointer) {
      mem.push_back(0); // append memory and set value to 0
    }
  }

  void print_mem() {
    cout << "mem(";

    for (char &c : mem) {
      cout << c << ",";
    }

    cout << "\b)" << endl;
  }
};

int main(int argc, char const *argv[]) {
  if (argc < 2) {
    cerr << "please assign .bf file" << endl;
    return 1;
  }

  ifstream ifs(argv[1]);

  if (ifs.fail()) {
    cerr << "failed to open the file '" << argv[1] << "'" << endl;
    return 1;
  }

  Interpreter interpreter(ifs);
  interpreter.interpret();

  cout << endl;

  return 0;
}
