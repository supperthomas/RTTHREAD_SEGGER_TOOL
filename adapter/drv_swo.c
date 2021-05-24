/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-05-24     supperthomas init
 *
 */
#include "rtdevice.h"
#include "board.h"

static struct rt_serial_device _serial_swo;
volatile int32_t ITM_RxBuffer = ITM_RXBUFFER_EMPTY;
typedef struct
{
    struct rt_serial_device *serial;
} drv_swo_cfg_t;

static drv_swo_cfg_t m_swo_cfg =
{
    .serial = &_serial_swo,
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
    ITM_SendChar(c);
    return RT_EOK;
}

static int _rtt_getc(struct rt_serial_device *serial)
{
    return ITM_ReceiveChar();
}

static struct rt_uart_ops _jlink_rtt_ops =
{
    _rtt_cfg,
    _rtt_ctrl,
    _rtt_putc,
    _rtt_getc
};

static void segger_swo_check(void)
{
    if (ITM_CheckChar())
    {
        rt_hw_serial_isr(&_serial_swo, RT_SERIAL_EVENT_RX_IND);
    }
}

int rt_hw_swo_init(void)
{
    rt_thread_idle_sethook(segger_swo_check);
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
    _serial_swo.ops = &_jlink_rtt_ops;
    _serial_swo.config = config;
    m_swo_cfg.serial = &_serial_swo;
    rt_hw_serial_register(&_serial_swo, "jlinkSWO", \
                          RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX,  &m_swo_cfg);

}


