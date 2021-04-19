// Copyright 2020 BoryaBes <box.bern@yandex.ru>
#include "htmlDownloader.hpp"

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

#include "htmlParser.hpp"
#include "iostream"
namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

void htmlDownloader::startDownload(std::vector<url> URLs, int depth) {
  downloaderPool_.enqueue(
      [this, URLs, depth]() { this->downloadPages(URLs, depth); }); //добавляет в пулл новую задачу(таск), которая запускает метод даунлоад пэджес
}

void htmlDownloader::downloadPages(std::vector<url> URLs, int depth) { //принимает массив юрэлов и глубину
  if (depth < 0) {
    return;
  }
  if (URLs.empty()) { //провреяем не пустой ли массив ссылок
    std::cerr << "Empty URLs" << std::endl;
  }
  std::vector<std::string> htmlPages; //инициализируем массив где будут храниться аштималями
  for (auto& url : URLs) { //итерируюсь по ссылке в массиве
    net::io_context ioc; //все ниже это код клиента

    tcp::resolver resolver(ioc);
    beast::tcp_stream stream(ioc);
    auto const results = resolver.resolve(url.domen, "80");
    try { //пробую подключиться к ссылке
      stream.connect(results); //устанавливаю соединение
    } catch (std::exception& e) { //если ссылка нерабочая то вывожу ошибку
      std::cerr << e.what() << std::endl;
      continue;
    }

    http::request<http::string_body> req{http::verb::get, url.uri, 10}; //методом гет получаю аштималь по ссылке
    req.set(http::field::content_type, "text/plain"); //заголовки
    req.set(http::field::host, "http-client-sync");

    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    http::write(stream, req); //записываю свой запрос в соединение
    beast::flat_buffer buffer; //буффер для получения ответа нашего аштималя
    http::response<http::string_body> res; //переменная для хранения ответа
    http::read(stream, buffer, res); //читаю ответ

    beast::error_code ec;
    stream.socket().shutdown(tcp::socket::shutdown_both, ec);
    if (ec && ec != beast::errc::not_connected) throw beast::system_error{ec};
    htmlPages.push_back(res.body()); //в рес записался ответа сервака(аштималь) аштималь записываю в массив
  }

  this->parserObj_.startParse(htmlPages, *this, depth); //объект в котором нахожусь при инициализации передал парсерОбж у которого есть метод тартПарс
}
