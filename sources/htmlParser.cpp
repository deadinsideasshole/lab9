// Copyright 2020 BoryaBes <box.bern@yandex.ru>

#include "htmlParser.hpp"

#include <gumbo.h>

#include "htmlDownloader.hpp"
#include "iostream"
#include "output.hpp"

static void search_for_images(GumboNode* node, std::vector<std::string>& imgs) { //указатель на гамбо*нод,Прямое объявление GumboNode, чтобы его можно было рекурсивно использовать в GumboNode.parent.
  if (node->type != GUMBO_NODE_ELEMENT) { //парсим тэги (гамбонотэлемент это любой тэг) ТОЧКА ВЫХОДА ИЗ РЕКУРСИВНОЙ ФУНКЦИИ КОТОРАЯ ИДЕТ В 21-23 СТРОКЕ
    return;
  }
  GumboAttribute* src; //Гамбоатрибут - Структура, представляющая один атрибут в теге HTML. Это пара «имя-значение», но она также включает информацию об исходных местоположениях и исходном исходном тексте.
  if (node->v.element.tag == GUMBO_TAG_IMG && //идет по всем встречающимся тэгам и проверяет, если тэг имг и эсэрси(название атрибута который хочу взять из массива)
      (src = gumbo_get_attribute(&node->v.element.attributes, "src"))) { //v.element.attributes это Массив GumboAttributes, содержащий атрибуты для этого тега в том порядке, в котором они были проанализированы.
    imgs.push_back(src->value); // достаю из объекта значение атрибута эсэрси и кладу в массив ссылок на картинки
  }

  GumboVector* children = &node->v.element.children; //рекурсивная функция
  for (unsigned int i = 0; i < children->length; ++i) {
    search_for_images(static_cast<GumboNode*>(children->data[i]), imgs); //ф-ия вызывает сама себя
  }
}

static void search_for_links(GumboNode* node, std::vector<url>& links) {
  if (node->type != GUMBO_NODE_ELEMENT) {
    return;
  }
  GumboAttribute* href; // ВМЕСТО ИМГ - А, вместо эсэрси Хреф
  if (node->v.element.tag == GUMBO_TAG_A && //проверяю что тэг А
      (href = gumbo_get_attribute(&node->v.element.attributes, "href")) && //ПЕРЕМЕННОЙ ХРЕФ ПРИСВАИВАЮ ТО ЧТО ХРАНИТСЯ В АТРИБУТЕ А
      (std::string(
          gumbo_get_attribute(&node->v.element.attributes, "href")->value) //с помощью стрлеки вэлью преобразуем в строку к которой можем обратиться
           .substr(0, 4) == "http") &&
      (std::string(
          gumbo_get_attribute(&node->v.element.attributes, "href")->value)
           .substr(0, 5) != "https")  //не должно быть равно аштипипиэс(только аштитипи)
     ) {
    url urlLink = url(); //объект структуры юэрэл, это для передачи в клиент отдельно домена и адреса на сайте
    std::string host = href->value; //храним полную ссылка
    if (host.find("http://") == 0) host = host.substr(7); //файнд метод ищет подстроку в строке которю мы задали,обрезаем заданную строку
    size_t i = 0; //используем не только в цикле поэтому инициализируем над ним
    if (host.size() != 0) {
      for ( ;i < host.size(); ++i) { //иду по каждому символу строки хост до слэша или вопроса то выхожу из цикла и в Ай сохраняется индект слэша или вопрос
        if ((host[i] == '/') || (host[i] == '?')) break;
      }
    } else {
      return;
    }
    urlLink.domen = host.substr(0, i); //беру символы с 0 до слэша(вырезаем название сайта)

    std::string target = href->value; //в таргет кладем всю ссылку ищем в нем аштитипи и выразем аштитипи, осталвяем сайт и адрес на сайте
    if (target.find("http://") == 0) {
      target = target.substr(7);
    }
    int pos = target.find('/'); //переменная пос равна индексу первого слэша
    if (pos == -1) { //если адрес слыша -1(последний элемент) то
      urlLink.uri = "/"; //в юрэй устанавливаем слэш
    } else {
      urlLink.uri = target.substr(pos); // если элемент не последний то вырзаем подстроку начиная с первого слэша
    }

    links.push_back(urlLink); //добавляю в массив ссылок объект юрллинк
  }

  GumboVector* children = &node->v.element.children;
  for (unsigned int i = 0; i < children->length; ++i) {
    search_for_links(static_cast<GumboNode*>(children->data[i]), links);
  }
}

void htmlParser::collectIMG(std::vector<std::string> pages,
                            htmlDownloader& downloader, int depth) {
  std::vector<std::string> imgs; //создаем массив строк в котором хранятся ссылки на картинки
  if (pages.empty()) { //если массив пустой то ниче не вернем
    return;
  }
  for (const auto& page : pages) { //идем по каждому аштималю в цикле
    GumboOutput* output = gumbo_parse(page.c_str()); //создаем указатель на объект типа ГамбоАутпут, конструктор гамбопарс ему передаем наш аштималь(си эстр преобразует строку в строку типа Си(в конце константный ноль)
    search_for_images(output->root, imgs); //документация гамбопарсера есть в примерах, руут указатель на корневой узел,(тэг образующий корень документа) и ссылку на массив с картинками
    gumbo_destroy_output(&kGumboDefaultOptions, output); //дестройаутпут освобождает память,Освободите память используемую для дерева синтаксического анализа и ошибок синтаксического анализа.
  }
  outputObj_.writeIMG(imgs); //отвечает за вывод ссылок на картинки в файл
  std::vector<url> links; //собираю ссылки которые передаю в доунлодер(ссылки не на картинки)
  for (const auto& page : pages) {
    GumboOutput* output = gumbo_parse(page.c_str());
    search_for_links(output->root, links); //парсю тэги а(просто ссылки)
    gumbo_destroy_output(&kGumboDefaultOptions, output);
  }

  downloader.startDownload(links, depth - 1); //захожу в каждую ссылку по тэга а
}

void htmlParser::startParse(std::vector<std::string> pages, //передаем аштимали,ссылку на даунлоадер,глубину
                            htmlDownloader& downloader, int depth) {
  parserPool_.enqueue([this, pages, depth, &downloader]() { //у пулла потоков один метод - юникву, который записыает задачу в пулл потока
    this->collectIMG(pages, downloader, depth);
  });
}
