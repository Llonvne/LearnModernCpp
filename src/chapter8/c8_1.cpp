//
// Created by 华邵钶 on 2023/1/14.
//
// 非静态数据成员默认初始化（C++11 C++20）
// 在C++11以前，对非静态数据成员初始化需要用到初始化列表，
// 当类的数据成员和构造函数较多时，编写构造函数会是一个令人头痛的问题：
import "string";
namespace DefaultInitialization {
namespace WithOutDefault {
class X {
 public:
	X() : a_(0), b_(0.), c_("hello world") {}
	X(int a) : a_(a), b_(0.), c_("hello world") {}
	X(double b) : a_(0), b_(b), c_("hello world") {}
	X(const std::string &c) : a_(0), b_(0.), c_(c) {}

 private:
	int a_;
	double b_;
	std::string c_;
};
}
// 类X有4个构造函数，为了在构造的时候初始化非静态数据成员，它们的初始化列表有一些冗余代码，
// 而造成的后果是维护困难且容易出错。为了解决这种问题，
// C++11标准提出了新的初始化方法，即在声明非静态数据成员的同时直接对其使用=或者{}（见第9章）初始化。
// 在此之前只有类型为整型或者枚举类型的常量静态数据成员才有这种声明默认初始化的待遇
namespace WithDefault {
class X {
 public:
	X() {}
	X(int a) : a_(a) {}
	X(double b) : b_(b) {}
	X(const std::string &c) : c_(c) {}

 private:
	int a_ = 0;
	double b_{0.};
	std::string c_{"hello world"};
};
// 最后来看一看非静态数据成员在声明时默认初始化需要注意的两个问题。
// 1．不要使用括号()对非静态数据成员进行初始化，因为这样会造成解析问题，所以会编译错误。
// 2．不要用auto来声明和初始化非静态数据成员，虽然这一点看起来合理，但是C++并不允许这么做。
}
}