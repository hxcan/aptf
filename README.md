# aptf - 营造

aptf（营造）是一个基于 Qt 的图形界面工具集。

## 项目状态

🔄 **正在进行 Qt6 迁移**

当前版本基于 Qt5，正在迁移到 Qt6 框架以获得更好的性能和长期支持。

## 主要功能

- 图形界面工具集
- 支持 TTS 语音合成
- 可定制的设置对话框
- 丰富的图标和素材资源

## 技术栈

- **当前**: Qt5, C++11
- **目标**: Qt6, C++17

## 构建说明

### Qt5 构建

```bash
qmake AptF.pro
make
```

### CMake 构建

```bash
mkdir build && cd build
cmake ..
make
```

## 目录结构

- `AptF.cpp/h` - 主窗口代码
- `AptfSettingDialog.cpp/h/ui` - 设置对话框
- `AptfTtsClient.cpp/h` - TTS 客户端
- `Images/` - 图标和图片资源
- `Material/` - 素材资源
- `sh/` - Shell 脚本工具

## 迁移计划

详见 Redmine 任务 #4848

## 许可证

待确定

## 联系方式

项目维护者：hxcan
