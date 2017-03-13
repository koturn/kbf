#include <cstdlib>
#include <iostream>
#include "ArgumentParser.hpp"
#include "Brainfuck.hpp"


int
main(int argc, const char* argv[])
{
  try {
    ArgumentParser ap(argv[0]);
    ap.add("heap-size", ArgumentParser::OptionType::REQUIRED_ARGUMENT, "Specify heap memory size", "HEAP_SIZE", 65536);
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
    std::size_t heapSize = ap.get<std::size_t>("heap-size");

    std::vector<std::string> args = ap.getArguments();
    if (args.size() == 0) {
      std::cerr << "Please specify one or more brainfuck source code" << std::endl;
      return EXIT_FAILURE;
    }

    Brainfuck bf;
    for (const auto& filename : args) {
      bf.load(filename);
      bf.trim();
      bf.execute(heapSize);
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  return EXIT_SUCCESS;
}
