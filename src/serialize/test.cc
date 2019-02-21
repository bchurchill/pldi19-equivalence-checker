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



#include "serialize.h"

#include <iostream>
#include <vector>

using namespace std;

struct A {

  static int counter;

  A() {
    me = ++counter;
  }

  void serialize(ostream& os) const {
    os << me << endl;
  }

  static A deserialize(istream& is) {
    A a;
    is >> a.me;
    return a;
  }

  int me;

};

int A::counter = 0;

int main(int argc, char** argv) {
  if (argc == 1) {
    stoke::serialize(cout, 5);
    vector<int> x = { 2, 3, 4};
    stoke::serialize(cout, x);
    vector<A> as = { A(), A(), A() };
    stoke::serialize(cout, as);
    A b;
    stoke::serialize(cout, b);
  } else {
    int x = stoke::deserialize<int>(cin);
    cout << "5 = " << x << endl;
    auto ys = stoke::deserialize<vector<int>>(cin);
    cout << "vector = ";
    for (auto it : ys)
      cout << it << endl;
    auto as = stoke::deserialize<vector<A>>(cin);
    cout << "as = " << endl;
    for (auto it : as)
      cout << it.me << endl;
    A b = stoke::deserialize<A>(cin);
    cout << "b = " << b.me << endl;
  }
}

