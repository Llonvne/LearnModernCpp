//
// Created by 华邵钶 on 2023/1/14.
// 范型 lambda 表达式
// C++14标准让lambda表达式具备了模版函数的能力，我们称它为泛型lambda表达式。
// 虽然具备模版函数的能力，但是它的定义方式却用不到template关键字。
// 实际上泛型lambda表达式语法要简单很多，我们只需要使用auto占位符即可
#include "future"
#include "vector"
#include "map"
namespace GenericsLambda::E1 {
void use() {
	auto foo = [](auto a) { return a; };
	int three = foo(3);
	auto hello = foo("Hello");
}
}

// 捕获*this
namespace CaptureThis {
// 还记得前面初始化捕获*this对象的代码吗？我们在捕获列表内复制了一份this指向的对象到tmp，
// 然后使用tmp的value。
// 没错，这样做确实解决了异步问题，但是这个解决方案并不优美。
// 试想一下，如果在lambda表达式中用到了大量this指向的对象，那我们就不得不将它们全部修改，
// 一旦遗漏就会引发问题。为了更方便地复制和使用*this对象，
// C++17增加了捕获列表的语法来简化这个操作，具体来说就是在捕获列表中直接添加[*this]，
// 然后在lambda表达式函数体内直接使用this指向对象的成员
namespace E1 {
class Work {
 private:
	int value;

 public:
	Work() : value(42) {}
	std::future<int> spawn() {
		return std::async([=, *this]() -> int { return value; });

		// 从结果可以看出，[*this]的语法让程序生成了一个*this对象的副本并存储在lambda表达式内，
		// 可以在lambda表达式内直接访问这个复制对象的成员，
		// 消除了之前lambda表达式需要通过tmp访问对象成员的尴尬。
	}
};
}
namespace E2 {
// 捕获[=, this]
// 在C++20标准中，又对lambda表达式进行了小幅修改。这一次修改没有加强lambda表达式的能力，
// 而是让this指针的相关语义更加明确。我们知道[=]可以捕获this指针，相似的，
// [=,*this]会捕获this对象的副本。
// 但是在代码中大量出现[=]和[=,*this]的时候我们可能很容易忘记前者与后者的区别。
// 为了解决这个问题，在C++20标准中引入了[=, this]捕获this指针的语法，
// 它实际上表达的意思和[=]相同，目的是让程序员们区分它与[=,*this]的不同
// [=, this]{}; // C++17 编译报错或者报警告， C++20成功编译
template<class T>
void g(T) {}

struct Foo {
	int n = 0;
	void f(int a) const {
		g([=](int k) { return n + a * k; });
	}
};
}
}

// 模板语法的泛型lambda表达式
namespace TemplateGenericsLambda {
// 了C++14标准中lambda表达式通过支持auto来实现泛型。
// 大部分情况下，这是一种不错的特性，
// 但不幸的是，这种语法也会使我们难以与类型进行互动，对类型的操作变得异常复杂。用提案文档的举例来说：
template<typename T>
struct is_std_vector : std::false_type {};

template<typename T>
struct is_std_vector<std::vector<T>> : std::true_type {};
auto f = [](auto vector) {
	static_assert(is_std_vector<decltype(vector)>::value, "123");

	// 获取类型
	using T = typename decltype(vector)::value_type;
};

// 进一步来说，decltype(obj)有时候并不能直接获取我们想要的类型。
auto f2 = [](const auto &x) {
	using T = decltype(x);
	T copy = x;
//	using Iterator = typename T::iterator;
// decltype(x)推导出来的类型并不是std::vector ，而是const std::vector &，
// 所以T copy = x;不是一个复制而是引用。对于一个引用类型来说，T::iterator也是不符合语法的，
// 所以编译出错。在提案文档中，作者很友好地给出了一个解决方案，
// 他使用了STL的decay，这样就可以将类型的cv以及引用属性删除
};

auto f3 = [](const auto &x) {
	using T = std::decay_t<decltype(x)>;
	T copy = x;
	using Iterator = typename T::iterator;
};
void use() {
	std::vector<int> v;
	f2(v);
	f3(v);
}

// 鉴于以上种种问题，C++委员会决定在C++20中添加模板对lambda的支持，语法非常简单：
auto f4 = []<typename T>(T t) {};
// 以上可以改写为
auto f5 = []<typename T>(T const &x) {
	T copy = x;
	using Iterator = typename T::iterator;
};
}

// 可构造可赋值的无状态 lambda 表达式
namespace ConstructableAndAssignableLambda {
// 我们提到了无状态lambda表达式可以转换为函数指针，
// 但遗憾的是，在C++20标准之前无状态的lambda表达式类型既不能构造也无法赋值，
// 这阻碍了许多应用的实现。
// 举例来说，我们已经了解了像std::sort和std::find_if这样的函数需要一个函数对象
// 或函数指针来辅助排序和查找，这种情况我们可以使用lambda表达式完成任务。
// 但是如果遇到std::map这种容器类型就不好办了，
// 因为std::map的比较函数对象是通过模板参数确定的，这个时候我们需要的是一个类型：
auto greater = [](auto x, auto y) { return x > y; };
std::map<std::string, int, decltype(greater)> mymap;
// 这段代码的意图很明显，它首先定义了一个无状态的lambda表达式greater，
// 然后使用decltype(greater)获取其类型作为模板实参传入模板。这个想法非常好，
// 但是在C++17标准中是不可行的，因为lambda表达式类型无法构造。编译器会明确告知，
// lambda表达式的默认构造函数已经被删除了（
// “note:a lambda closure typehas a deleted default constructor”）。

auto greater2 = [](auto x, auto y) { return x > y; };
std::map<std::string, int, decltype(greater)> mymap1, mymap2;
void use(){
	mymap1 = mymap2;
	// 这里mymap1 = mymap2;也会被编译器报错，
	// 原因是复制赋值函数也被删除了
	// （“note: a lambda closure type has a deleted copy assignment operator”）。
	// 为了解决以上问题，C++20标准允许了无状态lambda表达式类型的构造和赋值，
	// 所以使用C++20标准的编译环境来编译上面的代码是可行的。
}
}

int main() {
	TemplateGenericsLambda::use();
}