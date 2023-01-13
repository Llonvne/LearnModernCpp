//
// Created by 华邵钶 on 2023/1/13.
//
/* 值类别
 值类别是C++11标准中新引入的概念，具体来说它是表达式的一种属性，
 该属性将表达式分为3个类别，它们分别是左值（lvalue）、纯右值（prvalue）和将亡值（xvalue）

 表达式首先被分为了泛左值（glvalue)和右值（rvalue），
 其中泛左值被进一步划分为左值和将亡值，右值又被划分为将亡值和纯右值。

 1．所谓泛左值是指一个通过评估能够确定对象、位域或函数的标识的表达式。
 简单来说，它确定了对象或者函数的标识（具名对象）。

 2．而纯右值是指一个通过评估能够用于初始化对象和位域，或者能够计算运算符操作数的值的表达式。

 3．将亡值属于泛左值的一种，它表示资源可以被重用的对象和位域，
 通常这是因为它们接近其生命周期的末尾，另外也可能是经过右值引用的转换产生的。

 实际上，这里的左值（lvalue）就是我们上文中描述的C++98的左值，
 而这里的纯右值（prvalue）则对应上文中描述的C++98的右值
 从本质上说产生将亡值的途径有两种，第一种是使用类型转换将泛左值转换为该类型的右值引用。*/
#include "bits/stdc++.h"
using namespace std;
namespace E1 {
class BigMemoryPool {};
void use(BigMemoryPool pool) {
	auto x = static_cast<BigMemoryPool &&> (pool);
}
}

/* 我们称它为临时量实质化，指的是纯右值转换到临时对象的过程。
 每当纯右值出现在一个需要泛左值的地方时，临时量实质化都会发生，
 也就是说都会创建一个临时对象并且使用纯右值对其进行初始化，这也符合纯右值的概念，
 而这里的临时对象就是一个将亡值。*/

namespace E2 {
struct X {
	int a;
};

void use() {
	int b = X().a;
}
}

// 将左值转换为右值
namespace LtR {
int i = 0;
// int && k = i; 右值引用只能绑定右值
// 如果想完成将右值引用绑定到左值这个“壮举”还是有办法的。
// 在C++11标准中可以在不创建临时值的情况下显式地将左值通过static_cast转换为将亡值
int &&k = static_cast<int &&>(i);
// 通过值类别的内容我们知道将亡值属于右值，所以可以被右值引用绑定。值得注意的是，
// 由于转换的并不是右值，因此它依然有着和转换之前相同的生命周期和内存地址
// 既然这个转换既不改变生命周期也不改变内存地址，那它有什么存在的意义呢？实际上它的最大作用是让左值使用移动语义

class BigMemoryPool {};
void use() {
	BigMemoryPool my_pool1;
	BigMemoryPool my_pool2 = my_pool1;
	BigMemoryPool my_pool3 = static_cast<BigMemoryPool &&>(my_pool1);
	// 在这段代码中，my_pool1是一个BigMemoryPool类型的对象，也是一个左值，
	// 所以用它去构造my_pool2的时候调用的是复制构造函数。
	// 为了让编译器调用移动构造函数构造my_pool3，
	// 这里使用了static_cast<BigMemoryPool &&>(my_ pool1)将my_pool1强制转换为右值（也是将亡值，
	// 为了叙述思路的连贯性后面不再强调）。
	// 由于调用了移动构造函数，my_pool1失去了自己的内存数据，后面的代码也不能对my_pool1进行操作了。
}
}

namespace E3 {
class BigMemoryPool {};
void move_pool(BigMemoryPool &&pool) {
	cout << "call move_pool" << endl;
	BigMemoryPool my_pool(pool);
}
auto get_pool(const BigMemoryPool &pool) { return pool; }
auto make_pool() { return get_pool(BigMemoryPool()); }

void use() {
	move_pool(make_pool());
	// make_pool 返回一个了 左值，那么会调用复制构造函数复制一遍然后转递给 move_pool
	// copy big memory pool.
	// move big memory pool.
	// call move_pool
	// copy big memory pool.
}
// 解决方法一，转换成将亡值传入，使其调用移动构造函数
void use2() {
	move_pool(static_cast<BigMemoryPool &&>(make_pool()));
}

// 使用 move
void use3() {
	move_pool(std::move(make_pool()));
}
}

