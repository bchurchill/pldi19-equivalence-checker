// Copyright 2013-2019 Stanford University
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef STOKE_SRC_SERIALIZE_CHECK_STREAM_H
#define STOKE_SRC_SERIALIZE_CHECK_STREAM_H

#define CHECK_STREAM(is) { if(is.bad() || is.fail()) { \
                            std::cerr << "parsing failure at "        \
                                      << __FILE__ << ":" << __LINE__  \
                                      << std::endl;                   \
                            exit(1);                                  \
                          } else {                                    \
                            is >> std::dec;                           \
                          } }

#define CHECK_STREAM_RET(is) { if(is.bad() || is.fail()) { \
                            std::cerr << "parsing failure at "        \
                                      << __FILE__ << ":" << __LINE__  \
                                      << std::endl;                   \
                            return(is);                               \
                          } else {                                    \
                            is >> std::dec;                           \
                          } }

#define CHECK_STREAM_RET_VOID(is) { if(is.bad() || is.fail()) { \
                            std::cerr << "parsing failure at "        \
                                      << __FILE__ << ":" << __LINE__  \
                                      << std::endl;                   \
                            return;                                   \
                          } else {                                    \
                            is >> std::dec;                           \
                          } }

#endif
