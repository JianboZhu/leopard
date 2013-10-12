// Author: Jianbo Zhu (jbzhu2011@gmail.com)
//
// Rabbit is a smart(simple, functional, embeddable) online debug framework.
//

#ifndef CPP_RABBIT_RABBIT_RABBIT_H_
#define CPP_RABBIT_RABBIT_RABBIT_H_

#include "cpp_rabbit/rabbit/abstract_http_server.h"

namespace cpp_rabbit {
namespace rabbit {

// Get the default http server implemeted by Mongoose.
AbstractHttpServer& GetDefaultServer();

// Register all common handlers to 'server'
void RegisterCommonHandlers(AbstractHttpServer* server);

}  // namespace rabbit
}  // namespace cpp_rabbit

#endif  // CPP_RABBIT_RABBIT_RABBIT_H_