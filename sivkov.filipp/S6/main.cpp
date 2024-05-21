#include <iostream>
#include <iterator>
#include <random>
#include <deque>
#include <list>
#include <string>
#include <list.hpp>
#include <algorithm>
#include "sorts.hpp"


namespace sivkov
{
  template <typename T>
  void generateRandom(size_t numElements, const std::string& type, std::deque<T>& dq, List<T>& fwdList, std::list<T>& biList)
  {
    if (type == "ints" || type == "floats")
    {
      for (size_t i = 0; i < numElements; ++i)
      {
        T value;
        if (type == "ints")
        {
          value = std::rand() % 100;
        }
        else if (type == "floats")
        {
          value = std::rand() % 100;
        }
        dq.push_back(value);
        fwdList.push_back(value);
        biList.push_back(value);
      }
    }
    else
    {
      throw std::invalid_argument("Invalid type or template argument");
    }
  }
}

int main(int argc, char* argv[])
{
  using namespace sivkov;
  if (argc != 4)
  {
    std::cerr << "error arg in commadn line\n";
    return 1;
  }

  std::string direction = "";
  std::string type = "";
  size_t n = 0;
  try
  {
    direction = argv[1];
    type = argv[2];
    n = std::stoull(argv[3]);
  }
  catch (...)
  {
    std::cerr << "error\n";
  }


  List< int > fwdList;
  std::list< int > biList;
  std::list< int > biList2;
  std::deque< int > deq;
  std::deque< int > deq2;
  std::deque< int > deq3;


  try
  {
    generateRandom(n, type, deq, fwdList, biList);
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }

  for (const auto& el : fwdList)
  {
    std::cout << el << " ";
  }
  std::cout << std::endl;

  biList2 = biList;
  deq2 = deq;
  deq3 = deq;

  if ("ascending")
  {
    oddEvenSort(fwdList.begin(), fwdList.end(), std::less<int>());
    shellSort(biList.begin(), biList.end(), std::less< int >());
    biList2.sort(std::less< int >());
    oddEvenSort(deq.begin(), deq.end(), std::less< int >());
    shellSort(deq2.begin(), deq2.end(), std::less< int >());
    std::sort(deq3.begin(), deq3.end(), std::less< int >());
  }
  else if ("descending")
  {
    oddEvenSort(fwdList.begin(), fwdList.end(), std::greater<int>());
    shellSort(biList.begin(), biList.end(), std::greater< int >());
    biList2.sort(std::less< int >());
    oddEvenSort(deq.begin(), deq.end(), std::greater< int >());
    shellSort(deq2.begin(), deq2.end(), std::greater< int >());
    std::sort(deq3.begin(), deq3.end(), std::greater< int >());
  }
  else
  {
    std::cerr << "error second arg\n";
  }

  for (const auto& el : fwdList)
  {
    std::cout << el << " ";
  }
  std::cout << std::endl;

  for (const auto& el : biList)
  {
    std::cout << el << " ";
  }
  std::cout << std::endl;

  for (const auto& el : biList2)
  {
    std::cout << el << " ";
  }
  std::cout << std::endl;

  for (const auto& el : deq)
  {
    std::cout << el << " ";
  }
  std::cout << std::endl;

  for (const auto& el : deq2)
  {
    std::cout << el << " ";
  }
  std::cout << std::endl;

  for (const auto& el : deq3)
  {
    std::cout << el << " ";
  }
  std::cout << std::endl;

  return 0;
}
