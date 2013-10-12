// Author: Jianbo Zhu (jbzhu2011@gmail.com)

#include "cpp_rabbit/rabbit/mongoose_http_server.h"

#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include "cpp_rabbit/web_server/mongoose/mongoose.h"

#include "cpp_rabbit/rabbit/request_handler_registry.h"

// NOTE:
//   '/tmp/mongoose_error_log_file' is just an example,
//   and strongly recommend you to change it.
DEFINE_string(mongoose_error_log_file, "/tmp/mongoose_error_log_file",
              "The log file which Mongoose records error messages in.");
DEFINE_string(mongoose_worker_threads, "1",
              "The number of worker threads to handle requests");

using std::pair;
using std::string;
using std::vector;

namespace cpp_rabbit {
namespace rabbit {

namespace {
// Parse a string in a form of "n1=v1&n2=v2" into a list of pairs in a
// form of "<n1, v1>, <n2, v2>".
void GetParameters(const char* query,
                   vector<pair<string, string> >* const parameters) {
  parameters->clear();

  if (query == NULL) {
    return;
  }

  string query_string(query);

  vector<string> pairs;
  boost::split(pairs, query_string, boost::is_any_of("&"));

  for (vector<string>::const_iterator iter = pairs.begin();
      iter != pairs.end(); ++iter) {
     vector<string> key_value(2);
     boost::split(key_value, *iter, boost::is_any_of("="));

     if (key_value.size() != 2) {
      continue;
     }

     parameters->push_back(std::make_pair(key_value[0], key_value[1]));
  }
}

// Implement the 'begin_request_handler' function of Mongoose's callbacks
int begin_request_handler(struct mg_connection *conn) {
  const struct mg_request_info *ri = mg_get_request_info(conn);

  // Get the request path
  const string request_path(ri->uri);

  MongooseHttpServer* mg_http_server = (MongooseHttpServer*)(ri->user_data);
  RequestHandlerRegistry::RequestHandler handler =
    mg_http_server->Lookup(request_path);

  if (handler == NULL) {
    LOG(ERROR) << "No handler is registered for path: " << request_path;

    return 0;
  }

  vector<pair<string, string> > parameters;
  GetParameters(ri->query_string, &parameters);

  const string result = handler(parameters);

  mg_printf(conn, "HTTP/1.0 200 OK\r\nContent-Type: text/plain\r\n\r\n%s\n",
            result.c_str());

  return 1;
}

}  // namespace

MongooseHttpServer::MongooseHttpServer() {
  ctx_ = NULL;
}

MongooseHttpServer::~MongooseHttpServer() {
  if (ctx_) {
    Stop();
  }
}

bool MongooseHttpServer::DoStart(const string& listening_ports) {
  if (ctx_) {
    LOG(WARNING) << "Mongoose has already been started, just return.";
    return true;
  }

  const char* options[] = {
    "listening_ports", listening_ports.c_str(),
    "error_log_file", FLAGS_mongoose_error_log_file.c_str(),
    "num_threads", FLAGS_mongoose_worker_threads.c_str(),
    NULL};

  struct mg_callbacks callbacks;
  memset(&callbacks, 0, sizeof(callbacks));
  callbacks.begin_request = begin_request_handler;

  ctx_ = mg_start(&callbacks, this, options);

  if (ctx_ == NULL) {
    LOG(ERROR) << "Failed to start Mongoose!";

    return false;
  }

  return true;
}

void MongooseHttpServer::DoStop() {
  if (ctx_ == NULL) {
    return;
  }

  mg_stop(ctx_);
  ctx_ = NULL;
}

}  // namespace rabbit
}  // namespace cpp_rabbit
