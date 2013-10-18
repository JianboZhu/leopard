// Author: Jianbo Zhu (jbzhu2011@gmail.com)
//
// Leopard is a smart(simple, functional, embeddable) online debug framework.
//

#ifndef CPP_LEOPARD_LEOPARD_LEOPARD_H_
#define CPP_LEOPARD_LEOPARD_LEOPARD_H_

#include "cpp_leopard/leopard/abstract_http_server.h"

namespace cpp_leopard {
namespace leopard {

// Get the default http server implemeted by Mongoose.
AbstractHttpServer& GetDefaultServer();

// Register all common handlers to 'server'
void RegisterCommonHandlers(AbstractHttpServer* server);

}  // namespace leopard
}  // namespace cpp_leopard

#endif  // CPP_LEOPARD_LEOPARD_LEOPARD_H_
