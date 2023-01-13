//
// Created by llonv on 2023/1/13.
//
#include <iostream>
#include <functional>

/**
 * C++11标准增强了命名空间的特性，提出了内联命名空间的概念。
 * 内联命名空间能够把空间内函数和类型导出到父命名空间中，
 * 这样即使不指定子命名空间也可以使用其空间内的函数和类型了
 */

namespace Parent {
    namespace Child1 {
        void foo() {
            std::cout << "Child1::foo()" << std::endl;
        }
    }
    inline namespace Child2 {
        void foo() {
            std::cout << "Child2::foo()" << std::endl;
        }
    }
}

// 使用内联命名空间进行导出
namespace Version{
    namespace V1 {
        void foo() {
            std::cout << "foo at V1" << std::endl;
        }
    }

    inline namespace V2 {
        void foo() {
            std::cout << "foo at V2" << std::endl;
        }
    }
}

int main() {
    using namespace Parent;

    // 调用导出的 Child2 foo
    foo();

    Child1::foo();

    Child2::foo();

    // 调用 Version
    Version::foo();

    Version::V1::foo();
}