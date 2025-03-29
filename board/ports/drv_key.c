/**
 * @File:    flexible_button_demo.c
 * @Author:  MurphyZhao
 * @Date:    2018-09-29
 * 
 * Copyright (c) 2018-2019 MurphyZhao <d2014zjt@163.com>
 *               https://github.com/murphyzhao
 * All rights reserved.
 * License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Change logs:
 * Date        Author       Notes
 * 2018-09-29  MurphyZhao   First add
 * 2019-08-02  MurphyZhao   迁移代码到 murphyzhao 仓库
*/

#include <rtthread.h>
#include "flexible_button.h"
#include <stdint.h>
#include <rtdevice.h>
#include <board.h>

#ifdef BSP_USING_KEY

#define PIN_KEY1	GET_PIN(A, 0)
#define PIN_KEY2	GET_PIN(C, 13)

typedef enum
{
    USER_BUTTON_1 = 0,
    USER_BUTTON_2,
    USER_BUTTON_MAX
} user_button_t;

static flex_button_t user_button[USER_BUTTON_MAX];

static void btn_1_cb(flex_button_t *btn)
{
    rt_kprintf("btn_1_cb\n");
    switch (btn->event)
    {
        case FLEX_BTN_PRESS_DOWN:
            rt_kprintf("btn_1_cb [FLEX_BTN_PRESS_DOWN]\n");
            break;
        case FLEX_BTN_PRESS_CLICK:
            rt_kprintf("btn_1_cb [FLEX_BTN_PRESS_CLICK]\n");
            break;
        case FLEX_BTN_PRESS_DOUBLE_CLICK:
            rt_kprintf("btn_1_cb [FLEX_BTN_PRESS_DOUBLE_CLICK]\n");
            break;
        case FLEX_BTN_PRESS_SHORT_START:
            rt_kprintf("btn_1_cb [FLEX_BTN_PRESS_SHORT_START]\n");
            break;
        case FLEX_BTN_PRESS_SHORT_UP:
            rt_kprintf("btn_1_cb [FLEX_BTN_PRESS_SHORT_UP]\n");
            break;
        case FLEX_BTN_PRESS_LONG_START:
            rt_kprintf("btn_1_cb [FLEX_BTN_PRESS_LONG_START]\n");
            break;
        case FLEX_BTN_PRESS_LONG_UP:
            rt_kprintf("btn_1_cb [FLEX_BTN_PRESS_LONG_UP]\n");
            break;
        case FLEX_BTN_PRESS_LONG_HOLD:
            rt_kprintf("btn_1_cb [FLEX_BTN_PRESS_LONG_HOLD]\n");
            break;
        case FLEX_BTN_PRESS_LONG_HOLD_UP:
            rt_kprintf("btn_1_cb [FLEX_BTN_PRESS_LONG_HOLD_UP]\n");
            break;
    }
}

static void btn_2_cb(flex_button_t *btn)
{
    rt_kprintf("btn_2_cb\n");
    switch (btn->event)
    {
        case FLEX_BTN_PRESS_DOWN:
            rt_kprintf("btn_2_cb [FLEX_BTN_PRESS_DOWN]\n");
            break;
        case FLEX_BTN_PRESS_CLICK:
            rt_kprintf("btn_2_cb [FLEX_BTN_PRESS_CLICK]\n");
            break;
        case FLEX_BTN_PRESS_DOUBLE_CLICK:
            rt_kprintf("btn_2_cb [FLEX_BTN_PRESS_DOUBLE_CLICK]\n");
            break;
        case FLEX_BTN_PRESS_SHORT_START:
            rt_kprintf("btn_2_cb [FLEX_BTN_PRESS_SHORT_START]\n");
            break;
        case FLEX_BTN_PRESS_SHORT_UP:
            rt_kprintf("btn_2_cb [FLEX_BTN_PRESS_SHORT_UP]\n");
            break;
        case FLEX_BTN_PRESS_LONG_START:
            rt_kprintf("btn_2_cb [FLEX_BTN_PRESS_LONG_START]\n");
            break;
        case FLEX_BTN_PRESS_LONG_UP:
            rt_kprintf("btn_2_cb [FLEX_BTN_PRESS_LONG_UP]\n");
            break;
        case FLEX_BTN_PRESS_LONG_HOLD:
            rt_kprintf("btn_2_cb [FLEX_BTN_PRESS_LONG_HOLD]\n");
            break;
        case FLEX_BTN_PRESS_LONG_HOLD_UP:
            rt_kprintf("btn_2_cb [FLEX_BTN_PRESS_LONG_HOLD_UP]\n");
            break;
    }
}

static uint8_t button_key1_read(void)
{
    return rt_pin_read(PIN_KEY1);
}

static uint8_t button_key2_read(void)
{
    return rt_pin_read(PIN_KEY2);
}

static void button_scan(void *arg)
{
    while(1)
    {
        flex_button_scan();
        rt_thread_mdelay(20);
    }
}

static void user_button_init(void)
{
    int i;
    
    rt_memset(&user_button[0], 0x0, sizeof(user_button));

    user_button[USER_BUTTON_1].usr_button_read = button_key1_read;
    user_button[USER_BUTTON_1].cb = (flex_button_response_callback)btn_1_cb;

    user_button[USER_BUTTON_2].usr_button_read = button_key2_read;
    user_button[USER_BUTTON_2].cb = (flex_button_response_callback)btn_2_cb;

    rt_pin_mode(PIN_KEY1, PIN_MODE_INPUT); /* set KEY pin mode to input */
    rt_pin_mode(PIN_KEY2, PIN_MODE_INPUT); /* set KEY pin mode to input */

    for (i = 0; i < USER_BUTTON_MAX; i ++)
    {
        user_button[i].pressed_logic_level = 1; //[XH]按下是高电平(硬件下拉)
        user_button[i].click_start_tick = 20;
        user_button[i].short_press_start_tick = 100;
        user_button[i].long_press_start_tick = 200;
        user_button[i].long_hold_start_tick = 300;

        flex_button_register(&user_button[i]);
    }
}

int flex_button_main(void)
{
    rt_thread_t tid = RT_NULL;

    user_button_init();

    /* Create background ticks thread */
    tid = rt_thread_create("flex_btn", button_scan, RT_NULL, 1024, 10, 10);
    if(tid != RT_NULL)
    {
        rt_thread_startup(tid);
    }

    return 0;
}
#ifdef FINSH_USING_MSH
INIT_APP_EXPORT(flex_button_main);
#endif

#endif //BSP_USING_KEY