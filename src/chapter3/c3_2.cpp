#include <string>
#include <map>
//
// Created by 华邵钶 on 2023/1/13.
//
// 当一眼就能看出声明变量的初始化类型的时候可以使用auto
// 对于复杂的类型，例如lambda表达式、bind等直接使用auto
int main() {

	// 不使用 auto
	std::map<std::string, int> str2int;
	for (std::map<std::string, int>::const_iterator it = str2int.cbegin();
		 it != str2int.end(); ++it) {
	}

	for (std::pair<const std::string, int> &it : str2int) {
	}

	// 使用 auto
	for (auto it = str2int.cbegin(); it != str2int.end(); ++it) {}
	for (auto &it : str2int) {}

	//上面的代码如果不用auto来声明迭代器，
	// 那么我们需要编写std::map<std::string, int>::const_iterator
	// 和std::pair<conststd::string, int>来代替auto，
	// 而多出来的代码并不会增强代码的可读性，
	// 反而会让代码看起来冗余，因为通常我们一眼就能看明白it的具体类型。

	//对于第二条规则，我们有时候会遇到无法写出类型或者过于复杂的类型，
	// 或者即使能正确写出某些复杂类型，但是其他程序员阅读起来也很费劲，
	// 这种时候建议使用auto来声明，例如lambda表达式

	auto l = [](int a1, int a2) { return a1 + a2; };
	// 这里l的类型可能是一个这样的名称
	// xxx::<lambda_efdefb7231ea07622630c86251a36ed4>
	// （不同的编译器命名方法会有所不同），
	// 我们根本无法写出其类型，只能用auto来声明。

	auto b = std::bind(l, 5, std::placeholders::_1);
	// 你不会希望写出这个变量的类型的 ...
}