//
// Created by 华邵钶 on 2023/1/15.
// 类的特殊成员函数
// 1．默认构造函数。2．析构函数。
// 3．复制构造函数。4．复制赋值运算符函数。
// 5．移动构造函数（C++11新增）。6．移动赋值运算符函数（C++11新增）。

// 添加默认特殊成员函数的这条特性非常实用
// 我们虽然没有为City类添加复制赋值运算符函数City::operator= (const City &)，
// 但是编译器仍然可以成功编译代码，
// 并且在运行过程中正确地调用std::string和std::vector<std::string>的复制赋值运算符函数
#include "bits/stdc++.h"
namespace E1 {
class City {
	std::string name;
	std::vector<std::string> street_name;
};
void use() {
	City a, b;
	a = b;
}
}
// 1．声明任何构造函数都会抑制默认构造函数的添加。
// 2．一旦用自定义构造函数代替默认构造函数，类就将转变为非平凡类型。
// 3．没有明确的办法彻底禁止特殊成员函数的生成（C++11之前）。
namespace E2 {
class City {
	std::string name;
	std::vector<std::string> street_name;;
 public:
	explicit City(const char *n) : name(n) {}
};
void use() {
	City a("XS");
//	City b; 编译失败，自定义构造函数抑制了默认构造函数的生成

}
}
// 显式默认和显式删除
// C++11标准提供了一种方法能够简单有效又精确地控制默认特殊成员函数的添加和删除，
// 我们将这种方法叫作显式默认和显式删除。显式默认和显式删除的语法非常简单，
// 只需要在声明函数的尾部添加=default和=delete
namespace DefaultDelete {
struct Type {
	Type() = default;
	virtual ~Type() = delete;
	Type(const Type &);
};
Type::Type(const Type &) = default;
// 以上代码显式地添加了默认构造和复制构造函数，同时也删除了析构函数。
// 请注意，=default可以添加到类内部函数声明，也可以添加到类外部。
// 这里默认构造函数的=default就是添加在类内部，
// 而复制构造函数的=default则是添加在类外部。提供这种能力的意义在于，
// 它可以让我们在不修改头文件里函数声明的情况下，改变函数内部的行为

// =delete与=default不同，它必须添加在类内部的函数声明中，如果将其添加到类外部，那么会引发编译错误。

namespace Impl1 {
// type.h
struct type {
	type();
	int x;
};

// type1.cpp
type::type() = default;
}

namespace Impl2 {
// type.h
struct type {
	type();
	int x;
};
// type2.cpp
type::type() { x = 3; }
}
}

// 针对禁止调用某些函数的问题，我们可以使用= delete来删除特定函数，
// 相对于使用private限制函数访问，使用= delete更加彻底，它从编译层面上抑制了函数的生成，
// 所以无论调用者是什么身份（包括类的成员函数），都无法调用被删除的函数。
// 进一步来说，由于必须在函数声明中使用= delete来删除函数，
// 因此编译器可以在第一时间发现有代码错误地调用被删除的函数并且显示错误报告

namespace NC {
class NonCopyable {
 public:
	NonCopyable() = default;
	NonCopyable(const NonCopyable &) = delete;
	NonCopyable operator=(const NonCopyable &) = delete;
};

void use() {
	NonCopyable a, b;
//	a = b; 不可复制
// 请注意，由于显式地删除了复制构造函数，导致默认情况下编译器也不再自动添加默认构造函数，
// 因此我们必须显式地让编译器添加默认构造函数，否则会导致编译失败。
}
}

namespace OtherDelete {
void foo() = delete;
static void bar() = delete;
void use() {
//	foo();
//  bar();
}

struct type {
	void *operator new(std::size_t) = delete; // 禁止在堆中创建对象
};
type global_var;
void use2() {
	static type static_var;
	type auto_var;
//	auto var_ptr = new type();
}

// 显式删除类的析构函数在某种程度上和删除new运算符的目的正好相反，
// 它阻止类通过自动变量、静态变量或者全局变量的方式创建对象，但是却可以通过new运算符创建对象。
// 原因是删除析构函数后，类无法进行析构。所以像自动变量、静态变量或者全局变量这种会隐式调用析构函数的对象就无法创建了，
// 当然了，通过new运算符创建的对象也无法通过delete销毁
// 只有new创建对象会成功，其他创建和销毁操作都会失败，所以这样的用法并不多见，大部分情况可能在单例模式中出现。
struct type2 {
	~type2() = delete; // 只能 new 出来，另外都不行
};
//type2 t1;
//static type2 t2;
void use3() {
	auto t3 = new type2;
//	delete t3;
}
}

namespace ExplicitDelete {
// 在类的构造函数上同时使用explicit和=delete是一个不明智的做法
struct type {
	type(long long) {}
	explicit type(long) = delete;
};
void foo(type) {}
void use() {
//	foo(type(58));
// 原因是type(58)显式调用了构造函数，但是explicittype(long)却被删除了。
// foo(58)可以通过编译，因为编译器会选择type(long long)来构造对象。
	foo(58);
}
}
