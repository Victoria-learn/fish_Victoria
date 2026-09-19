#pragma once // 防止头文件被重复包含。

#include "EnemyFish.h" // 引入 NPC 鱼类的定义。
#include "Random.h" // 引入随机数工具类的定义。

#include <cstddef> // 引入 std::size_t 类型。
#include <vector> // 引入 std::vector 容器。

// FishSchool 负责创建、保存、更新和清理所有 NPC 鱼。
// 它组合（拥有）一个 vector<EnemyFish>，因此鱼的生命周期由它管理。
// 它依赖一个外部 Random 对象来生成随机参数，但不拥有 Random，这就是依赖关系的例子。
class FishSchool { // 定义鱼群类。
public:
    explicit FishSchool(Random& random); // 声明构造函数，传入外部随机数工具的引用。

    void reset(std::size_t targetCount, const Rect& water); // 按目标数量重建整个鱼群。
    void update(float deltaTime, const Rect& water); // 更新所有鱼，回收已游出去的鱼后补齐数量。
    void draw() const; // 绘制所有鱼，const 表示不修改鱼群状态。

    std::size_t size() const; // 返回当前鱼的数量，供 HUD 显示。

private:
    EnemyFish createRandomFish(const Rect& water); // 在水域左侧或右侧外侧生成一条参数随机的鱼。
    void removeRecycledFish(const Rect& water); // 回收已经完全游出水域的鱼。
    void refillToTargetCount(const Rect& water); // 补充新鱼，使数量回到目标值。

    std::vector<EnemyFish> fishes_; // 保存所有 NPC 鱼对象，是本类拥有的资源。
    Random& random_; // 引用外部随机数工具，本类不负责创建和销毁它。
    std::size_t targetCount_; // 保存需要维持的鱼的数量。
}; // 鱼群类定义结束。
