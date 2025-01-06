@echo off
REM 编译 C++ 源代码
g++ -o fla main.cpp

REM 检查编译是否成功
if %errorlevel% neq 0 (
    echo 编译失败
    exit /b %errorlevel%
)

REM 运行生成的可执行文件
.\fla.exe ..\pda\anbn.pda aaabbb

REM 暂停以查看输出
pause