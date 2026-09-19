#include "Game.h" // 引入 Game 类的声明。

int main() { // 程序从 main 函数开始执行。
    Game game; // 创建一个游戏对象，窗口尺寸和规则都来自 GameRules。
    game.run(); // 调用成员函数启动游戏。
    return 0; // 返回 0 表示程序正常结束。
} // main 函数结束。
