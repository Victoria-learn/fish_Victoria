#include "FishSchool.h" // 引入鱼群类的成员声明。

#include <algorithm> // 引入 std::remove_if 算法。

#include "GameRules.h" // 引入鱼群生成相关的规则常量。

FishSchool::FishSchool(Random& random) // 定义构造函数。
    : fishes_(), // 先创建一条鱼都不装的空容器。
      random_(random), // 保存外部随机数工具的引用。
      targetCount_(0) {} // 目标数量先设为 0，等 reset 时再确定。

void FishSchool::reset(std::size_t targetCount, const Rect& water) { // 定义重建鱼群函数。
    targetCount_ = targetCount; // 记录本阶段需要维持的鱼的数量。
    fishes_.clear(); // 清空旧鱼群，避免上一局的对象残留下来。
    refillToTargetCount(water); // 按新目标数量重新生成一整批鱼。
} // 重建鱼群函数结束。

void FishSchool::update(float deltaTime, const Rect& water) { // 定义鱼群更新函数。
    for (EnemyFish& fish : fishes_) { // 用范围 for 遍历容器中的每一条鱼。
        fish.update(deltaTime); // 让这条鱼按自己的速度前进。
    } // 遍历结束。

    removeRecycledFish(water); // 回收已经完全游出水域的鱼。
    refillToTargetCount(water); // 补充新鱼，使鱼群数量保持稳定。
} // 鱼群更新函数结束。

void FishSchool::draw() const { // 定义鱼群绘制函数。
    for (const EnemyFish& fish : fishes_) { // 遍历容器中的每一条鱼，const 表示不修改它们。
        fish.draw(); // 调用这条鱼自己的绘制函数。
    } // 遍历结束。
} // 鱼群绘制函数结束。

std::size_t FishSchool::size() const { return fishes_.size(); } // 返回当前鱼的数量。

EnemyFish FishSchool::createRandomFish(const Rect& water) { // 定义随机生成函数。
    const float radius = random_.nextFloat(GameRules::kNpcMinRadius, GameRules::kNpcMaxRadius); // 随机体型大小。
    const float speed = random_.nextFloat(GameRules::kNpcMinSpeed, GameRules::kNpcMaxSpeed); // 随机游动速度。

    // 用一次随机数决定这条鱼从左边还是从右边出现，保证两侧机会均等。
    const bool fromLeftSide = random_.nextFloat(0.0f, 1.0f) < 0.5f; // 随机值小于 0.5 就从左侧出现。

    // 纵坐标必须留出鱼身的垂直半高，否则鱼的上下边缘会超出水域。
    const float halfHeight = radius * GameRules::kFishBodyHeightRatio; // 计算鱼身的垂直半高。
    const float positionY = random_.nextFloat(water.top + halfHeight, water.bottom - halfHeight); // 在水域内随机取纵坐标。

    Point position; // 保存生成位置。
    Point velocity; // 保存游动速度。
    position.y = positionY; // 两个分支的纵坐标相同，先统一赋值。
    velocity.y = 0.0f; // NPC 鱼只做水平游动，垂直速度恒为 0。
    if (fromLeftSide) { // 从左侧出现的情况。
        position.x = water.left - radius; // 鱼头正好贴在水域左边界之外，鱼身大部分还在画面外。
        velocity.x = speed; // 水平速度向右，鱼会游进水里面。
    } else { // 从右侧出现的情况。
        position.x = water.right + radius; // 鱼头正好贴在水域右边界之外。
        velocity.x = -speed; // 水平速度向左，鱼会游进水里面。
    } // 生成位置与速度设置结束。

    return EnemyFish(position, radius, velocity, GameRules::colorForRadius(radius)); // 返回构造好的鱼对象。
} // 随机生成函数结束。

void FishSchool::removeRecycledFish(const Rect& water) { // 定义回收函数。
    // remove_if 只把需要保留的元素往前搬，真正的删除要配合 erase 完成，这是 STL 的惯用写法。
    const auto newEnd = std::remove_if(fishes_.begin(), fishes_.end(), // 在整段区间内查找需要回收的鱼。
                                       [&water](const EnemyFish& fish) { // 用 lambda 描述回收条件。
                                           return fish.isReadyToRecycle(water); // 完全游出水域的鱼才回收。
                                       }); // lambda 结束。
    fishes_.erase(newEnd, fishes_.end()); // 擦除尾部被标记为回收的元素。
} // 回收函数结束。

void FishSchool::refillToTargetCount(const Rect& water) { // 定义补鱼函数。
    while (fishes_.size() < targetCount_) { // 只要数量不足就继续生成。
        fishes_.push_back(createRandomFish(water)); // 把新生成的鱼加入容器末尾。
    } // 补鱼循环结束。
} // 补鱼函数结束。
