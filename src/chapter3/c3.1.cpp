//
// Created by llonv on 2023/1/13.
//
// C++11标准赋予了auto新的含义：声明变量时根据初始化
// 表达式自动推断该变量的类型、声明函数时函数返回值的占位符。

#include <vector>
#include "initializer_list"
#include "iostream"

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
auto *pn = &n, m = 10;
// &pn 类型是 int *, auto 被推导为 int, 于是 m 被声明为 int

// 以下将无法通过编译
//auto *pn2 = &n,m2 = 10.0;

// 编译器总是使用表达能力更强的类型
auto i2 = true ? 5 : 8.0; // i -> double

// auto 与 new (我不建议像上面这样使用auto，因为它会破坏代码的可读性)
auto int1 = new auto(1);
auto *intptr = new auto(1);

namespace autoIgnoreCV {
// auto 声明的类型是按值初始化，那么推导出来的类型会忽略 cv 限定符号
// 在使用auto声明变量时，既没有使用引用，也没有使用指针，那么编译器在推导的时候会忽略const和volatile限定符

// 该原理十分合理，对于按值初始化，不必要继承 cv 限定符号，而对于 & 或者 *,如果不继承 cv 符号会破坏原本的 cv 属性
// 即让 const 变得荒唐
const int i = 5;
auto j = i; // j -> int,not const int
auto &m = i; // auto -> const int, m -> const int &
auto &k = i; // auto -> const int, m -> const int *

// 当然你可以为其添加 cv 限定符号
const auto n = i; //auto -> int , n -> const int
}

namespace autoIgnoreReference {
// 使用auto声明变量初始化时，目标对象如果是引用，则引用属性会被忽略
// auto 与 C/C++ 语言有着相同的偏好，即默认倾向于按值传递
int i = 5;
int &j = i;
auto m = j;
}
namespace autoLeftToReference {
// TODO 当学到万能引用声明变量 回顾
// 使用auto和万能引用声明变量时（见第6章），对于左值会将auto推导为引用类型
// 因为i是一个左值，所以m的类型被推导为int&，auto被推导为int&，这其中用到了引用折叠的规则。
// 而5是一个右值，因此j的类型被推导为int&&，auto被推导为int
int i = 5;
auto &&m = i; // auto -> int&
auto &&j = 5; // auto -> int
}

namespace autoDeclareArrayOrFunction {
int i[5];
auto m = i; // auto -> int *
int sum(int a1, int a2) { return a1 + a2; }
auto j = sum; // auto -> int(*)(int,int)
// i -> int[], m -> int*
// sum 是函数,j 退化为函数指针
}

namespace autoWithInitializerList {
// 直接使用 {} 只能是单个元素，里面的元素是什么类型，就推导为什么类型
auto x1{1};

// 列表中可以包含单个或者多个元素，auto类型被推导为std::initializer_list<T>，
// 其中T是元素类型。请注意，在列表中包含多个元素的时候，元素的类型必须相同，否则编译器会报错
auto x = {1, 2, 3};

// 编译失败，元素类型不同
//auto x2 = {1, 2.0};

// 编译失败不为单个元素
//auto x3{3, 4};

// x4 为 initializer_list<int>
auto x4 = {1};

// 你可以简单理解为 当你直接使用 {},就是直接初始化前面的元素，否则将会组成一个 initializer_list<T>
}

namespace autoExercise {
class Base {
 public:
	virtual void f() {
		std::cout << "Base::f()" << std::endl;
	};
};
class Derived : public Base {
 public:
	virtual void f() override {
		std::cout << "Derived::f()" << std::endl;
	};
};
Base *d = new Derived();
auto b = *d; // when b.f(); which ?
// 有Derived和Base之间的继承关系，并且Derived重写了Base的f函数。
// 代码使用new创建了一个Derived对象，并赋值于基类的指针类型变量上。
// 读者知道d->f()一定调用的是Derived的f函数。但是b.f()调用的又是谁的f函数呢？
// 实际上，由于auto b = *d这一句代码是按值赋值的，因此auto会直接推导为Base。
// 代码自然会调用Base的复制构造函数，也就是说Derived被切割成了Base，这里的b.f()最终调用Base的f函数。
// 那么进一步发散，如果代码写的是auto &b = *d，结果又会如何呢？这个就交给读者自己验证了
// 该结论与 C++ 多态条件也符合，即必须是指针或者引用

auto &b2 = *d;
// 由于增加的 & ，此时 auto 将按引用传递 -> Base& 符合 多态条件
}

int main() {
	autoExercise::b.f();
	autoExercise::b2.f();

	// 什么是切割 以上 auto 的行为类似于第二行
	autoExercise::Base *m = new autoExercise::Derived();
	autoExercise::Base m2 = *m;
//	m2.f();
}
