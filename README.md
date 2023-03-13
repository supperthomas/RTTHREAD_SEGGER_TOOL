# RTTHREAD_SEGGER_TOOL

## Introduce

这个软件包主要将JLINK作为RTT 的console口来使用，能够节省一些设备的UART资源。该软件包必须依赖于JLINK之上的，手上必须要有JLINK相关的硬件调试器才能使用。

## 如何使用

rt_hw_jlink_rtt_init 初始化必须在`rt_console_set_device(RT_CONSOLE_DEVICE_NAME);`

rtconfig中的console也要设置为`jlinkRtt`

实现这两点，基本可以将console设置为segger_rtt

```
#define RT_CONSOLE_DEVICE_NAME "jlinkRtt"
```

### JLINK_RTT_VIEWER配置

根据你当前的芯片以及JTAG还是SWD来选择

![image-20210523144320179](images/image-20210523144320179.png)

这边需要注意的是最下面的地址，指的是代码中&_SEGGER_RTT的地址，有些芯片支持自动识别地址，有些芯片不支持自动识别地址。可以设置search 范围

```
0x20000000 0x1000
```

### SEGGER_RTT 地址

segger_rtt的地址，这边通过修改代码，将地址默认设置在0x20000000上面，目前只支持KEIL这样做，后面会支持其他的

```
SEGGER_RTT_PUT_CB_SECTION(SEGGER_RTT_CB_ALIGN(SEGGER_RTT_CB _SEGGER_RTT))__attribute__((section(".ARM.__at_0x20000000")));
```

## 使用技巧

可以在RTT viewer 连上之后，打开putty

配置如下：

![image-20210523144722163](images/image-20210523144722163.png)

![image-20210523145133760](images/image-20210523145133760.png)

也可以在RTT_VIEWER里面输入，putty和VIEWER只能用一种。输入Enter或者回车可以输入命令

![image-20210523145349136](images/image-20210523145349136.png)



## 注意事项

你需要有JLINK连接。 在STM32或者nordic开发板上测试都是可以用的。其他的JLINK应该也是通用的，这个省去了调试的UART串口的占用。可以用这个口打印log或者console调试。