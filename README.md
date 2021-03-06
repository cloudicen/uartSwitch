# 适用于智能家居的多控开关固件

## 概述

这个项目主要是为了解决智能开关的改造问题，最多支持八路开关，支持串口控制，按键（自回弹开关面板）控制，普通开关面板控制。可配合esp8266等，刷入espHome固件作为上位机，通过串口读取开关状态和控制开关，并接入HomeAssistant。

## 配置

硬件：

- 主控推荐arduino nano/pro mini/mini ,IO数量较多，价格便宜。
- 继电器或晶闸管模块，5v高电平触发版本的。
- 开关接线一端连接单片机输入针脚，另一端接地，开关触发就是给单片机一个低电平。

固件配置：

- 选择开关版本，默认普通的开关（非回弹），取消注释即是回弹开关或其他按钮。
- 选择针脚数量，最大数目为8（8对，8路输入，8路输出）。
- 选择IO针脚位置，直接把用到的针脚名填入数组中，输入针脚和输出针脚在数组中的位置一一对应。
- 选择串口通信波特率，默认115800。

## 使用

查询命令：

上位机以十六进制发送字节 0x18，查询开关的状态，返回值为两个十六进制字节：{0x33,data}，其中0x33是固定的返回数据头，data是开关的状态
例如：现在有八路继电器，其状态为{开，关，开，开，关，关，开，开}。每一位的开关用01表示，则开关状态表示为二进制为：10110011 --> 转换为十六进制就是：0xb3
所以返回的数据就是{0x33，0xb3}
注：编号越小的开关（在针脚定义数组中靠前的），在数据中占据的是高位，八位开关，第一个开关的状态就是二进制表示中最开头的那一位。


设置命令：

上位机发送 {0x29，data}设置开关的状态，data的格式和查询的返回值一样，是用二进制表示开关状态后转换成十六进制发送。返回值为0x33，确认收到消息。
