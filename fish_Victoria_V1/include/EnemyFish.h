#pragma once // 防止头文件被重复包含。

#include "Geometry.h" // 引入 Point 和 Rect 的定义。

// EnemyFish 表示一条由程序控制的 NPC 鱼。
// 它只负责保存自己的状态、按速度移动和绘制自己，不负责创建或删除其他鱼。
// NPC 鱼只在水平方向游动，从水域左右两侧进入、也从左右两侧离开，上下边界与它无关。
// 实验二刻意不与 PlayerFish 合并，实验三会从这个重复中提取公共基类 Fish。
class EnemyFish { // 定义 NPC 鱼类。
public:
    EnemyFish(Point position, float radius, Point velocity, int bodyColor); // 声明构造函数。

    void update(float deltaTime); // 按自身速度更新位置，deltaTime 单位是秒。
    void draw() const; // 绘制这条鱼，const 表示不修改对象状态。

    // 判断这条鱼是否可以回收：整条鱼完全离开水域，并且继续向外游出回收边距。
    // 不用"碰到边界就删"，是因为鱼本身是在水域外侧生成的，碰到就删会让它刚出生就消失。
    bool isReadyToRecycle(const Rect& water) const; // 声明回收判定函数。

    Point getPosition() const; // 返回鱼的中心坐标。
    float getRadius() const; // 返回鱼的半径，实验三用它比较体型。

private:
    Point position_; // 保存鱼的中心点坐标。
    float radius_; // 保存鱼的体型大小。
    Point velocity_; // 保存每秒的位移量，包含方向信息。
    int bodyColor_; // 保存鱼身填充颜色。
    bool facingRight_; // 保存当前朝向，决定尾鳍画在左侧还是右侧。
}; // NPC 鱼类定义结束。
