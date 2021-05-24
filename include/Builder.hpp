//
// Created by hacker on 5/17/21.
//

#ifndef TEMPLATE_BUILDER_HPP
#define TEMPLATE_BUILDER_HPP
#include <iostream>
#include <boost/process.hpp>
#include <async++.h>
class Builder {
 public:
  Builder(std::string config, bool install, bool pack, int timeout);
  bool NewTask(std::string task);
 private:
  std::string  _config;
  bool _install, _pack;
  int _timeout;
  boost::process::child _child;
};

#endif  // TEMPLATE_BUILDER_HPP
