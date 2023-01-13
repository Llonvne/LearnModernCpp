//
// Created by llonv on 2023/1/13.
//
// C++11标准赋予了auto新的含义：声明变量时根据初始化
// 表达式自动推断该变量的类型、声明函数时函数返回值的占位符。

auto i = 5; // -> int
auto str = "hello auto"; // const char *
auto sum(int a1, int a2) -> int { // 推到类型后置，auto为占位符
  return a1 + a2;
}

// auto要求编译器自动完成变量类型的推导工作
// 编译器必须要能够在编译期间推导出类型，否则将会编译失败

// 使用auto占位符声明变量的时候必须初始化变量
/**
* auto i;
* i = 5;
*/

// 当用一个auto关键字声明多个变量的时候，编译器遵从由左往右的推导规则，以最左边的表达式推断auto的具体类型
auto n = 5;
auto *pn = &n,m = 10;
// &pn 类型是 int *, auto 被推导为 int, 于是 m 被声明为 int

// 以下将无法通过编译
//auto *pn2 = &n,m2 = 10.0;

// 编译器总是使用表达能力更强的类型
auto i2 = true ? 5 : 8.0; // i -> double

// auto 与 new (我不建议像上面这样使用auto，因为它会破坏代码的可读性)
auto int1 = new auto(1);
auto * intptr = new auto(1);