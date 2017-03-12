#include <cstdlib>
#include <iostream>
#include "ArgumentParser.hpp"
#include "Brainfuck.hpp"


int
main(int argc, const char* argv[])
{
  try {
    ArgumentParser ap(argv[0]);
    ap.add("enable-synchronize-with-stdio", "Disable synchronization between std::cout/std::cin and <cstdio>");
    ap.add('h', "help", "Show help and exit this program");
    ap.parse(argc, argv);

    if (ap.get<bool>("help")) {
      ap.showUsage();
      return EXIT_SUCCESS;
    }
    if (!ap.get<bool>("enable-synchronize-with-stdio")) {
      std::cin.tie(nullptr);
      std::ios::sync_with_stdio(false);
    }

    Brainfuck bf;
    bf.loadSource("+++++++++[>++++++++>+++++++++++>+++++<<<-]>.>++.+++++++..+++.>-. ------------.<++++++++.--------.+++.------.--------.>+.");
    bf.trim();
    bf.execute();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  return EXIT_SUCCESS;
}
