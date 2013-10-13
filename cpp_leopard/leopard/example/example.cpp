// Author: Jianbo Zhu (jbzhu2011@gmail.com)
//
// An example to show how to use 'leopard'
//

#include <string>
#include <utility>
#include <vector>

#include <boost/bind.hpp>

#include "cpp_leopard/leopard/leopard.h"

using namespace cpp_leopard::leopard;
using namespace std;

// Note:
// For convenience, this file unit does not follow the
// google coding style in a way.

typedef vector<pair<string, string> > vector_pair_type;

string non_member_function(const vector_pair_type& parameters) {
  return "A non-member function";;
}

class Foo {
 public:
  string operator() (const vector_pair_type& parameters) {
    return "A function object";
  }

  string NonStaticMemberFunction(const vector_pair_type& parameters) const {
    return "A non-static member function";
  }

  static string StaticMemberFunction(const vector_pair_type& parameters) {
    return "A static member function";
  }
};

int main() {
  AbstractHttpServer& server = GetDefaultServer();
  RegisterCommonHandlers(&server);

  if (!server.Start()) {
    return -1;
  }

  // Register a free function(non-member function)
  server.RegisterHandler("/function/non_member_function",
                         non_member_function);

  // Register a functor(function object)
  server.RegisterHandler("/function/function_object", Foo());

  // Register a static member function
  server.RegisterHandler("/function/static_member_function",
                         Foo::StaticMemberFunction);

  // Register a member function with boost::bind or std::bindxxx
  // Note: I have a while loop below, so the local var 'foo' will
  // never be destroyed. It's just an example here. Beware don't
  // do this in your project.
  Foo foo;
  server.RegisterHandler("/function/non_static_member_function",
                         boost::bind(&Foo::NonStaticMemberFunction, &foo, _1));

  while (true) {
    sleep(10);
  }
}
