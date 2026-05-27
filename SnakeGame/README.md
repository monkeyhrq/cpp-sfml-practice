# SnakeGame 貪吃蛇

使用 C++ 與 SFML 製作的經典貪吃蛇遊戲。

## 示範影片

https://github.com/monkeyhrq/cpp-sfml-practice/blob/main/SnakeGame/貪吃蛇%20.mp4

## 技術

- 語言：C++
- 函式庫：SFML 2.x
- IDE：Visual Studio 2022

## 遊戲規則

- 方向鍵控制蛇的移動
- 吃到食物會變長，吃滿 10 個過關
- 吃到第 3 及第 6 個食物時自動加速
- 撞牆或咬到自己為失敗，按任意鍵重新開始

## 如何執行

1. 安裝 SFML 2.x（建議透過 [vcpkg](https://github.com/microsoft/vcpkg)）
2. 使用 Visual Studio 開啟 `SnakeGame.sln`
3. 建置並執行
