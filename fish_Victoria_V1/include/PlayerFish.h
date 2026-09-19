#pragma once // 防止头文件被重复包含。

#include "Geometry.h" // 引入 Point 和 Rect 类型的定义。

// 玩家鱼只负责保存自身状态、处理移动和绘制。
// 游戏流程由 Game 类负责，避免一个类承担所有工作。
class PlayerFish { // 定义玩家鱼类。
public:
    PlayerFish(Point startPosition, float radius); // 声明构造函数。

    // 返回 false 表示玩家按下了 ESC，请求结束游戏。
    bool move(float deltaTime, const Rect& waterArea); // 读取输入并更新位置，位置会被限制在水域内。
    void draw() const; // 绘制玩家鱼，const 表示不修改对象状态。

    Point getPosition() const; // 返回玩家鱼的位置。
    float getRadius() const; // 返回玩家鱼的半径。

private:
    Point position_; // 保存玩家鱼中心点坐标。
    float radius_; // 保存玩家鱼的大小。
    float speed_; // 保存玩家鱼每秒移动的距离。
    bool facingRight_; // 保存当前朝向，决定尾鳍画在左侧还是右侧。
}; // 玩家鱼类定义结束。
