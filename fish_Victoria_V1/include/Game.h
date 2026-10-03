#pragma once

#include "FishSchool.h" 
#include "PlayerFish.h" 
#include "Random.h"

// Game 管理窗口、主循环，并组合本局游戏需要的全部对象

class Game {
public:
    Game(); // 声明游戏对象构造函数，窗口尺寸取自 GameRules
    void run();

private:
    void initialize(); // 初始化 EasyX 窗口并建立初始鱼群
    void processInput(float deltaTime); // 处理玩家输入
    void update(float deltaTime); // 更新游戏状态
    void render() const; // 绘制游戏画面
    void drawWater() const; // 绘制水域背景
    void drawHud() const; // 绘制顶部信息条

    Random random_; // 组合关系：Game 拥有随机数工具，整个程序共用一个。
    PlayerFish playerFish_; // 组合关系：Game 拥有唯一的玩家鱼。
    FishSchool fishSchool_; // 组合关系：Game 拥有鱼群，鱼群内部再拥有每一条 NPC 鱼。
    bool running_; // 保存主循环是否继续运行。
};
