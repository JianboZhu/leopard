// Author: Jianbo Zhu (jbzhu2011@gmail.com)
//

#include "cpp_rabbit/rabbit/handler/gflags_handler.h"

#include <utility>

#include <gflags/gflags.h>
#include <glog/logging.h>

#include "cpp_rabbit/util/basic_types.h"

using std::pair;
using std::string;
using std::vector;

using google::CommandLineFlagInfo;

namespace cpp_rabbit {
namespace rabbit {

namespace {
// Try to find a flag in 'flags_info' which contains all flags of its host
// application program. If found, return true; otherwise, return false.
inline bool FindFlag(const string& flag_name,
                     const vector<CommandLineFlagInfo>& flags_info) {
  for (vector<CommandLineFlagInfo>::const_iterator iter = flags_info.begin();
      iter != flags_info.end(); ++iter) {
    if (flag_name != iter->name) {
      continue;
    }

    return true;
  }

  return false;
}

inline bool MatchFile(const vector<pair<string, string> >& target_files,
                      const string& file_name) {
  // If there is no 'target_files', print all flags
  if (target_files.empty()) {
    return true;
  }

  for (vector<pair<string, string> >::const_iterator iter =
      target_files.begin(); iter != target_files.end(); ++iter) {
    if (file_name.find(iter->second) != string::npos) {
      return true;
    }
  }

  return false;
}

inline void AppendFileName(const string& file_name, string* response) {
  // Insert a blank line between flags of two files.
  if (*response != "") {
    response->append("\n");
  }

  response->append(file_name)
          .append(":\n****************************************\n");
}

inline void AppendFlagInfo(const string& flag_name,
                           const string& flag_value,
                           string* response) {
  response->append(flag_name)
          .append(" = ")
          .append(flag_value)
          .append("\n");
}

}  // namespace

string GFlagsHandler::GetFlags(
    const vector<pair<string, string> >& target_files) {
  vector<CommandLineFlagInfo> flags_info;
  // This function is thread safe
  google::GetAllFlags(&flags_info);

  string response;
  bool is_file_name_matched = false;
  string current_file_name;
  for (vector<CommandLineFlagInfo>::const_iterator iter = flags_info.begin();
      iter != flags_info.end(); ++iter) {
    const string& file_name = iter->filename;

    if (current_file_name == file_name) {
      // If 'target_files' are specified, only print flags of 'target_files'
      if (is_file_name_matched) {
        AppendFlagInfo(iter->name, iter->current_value, &response);

        continue;
      }
    }

    current_file_name = file_name;

    // Should flags of this file be presented?
    is_file_name_matched = MatchFile(target_files, file_name);
    if (!is_file_name_matched) {
      continue;
    }

    AppendFileName(file_name, &response);
    AppendFlagInfo(iter->name, iter->current_value, &response);

  }

  return response;
}

string GFlagsHandler::SetFlags(
    const vector<pair<string, string> >& new_flag_values) {
  vector<CommandLineFlagInfo> flags_info;
  google::GetAllFlags(&flags_info);

  string error_msg;
  for (vector<pair<string, string> >::const_iterator iter =
      new_flag_values.begin(); iter != new_flag_values.end(); ++iter) {
    if (!FindFlag(iter->first, flags_info)) {
      // Failed to find this flag
      error_msg.append("This flag does not exist: ")
               .append(iter->first)
               .append("\n");

      continue;
    }

    // It's thread safe too
    LOG(INFO) << google::SetCommandLineOptionWithMode(iter->first.c_str(),
                                                      iter->second.c_str(),
                                                      google::SET_FLAGS_VALUE);
  }

  return error_msg == "" ? "OK" : error_msg;
}

}  // namespace rabbit
}  // namespace cpp_rabbit