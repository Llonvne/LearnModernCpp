//
// Created by 华邵钶 on 2023/1/14.
//
import "map";
import "vector";
import "set";
import "list";
import "initializer_list";
#include "iostream"
// 列表初始化（C++11 C++20）
// 初始化变量的传统方法
namespace InitializationVariables {
struct C {
	C(int a) {}
};

void use() {
	int x = 5;
	int x1(8);
	C x2 = 4;
	C x3(4);
}

// 一般来说，我们称使用括号初始化的方式叫作直接初始化，而使用等号初始化的方式叫作拷贝初始化（复制初始化）。
// 请注意，这里使用等号对变量初始化并不是调用等号运算符的赋值操作。
// 实际情况是，等号是拷贝初始化，调用的依然是直接初始化对应的构造函数，只不过这里是隐式调用而已
// 使用括号和等号只是直接初始化和拷贝初始化的代表，还有一些经常用到的初始化方式也属于它们。
// 比如new运算符和类构造函数的初始化列表就属于直接初始化，而函数传参和return返回则是拷贝初始化。
// 前者比较好理解，后者可以通过具体的例子来理解：

void foo(C c) {}
C bar() { return 5; }
void use2() {
	foo(8); // 拷贝初始化
	C c = bar(); // 拷贝初始化
}
}
// ListInitialization
namespace ListInitialization {
// C++11标准引入了列表初始化，它使用大括号{}对变量进行初始化，和传统变量初始化的规则一样，
// 它也区分为直接初始化和拷贝初始化
struct C {
	C(std::string a, int b) {}
	C(int a);
};

void foo(C) {}
C bar() { return {"world", 5}; }
void use() {
	int x = {5};// 拷贝初始化
	int x1{8}; // 拷贝初始化
	C x2 = {4}; // 拷贝
	C x3{4}; // 直接
	foo({8}); // 拷贝
	foo({"hello", 8}); // 拷贝
	C x4 = bar(); // 拷贝
	C *x5 = new C{"hi", 42}; // 直接
}

void use2() {
	// 初始化 STL
	std::vector<int> x{1, 2, 3, 4};
	std::list<int> x2{1, 2, 3, 4};
	std::set<int> x3{1, 2, 3, 4};
	std::map<int, int> x4{{1, 2}, {2, 3}};

	// 也可以在 花括号 与 变量名加等号
}
}

namespace InitializerList {
// 标准容器之所以能够支持列表初始化，离不开编译器支持的同时，
// 它们自己也必须满足一个条件：支持std::initializer_list为形参的构造函数。
// std::initializer_list简单地说就是一个支持begin、end以及size成员函数的类模板
struct C {
	C(std::initializer_list<std::string> a) {
		// std:: initializer_list的begin和end函数并不是返回的迭代器对象，而是一个常量对象指针const T *
		for (const std::string *item = a.begin(); item != a.end(); ++item) {
			std::cout << *item << " ";
		}
		std::cout << std::endl;
	}
};
void use() {
	C c{"hello", "world", "123"};
}
namespace ImplicitNarrowing {
namespace E1 {
// 隐式缩窄转换问题
int x = 12345;
char y = x;
// 在C++中哪些属于隐式缩窄转换呢？在C++标准里列出了这么4条规则。
// 1．从浮点类型转换整数类型。
// 2．从long double转换到double或float，或从double转换到float，
// 除非转换源是常量表达式以及转换后的实际值在目标可以表示的值范围内。
// 3．从整数类型或非强枚举类型转换到浮点类型，除非转换源是常量表达式，
// 转换后的实际值适合目标类型并且能够将生成目标类型的目标值转换回原始类型的原始值。
// 4．从整数类型或非强枚举类型转换到不能代表所有原始类型值的整数类型，除非源是一个常量表达式，
// 其值在转换之后能够适合目标类型。
}
namespace E2 {
int x = 999;
const int y = 999;
const int z = 99;
const double cdb = 99.9;
double db = 99.9;
char c1 = x;  // 编译成功，传统变量初始化支持隐式缩窄转换
//char c2{ x }; // 编译失败，可能是隐式缩窄转换，对应规则4
//char c3{ y }; // 编译失败，确定是隐式缩窄转换，999超出char能够适应的范围，对应规则4
char c4{z}; // 编译成功，99在char能够适应的范围内，对应规则4
unsigned char uc1 = {5};  // 编译成功，5在unsigned char能够适应的范围内，
// 对应规则4
//unsigned char uc2 = { -1 }; // 编译失败，unsigned char不能够适应负数，对应规则4
//unsigned int ui1 = { -1 };  //编译失败，unsigned int不能够适应负数，对应规则4
//signed int si1 = { (unsigned int)-1 }; //编译失败，signed int不能够适应-1所对应的
//unsigned int，通常是4294967295，对应规则4
//int ii = { 2.0 };  // 编译失败，int不能适应浮点范围，对应规则1
//float f1{ x };     // 编译失败，float可能无法适应整数或者互相转换，对应规则3
float f2{7};     // 编译成功，7能够适应float，且float也能转换回整数7，对应规则3
//float f3{ cdb };   // 编译成功，99.9能适应float，对应规则2
//float f4{ db };    // 编译失败，可能是隐式缩窄转无法表达double，对应规则2
}
}

namespace PriorityIssuesOfListInitialization {
// 列表初始化既可以支持普通的构造函数，也能够支持以std::initializer_list为形参的构造函数。
// 如果这两种构造函数同时出现在同一个类里，那么编译器会如何选择构造函数呢？
std::vector<int> x1(5, 5);
std::vector<int> x2{5, 5};
// 如果有一个类同时拥有满足列表初始化的构造函数，且其中一个是以std::initializer_list为参数，
// 那么编译器将优先以std::initializer_ list为参数构造函数。
// 由于这个特性的存在，我们在编写或阅读代码的时候就一定需要注意初始化代码的意图是什么，应该选择哪种方法对变量初始化。
}

namespace SpecifyingInitialization {
// 为了提高数据成员初始化的可读性和灵活性，C++20标准中引入了指定初始化的特性。
// 该特性允许指定初始化数据成员的名称，从而使代码意图更加明确。
struct Point {
	int x;
	int y;
};

Point p{.x = 1, .y = 4};

// 或者
struct PointWithValue {
	Point point;
	int value;
};

PointWithValue p2{.value = 0};

// 该方法只能使用聚合对象 没有方法的对象

//可以在初始化内部对象使用初始化列表
PointWithValue p3{.point{1, 2}};
// 不能使用超过一次的运算符, C 允许
//PointWithValue p4{.point.x = 1};
// 如果确实想用，可以使用以下形式
PointWithValue p5{.point{.x=5}};
// 一旦使用指定初始化，不可以混用其他方法，初始化其他成员  C 允许
//PointWithValue p5{.point{1, 2}, 5};
// 指定初始化在C语言中处理数组的能力，当然在C++中这同样是被禁止的,
// C++标准中给出的禁止理由非常简单，它的语法和lambda表达式冲突了。
//int arr[3] = {[1] = 5};
}
}
int main() {
}