#include "EnemyFish.h" // 引入 NPC 鱼类的成员声明。

#include <graphics.h> // 引入 EasyX 绘图函数。
#include <windows.h> // 引入 POINT 结构体和 LONG 类型。

#include "GameRules.h" // 引入集中定义的形状和颜色规则。

EnemyFish::EnemyFish(Point position, float radius, Point velocity, int bodyColor) // 定义构造函数。
    : position_(position), // 用参数初始化中心坐标。
      radius_(radius), // 用参数初始化体型大小。
      velocity_(velocity), // 用参数初始化速度向量。
      bodyColor_(bodyColor), // 用参数初始化身体颜色。
      facingRight_(velocity.x >= 0.0f) {} // 水平速度向右则朝右，否则朝左。

void EnemyFish::update(float deltaTime) { // 定义更新函数。
    position_.x += velocity_.x * deltaTime; // 按时间增量更新横坐标。
    position_.y += velocity_.y * deltaTime; // 按时间增量更新纵坐标。
} // 更新函数结束。

void EnemyFish::draw() const { // 定义绘制函数。
    const float direction = facingRight_ ? 1.0f : -1.0f; // 朝右时为 1，朝左时为 -1，用于翻转绘制方向。

    setfillcolor(bodyColor_); // 设置鱼身填充颜色。
    solidellipse(static_cast<int>(position_.x - radius_), // 鱼身椭圆的左边界。
                 static_cast<int>(position_.y - radius_ * GameRules::kFishBodyHeightRatio), // 鱼身椭圆的上边界。
                 static_cast<int>(position_.x + radius_), // 鱼身椭圆的右边界。
                 static_cast<int>(position_.y + radius_ * GameRules::kFishBodyHeightRatio)); // 鱼身椭圆的下边界。

    // 尾鳍画在朝向的反方向，因此三个顶点都要乘上 direction。
    POINT tail[3] = { // 定义尾鳍三角形的三个顶点。
        {static_cast<LONG>(position_.x - direction * radius_), // 第一个顶点贴着鱼身末端。
         static_cast<LONG>(position_.y)}, // 第一个顶点与鱼心同高。
        {static_cast<LONG>(position_.x - direction * radius_ * GameRules::kFishTailScale), // 第二个顶点在尾鳍上尖。
         static_cast<LONG>(position_.y - radius_ * GameRules::kFishBodyHeightRatio)}, // 第二个顶点在上方。
        {static_cast<LONG>(position_.x - direction * radius_ * GameRules::kFishTailScale), // 第三个顶点在尾鳍下尖。
         static_cast<LONG>(position_.y + radius_ * GameRules::kFishBodyHeightRatio)}}; // 第三个顶点在下方。
    solidpolygon(tail, 3); // 根据三个顶点绘制实心尾鳍。

    setfillcolor(GameRules::kFishEyeColor); // 设置鱼眼填充颜色。
    solidcircle(static_cast<int>(position_.x + direction * radius_ * GameRules::kFishEyeOffsetRatio), // 鱼眼画在朝向一侧。
                static_cast<int>(position_.y - radius_ * GameRules::kFishEyeHeightRatio), // 鱼眼略高于中心。
                GameRules::kFishEyeRadius); // 使用规则中定义的鱼眼半径。
} // 绘制函数结束。

bool EnemyFish::isReadyToRecycle(const Rect& water) const { // 定义回收判定函数。
    const Rect bounds = GameRules::fishOuterBounds(position_, radius_, facingRight_); // 计算整条鱼的外接矩形。
    // 鱼只水平游动，垂直坐标在生成时就落在水域内且此后不再变化，所以不需要判断上下方向。
    const bool goneLeft = bounds.right < water.left - GameRules::kNpcRecycleMargin; // 整条鱼游出左边界并超出回收边距。
    const bool goneRight = bounds.left > water.right + GameRules::kNpcRecycleMargin; // 整条鱼游出右边界并超出回收边距。
    return goneLeft || goneRight; // 任一侧满足条件就说明这条鱼可以回收。
} // 回收判定函数结束。

Point EnemyFish::getPosition() const { return position_; } // 返回鱼的中心坐标。
float EnemyFish::getRadius() const { return radius_; } // 返回鱼的体型半径。
