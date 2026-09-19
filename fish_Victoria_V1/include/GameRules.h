#pragma once // 防止头文件被重复包含。

#include <windows.h> // 引入 RGB 宏和 COLORREF 类型，颜色常量需要它。

#include <cstddef> // 引入 std::size_t 类型。

#include "Geometry.h" // 引入 Point 和 Rect 的定义。

// GameRules 集中保存所有游戏规则数值，对应非功能需求 NFR-06：不允许魔法数字散落在代码中。
// 这里只放常量和纯计算函数，不放可变状态，因此使用命名空间而不是类。
namespace GameRules { // 开始定义规则命名空间。

// ---------- 窗口与活动水域 ----------
constexpr int kWindowWidth = 1000; // 窗口宽度，单位像素。
constexpr int kWindowHeight = 700; // 窗口高度，单位像素。
constexpr int kHudHeight = 60; // 顶部 HUD 条高度，该区域不属于水域，鱼不可进入。

// 返回当前的水域矩形：HUD 以下的整块区域才是鱼可以活动的范围。
inline Rect waterArea() { // 定义水域计算函数。
    return Rect{0.0f, // 左边界与窗口左边界重合。
                static_cast<float>(kHudHeight), // 上边界位于 HUD 条下沿。
                static_cast<float>(kWindowWidth), // 右边界与窗口右边界重合。
                static_cast<float>(kWindowHeight)}; // 下边界与窗口下边界重合。
} // 水域计算函数结束。

// ---------- 鱼的外形比例 ----------
// 玩家鱼和 NPC 鱼目前都画成同一个形状，因此形状比例集中在这里定义。
constexpr float kFishBodyHeightRatio = 0.7f; // 鱼身椭圆的垂直半径占半径的比例。
constexpr float kFishTailScale = 1.7f; // 尾鳍尖端相对鱼身半径的伸出倍数。
constexpr float kFishEyeOffsetRatio = 0.45f; // 鱼眼中心相对半径的水平偏移比例。
constexpr float kFishEyeHeightRatio = 0.25f; // 鱼眼中心相对半径的垂直偏移比例。
constexpr int kFishEyeRadius = 4; // 鱼眼半径，单位像素。

// 根据鱼的中心、半径和朝向，计算整条鱼（含尾鳍）的外接矩形。
// 边界规则要求尾鳍也不能越界，所以不能只用鱼身半径计算。
inline Rect fishOuterBounds(Point center, float radius, bool facingRight) { // 定义外形包围盒计算函数。
    const float halfHeight = radius * kFishBodyHeightRatio; // 计算鱼身的垂直半高。
    const float leftReach = facingRight ? radius * kFishTailScale : radius; // 朝右时左侧是尾鳍，否则左侧是鱼头。
    const float rightReach = facingRight ? radius : radius * kFishTailScale; // 朝右时右侧是鱼头，否则右侧是尾鳍。
    return Rect{center.x - leftReach, // 左边界等于中心减去左侧伸出长度。
                center.y - halfHeight, // 上边界等于中心减去垂直半高。
                center.x + rightReach, // 右边界等于中心加上右侧伸出长度。
                center.y + halfHeight}; // 下边界等于中心加上垂直半高。
} // 外形包围盒计算函数结束。

// 把鱼的中心点修正到水域内，保证整条鱼（含尾鳍）不越界。
inline Point clampFishCenter(Point center, float radius, bool facingRight, const Rect& water) { // 定义边界修正函数。
    const Rect bounds = fishOuterBounds(center, radius, facingRight); // 先算出当前整条鱼的外接矩形。
    float fixedX = center.x; // 保存修正后的横坐标，先假设不需要修正。
    float fixedY = center.y; // 保存修正后的纵坐标，先假设不需要修正。
    if (bounds.left < water.left) fixedX += water.left - bounds.left; // 左边越界就把鱼整体向右推回。
    if (bounds.right > water.right) fixedX -= bounds.right - water.right; // 右边越界就把鱼整体向左推回。
    if (bounds.top < water.top) fixedY += water.top - bounds.top; // 上边越界就把鱼整体向下推回。
    if (bounds.bottom > water.bottom) fixedY -= bounds.bottom - water.bottom; // 下边越界就把鱼整体向上推回。
    return Point{fixedX, fixedY}; // 返回修正后的中心点。
} // 边界修正函数结束。

// ---------- 玩家鱼规则 ----------
constexpr float kPlayerStartRadius = 42.0f; // 玩家鱼的初始半径。
constexpr float kPlayerSpeed = 260.0f; // 玩家鱼每秒移动的像素数。

// ---------- NPC 鱼群规则 ----------
constexpr std::size_t kNpcTargetCount = 12; // 水域中需要维持的 NPC 鱼数量。
constexpr float kNpcMinRadius = 12.0f; // NPC 鱼的最小半径。
constexpr float kNpcMaxRadius = 70.0f; // NPC 鱼的最大半径。
constexpr float kNpcMinSpeed = 60.0f; // NPC 鱼的最小速度，单位像素每秒。
constexpr float kNpcMaxSpeed = 170.0f; // NPC 鱼的最大速度，单位像素每秒。

// NPC 鱼只在水平方向游动，因此只能从水域左右两侧进入，也只能从左右两侧离开。
// 这样上下边界对 NPC 不构成出口，鱼群的进出规则保持单一，便于解释和验收。
constexpr float kNpcRecycleMargin = 30.0f; // 整条鱼完全离开水域后，还要再向外游出这么多像素才回收。

// ---------- 随机种子 ----------
// 需求 NFR-03 要求可复现，因此默认使用固定种子，每次运行生成同一批鱼，便于定位问题。
constexpr unsigned int kRandomSeed = 20250917u; // 固定随机种子，格式是年月日。

// ---------- 颜色 ----------
// 颜色常量使用 const int 而不是 constexpr，因为 RGB 宏来自 Windows 头文件。
const int kBackgroundColor = RGB(18, 42, 66); // 窗口背景色，即 HUD 条的深色。
const int kWaterColor = RGB(135, 206, 235); // 水域颜色，浅蓝色。
const int kHudTextColor = RGB(255, 255, 255); // HUD 文字颜色，白色。
const int kPlayerBodyColor = RGB(255, 190, 60); // 玩家鱼身体颜色，橙黄色。
const int kFishEyeColor = RGB(30, 30, 30); // 鱼眼颜色，近黑色。
const int kNpcColorSmall = RGB(144, 224, 239); // 小型 NPC 鱼颜色，浅青色。
const int kNpcColorMedium = RGB(120, 200, 140); // 中型 NPC 鱼颜色，绿色。
const int kNpcColorLarge = RGB(240, 160, 80); // 大型 NPC 鱼颜色，橙色。
const int kNpcColorHuge = RGB(220, 80, 90); // 特大型 NPC 鱼颜色，红色。

constexpr float kNpcSmallRadiusLimit = 24.0f; // 小于此半径的 NPC 算小型鱼。
constexpr float kNpcMediumRadiusLimit = 40.0f; // 小于此半径的 NPC 算中型鱼。
constexpr float kNpcLargeRadiusLimit = 56.0f; // 小于此半径的 NPC 算大型鱼，其余算特大型。

// 按半径选择 NPC 鱼的颜色，让体型差异在画面上一眼可辨。
inline int colorForRadius(float radius) { // 定义颜色选择函数。
    if (radius < kNpcSmallRadiusLimit) return kNpcColorSmall; // 小型鱼使用浅青色。
    if (radius < kNpcMediumRadiusLimit) return kNpcColorMedium; // 中型鱼使用绿色。
    if (radius < kNpcLargeRadiusLimit) return kNpcColorLarge; // 大型鱼使用橙色。
    return kNpcColorHuge; // 其余都按特大型鱼使用红色。
} // 颜色选择函数结束。

} // 规则命名空间结束。