namespace UniversalReferences {
// 量左值引用既可以引用左值又可以引用右值，是一个几乎万能的引用，
// 但可惜的是由于其常量性，导致它的使用范围受到一些限制。
// 其实在C++11中确实存在着一个被称为“万能”的引用，
// 它看似是一个右值引用，但其实有着很大区别，请看下面的代码

void foo(int &&i) {} // R
template<class T>
void bar(T &&t) {} // U
int get_val() { return 5; }
int &&x = get_val(); // R
auto &&y = get_val(); // U

// 函数foo的形参i和变量x是右值引用，
// 而函数模板的形参t和变量y则是万能引用。
// 我们知道右值引用只能绑定一个右值，
// 但是万能引用既可以绑定左值也可以绑定右值，甚至const和volatile的值都可以绑定

int i = 42;
const int j = 11;

template<class T>
void foo(std::vector<int> &&t) {}

void use() {
	bar(i);
	bar(j);
	bar(get_val());

	auto &&x = i;
	auto &&y = j;
	auto &&z = get_val();
	// 所谓的万能引用是因为发生了类型推导，在T&&和auto&&的初始化过程中都会发生类型的推导，
	// 如果已经有一个确定的类型，比如int &&，则是右值引用。
	// 在这个推导过程中，初始化的源对象如果是一个左值，则目标对象会推导出左值引用；
	// 反之如果源对象是一个右值，则会推导出右值引用，不过无论如何都会是一个引用类型。

	// 万能引用能如此灵活地引用对象，实际上是因为在C++11中添加了一套引用叠加推导的规则——引用折叠。
	// 在这套规则中规定了在不同的引用类型互相作用的情况下

	// 只要有左值引用参与进来，最后推导的结果就是一个左值引用。
	// 只有实际类型是一个非引用类型或者右值引用类型时，最后推导出来的才是一个右值引用

	// 值得一提的是，万能引用的形式必须是T&&或者auto&&，
	// 也就是说它们必须在初始化的时候被直接推导出来，如果在推导中出现中间过程，则不是一个万能引用
	std::vector<int> v{1, 2, 3};
//	foo(v);
// 在上面的代码中，foo(v)无法编译通过，因为foo的形参t并不是一个万能引用，而是一个右值引用。
// 因为foo的形参类型是std::vector<T>&&而不是T&&，所以编译器无法将其看作一个万能引用处理。
}

}

namespace PerfectForwarding {
template<class T>
void show_type(T t) {
	cout << typeid(t).name() << endl;
}
template<class T>
void normal_forwarding(T t) {
	show_type(t);
}
template<class T>
void normal_forwarding2(T &t) {
	show_type(t);
}
template<class T>
void normal_forwarding3(const T &t) {
	show_type(t);
}
template<class T>
void perfect_forwarding(T &&t) {
	show_type(static_cast<T &&>(t));
}
template<class T>
void perfect_forward2(T &&t) {
	show_type(std::forward<T>(t));
}
auto get_str(const string &str) {
	return str;
}
void use() {
	string s = "hello world";
	normal_forwarding(s);
	// 函数normal_forwarding是一个常规的转发函数模板，它可以完成字符串的转发任务。但是它的效率却令人堪忧。
	// 因为normal_forwarding按值转发，也就是说std::string在转发过程中会额外发生一次临时对象的复制。
	// 其中一个解决办法是将void normal_forwarding(T t)替换为void normal_forwarding(T &t)，
	// 这样就能避免临时对象的复制。不过这样会带来另外一个问题，如果传递过来的是一个右值，则该代码无法通过编译
	//	normal_forwarding2(static_cast<string &&>(s)); 无法通过编译，转换后为右值
	// 万能引用的出现改变了这个尴尬的局面。
	// 上文提到过，对于万能引用的形参来说，如果实参是给左值，则形参被推导为左值引用；
	// 反之如果实参是一个右值，则形参被推导为右值引用，
	// 所以下面的代码无论传递的是左值还是右值都可以被转发，
	// 而且不会发生多余的临时复制
	perfect_forwarding(static_cast<string &&>(s));
	perfect_forwarding(s);
	perfect_forwarding(get_str(s));
	// 唯一可能需要注意的是show_type(static_cast<T&&>(t));中的类型转换，之所以这里需要用到类型转换，
	// 是因为作为形参的t是左值。为了让转发将左右值的属性也带到目标函数中，这里需要进行类型转换。
	// 当实参是一个左值时，T被推导为std::string&，于是static_cast<T&&>被推导为static_cast<std::string&>，
	// 传递到show_type函数时继续保持着左值引用的属性；
	// 当实参是一个右值时，T被推导为std::string，
	// 于是static_cast <T&&>被推导为static_cast<std::string&&>，所以传递到show_type函数时保持了右值引用的属性。

	// 和移动语义的情况一样，显式使用static_cast类型转换进行转发不是一个便捷的方法。
	// 在C++11的标准库中提供了一个std::forward函数模板，在函数内部也是使用static_cast进行类型转换，
	// 只不过使用std::forward转发语义会表达得更加清晰，std::forward函数模板的使用方法也很简单：
	// 其中std::move一定会将实参转换为一个右值引用，并且使用std::move不需要指定模板实参，
	// 模板实参是由函数调用推导出来的。而std::forward会根据左值和右值的实际情况进行转发，在使用的时候需要指定模板实参。
}
}

int main() {
	PerfectForwarding::use();
}

