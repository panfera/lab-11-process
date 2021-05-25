//
// Created by hacker on 5/17/21.
//
//Copyright 2021 pan_fera

#ifndef INCLUDE_BUILDER_HPP_
#define INCLUDE_BUILDER_HPP_
#include <iostream>
#include <boost/process.hpp>
#include <async++.h>
#include <string>
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

#endif  // INCLUDE_BUILDER_HPP_
