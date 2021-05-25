//
// Created by hacker on 5/17/21.
//
//Copyright 2021 pan_fera

#include "Builder.hpp"

namespace bp = boost::process;
std::atomic<bool> state = false;
Builder::Builder(std::string config, bool install, bool pack, int timeout):
_config(config), _install(install), _pack(pack), _timeout(timeout)
{
  if (_timeout == 0) {
    std::cout << "Timeout = 0" << std::endl;
    return;
  }

   /* auto timer = async::spawn([this] {
      std::cout << "Timer started" << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(_timeout));
      if (_child.running()) {
        std::cout << "Time is exceed."
                  << std::endl << "Terminating" << std::endl;
        _child.terminate();
      }
    });*/

  auto start = std::chrono::high_resolution_clock::now();

  auto task1 = async::spawn([this]{
    bool success = NewTask(
        "-H. -B_builds -DCMAKE_INSTALL_PREFIX=_install "
        "-DCMAKE_BUILD_TYPE=" +
        _config);

    if (!success) return;
});

  auto task2 = task1.then([this]{
    bool success = NewTask("--build _builds");
    if (!success) return;
});

  auto task3 = task2.then([this]{
    if (_install) {
      bool success = NewTask("--build _builds --target install");
      if (!success) return;
    }
});

  auto task4 = task3.then([this]{
    if (_pack) {
      bool success = NewTask("--build _builds --target package");
      if (!success) return;
    }
    state = true;
    std::cout<<state<<std::endl;
  });

  int seconds = 0;
  do{
  auto end = std::chrono::high_resolution_clock::now();
  seconds = std::chrono::duration_cast<std::chrono::seconds>
      (end - start).count();}
  while(seconds < _timeout && !state);
}

bool Builder::NewTask(std::string task) {
  bp::ipstream stream;
  auto cmake_path = boost::process::search_path("cmake");


  std::cout << std::endl << std::endl
            << "Executing Task : " << std::endl << task
            << std::endl << std::endl;

  bp::child child(cmake_path.string() + " " + task,
                  boost::process::std_out > stream);
  _child = std::move(child);

  for (std::string line;
       _child.running() && std::getline(stream, line);) {
    std::cout << line << std::endl;
  }

  _child.wait();

  auto exit_code = _child.exit_code();

  if (exit_code != 0) {
    std::cout << "Non zero exit code. Exiting..." << std::endl;
    return false;
  } else {
    return true;
  }
}
