This project is developed using Tuya SDK, which enables you to quickly develop branded
apps connecting and controlling smart scenarios of many devices.
For more information, please check Tuya Developer Website

#2021-03-11 实现OLED显示温湿度及当前剩余电量，通过WB3S上传温湿度、环境光、电池状态等数据，手机使用涂鸦智能APP绑定模块 成功收到模块上报的数据并实时更新。  PS：功能暂时尚未全部实现，代码会不断优化。

## -温湿度计--
&amp;gt; **V1.0功能说明(当前)：**  
* 1、1.3寸oled显示时间日期、温度、湿度、电量等数据 
* * 2、按键进行相关功能设置 
* * 3、使用DS3231作为时钟IC，方便离线时提供较准确的时间 
* * 4、使用SHT30作为温湿度传感器，动态性较好，精度较高
* * 5、使用VEML6030对周围环境光进行采样，实现oled亮度自动调节，夜间熄屏运行 
* * 6、通过涂鸦智能的WB3S无线模块实现APP控制及查看当前环境数据


