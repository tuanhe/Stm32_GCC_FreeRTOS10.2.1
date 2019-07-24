# STM32 Makefile FreeRTOS 10.2.1


## 项目参考 
(转自：将Freertos9.0版本更新到最新版本.源作者makefile写的清晰明了，值得学习！)   
https://github.com/freelamb/stm32f10x_makefile_freertos   


## 环境
开发平台:archlinux
编译器：arm-none-eab-gcc
项目管理:makefile

## 使用
### 编译工程
```
$ make
```

### 清除编译输出

```
$ make clean
```

### 通过stlink下载程序到MCU
```
$ make flash
```

### 擦除程序
```
$ make erase
```


```

## st-link Debug

start debug
```
$ st-util
```

open shell in project root dir
```
$ arm-none-eabi-gdb *.elf

GNU gdb (GNU Tools for ARM Embedded Processors) 7.10.1.20160923-cvs
Copyright (C) 2015 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "--host=x86_64-apple-darwin10 --target=arm-none-eabi".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
<http://www.gnu.org/software/gdb/documentation/>.
For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from z2_cmcc_b_app.elf...done.
...
(gdb) target extended-remote :4242
...
```

Clion configure debug reference

http://www.jianshu.com/p/4635a2380698

## Example

mcu: STM32F103C8T6

GPIOB11--Led

