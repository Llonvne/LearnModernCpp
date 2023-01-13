//
// Created by llonv on 2023/1/13.
//

#include "iostream"

/**
* C++17标准允许使用一种更简洁的形式描述嵌套命名空间
*/
namespace A::B::C {
int foo() {
  return 5;
}
}

// 如上写法等同于
// namespace A {
//    namespace B {
//        namespace C {
//            int foo2() { return 5; }
//        }
//    }
//}

// 在C++20中，我们可以这样定义内联命名空间：
namespace A::B::
inline D{
int foo() {
  return 5;
}

}

//等价于
namespace A {
namespace B {
inline namespace D {
int foo2() {
  return 5;
}
}
}
}

int main() {
  std::cout << __cplusplus << std::endl;
}