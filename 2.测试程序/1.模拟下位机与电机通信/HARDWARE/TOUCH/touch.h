/**
 ****************************************************************************************************
 * @file        touch.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-10-15
 * @brief       触摸屏驱动代码
 * @note        支持电阻、电容式触摸屏
 *              触摸屏驱动支持ADS7843/7846/UH7843/7846/XPT2046/TSC2046/GT9147/GT9271/FT5206等
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 * 
 * 实验平台:正点原子 APM32F407最小系统板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 * 
 ****************************************************************************************************
 */

#ifndef __TOUCH_H
#define __TOUCH_H

#include "sys.h"

/* 电阻触摸屏引脚定义 */
#define T_PEN_GPIO_PORT             GPIOB
#define T_PEN_GPIO_PIN              GPIO_PIN_1
#define T_PEN_GPIO_CLK_ENABLE()     do{ RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOB); }while(0)

#define T_CS_GPIO_PORT              GPIOC
#define T_CS_GPIO_PIN               GPIO_PIN_13
#define T_CS_GPIO_CLK_ENABLE()      do{ RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOC); }while(0)

#define T_MISO_GPIO_PORT            GPIOB
#define T_MISO_GPIO_PIN             GPIO_PIN_2
#define T_MISO_GPIO_CLK_ENABLE()    do{ RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOB); }while(0)

#define T_MOSI_GPIO_PORT            GPIOF
#define T_MOSI_GPIO_PIN             GPIO_PIN_11
#define T_MOSI_GPIO_CLK_ENABLE()    do{ RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOF); }while(0)

#define T_CLK_GPIO_PORT             GPIOB
#define T_CLK_GPIO_PIN              GPIO_PIN_0
#define T_CLK_GPIO_CLK_ENABLE()     do{ RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOB); }while(0)

/* 电阻触摸屏引脚IO操作 */
#define T_MOSI(x)                   do{ x ?                                                 \
                                        GPIO_SetBit(T_MOSI_GPIO_PORT, T_MOSI_GPIO_PIN) :    \
                                        GPIO_ResetBit(T_MOSI_GPIO_PORT, T_MOSI_GPIO_PIN);   \
                                    }while(0)

#define T_CLK(x)                    do{ x ?                                                 \
                                        GPIO_SetBit(T_CLK_GPIO_PORT, T_CLK_GPIO_PIN) :      \
                                        GPIO_ResetBit(T_CLK_GPIO_PORT, T_CLK_GPIO_PIN);     \
                                    }while(0)

#define T_CS(x)                     do{ x ?                                                 \
                                        GPIO_SetBit(T_CS_GPIO_PORT, T_CS_GPIO_PIN) :        \
                                        GPIO_ResetBit(T_CS_GPIO_PORT, T_CS_GPIO_PIN);       \
                                    }while(0)

#define T_PEN                       GPIO_ReadInputBit(T_PEN_GPIO_PORT, T_PEN_GPIO_PIN)
#define T_MISO                      GPIO_ReadInputBit(T_MISO_GPIO_PORT, T_MISO_GPIO_PIN)

/* 触摸相关定义 */
#define TP_PRES_DOWN                0x8000  /* 触屏被按下 */
#define TP_CATH_PRES                0x4000  /* 有按键按下了 */
#define CT_MAX_TOUCH                10      /* 电容屏支持的点数,固定为5点 */

/* 触摸屏控制结构体 */
typedef struct
{
    uint8_t (*init)(void);      /* 初始化触摸屏控制器 */
    uint8_t (*scan)(uint8_t);   /* 扫描触摸屏，0：屏幕扫描；1：物理坐标 */
    void (*adjust)(void);       /* 触摸屏校准 */
    uint16_t x[CT_MAX_TOUCH];   /* 当前坐标 */
    uint16_t y[CT_MAX_TOUCH];   /* 电容屏有最多10组坐标，电阻屏则用x[0]和y[0]代表此次扫描时触屏的坐标
                                 * 用x[9]和y[9]存储第一次按下时的坐标
                                 */
    uint16_t sta;               /* 笔的状态
                                 * b15：1：按下；0：松开
                                 * b14：0：没有按键按下；1：有按键按下
                                 * b13~b10：保留
                                 * b9~b0：电容触摸屏按下的点数（0：未按下；1：按下）
                                 */
    /* 5点校准触摸屏校准参数（电容屏不需要校准） */
    float xfac;                 /* 5点校准法x方向比例因子 */
    float yfac;                 /* 5点校准法y方向比例因子 */
    short xc;                   /* 中心X坐标物理值（AD值） */
    short yc;                   /* 中心Y坐标物理值（AD值） */
    uint8_t touchtype;          /* 新增的参数,当触摸屏的左右上下完全颠倒时需要用到
                                 * b0：0：竖屏（适合左右为X坐标，上下为Y坐标的TP）
                                 *     1：横屏（适合左右为Y坐标，上下为X坐标的TP）
                                 * b1~6：保留
                                 * b7：0：电阻屏
                                 *     1：电容屏
                                 */
} _m_tp_dev;
extern _m_tp_dev tp_dev;

/* 函数声明 */
void tp_draw_big_point(uint16_t x, uint16_t y, uint16_t color); /* 画一个大点 */
void tp_save_adjust_data(void);                                 /* 保存校准参数 */
uint8_t tp_scan(uint8_t mode);                                  /* 触摸屏扫描 */
void tp_adjust(void);                                           /* 触摸屏校准 */
uint8_t tp_init(void);                                          /* 初始化触摸屏 */

#endif
