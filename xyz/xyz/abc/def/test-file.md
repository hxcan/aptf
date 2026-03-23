# 测试文件

这个文件用于验证 create_github_commit 工具在创建全新嵌套目录时的行为。

如果路径正确，应该位于：`xyz/abc/def/test-file.md`
如果 bug 存在，会错误地创建为：`xyz/xyz/abc/def/test-file.md` 或类似的嵌套错误路径。

测试时间：2026-03-23