#include <cstdlib>
#include <array>
#include <exception>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include "ArgumentParser.hpp"
#include "Brainfuck.hpp"


int
main(int argc, const char* argv[])
{
  std::unordered_map<std::string, Brainfuck::Target> targetMap{
    {"c", Brainfuck::Target::kC},
    {"xbyakc", Brainfuck::Target::kXbyakC}
  };

  try {
    ArgumentParser ap(argv[0]);
    ap.add('h', "help", "Show help and exit this program");
    ap.add('m', "minify", "Remove all non-brainfuck characters from source code");
    ap.add('t', "target", ArgumentParser::OptionType::kRequiredArgument,
        "Specify target language" + ap.getNewlineDescription()
        + "- c:      Transpile to C source" + ap.getNewlineDescription()
        + "- xbyakc: Dump xbyak code as C source", "TARGET", "");
    ap.add('O', "optimize", ArgumentParser::OptionType::kRequiredArgument,
        "Specify optimization level" + ap.getNewlineDescription()
        + "Default value: 1" + ap.getNewlineDescription()
        + "- 0: Execute directly" + ap.getNewlineDescription()
        + "- 1: Compile to IR code and execute" + ap.getNewlineDescription()
        + "- 2: Compile to native code and execute", "LEVEL", 1);
    ap.add("dump-ir", "Dump IR code");
    ap.add("enable-synchronize-with-stdio", "Disable synchronization between std::cout/std::cin and <cstdio>");
    ap.add("heap-size", ArgumentParser::OptionType::kRequiredArgument,
        "Specify heap memory size" + ap.getNewlineDescription()
        + "Default value: 65536", "HEAP_SIZE", 65536);
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
    int optLevel = ap.get<int>("optimize");

    std::vector<std::string> args = ap.getArguments();
    if (args.size() == 0) {
      std::cerr << "Please specify one or more brainfuck source code" << std::endl;
      return EXIT_FAILURE;
    }

    Brainfuck bf;
    if (ap.get<bool>("minify")) {
      for (const auto& filename : args) {
        bf.load(filename);
        bf.trim();
        std::cout << bf.getSource() << std::endl;
        return EXIT_SUCCESS;
      }
    }
    if (ap.get<bool>("dump-ir")) {
      for (const auto& filename : args) {
        bf.load(filename);
        bf.trim();
        bf.compile();
        bf.dumpIR();
      }
      return EXIT_SUCCESS;
    }
    const std::string& target = ap.get("target");
    if (target != "") {
      bf.load(args[0]);
      bf.trim();
      bf.compile(Brainfuck::CompileType::kJit);
      if (targetMap.find(target) == targetMap.end()) {
        std::cerr << "Option -t, --target: Invalid value: \"" << target << "\" is specified" << std::endl;
        return EXIT_FAILURE;
      } else {
        bf.emit(std::cout, targetMap[target]);
        return EXIT_SUCCESS;
      }
    }

    for (const auto& filename : args) {
      if (filename == "-") {
        bf.load(std::cin);
      } else {
        bf.load(filename);
      }
      bf.trim();
      if (optLevel == 1) {
        bf.compile(Brainfuck::CompileType::kIR);
      } else if (optLevel > 1){
        bf.compile(Brainfuck::CompileType::kJit);
      }
      bf.execute(heapSize);
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  return EXIT_SUCCESS;
}
