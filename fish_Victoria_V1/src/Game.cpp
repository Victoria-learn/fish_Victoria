#include "Game.h" // 引入游戏类的成员声明。

#include <graphics.h> // 引入 EasyX 窗口和绘图函数。
#include <windows.h> // 引入 Sleep 函数。
#include <tchar.h> // 引入 TCHAR 类型和 _stprintf_s 格式化函数。

#include <chrono> // 引入高精度计时工具。

#include "GameRules.h" // 引入集中定义的窗口、水域和鱼群规则。

namespace { // 创建只在当前源文件中可见的匿名命名空间。
constexpr float kMaxFrameSeconds = 0.05f; // 单帧最大计入时间，防止窗口卡顿后出现瞬移。
constexpr int kFrameSleepMilliseconds = 16; // 每帧休眠毫秒数，使运行速度接近每秒 60 帧。
constexpr int kHudTextHeight = 16; // HUD 文字的字号。
constexpr int kHudTextLeft = 20; // 左侧文字距窗口左边的像素数。
constexpr int kHudTextRight = 560; // 右侧文字距窗口左边的像素数。
} // 匿名命名空间结束。

Game::Game() // 定义游戏类构造函数。
    : random_(GameRules::kRandomSeed), // 用固定种子创建随机数工具，保证场景可复现。
      playerFish_(Point{static_cast<float>(GameRules::kWindowWidth) / 2.0f, // 玩家鱼初始位于水域水平中心。
                        static_cast<float>(GameRules::kHudHeight) +
                            static_cast<float>(GameRules::kWindowHeight - GameRules::kHudHeight) / 2.0f}, // 初始位于水域垂直中心。
                  GameRules::kPlayerStartRadius), // 使用规则中定义的初始体型。
      fishSchool_(random_), // 把随机数工具以引用方式交给鱼群，鱼群不拥有它。
      running_(true) {} // 主循环初始状态为继续运行。

void Game::run() { // 定义游戏运行函数。
    initialize(); // 初始化窗口并建立初始鱼群。

    BeginBatchDraw(); // 开始批量绘图，减少画面闪烁。

    auto previous = std::chrono::steady_clock::now(); // 记录上一帧时间。
    while (running_) { // 只要 running_ 为 true 就持续运行。
        const auto current = std::chrono::steady_clock::now(); // 获取当前时间。
        float deltaTime = std::chrono::duration<float>(current - previous).count(); // 计算两帧间隔。
        previous = current; // 将当前时间保存为下一帧的上一帧时间。

        // 窗口被系统暂时阻塞后，不能把整段停顿当作一次移动距离。
        if (deltaTime > kMaxFrameSeconds) deltaTime = kMaxFrameSeconds; // 限制单帧最大移动时间。

        processInput(deltaTime); // 处理键盘输入。
        update(deltaTime); // 更新游戏状态。
        render(); // 绘制当前画面。
        Sleep(kFrameSleepMilliseconds); // 暂停片刻，使运行速度接近每秒 60 帧。
    } // 主循环结束。

    EndBatchDraw(); // 结束批量绘图。
    closegraph(); // 关闭 EasyX 窗口。
} // run 函数结束。

void Game::initialize() { // 定义窗口初始化函数。
    initgraph(GameRules::kWindowWidth, GameRules::kWindowHeight); // 按规则中的尺寸创建窗口。
    setbkcolor(GameRules::kBackgroundColor); // 设置深色背景，它同时是 HUD 条的底色。
    cleardevice(); // 使用背景色清空窗口。
    fishSchool_.reset(GameRules::kNpcTargetCount, GameRules::waterArea()); // 按目标数量生成第一批 NPC 鱼。
} // initialize 函数结束。

void Game::processInput(float deltaTime) { // 定义输入处理函数。
    // 输入只在 PlayerFish 中读取一次，避免同一按键被多个模块消费。
    running_ = playerFish_.move(deltaTime, GameRules::waterArea()); // 将输入交给玩家鱼处理。
} // processInput 函数结束。

void Game::update(float deltaTime) { // 定义游戏状态更新函数。
    fishSchool_.update(deltaTime, GameRules::waterArea()); // 让鱼群移动、清理越界鱼并补充新鱼。
} // update 函数结束。

void Game::render() const { // 定义画面绘制函数。
    cleardevice(); // 清除上一帧画面。
    drawWater(); // 先绘制水域背景。
    fishSchool_.draw(); // 再绘制 NPC 鱼群。
    playerFish_.draw(); // 玩家鱼最后绘制，保证它显示在最上层。
    drawHud(); // 最后绘制 HUD，避免鱼溢出到信息条上。
    FlushBatchDraw(); // 将批量绘图内容显示到窗口。
} // render 函数结束。

void Game::drawWater() const { // 定义水域绘制函数。
    const Rect water = GameRules::waterArea(); // 取出当前水域矩形。
    setfillcolor(GameRules::kWaterColor); // 设置水域填充颜色。
    // EasyX 的矩形右下角是闭区间，因此减一后正好贴住窗口边缘。
    solidrectangle(static_cast<int>(water.left), // 矩形左边界。
                   static_cast<int>(water.top), // 矩形上边界。
                   static_cast<int>(water.right) - 1, // 矩形右边界。
                   static_cast<int>(water.bottom) - 1); // 矩形下边界。
} // 水域绘制函数结束。

void Game::drawHud() const { // 定义信息条绘制函数。
    setfillcolor(GameRules::kBackgroundColor); // 设置信息条底色。
    solidrectangle(0, 0, GameRules::kWindowWidth - 1, GameRules::kHudHeight - 1); // 绘制顶部信息条。

    setbkmode(TRANSPARENT); // 设置文字背景透明。
    settextcolor(GameRules::kHudTextColor); // 设置文字颜色。
    settextstyle(kHudTextHeight, 0, _T("Microsoft YaHei")); // 设置字体大小和字体名称。

    outtextxy(kHudTextLeft, 10, _T("大鱼吃小鱼 · 实验 2：组合与依赖")); // 绘制实验标题。
    outtextxy(kHudTextLeft, 32, _T("WASD / 方向键：移动    ESC：退出")); // 绘制操作提示。

    TCHAR fishCountText[64]; // 保存格式化后的鱼群数量文字。
    _stprintf_s(fishCountText, _T("鱼群数量：%d"), static_cast<int>(fishSchool_.size())); // 把数量格式化成文字。
    outtextxy(kHudTextRight, 10, fishCountText); // 绘制鱼群数量。

    TCHAR seedText[64]; // 保存格式化后的随机种子文字。
    _stprintf_s(seedText, _T("随机种子：%u"), random_.seed()); // 把种子格式化成文字，便于复现问题。
    outtextxy(kHudTextRight, 32, seedText); // 绘制随机种子。
} // 信息条绘制函数结束。
