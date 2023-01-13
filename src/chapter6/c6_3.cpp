//
// Created by 华邵钶 on 2023/1/13.
//
#include "bits/stdc++.h"
using namespace std;
struct X {
	X() = default;
	X(const X &) = default;
	X(X &&) {
		cout << "move ctor";
	}
};

auto f(X x) { return x; }
int main() {
	X r = f(X{});
	// move ctor

	// 函数f直接返回调用者传进来的实参x，
	// 在main函数中使用r接收f函数的返回值。
	// 关键问题是，这个赋值操作究竟是如何进行的。
	// 从代码上看，将r赋值为x应该是一个复制，对于旧时的标准这是没错的。
	// 但是对于支持移动语义的新标准，这个地方会隐式地采用移动构造函数来完成数据的交换。
	// 编译运行以上代码最终会显示move ctor字符串。
}