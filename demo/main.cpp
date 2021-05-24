#include <iostream>
#include <boost/program_options.hpp>
#include <iostream>
#include "Builder.hpp"
#include <regex>

namespace po = boost::program_options;

int main(int argc, char* argv[]){


  po::options_description desc("Allowed options");
  desc.add_options()("help", ": выводим вспомогательное сообщение")(
      "config", po::value<std::string>(),
      ": указываем конфигурацию сборки (по умолчанию Debug)")(
      "install", ":добавляем этап установки (в директорию _install)")(
      "pack", ": добавляем этап упаковки (в архив формата tar.gz)")(
      "timeout", po::value<int>(),": указываем время ожидания (в секундах)");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);

  std::string help_mes= \
 R"(
Usage: builder [options]
Allowed options:
  --help                    : выводим вспомогательное сообщение
  --config <Release|Debug>  : указываем конфигурацию сборки (по умолчанию Debug)
  --install                 : добавляем этап установки (в директорию _install)
  --pack                    : добавляем этап упаковки (в архив формата tar.gz)
  --timeout <count>         : указываем время ожидания (в секундах)
)";
  if (vm.count("help")) {
    std::cout << help_mes <<std::endl;
    return 0;
  }
  std::string config = "Debug";
  bool install = (vm.count("install"))? true : false;
  bool pack = (vm.count("pack"))? true : false;
  int timeout = 100;

  if (vm.count("config")){
    config = vm["config"].as<std::string>();
    if (config != "Debug" || config != "Release")
      config = "Debug";
  }

  if (vm.count("timeout")){
    timeout = vm["timeout"].as<int>();
  }

  Builder a(config, install, pack, timeout);

  return 0;
}
/*
int main()
{
  auto task1 = async::spawn([] {
    std::cout << "Task 1 executes asynchronously" << std::endl;
  });
  auto task2 = async::spawn([]() -> int {
    std::cout << "Task 2 executes in parallel with task 1" << std::endl;
    return 42;
  });
  auto task3 = task2.then([](int value) -> int {
    std::cout << "Task 3 executes after task 2, which returned "
              << value << std::endl;
    return value * 3;
  });
  auto task4 = async::when_all(task1, task3);
  auto task5 = task4.then([](std::tuple<async::task<void>,
  async::task<int>> results) {
    std::cout << "Task 5 executes after tasks 1 and 3. Task 3 returned "
              << std::get<1>(results).get() << std::endl;
  });

  task5.get();
  std::cout << "Task 5 has completed" << std::endl;

  async::parallel_invoke([] {
    std::cout << "This is executed in parallel..." << std::endl;
  }, [] {
    std::cout << "with this" << std::endl;
  });

  async::parallel_for(async::irange(0, 5), [](int x) {
    std::cout << x;
  });
  std::cout << std::endl;

  int r = async::parallel_reduce({1, 2, 3, 4}, 0, [](int x, int y) {
    return x + y;
  });
  std::cout << "The sum of {1, 2, 3, 4} is " << r << std::endl;
}

// Output (order may vary in some places):
// Task 1 executes asynchronously
// Task 2 executes in parallel with task 1
// Task 3 executes after task 2, which returned 42
// Task 5 executes after tasks 1 and 3. Task 3 returned 126
// Task 5 has completed
// This is executed in parallel...
// with this
// 01234
// The sum of {1, 2, 3, 4} is 10
*/