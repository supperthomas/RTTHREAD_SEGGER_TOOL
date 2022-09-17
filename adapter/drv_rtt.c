/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-05-23     supperthomas init
 *
 */
#include "rtdevice.h"
#include "SEGGER_RTT.h"

static struct rt_serial_device _serial_jlink_rtt;

typedef struct
{
    struct rt_serial_device *serial;
} drv_jlink_rtt_cfg_t;

static drv_jlink_rtt_cfg_t m_jlink_rtt_cfg =
{
    .serial = &_serial_jlink_rtt,
};


static rt_err_t _rtt_cfg(struct rt_serial_device *serial, struct serial_configure *cfg)
{
    return RT_EOK;
}

static rt_err_t _rtt_ctrl(struct rt_serial_device *serial, int cmd, void *arg)
{
    return RT_EOK;
}
static int _rtt_putc(struct rt_serial_device *serial, char c)
{
    SEGGER_RTT_PutChar(RTT_DEFAULT_BUFFER_INDEX, c);
    return RT_EOK;
}

static int _rtt_getc(struct rt_serial_device *serial)
{
    return SEGGER_RTT_GetKey();
}

static struct rt_uart_ops _jlink_rtt_ops =
{
    _rtt_cfg,
    _rtt_ctrl,
    _rtt_putc,
    _rtt_getc
};

static void segger_rtt_check(void)
{
    if (SEGGER_RTT_HasKey())
    {
        rt_hw_serial_isr(&_serial_jlink_rtt, RT_SERIAL_EVENT_RX_IND);
    }
}

int rt_hw_jlink_rtt_init(void)
{
    SEGGER_RTT_Init();
    rt_thread_idle_sethook(segger_rtt_check);
    SEGGER_RTT_SetTerminal(RTT_DEFAULT_TERMINAL_INDEX);

    SEGGER_RTT_ConfigUpBuffer(RTT_DEFAULT_BUFFER_INDEX, "RTTUP", NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);

    SEGGER_RTT_ConfigDownBuffer(RTT_DEFAULT_BUFFER_INDEX, "RTTDOWN", NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);

    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
    _serial_jlink_rtt.ops = &_jlink_rtt_ops;
    _serial_jlink_rtt.config = config;
    m_jlink_rtt_cfg.serial = &_serial_jlink_rtt;
    rt_hw_serial_register(&_serial_jlink_rtt, "jlinkRtt", \
                          RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX,  &m_jlink_rtt_cfg);
    
   // SEGGER_RTT_printf(0,"\r\n SEGGER_RTT ADDRESS:%p \r\n",&_SEGGER_RTT);
	
	return 0;
}
//INIT_BOARD_EXPORT(rt_hw_jlink_rtt_init);


