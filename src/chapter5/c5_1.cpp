//
// Created by 华邵钶 on 2023/1/13.
//
// 函数返回类型
#include "bits/stdc++.h"
using namespace std;
auto foo() -> int {
	return 42;
}
// 以上代码中的函数声明等同于int foo()，
// 只不过采用了函数返回类型后置的方法，
// 其中auto是一个占位符，函数名后->紧跟的int才是真正的返回类型,
// 而在返回类型比较复杂的时候，比如返回一个函数指针类型，返回类型后置可能会是一个不错的选择

namespace bar {
int bar_impl(int x) {
	cout << x << endl;
	return x;
}
typedef int(*bar)(int);
bar foo1() {
	return bar_impl;
}
auto foo2() -> int (*)(int) {
	return bar_impl;
}
/**
 * 函数foo2的返回类型不再是简单的int而是函数指针类型。
 * 使用传统函数声明语法的foo1无法将函数指针类型作为返回类型直接使用，
 * 所以需要使用typedef给函数指针类型创建别名bar，再使用别名作为函数foo1的返回类型。
 * 而使用函数返回类型后置语法的foo2则没有这个问题。
 * 同样，auto作为返回类型占位符，在->后声明返回的函数指针类型int(*)(int)即可
 */
void use() {
	auto func = foo2();
	func(58);
}
}

int main() {
	bar::use();
}
