#include <iostream>
#include <fstream>
#include "stack.hpp"
#include "queue.hpp"
#include "input.hpp"
#include "types.hpp"
#include "convertpostfix.hpp"

int main(int argc, char* argv[])
{
  using namespace piyavkin;
  List< Queue< InputType > > queues;
  if (argc == 1)
  {
    input(std::cin, queues);
  }
  else if (argc == 2)
  {
    std::ifstream in(argv[1]);
    input(in, queues);
  }
  else
  {
    std::cerr << "The file is not specified correctly\n";
    return 1;
  }
  postfix_t postfix;
  convertPostfix(queues, postfix);
  std::cout << '\n';
}