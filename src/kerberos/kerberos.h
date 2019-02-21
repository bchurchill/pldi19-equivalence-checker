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


#ifndef STOKE_KERBEROS_KERBEROS
#define STOKE_KERBEROS_KERBEROS

#include <cstdlib>
#include <iostream>

namespace stoke {

/** On kerberos systems with AFS, we have problems where we loose permissions during
  long-running jobs.  If we run these commands before any file I/O, the risk of this
  is much less. */

static void renew_kerberos_permissions() {

#ifdef STOKE_ENABLE_KERBEROS_RENEW

  /* Renew ticket granting ticket */
  auto code = system("kinit -R");
  if (code) {
    std::cerr << "kinit -R failed" << std::endl;
  }

  /* Renew AFS ticket */
  code = system("aklog");
  if (code) {
    std::cerr << "aklog failed" << std::endl;
  }

#endif

}

}
#endif
