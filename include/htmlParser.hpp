// Copyright 2020 BoryaBes <box.bern@yandex.ru>
#ifndef INCLUDE_HTMLPARSER_HPP_
#define INCLUDE_HTMLPARSER_HPP_
#include "ThreadPool.h"
#include "iostream"
#include "output.hpp"
#include "string"
#include "vector"

class htmlDownloader;
struct url {
  std::string domen; //домен
  std::string uri; //адрес после названия сайта
};
class htmlParser { //
 private:
  ThreadPool parserPool_; //пулл потоков парсера
  outPut& outputObj_; //ссылка на объект отвечающий за вывод в файла
  void collectIMG(std::vector<std::string> pages, htmlDownloader& downloader,
                  int depth); //метод собирает ссылки на картинки

 public:
  htmlParser(outPut& outputObj, int threadNum) //инциализация полей через конструкторы
      : parserPool_(threadNum), outputObj_(outputObj) {}
  void startParse(std::vector<std::string> pages, htmlDownloader& downloader, //передаем массив аштималей, ссылку на объект аштималь даунлодера и глубину
                  int depth);

  /*static std::vector<std::string> collectLinks(std::vector<std::string>
  pages); void startParse(std::vector<std::string>& pages, ThreadPool&
  downloaderPool, ThreadPool& parserPool, ThreadPool& outputPool, std::string
  path);*/
};
#endif  // INCLUDE_HTMLPARSER_HPP_
