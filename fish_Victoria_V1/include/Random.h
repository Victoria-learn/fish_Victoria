#pragma once // 防止头文件被重复包含。

#include <random> // 引入 std::mt19937 随机数引擎和分布类型。

// Random 封装随机数引擎，让鱼群的生成结果可以被固定种子复现。
// 需求 NFR-03 要求可复现，因此不允许在代码中直接散落 rand() 调用。
class Random { // 定义随机数工具类。
public:
    explicit Random(unsigned int seed); // 声明构造函数，seed 决定整局游戏的随机序列。

    float nextFloat(float minInclusive, float maxExclusive); // 返回 [min, max) 区间内的浮点数。

    unsigned int seed() const; // 返回当前种子，用于在界面上显示以便复现问题。

private:
    std::mt19937 engine_; // 保存随机数引擎，它是本类唯一的可变状态。
    unsigned int seed_; // 保存构造时传入的种子，仅用于查询和显示。
}; // 随机数工具类定义结束。
