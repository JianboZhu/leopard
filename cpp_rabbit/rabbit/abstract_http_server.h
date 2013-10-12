// Author: Jianbo Zhu (jbzhu2011@gmail.com)
//
// A C++ wrapper for a specific C/C++ web server; through this wrapper,
// Clients don't need care about which web server it is and any details
// about it. Besides, 'AbstractHttpServer' uses a registry to provides the
// registering and unregistering service.
//
// Any class implemented 'AbstractHttpServer' must implement the following two
// methods: 'Start' and 'Stop'.
//
// Usage:
//  std::unique_ptr<AbstractHttpServer> server(new MongooseHttpServer());
//
//  server->RegisterHandler("path1", handler1);
//  server->RegisterHandler("path2", handler2);
//
//  // Call the 'UnregisterHandler' method when you want to remove a handler.
//  server->UnregisterHandler(handler_request_path);
//
//  // Call the 'Lookup' method when you want to lookup a handler.
//  // Normally, you do not need this method.
//  RequestHandlerRegistry::RequestHandler handler =
//      server->Lookup(hanlder_request_path);
//
//  // NOTE: The calling order of 'Start' and registry functions
//  // does not matter. That means you can register a handler after
//  // the server started.
//  bool is_started = server->Start();
//  if (!is_started) {
//    // Do something
//  }
//
//  // Call the 'Stop' method when you want to shut dowm this web server.
//  server->Stop();
//

#ifndef CPP_RABBIT_RABBIT_ABSTRACT_HTTP_SERVER_H_
#define CPP_RABBIT_RABBIT_ABSTRACT_HTTP_SERVER_H_

#include <memory>
#include <utility>
#include <vector>

#include "cpp_rabbit/rabbit/request_handler_registry.h"

namespace cpp_rabbit {
namespace rabbit {

// NOTE:
//   These three methods are Thread Safe: 'RegisterHandler',
//   'UnregisterHandler' and 'Lookup'.
//   However, please notice that methods of 'Start' and 'Stop' in
//   MongooseHttpServer are implemented to be UNTHREAD SAFE.
class AbstractHttpServer {
 public:
  AbstractHttpServer();

  virtual ~AbstractHttpServer();

  // Start the http server
  bool Start();

  // Stop the http server
  void Stop();

  // Register a handler to the registry
  inline bool RegisterHandler(
      const std::string& name,
      const RequestHandlerRegistry::RequestHandler& handler) {
    return registry_->RegisterHandler(name, handler);
  }

  // Remove a handler from the registry
  inline void UnregisterHandler(const std::string& name) {
    registry_->UnregisterHandler(name);
  }

  // Lookup a handler in the registry
  inline RequestHandlerRegistry::RequestHandler Lookup(
      const std::string& name) const {
    return registry_->Lookup(name);
  }

 protected:
  std::unique_ptr<RequestHandlerRegistry> registry_;

 private:
  // Get a manifest of all registered handlers
  std::string GetHandlerManifest(
      const std::vector<std::pair<std::string, std::string> >&
      parameters) const;

  // Implement the main logic of 'Start'
  virtual bool DoStart(const std::string& listening_ports) = 0;

  // Implement the main logic of 'Stop'
  virtual void DoStop() = 0;
};

}  // namespace rabbit
}  // namespace cpp_rabbit

#endif  // CPP_RABBIT_RABBIT_ABSTRACT_HTTP_SERVER_H_
