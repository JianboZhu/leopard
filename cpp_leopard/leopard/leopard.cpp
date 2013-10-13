// Author: Jianbo Zhu (jbzhu2011@gmail.com)

#include "cpp_leopard/leopard/leopard.h"

#include <boost/thread/once.hpp>
#include <glog/logging.h>

#include "cpp_leopard/leopard/abstract_http_server.h"
#include "cpp_leopard/leopard/handler/gflags_handler.h"
#include "cpp_leopard/leopard/mongoose_http_server.h"

namespace cpp_leopard {
namespace leopard {

namespace {

boost::once_flag once = BOOST_ONCE_INIT;

std::unique_ptr<AbstractHttpServer> g_default_server;

void InitDefaultServer() {
  g_default_server.reset(new MongooseHttpServer);
}

}  // namespace

AbstractHttpServer& GetDefaultServer() {
  // As its name, only call once at the first time
  boost::call_once(InitDefaultServer, once);

  return *g_default_server;
}

void RegisterCommonHandlers(AbstractHttpServer* server) {
  DCHECK(server != NULL) << "The http server is null!";

  server->RegisterHandler("/gflags/get_flags", GFlagsHandler::GetFlags);
  server->RegisterHandler("/gflags/set_flags", GFlagsHandler::SetFlags);
}

}  // namespace leopard
}  // namespace cpp_leopard
