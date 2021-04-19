// Copyright 2020 BoryaBes <box.bern@yandex.ru>
#ifndef INCLUDE_OUTPUT_HPP_
#define INCLUDE_OUTPUT_HPP_
#include "ThreadPool.h"
#include "fstream"
#include "iostream"
#include "vector"
#include "string"
class outPut {
 private:
  std::string path_; //путь к файлу
  ThreadPool pool; //пуул для вывода в файл
  std::ofstream file; //объект в который записываю
  void writeFile(std::vector<std::string> imgs); //метод записи ссылок на картинки в файл

 public:
  explicit outPut(std::string path) : path_(path), pool(1), file(path) {} //конструктору передаю путь к файлу, инициализирую приватные поля
  void writeIMG(std::vector<std::string> imgs);
  ~outPut() { file.close(); }
};

#endif  // INCLUDE_OUTPUT_HPP_
