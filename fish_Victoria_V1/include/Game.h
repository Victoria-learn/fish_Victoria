#pragma once // 防止头文件被重复包含。

#include "FishSchool.h" // 引入鱼群类的定义。
#include "PlayerFish.h" // 引入玩家鱼类的定义。
#include "Random.h" // 引入随机数工具类的定义。

// Game 管理窗口、主循环，并组合本局游戏需要的全部对象。
// 本类只做流程协调：输入交给 PlayerFish，鱼群交给 FishSchool，自身不实现每条鱼的行为。
class Game { // 定义游戏控制类。
public:
    Game(); // 声明游戏对象构造函数，窗口尺寸取自 GameRules。
    void run(); // 启动游戏主循环。

private:
    void initialize(); // 初始化 EasyX 窗口并建立初始鱼群。
    void processInput(float deltaTime); // 处理玩家输入。
    void update(float deltaTime); // 更新游戏状态。
    void render() const; // 绘制游戏画面。
    void drawWater() const; // 绘制水域背景。
    void drawHud() const; // 绘制顶部信息条。

    // 成员声明顺序就是构造顺序：随机数先构造，才能传给后面的鱼群。
    Random random_; // 组合关系：Game 拥有随机数工具，整个程序共用一个。
    PlayerFish playerFish_; // 组合关系：Game 拥有唯一的玩家鱼。
    FishSchool fishSchool_; // 组合关系：Game 拥有鱼群，鱼群内部再拥有每一条 NPC 鱼。
    bool running_; // 保存主循环是否继续运行。
}; // 游戏控制类定义结束。
