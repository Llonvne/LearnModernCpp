#include "bits/stdc++.h"
using namespace std;
namespace leftValueAndRightValue {
//
// Created by 华邵钶 on 2023/1/13.
// 右值引用
int x = 1; // x:L 1:R
int y = 3; // y:L 3:R
int z = x + y; // z:L x + y : R

/**
 * 在C++中所谓的左值一般是指一个指向特定内存的具有名称的值（具名对象），
 * 它有一个相对稳定的内存地址，并且有一段较长的生命周期。
 * 而右值则是不指向稳定内存地址的匿名值（不具名对象），它的生命周期很短，通常是暂时性的。
 *
 * 基于这一特征，我们可以用取地址符&来判断左值和右值，能取到内存地址的值为左值，
 * 否则为右值
 */

namespace E1 {
int x = 1;
int get_val() { return x; }
void set_val(int val) { x = val; }
void use() {
	x++; // R 生成一份临时赋值，然后增加 x，然后返回临时赋值 x
	++x; // L 递增后立刻返回
	int y = get_val(); // get_val函数，该函数返回了一个全局变量x，
	// 虽然很明显变量x是一个左值，但是它经过函数返回以后变成了一个右值。
	// 原因和x++类似，在函数返回的时候编译器并不会返回x本身，而是返回x的临时复制
	set_val(6);

	// 最后需要强调的是，通常字面量都是一个右值，除字符串字面量以外
}
}
}

namespace leftReference {
// 非常量左值的引用对象很单纯，它们必须是一个左值。
// 对于这一点，常量左值引用的特性显得更加有趣，它除了能引用左值，还能够引用右值
//	int &x1 = 7;  无法通过编译
const int &x = 1;
// 请注意，虽然在结果上const int &x =11和const int x = 11是一样的，
// 但是从语法上来说，前者是被引用了，所以语句结束后11的生命周期被延长，
// 而后者当语句结束后右值11应该被销毁。
// 虽然常量左值引用可以引用右值的这个特性在赋值表达式中看不出什么实用价值，
// 但是在函数形参列表中却有着巨大的作用。
// 一个典型的例子就是复制构造函数和复制赋值运算符函数，
// 通常情况下我们实现的这两个函数的形参都是一个常量左值引用
class X {
 public:
	X() {}
	X(X &) {}
	X &operator=(const X &) { return *this; }
};

X make_x() {
	return X();
}

void use() {
	X x1;
	X x2(x1);
	X x3(make_x());
	x3 = make_x();
}
}

namespace rightReference {
// 右值引用是一种引用右值且只能引用右值的方法。
// 在语法方面右值引用可以对比左值引用，在左值引用声明中，需要在类型后添加&，
// 而右值引用则是在类型后添加&&
// 右值引用的特点之一是可以延长右值的生命周期
int i = 0;
int &j = i;   // L
int &&k = 11; // R

namespace E1 {
class X {
 public:
	X() { cout << "X ctor ..." << endl; }
	X(const X &x) { cout << "X copy ctor" << endl; }
	~X() { cout << "X dtoe" << endl; }
	void show() { cout << "show X" << endl; }
};
auto make_x() { return X(); }
void use() {
	X &&x2 = make_x();
	x2.show();
//	X ctor make_x 默认构造
//	X copy ctor return 复制构造
//	X dtor dtor 临时对象
//	show X
//	X dtor
//	让我们想一下如果将X &&x2 = make_x()这句代码替换为X x2 = make_x()
//	会发生几次构造。在没有进行任何优化的情况下应该是3次构造，
//	首先make_x函数中x1会默认构造一次，
//	然后return x1会使用复制构造产生临时对象，
//	接着X x2 = make_x()会使用复制构造将临时对象复制到x2，
//	最后临时对象被销毁
// 所以我们可以在X &&x2 = make_x()语句结束后继续调用show函数而不会发生任何问题。
// 对性能敏感的读者应该注意到了，延长临时对象生命周期并不是这里右值引用的最终目标，
// 其真实目标应该是减少对象复制，提升程序性能。	。

}
}

namespace E2 {
class BigMemoryPool {
	char *_pool;
 public:
	static const int PoolSize = 4096;
	BigMemoryPool() : _pool(new char[PoolSize]) {};
	~BigMemoryPool() {
		delete[] this->_pool;
	}
	BigMemoryPool(const BigMemoryPool &other) : _pool(new char[PoolSize]) {
		cout << "copy big memory pool ..." << endl;
		::memcpy(_pool, other._pool, PoolSize);
	}
};

auto get_pool(const BigMemoryPool &pool) { return pool; }
auto make_pool() { return get_pool(BigMemoryPool()); }
void use() {
	auto pool = make_pool();
//	copy big memory pool.
//	copy big memory pool.
//	copy big memory pool.
// 可以看到BigMemoryPool my_pool = make_pool();调用了3次复制构造函数。
// 1．get_pool返回的BigMemoryPool临时对象调用复制构造函数复制了pool对象。
// 2．make_pool返回的BigMemoryPool临时对象调用复制构造函数复制了get_pool返回的临时对象。
// 3．main函数中my_pool调用其复制构造函数复制make_pool返回的临时对象。
}
}

namespace E3 {
class BigMemoryPool {
	char *_pool;
 public:
	static const int PoolSize = 4096;
	BigMemoryPool() : _pool(new char[PoolSize]) {};
	~BigMemoryPool() {
		delete[] this->_pool;
	}

	BigMemoryPool(BigMemoryPool &&other) noexcept {
		cout << "move big memory pool" << endl;
		this->_pool = other._pool;
		other._pool = nullptr;
	}

	BigMemoryPool(const BigMemoryPool &other) : _pool(new char[PoolSize]) {
		cout << "copy big memory pool" << endl;
		std::memcpy(this->_pool, other._pool, PoolSize);
	}

	BigMemoryPool &operator=(BigMemoryPool &&other)  noexcept {
		cout << "move(=) big memory pool" << endl;
		delete[] this->_pool;
		this->_pool = other._pool;
		other._pool = nullptr;
		return *this;
	}
};
auto get_pool(const BigMemoryPool &pool) { return pool; }
auto make_pool() { return get_pool(BigMemoryPool()); }
void use() {
	auto pool = make_pool();
}
}
}