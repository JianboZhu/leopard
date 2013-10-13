// Author: Jianbo Zhu (jbzhu2011@gmail.com)
//
// This is a common handler which can be applied to any application
// that uses gflags.
//
// Here, two types of actions are provided: read(GetAllFlags) and
// wirte(SetFlags).
//
// Usage:
// (1) GetFlags
//          Now, this handler supports getting flags of several specified
//      files as parameters passed to this handler. If there is no file
//      specified, it will return all the flags of the host application program,
//      even the 'leopard' project and the 'gflags' project itself.
//
//      A valid request url looks like:
//          "http://10.10.101.101:8081/gflags/get_gflags?file=hello_world.cpp
//          &file=world_hello.cpp"
//      Where, we want to get flags from 'hello_world.cpp' and
//      'world_hello.cpp'.
//      Actually, the key 'file' is useless, you can change it to any word
//      you want. Another point you need to know is that, the real filename
//      (path) recorded by 'gflags' may be 'example/hello_world.cpp',
//      a tricky detail behind the code is any sub-string of the real filename
//      works! That means, you can use 'hello_world' to get flags of
//      'example/hello_world.cpp', of course, other files contaning the
//      string 'hello_world' will be responsed too.
//
//  (2) SetFlags
//      This handler can be used to set a list of flags.
//      A valid request url can be like:
//          "http://10.10.101.10:8081/gflags/set_flags?log_file=/tmp/log_file&
//          thread_num=10&rate=0.12"
//      If sth wrong happened, a error message will be reponsed; otherwise,
//      a successful message will be sent to you.
//

#ifndef CPP_LEOPARD_LEOPARD_HANDLER_GFLAGS_HANDLER_H_
#define CPP_LEOPARD_LEOPARD_HANDLER_GFLAGS_HANDLER_H_

#include <string>
#include <utility>
#include <vector>

namespace cpp_leopard {
namespace leopard {

// NOTE:
// (1) this class can NOT be instantiated
// (2) and it's THREAD SAFE
class GFlagsHandler {
 public:
  // Get flags of the indicated 'targtet_files', specially, if 'target_files'
  // is not specified, it'll return all flags of the host project.
  //
  // Results are sorted by files' name in alphabetical order first,
  // and by flags' name in a second.
  static std::string GetFlags(
      const std::vector<std::pair<std::string, std::string> >& target_files);

  // Assign to one or more flags with new values.
  // An error or successful message will be returned.
  static std::string SetFlags(
      const std::vector<std::pair<std::string, std::string> >& new_flag_values);

 private:
  GFlagsHandler();
};

}  // namespace leopard
}  // namespace cpp_leopard

#endif  // CPP_LEOPARD_LEOPARD_HANDLER_GFLAGS_HANDLER_H_
