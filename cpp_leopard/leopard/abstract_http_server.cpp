// Author: Jianbo Zhu (jbzhu2011@gmail.com)
//

#include "cpp_leopard/leopard/abstract_http_server.h"

#include <set>
#include <vector>

#include <boost/bind.hpp>
#include <gflags/gflags.h>
#include <glog/logging.h>

// NOTE:
//    (1) If listening_ports is specified as '0', random listening ports
//        will be generated and registered to zookeeper.
//    (2) You can specify more than one port using a string seperated by
//        comma, for instance, "8081, 8082, 8083". Of course, a string like
//        "0, 0, 0, 0" is ok becasue four random ports will be generated.
DEFINE_string(leopard_listening_ports, "0", "Listening ports of the embedded http server.");

using std::pair;
using std::set;
using std::string;
using std::vector;

namespace cpp_leopard {
namespace leopard {
AbstractHttpServer::AbstractHttpServer() :registry_(
     new RequestHandlerRegistry()) {
  // 'handler_manifest_handler' is registered in default
  RegisterHandler(
      "/manifest",
      boost::bind(&AbstractHttpServer::GetHandlerManifest, this, _1));
}

AbstractHttpServer::~AbstractHttpServer() {}

bool AbstractHttpServer::Start() {
  const bool started = DoStart(FLAGS_leopard_listening_ports);

  return started;
}

void AbstractHttpServer::Stop() {
  DoStop();
}

string AbstractHttpServer::GetHandlerManifest(
    const vector<pair<string, string> >& parameters) const {
  set<string> handler_name_set;
  registry_->GetHandlerNameSet(&handler_name_set);

  string response;
  for (set<string>::const_iterator iter = handler_name_set.begin();
      iter != handler_name_set.end(); ++iter) {
    response.append(*iter)
            .append("\n");
  }

  return response;
}

}  // namespace leopard
}  // namespace cpp_leopard
