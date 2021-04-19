// Copyright 2020 BoryaBes <box.bern@yandex.ru>

#include "output.hpp"

#include <fstream>
#include <iostream>

#include "ThreadPool.h"
void outPut::writeFile(std::vector<std::string> imgs) {
  try { //пытаемся идти по каждой ссылке и записываем в файл, если ошибка то выводим ошибку
    for (auto& img : imgs) {
      file << img << std::endl;
    }
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

}

void outPut::writeIMG(std::vector<std::string> imgs) { //добавление новое задание в пулл потоков вывода
  pool.enqueue([imgs, this]() {
    this->writeFile(imgs);
  });
}
