// Copyright 2020 BoryaBes <box.bern@yandex.ru>

#ifndef INCLUDE_HTMLDOWNLOADER_HPP_
#define INCLUDE_HTMLDOWNLOADER_HPP_
#include <boost/beast/http/message.hpp>
#include "vector"
#include "ThreadPool.h"
#include "htmlParser.hpp"
#include "iostream"
class htmlDownloader { //
 private:
  ThreadPool downloaderPool_; //пулл потоков для хранения ссылок по которым доуанлодер будет скачивать аштималь
  htmlParser& parserObj_; //ссылка на объект класса куда передаю аштимали
  void downloadPages(std::vector<url> URLs, int depth); //методу передаю массив ссылок и глубину, итерируется по каждому элнту массива(по каждой ссылке) и скачивает аштималь

 public:
  void startDownload(std::vector<url> URLs, int depth);
  htmlDownloader(htmlParser& parserObj, int threadNum) //конструктор иницилизируем приватные поля доунлоадПулл и парсер обжект
      : downloaderPool_(threadNum), parserObj_(parserObj) {}
};

#endif  // INCLUDE_HTMLDOWNLOADER_HPP_
