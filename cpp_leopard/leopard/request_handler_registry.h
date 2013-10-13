// Author: Jianbo Zhu (jbzhu2011@gmail.com)
//
// RequestHandlerRegistry is a registry that maps names to the
// corresponding registered objects.
// A server can register objects with the registry, so that
// they can be looked up.
// When somewhere wishes to invoke a registered object, it must
// first lookup this registry using its name. The registry returns
// a reference to the registered object.

#ifndef CPP_LEOPARD_LEOPARD_REQUEST_HANDLER_REGISTRY_H_
#define CPP_LEOPARD_LEOPARD_REQUEST_HANDLER_REGISTRY_H_

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <boost/function.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/unordered_map.hpp>

#include "cpp_leopard/util/macros.h"

namespace cpp_leopard {
namespace leopard {

// NOTE: This class is THREAD SAFE
// actually, now we use boost::mutex to provide thread-safe mechanism,
// however, my original intention is using a concurrent container which
// is more efficient, to achieve the goal of thread-safe. there is indeed
// some implementations of concurrent container like TBB from Intel.
// If we need a improvement of efficiency in the future, try those concurrent
// containers.
class RequestHandlerRegistry {
 public:
  typedef boost::function1<std::string,
    const std::vector<std::pair<std::string, std::string> >&> RequestHandler;

  typedef boost::unordered_map<std::string, RequestHandler> RequestHandlerMap;

  RequestHandlerRegistry();
  ~RequestHandlerRegistry();

  // Register a handler to this registry
  bool RegisterHandler(const std::string& name, const RequestHandler& handler);

  // Remove a handler from this registry
  void UnregisterHandler(const std::string& name);

  // Looks up a handler using its name.
  // If found, return it; otherwise return null.
  RequestHandler Lookup(const std::string& name) const;

  // Get a set of handlers' name
  void GetHandlerNameSet(std::set<std::string>* handler_name_set) const;

 private:
  // An container where store a handler into and retrieve a handler from
  RequestHandlerMap container_;

  // Provide the thread safe mechanism.
  // A shared_mutex is better, but I wish to use the
  // 'scoped_lock' feature in 'mutex'.
  mutable boost::mutex mutex_;

  DISABLE_COPY_AND_ASSIGN(RequestHandlerRegistry);
};

}  // namespace leopard
}  // namespace cpp_leopard

#endif  // CPP_LEOPARD_LEOPARD_REQUEST_HANDLER_REGISTRY_H_
