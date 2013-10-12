// Author: Jianbo Zhu (jbzhu2011@gmail.com)
//
// An implementation of 'AbstractHttpServer' using Mongoose: a simple,
// functional and embeddable web server in C language.
//
// NOTE:
//   The form of a request url to this web server should like this:
//   "http://10.10.143.237:8081/debug/order_item?key1=value1&key2=value2"
//
//   There are two points need to explain:
//      (1) A valid request path corresponding to a handler refers to the
//      substring between the listening port and the first '?' symbol.
//      (2) A valid parmater list refers to the substring after the first '?'
//      symbol.
//
//      Take the above url as an example, where, '/debug/order_item' is the
//      request path, and 'key1=value1&key2=value2' is the parameter list.
//

#ifndef CPP_RABBIT_RABBIT_MONGOOSE_HTTP_SERVER_H_
#define CPP_RABBIT_RABBIT_MONGOOSE_HTTP_SERVER_H_

#include "cpp_rabbit/util/macros.h"

#include "cpp_rabbit/rabbit/abstract_http_server.h"

struct mg_context;

namespace cpp_rabbit {
namespace rabbit {

// NOT THREAD SAFE.
// It CAN be used in a multithread enviroment but the same instance
// can't be operated by different threads in the same time.
class MongooseHttpServer: public AbstractHttpServer {
 public:
  MongooseHttpServer();
  ~MongooseHttpServer();

 private:
  // Records the context of mongoose.
  struct mg_context* ctx_;

  // Start the http server
  bool DoStart(const std::string& listening_ports);

  // Stop the http server
  void DoStop();

  DISABLE_COPY_AND_ASSIGN(MongooseHttpServer);
};

}  // namespace rabbit
}  // namespace cpp_rabbit

#endif  // CPP_RABBIT_RABBIT_MONGOOSE_HTTP_SERVER_H_