//
// Created by 华邵钶 on 2023/1/13.
//
#include "iostream"
using namespace std;

namespace DerivingReturnType {
// C++14标准支持对返回类型声明为auto的推导
auto sum(int a1, int a2) { return a1 + a2; }
// 请注意，如果有多重返回值，那么需要保证返回值类型是相同的
}

namespace ParadigmaticAnonymousFunctions {

// 泛型的lambda表达式
auto l = [](auto a1, auto a2) {
	return a1 + a2;
};

// lambda表达式返回auto引用
auto l2 = [](auto &i) -> auto & { return ++i; };

void use() {
	// a1 -> int a2 -> double return -> double
	std::cout << l(1, 2.0) << std::endl;
	// a1 -> double a2 -> int return -> double
	std::cout << l(2.0, 1) << std::endl;

	int j = 3;
	std::cout << l2(j) << std::endl;
	std::cout << j << std::endl;
}
}

//  非类型模板形参占位符
namespace UntypedTemplateParameterPlaceholders {
template<auto N>
void f() {
	cout << N << endl;
}
void use() {
	f<5>();
	f<'c'>();
	// 模版参数不能为 double
//	f<5.0>();
}
}

int main() {
//	ParadigmaticAnonymousFunctions::use();
	UntypedTemplateParameterPlaceholders::use();
	return 0;
}