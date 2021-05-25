#include <iostream>
#include <boost/program_options.hpp>
#include "Builder.hpp"

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
