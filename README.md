# stm32-remote-control

A remote control made using STM32, 2.4G communication. / 使用 STM32 制作的遥控器，2.4G 无线通信。

基于 **STM32F103C8T6 + NRF24L01** 的 2.4G 遥控器（发送端）固件，Keil MDK5 工程，使用 ST 标准外设库（StdPeriph Lib）开发。

## 功能

- **四路摇杆采集**：ADC1 扫描模式 + DMA1 循环搬运，PA1 / PA6 / PA2 / PA3 四个通道自动采集到全局数组 `ad_ww[4]`
- **数据归一化**：`DataProcess()` 把 12 位 ADC 值（0~4095）减去中点 2048，并设置 ±100 死区，再线性映射为 -100 ~ +100 的 `int8_t`
- **六路按键**：PB2 / PB10 / PB11 / PB12 / PB13 / PB14 上拉输入、低电平有效，键值 1~6，20 ms 周期扫描并做边沿消抖
- **OLED 显示**：0.96 寸 SSD1306（I2C 从机地址 0x78），PB8 = SCL、PB9 = SDA 软件 I2C，实时显示四路通道值、按键值与 NRF24L01 状态寄存器
- **2.4G 无线发送**：NRF24L01 软件 SPI（PB3 = SCK、PB5 = MOSI、PB4 = MISO、PB7 = CE、PA15 = CSN），每 80 ms 发送一包数据
- **电源维持**：TIM1 每 5 s 调用 `Open()`，把 PB15 拉低 55 ms，防止 TP4336 电源芯片进入低功耗而关机

## 硬件连接

| 外设 | 引脚 | 说明 |
| --- | --- | --- |
| 摇杆通道 1~4 | PA1 / PA6 / PA2 / PA3 | ADC1_IN1 / IN6 / IN2 / IN3，扫描顺序对应 `ad_ww[0..3]` |
| 按键 1~6 | PB2 / PB10 / PB11 / PB12 / PB13 / PB14 | 上拉输入，按下为低电平 |
| 电源控制 | PB15 | 推挽输出，低电平维持供电（TP4336） |
| OLED | PB8 / PB9 | 软件 I2C 的 SCL / SDA |
| NRF24L01 | PB3 / PB5 / PB4 / PB7 / PA15 | SCK / MOSI / MISO / CE / CSN（软件 SPI） |

## 无线参数

收发双方必须保持一致，否则无法通信：

- 射频通道：2（2402 + 2 = 2.402 GHz）
- 通信速率：2 Mbps，发射功率 0 dBm
- 地址：`0x11 0x22 0x33 0x44 0x55`（5 字节，收发地址相同）
- 开启自动应答，自动重传间隔 250 us、重传 3 次，CRC 1 字节使能
- 数据包宽度固定 32 字节（`NRF24L01_TX_PACKET_WIDTH`）

## 数据包格式

| 字节 | 内容 |
| --- | --- |
| 0 | `0x00`，包头 / 保留 |
| 1 | 通道 1 归一化值 `LAA`，`int8_t`，范围 -100 ~ +100 |
| 2 | 通道 2 归一化值 `LBB` |
| 3 | 通道 3 归一化值 `LCC` |
| 4 | 通道 4 归一化值 `LDD` |
| 5 | 按键键值 `0~6`，0 表示无按键 |
| 6~31 | 未使用（包宽固定 32 字节） |

发送成功（收到应答）后按键值会被清零，避免重复触发。

## 目录结构

```
├── Hardware/     遥控器外设驱动：AD（摇杆采集）、Key、OLED、NRF24L01
├── Library/      STM32F10x 标准外设库
├── Start/        启动文件、CMSIS 内核文件、stm32f10x.h
├── System/       延时（Delay）与定时器（Timer）
├── User/         主程序 main.c、中断服务 stm32f10x_it.c、库配置 stm32f10x_conf.h
├── RTE/          Keil 运行时环境（器件调试配置）
├── DebugConfig/  Keil 调试器配置
├── project.uvprojx  Keil MDK5 工程文件
└── keilkill.bat     一键清理 Keil 编译中间文件
```

## 编译与下载

1. 使用 **Keil MDK5**（ARMCC v5 编译器）打开 `project.uvprojx`；
2. 器件为 `STM32F103C8`，需要安装器件支持包 `Keil.STM32F1xx_DFP`；
3. 目标配置：IROM 64 KB、IRAM 20 KB，系统时钟 72 MHz；
4. 编译后通过 ST-Link / J-Link 下载到遥控器板。

`Objects/`、`Listings/` 为编译产物，已加入 `.gitignore` 不纳入版本管理，可用 `keilkill.bat` 清理。

## 说明

- 本仓库是遥控器的**发送端**代码，接收端（小车 / 云台）不在本仓库内；接收端需使用与上面一致的射频参数、地址与包宽。
- OLED 与 NRF24L01 驱动参考江协科技（jiangxiekeji.com）教程代码修改。
- 源码为 UTF-8 编码，可直接查看中文注释。
- pcb代码烧录的接口不太方便，需重新设计，不过也能凑合用
