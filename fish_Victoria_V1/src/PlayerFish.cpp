#include "PlayerFish.h" // 引入玩家鱼类的成员声明。

#include <graphics.h> // 引入 EasyX 绘图函数。
#include <windows.h> // 引入 Windows 键盘和坐标类型。

#include <cmath> // 引入 sqrt 开平方函数。

#include "GameRules.h" // 引入集中定义的形状、速度和颜色规则。

PlayerFish::PlayerFish(Point startPosition, float radius) // 定义玩家鱼构造函数。
    : position_(startPosition), // 用参数初始化中心坐标。
      radius_(radius), // 用参数初始化体型大小。
      speed_(GameRules::kPlayerSpeed), // 速度取自规则常量，不在代码中写死数字。
      facingRight_(true) {} // 初始朝向设为向右。

bool PlayerFish::move(float deltaTime, const Rect& waterArea) { // 定义移动函数。
    // 使用 GetAsyncKeyState 在每一帧查询按键状态，因此按住按键会连续移动。
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) return false; // 检测 ESC，返回 false 请求退出。
    float dx = 0.0f; // 保存水平方向的移动量。
    float dy = 0.0f; // 保存垂直方向的移动量。
    if (GetAsyncKeyState('W') & 0x8000 || GetAsyncKeyState(VK_UP) & 0x8000) dy -= 1.0f; // 按上键向上移动。
    if (GetAsyncKeyState('S') & 0x8000 || GetAsyncKeyState(VK_DOWN) & 0x8000) dy += 1.0f; // 按下键向下移动。
    if (GetAsyncKeyState('A') & 0x8000 || GetAsyncKeyState(VK_LEFT) & 0x8000) dx -= 1.0f; // 按左键向左移动。
    if (GetAsyncKeyState('D') & 0x8000 || GetAsyncKeyState(VK_RIGHT) & 0x8000) dx += 1.0f; // 按右键向右移动。

    // 斜向移动也保持与单轴移动相同的速度。
    const float directionLength = std::sqrt(dx * dx + dy * dy); // 计算方向向量长度。
    if (directionLength > 0.0f) { // 只有正在移动时才进行归一化。
        dx /= directionLength; // 归一化水平方向。
        dy /= directionLength; // 归一化垂直方向。
    } // 归一化处理结束。

    if (dx > 0.0f) facingRight_ = true; // 正在向右移动时把朝向改为向右。
    else if (dx < 0.0f) facingRight_ = false; // 正在向左移动时把朝向改为向左。

    position_.x += dx * speed_ * deltaTime; // 根据时间增量更新横坐标。
    position_.y += dy * speed_ * deltaTime; // 根据时间增量更新纵坐标。

    // 修正位置时把尾鳍一起考虑，否则鱼尾会画出水域。
    position_ = GameRules::clampFishCenter(position_, radius_, facingRight_, waterArea); // 限制整条鱼在水域内。
    return true; // 没有按 ESC，通知游戏继续运行。
} // move 函数结束。

void PlayerFish::draw() const { // 定义玩家鱼绘制函数。
    const float direction = facingRight_ ? 1.0f : -1.0f; // 朝右时为 1，朝左时为 -1，用于翻转绘制方向。

    setfillcolor(GameRules::kPlayerBodyColor); // 设置鱼身填充颜色。
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
} // draw 函数结束。

Point PlayerFish::getPosition() const { return position_; } // 返回玩家鱼坐标。
float PlayerFish::getRadius() const { return radius_; } // 返回玩家鱼半径。
