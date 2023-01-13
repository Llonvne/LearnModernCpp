//
// Created by llonv on 2023/1/13.
//
// 2.1　内联命名空间的定义和使用

namespace S1 {
    void foo() {}
}

namespace S2 {
    void foo() {}
}

using namespace S1;

int main_() {

    // 将使用 S1 的 foo 函数
    foo();

    // 使用全限定名来调用 S2 的 foo
    S2::foo();
}