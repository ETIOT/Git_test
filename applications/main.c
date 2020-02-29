/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-09-09     RT-Thread    first version
 */

#include <rtthread.h>
#include <board.h>
#include <rtdevice.h>
#include <drv_soft_i2c.h>

#include <arpa/inet.h>         /* ���� ip_addr_t �ȵ�ַ��ص�ͷ�ļ� */
#include <netdev.h>            /* ����ȫ���� netdev ��ز����ӿں��� */
#include <ntp.h>               /* Ҫʹ��NTP�����ṩ��API�����Ȱ���ͷ�ļ�*/

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>



/* PLEASE DEFINE the LED0 pin for your board, such as: PA5 */
#define LED0_PIN    GET_PIN(C, 13)


int register_i2c(void)
    {
        rt_hw_i2c_init("i2c1", GET_PIN(B,6), GET_PIN(B,7));

        return RT_EOK;
    }
    //ע�ᵽϵͳ�У��Զ���ʼ���豸
    INIT_BOARD_EXPORT(register_i2c);

    extern int sht30_collect(void);




int main(void)
{
    int count = 1;
    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);

    sht30_collect();

    //��ȡ��������
    struct netdev* net = netdev_get_by_name("esp0");
    //�����жϵ�ǰ�����Ƿ���������
    while(netdev_is_internet_up(net) != 1)
        {
           rt_thread_mdelay(200);
        }
        //��ʾ��ǰ�����Ѿ���
    rt_kprintf("network is ok!\n");


    //NTP�Զ���ʱ
    time_t cur_time;
    cur_time = ntp_sync_to_rtc(NULL);
    if (cur_time)
     {
            rt_kprintf("Cur Time: %s", ctime((const time_t*) &cur_time));
     }
    else
     {
            rt_kprintf("NTP sync fail.\n");
     }


    while (count++)
    {
        /* set LED0 pin level to high or low */
        rt_pin_write(LED0_PIN, count % 2);
       // LOG_D("Hello RT-Thread!");
        rt_thread_mdelay(1000);
    }

    return RT_EOK;
}
