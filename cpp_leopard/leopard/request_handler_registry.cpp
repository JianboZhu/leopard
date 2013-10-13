// Author: Jianbo Zhu (jbzhu2011@gmail.com)

#include "cpp_leopard/leopard/request_handler_registry.h"

#include <set>
#include <string>

#include <boost/function.hpp>
#include <boost/unordered_map.hpp>

using std::set;
using std::string;

using boost::unordered_map;

namespace cpp_leopard {
namespace leopard {

RequestHandlerRegistry::RequestHandlerRegistry() {
}

RequestHandlerRegistry::~RequestHandlerRegistry() {
}

bool RequestHandlerRegistry::RegisterHandler(const string& name,
                                             const RequestHandler& handler) {
  boost::mutex::scoped_lock lock(mutex_);

  RequestHandlerMap::const_iterator iter = container_.find(name);
  if (iter != container_.end()) {
    // A handler called "name" has already existed, so return false;
    return false;
  }

  container_.insert({{name, handler}});

  return true;
}

void RequestHandlerRegistry::UnregisterHandler(const string& name) {
  boost::mutex::scoped_lock lock(mutex_);
  container_.erase(name);
}

RequestHandlerRegistry::RequestHandler RequestHandlerRegistry::Lookup(
    const string& name) const {
  boost::mutex::scoped_lock lock(mutex_);

  RequestHandlerMap::const_iterator iter = container_.find(name);
  if (iter == container_.end()) {
    return NULL;
  }

  return iter->second;
}

void RequestHandlerRegistry::GetHandlerNameSet(
    set<string>* handler_name_set) const {
  handler_name_set->clear();

  boost::mutex::scoped_lock lock(mutex_);

  for (RequestHandlerMap::const_iterator iter = container_.begin();
      iter != container_.end(); ++iter) {
    handler_name_set->insert(iter->first);
  }
}

}  // namespace leopard
}  // namespace cpp_leopard
