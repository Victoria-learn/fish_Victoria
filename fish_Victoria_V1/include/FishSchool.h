#pragma once

#include "EnemyFish.h" // NPC 鱼类
#include "Random.h" // 随机数

#include <cstddef> 
#include <vector>

// FishSchool 负责创建、保存、更新和清理所有 NPC 鱼
// 它依赖一个外部 Random 对象来生成随机参数，但不拥有 Random，依赖关系
class FishSchool {
public:
    explicit FishSchool(Random& random); //传入外部随机数工具

    void reset(std::size_t targetCount, const Rect& water); // 按目标数量重建整个鱼群。
    void update(float deltaTime, const Rect& water); // 更新所有鱼，回收已游出去的鱼后补齐数量。
    void draw() const; // 绘制所有鱼，不修改鱼群状态。

    std::size_t size() const; // 返回当前鱼的数量

private:
    EnemyFish createRandomFish(const Rect& water); // 在水域左侧或右侧外侧生成一条参数随机的鱼。
    void removeRecycledFish(const Rect& water); // 回收已经完全游出水域的鱼。
    void refillToTargetCount(const Rect& water); // 补充新鱼，使数量回到目标值。

    std::vector<EnemyFish> fishes_; // 保存所有 NPC 鱼对象
    Random& random_; // 引用外部随机数工具，本类不负责创建和销毁它。
    std::size_t targetCount_; // 需要维持的鱼的数量
};
