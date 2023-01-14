//
// Created by 华邵钶 on 2023/1/13.
//
// Lambda
// [ captures ] ( params ) specifiers exception -> ret { body }
// [ captures ] —— 捕获列表，它可以捕获当前函数作用域的零个或多个变量，变量之间用逗号分隔。
// 在对应的例子中，[x]是一个捕获列表，不过它只捕获了当前函数作用域的一个变量x，在捕获了变量之后，
// 我们可以在lambda表达式函数体内使用这个变量，比如return x * y。
// 另外，捕获列表的捕获方式有两种：按值捕获和引用捕获，下文会详细介绍。
//　
// ( params ) —— 可选参数列表，语法和普通函数的参数列表一样，在不需要参数的时候可以忽略参数列表。
//  对应例子中的(int y)。　　
//
//  specifiers —— 可选限定符，C++11中可以用mutable，它允许我们在lambda表达式函数体内改变按值捕获的变量，
//  或者调用非const的成员函数。上面的例子中没有使用说明符。　　
//
//  exception —— 可选异常说明符，我们可以使用noexcept来指明lambda是否会抛出异常。
//  对应的例子中没有使用异常说明符。　　
//
//  ret —— 可选返回值类型，不同于普通函数，lambda表达式使用返回类型后置的语法来表示返回类型，
//  如果没有返回值（void类型），可以忽略包括->在内的整个部分。
//  另外，我们也可以在有返回值的情况下不指定返回类型，
//  这时编译器会为我们推导出一个返回类型。对应到上面的例子是->int。　　
//
//  { body } —— lambda表达式的函数体，这个部分和普通函数的函数体一样。对应例子中的{ return x * y; }。
#include "bits/stdc++.h"
using namespace std;
namespace E1 {
void use() {
	int x = 5;
	auto foo = [x](int y) -> int { return x * y; };
	std::cout << foo(8) << std::endl;
}
}
namespace Scope {
// 捕获列表中的变量存在于两个作用域——lambda表达式定义的函数作用域以及lambda表达式函数体的作用域。
// 前者是为了捕获变量，后者是为了使用变量。另外，标准还规定能捕获的变量必须是一个自动存储类型。
// 简单来说就是非静态的局部变量
int x = 0;
void use() {
	int y = 0;
	static int z = 0;
	// auto foo = [x, y, z] {};
	// 以上代码可能是无法通过编译的，其原因有两点：第一，变量x和z不是自动存储类型的变量；
	// 第二，x不存在于lambda表达式定义的作用域。这里可能无法编译，
	// 因为不同编译器对于这段代码的处理会有所不同

	// 那么如果想在lambda表达式中使用全局变量或者静态局部变量该怎么办呢？
	// 马上能想到的办法是用参数列表传递全局变量或者静态局部变量，其实不必这么麻烦，直接用就行了
}
}
namespace CapturesValueOrReference {
void use() {
	// 捕获值的语法，在[]中直接写入变量名，如果有多个变量，则用逗号分隔
	int x = 5, y = 8;
	auto foo = [x, y] { return x * y; };

	// 捕获引用的语法与捕获值只有一个&的区别，要表达捕获引用我们只需要在捕获变量之前加上&，
	// 类似于取变量指针。只不过这里捕获的是引用而不是指针，在lambda表达式内可以直接使用变量名访问变量而不需解引用
	auto foo2 = [&x, &y] { return x * y; };

	// 原因是我们无法改变捕获变量的值。
	// 这就引出了lambda表达式的一个特性：捕获的变量默认为常量，
	// 或者说lambda是一个常量函数（类似于常量成员函数）x += 1; 编译失败，无法改变捕获变量的值

	// 尝试改变值
	auto foo3 = [x, y] {
//		x += 1; 编译失败 不被允许改变
	};

	auto foo4 = [&x, &y] {
		x += 1; // 允许改变，因为改变的引用指向的值，而不是引用本身
	};

	// 使用 mutable 记号移除常量限制
	auto foo5 = [x, y] mutable {
		x += 1; // 允许修改变量
		cout << "123";
	};
	// 当lambda表达式捕获值时，表达式内实际获得的是捕获变量的复制，
	// 我们可以任意地修改内部捕获变量，但不会影响外部变量。
	// 而捕获引用则不同，在lambda表达式内修改捕获引用的变量，对应的外部变量也会被修改

	// 虽然在lambda表达式内修改x不会影响外部x的值，但是它却能影响下次调用lambda表达式时x的值。
	// 更具体来说，当第一次调用foo的时候，x的值从5增加到6，这个状态持续到第二次调用foo，然后将x的值从6增加到7

	// 对于捕获值的lambda表达式还有一点需要注意，捕获值的变量在lambda表达式定义的时候已经固定下来了，
	// 无论函数在lambda表达式定义后如何修改外部变量的值，lambda表达式捕获的值都不会变化
	// 如果捕获值，那么在定义后就被确定下来
	// 捕获引用，地址被确定下来
}
}
namespace SpecialCaptures {
class A {
 public:
	void print() { cout << "class A" << endl; }
	void test() {
		auto foo = [this] {
			this->print();
			this->x = 5;
		};
		foo();
	}
 private:
	int x;
};
void use() {
	A a;
	a.test();
}
}
namespace LambdaPrinciple {
// 我们从函数对象开始深入探讨lambda表达式的实现原理
class Bar {
 public:
	Bar(int x, int y) : x(x), y(y) {}
	int operator()() const { return this->x * this->y; }
 private:
	int x;
	int y;
};
void use() {
	int x = 5, y = 8;
	auto foo = [x, y] { return x * y; };
	Bar bar(x, y);
	cout << "foo() = " << foo() << endl;
	cout << "bar() = " << bar() << endl;

	// 1．使用lambda表达式不需要我们去显式定义一个类，这一点在快速实现功能上有较大的优势。
	// 2．使用函数对象可以在初始化的时候有更加丰富的操作，例如Bar bar(x+y, x *y)，
	// 而这个操作在C++11标准的lambda表达式中是不允许的。

	// 另外，在Bar初始化对象的时候使用全局或者静态局部变量也是没有问题的。
	// lambda表达式的优势在于书写简单方便且易于维护，而函数对象的优势在于使用更加灵活不受限制，但总的来说它们非常相似。
	// 而实际上这也正是lambda表达式的实现原理。

	// lambda表达式在编译期会由编译器自动生成一个闭包类，在运行时由这个闭包类产生一个对象，
	// 我们称它为闭包。在C++中，所谓的闭包可以简单地理解为一个匿名且可以包含定义时作用域上下文的函数对象。
	// 现在让我们抛开这些概念，观察lambda表达式究竟是什么样子的。

	auto foo2 = [=] { return x * y; };
}
}
namespace StatelessLambdaExpressions {
void f(void (*func)()) {
}
void g() {
	f([] {}); // 其中 []{} 是一个 lambda 函数转换为函数指针 void(*)()
}

void f(void(&func)()) {
}
void g2() { f([] {}); } // 其中 []{} 是一个 lambda 函数转换为函数指针 void(&)()
}
namespace UseOnSTL {
void use() {
	vector<int> x = {1, 2, 3, 4, 5};
	cout << *find_if(x.cbegin(), x.cend(), [](int i) { return (i % 3) == 0; }) << endl;
}
}
namespace GeneralizedCapture {
// C++14标准中定义了广义捕获，所谓广义捕获实际上是两种捕获方式，第一种称为简单捕获，这种捕获就是我们在前文中提到的捕获方法，
// 即[identifier]、[&identifier]以及[this]等。
// 第二种叫作初始化捕获，这种捕获方式是在C++14标准中引入的，它解决了简单捕获的一个重要问题，
// 即只能捕获lambda表达式定义上下文的变量，而无法捕获表达式结果以及自定义捕获变量名

void use() {
	int x = 5;
	auto foo = [x = x + 1] { return x; };
	// C++14标准对这样的捕获进行了支持，在这段代码里捕获列表是一个赋值表达式，
	// 不过这个赋值表达式有点特殊，因为它通过等号跨越了两个作用域。
	// 等号左边的变量x存在于lambda表达式的作用域，而等号右边x存在于main函数的作用域
	cout << foo();

	auto foo2 = [r = x + 1] { return r; };

	cout << foo2();

	// 初始化捕获在某些场景下是非常实用的，这里举两个例子，第一个场景是使用移动操作减少代码运行的开销
	string x1 = "Hello World";
	auto foo3 = [str = std::move(x1)] {
		return str + " Llonvne";
	};
	cout << foo3();
}

// 第二个场景是在异步调用时复制this对象，
// 防止lambda表达式被调用时因原始this对象被析构造成未定义的行为，比如：
namespace W1 {
class Work {
	int value;
 public:
	Work() : value(42) {}
	std::future<int> spawn() {
		return std::async([=]() { return value; });
	}
};
}
namespace W2 {
class Work {
	int value;
 public:
	Work() : value(42) {}
	std::future<int> spawn() {
		return std::async([=,tmp = *this]() { return tmp.value; });
	}
};
// 以上代码使用初始化捕获，将*this复制到tmp对象中，然后在函数体内返回tmp对象的value。
// 由于整个对象通过复制的方式传递到lambda表达式内，因此即使this所指的对象析构了也不会影响lambda表达式的计算。
// 编译运行修改后的代码，程序正确地输出f.get() = 42。
};

future<int> foo() {
	W1::Work tmp;
	return tmp.spawn();
}

void use2() {
	auto f = foo();
	f.wait();
	cout << f.get() << endl;
	// f.get() = 32766, expect 42
}
}
int main() {
	GeneralizedCapture::use2();
};