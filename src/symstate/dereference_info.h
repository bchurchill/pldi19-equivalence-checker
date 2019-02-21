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


#ifndef STOKE_SRC_SYMSTATE_DEREFINFO_H
#define STOKE_SRC_SYMSTATE_DEREFINFO_H

namespace stoke {

class DereferenceInfo {

public:

  DereferenceInfo() {
    line_number = 0;
    invariant_number = 0;
    is_invariant = false;
    is_rewrite = false;
    implicit_dereference = false;
    stack_dereference = false;
  }

  size_t line_number;
  size_t invariant_number;
  bool is_invariant;
  bool is_rewrite;
  bool implicit_dereference;
  bool stack_dereference;

  bool operator<(const DereferenceInfo& other) const {

    if (line_number != other.line_number)
      return line_number < other.line_number;

    if (invariant_number != other.invariant_number)
      return invariant_number < other.invariant_number;

    if (is_invariant != other.is_invariant)
      return is_invariant < other.is_invariant;

    if (is_rewrite != other.is_rewrite)
      return is_rewrite < other.is_rewrite;

    if (implicit_dereference != other.implicit_dereference)
      return implicit_dereference < other.implicit_dereference;

    return false;
  }

};

typedef std::map<DereferenceInfo, uint64_t> DereferenceMap;
typedef std::vector<DereferenceMap> DereferenceMaps;

};

#endif
