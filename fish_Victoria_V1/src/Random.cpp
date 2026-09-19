#include "Random.h" // 引入随机数工具类的成员声明。

Random::Random(unsigned int seed) // 定义构造函数。
    : engine_(seed), seed_(seed) {} // 用种子初始化引擎，并保存种子供界面显示。

float Random::nextFloat(float minInclusive, float maxExclusive) { // 定义浮点随机函数。
    std::uniform_real_distribution<float> distribution(minInclusive, maxExclusive); // 建立指定区间的均匀分布。
    return distribution(engine_); // 用引擎产生一个符合分布的浮点数。
} // 浮点随机函数结束。

unsigned int Random::seed() const { return seed_; } // 返回保存的种子值。
