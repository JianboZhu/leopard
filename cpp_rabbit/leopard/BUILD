# Author: Jianbo Zhu (jbzhu2011@gmail.com)
#
# blade BUILD file for the project, rabbit in cpp version

cc_library(
    name = 'abstract_http_server',
    srcs = 'abstract_http_server.cpp',
    deps = [
      '#gflags',
      '#glog'
])

cc_library(
    name = 'rabbit',
    srcs = 'rabbit.cpp',
    deps = [ 
      ':mongoose_http_server',
      ':request_handler_registry',
      '//cpp_rabbit/rabbit/handler:gflags_handler',
      '#glog'
])

cc_library(
    name = 'mongoose_http_server',
    srcs = 'mongoose_http_server.cpp',
    deps = [
      ':abstract_http_server',
      '//cpp_rabbit/web_server/mongoose:mongoose',
      '#boost_system',
      '#boost_thread',
      '#gflags',
      '#glog'
])

cc_library(
    name = 'request_handler_registry',
    srcs = 'request_handler_registry.cpp',
    deps = [
      '#boost_system',
      '#boost_thread',
])
