//
// Created by 华邵钶 on 2023/1/13.
// decltype
#include "bits/stdc++.h"
using namespace std;
namespace typeofAndTypeid {
int a = 0;
//typeof(a) b = 5;
// GCC的扩展提供了一个名为typeof的运算符。通过该运算符可以获取操作数的具体类型。
// 这让使用GCC的程序员在很早之前就具有了对对象类型进行推导的能力

void use() {
	double x2 = 5.5;
	cout << typeid(a).name() << endl;
	cout << typeid(x2).name() << endl;

//	auto k {typeid(1)};
	auto &k{typeid(1)};
	cout << k.name();
}

// typeid的返回值是一个左值，且其生命周期一直被扩展到程序生命周期结束
// typeid返回的std::type_info删除了复制构造函数，
// 若想保存std::type_info，只能获取其引用或者指针
// typeid的返回值总是忽略类型的 cv 限定符，也就是typeid(const T)==typeid(T)
}

namespace useDecltype {

namespace basic {
// 为了用统一方法解决上述问题，C++11标准引入了decltype说明符，
// 使用decltype说明符可以获取对象或者表达式的类型，其语法与typeof类似
int x1 = 0;
decltype(x1) x2 = 0;

double x3 = 0;
decltype(x1 + x3) x4 = 1.0;

//decltype({1,3}) x5;  {1,5} 不是表达式

// 在非静态成员变量中使用 decltype
struct S {
	int x1;
	decltype(x1) x2;
	double x3;
	decltype(x2 + x3) x4;
};

decltype(x1) sum(decltype(x1) a1, decltype(a1) a2) {
	return a1 + a2;
}
auto x5 = sum(1, 2);
}

namespace templateUsage {

// 第一种加法函数
template<class T>
T sum(T a1, T a2) { return a1 + a2; }

auto x1 = sum(5, 10);

// 代码看上去很好，但是并不能适应所有情况，因为调用者如果传递不同类型的实参，则无法编译通过：
// auto x1 = sum(5.0,10); // 编译失败,无法推导 T 的类型

// 既然如此，我们只能编写一个更加灵活的函数模板：
// 第二种加法函数
template<class R, class T1, class T2>
R sum(T1 a1, T2 a2) {
	return a1 + a2;
}

auto x2 = sum<double>(5, 10.0);
//这样好像可以满足我们泛化sum函数的要求了。但美中不足的是我们必须为函数模板指定返回值类型。
// 为了让编译期完成所有的类型推导工作，我们决定继续优化函数模板

template<class T1, class T2>
auto sum(T1 t1, T2 t2) -> decltype(t1 + t2) {
	return t1 + t2;
}
auto x3 = sum(5.0, 10);
// 解释一下这段代码，auto是返回类型的占位符，参数类型分别是T1和T2，
// 我们利用decltype说明符能推断表达式的类型特性，在函数尾部对auto的类型进行说明

// 上述用法只推荐在C++11标准的编译环境中使用，
// 因为C++14标准已经支持对auto声明的返回类型进行推导了，
// 所以以上代码可以简化为：
template<class T1, class T2>
auto sum(T1 t1, T2 t2) {
	return t1 + t2;
}

// 读者肯定有疑问了，在C++14中decltype的作用又被auto代替了。
// 是否从C++14标准以后decltype就没有用武之地了呢？并不是这样的，
// auto作为返回类型的占位符还存在一些问题，请看下面的例子

// 我们期望return_ref返回的是一个T的引用类型，但是如果编译此段代码，则必然会编译失败，因为auto被推导为值类型
template<class T>
auto return_ref_auto(T &t) {
	return t;
}

// 如果想正确地返回引用类型，则需要用到decltype说明符，例如
template<class T>
auto return_ref_decl(T &t) -> decltype(t) {
	return t;
}
}

namespace decltypeRules {
template<class T>
void outTypename() { cout << typeid(T).name() << endl; }
#define outT(T) outTypename<T>()
/**
 * decltype 推导规则:
 *
 * 如果e是一个未加括号的标识符表达式（结构化绑定除外）或者未加括号的类成员访问，
 * 则decltype(e)推断出的类型是e的类型T。
 * 如果并不存在这样的类型，或者e是一组重载函数，则无法进行推导
 *
 * 如果e是一个函数调用或者仿函数调用，那么decltype(e)推断出的类型是其返回值的类型。
 *
 * 如果e是一个类型为T的左值，则decltype(e)是T&。
 *
 * 如果e是一个类型为T的将亡值，则decltype(e)是T&&。
 *
 * 除去以上情况，则decltype(e)是T。
 */
const int &&foo();
int i;
struct A {
	double x;
};
const A *a = new A();
void use() {
	// decltype(foo())满足规则2和规则4，foo函数的返回类型是const int&&，所以推导结果也为const int&&
	decltype(foo()) x1 = 1; // -> const int &&
	decltype(i) x2 = 1; // -> int
	decltype(a->x) x3 = 1.0; // -> double
	decltype((a->x)) x4 = 2.0; // const double &
	// 由于decltype((a->x))推导的是一个带括号的表达式(a->x)，
	// 因此规则1不再适用，但很明显a->x是一个左值，又因为a带有const限定符，
	// 所以其类型被推导为const double&。

	int i;
	int *j;
	int n[10];
	const int &&foo();
	decltype(static_cast<short>(i));    // ->short
	decltype(j);                   // ->int*
	decltype(n);                   // decltype(n)推导类型为int[10],与 auto 不同，decltype 不会退化为指针
	decltype(foo);                 // decltype(foo)推导类型为int const && (void),与 auto 不同，decltype 不会退化为指针
	struct A { int operator()() { return 0; }};
	A a;
	decltype(a());                 // ->int
	decltype(i = 0);                      // ->int&
	decltype(0, i);                      // ->int&
	decltype(i, 0);                      // ->int
	decltype(n[5]);                     // ->int&
	decltype(*j);                       // ->int&
	decltype(static_cast<int &&>(i));    // ->int&&
	decltype(i++);                      // ->int
	decltype(++i);                      // int&
	decltype("hello world");            // ->const char(&)[12]

	// 可以确认以上例子中的表达式都不是标识符表达式，这样就排除了规则1 ，标识符表达式表示只有一个标识符号
	// i=0和0,i表达式都返回左值i，所以推导类型为int&。
	// n[5]返回的是数组n中的第6个元素，也是左值，所以推导类型为int&。
	// static_cast<int&&>(i)被转换为一个将亡值类型，所以其推导类型为int&&。
	// hello world是一个常量数组的左值，其推导类型为const char(&)[12]。
}

#undef outT
}

namespace decltypeOnCV {
// 通常情况下，decltype(e)所推导的类型会同步e的cv限定符
const volatile int i = 0;
decltype(i) x = 1; // -> const volatile int

// 当e是未加括号的成员变量时，父对象表达式的cv限定符会被忽略，不能同步到推导结果
struct A {
	double x;
};
const A *a = new A();
decltype(a->x) x2; // -> double

decltype((a->x)) x3 = 1; // const double &

}

namespace decltypeAuto {
/**
 * 在C++14标准中出现了decltype和auto两个关键字的结合体：decltype(auto)。
 * 它的作用简单来说，就是告诉编译器用decltype的推导表达式规则来推导auto。
 * 另外需要注意的是，decltype(auto)必须单独声明，也就是它不能结合指针、引用以及cv限定符
 */
int i;
int &&f();
auto x1a = i; // -> int
decltype(auto) x1d = i; // ->int
decltype(auto) x2d = (i); // -> int&
auto x3a = f(); // ->int
decltype(auto) x3d = f(); // ->int&&
auto x4a = {1, 2, 3};
//decltype(auto) x4d = {1,2,3}; 不是表达式
auto *x5a = &i; // ->int*
//decltype(auto) *x5d = &i; decltype(auto)必须单独（唯一）声明，也就是它不能结合指针、引用以及cv限定符

//接下来让我们看一看decltype(auto)是如何发挥作用的。还记得decltype不可被auto代替的例子吗？
// return_ref想返回一个引用类型，但是如果直接使用auto，则一定会返回一个值类型。
// 这让我们不得不采用返回类型后置的方式声明返回类型。
// 现在有了decltype(auto)组合，我们可以进一步简化代码，消除返回类型后置的语法，例如：
template<class T>
decltype(auto) return_ref(T &t) {
	return t;
}
}

namespace decltypeAutoOnUntypedTemplateParameterPlaceholders {
//与auto一样，在C++17标准中decltype(auto)也能作为非类型模板形参的占位符，其推导规则和上面介绍的保持一致
template<decltype(auto) N>
void f() {
	cout << N << endl;
}

static const int x = 11;
static int y = 7;

void use(){
	f<x>();
	f<(x)>();
//	f<y>(); // 因为y不是一个常量，所以编译器无法对函数模板进行实例化
	f<(y)>(); // 而f<(y)>()则没有这种问题，因为(y)被推断为了引用类型，
	// 恰好对于静态对象而言内存地址是固定的，所以可以顺利地通过编译，最终N被推导为int&类型。
}

}
}

int main() {
	useDecltype::decltypeAutoOnUntypedTemplateParameterPlaceholders::use();
	return 0;
}
