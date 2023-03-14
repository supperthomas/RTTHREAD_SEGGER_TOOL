/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
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


static rt_err_t _swo_cfg(struct rt_serial_device *serial, struct serial_configure *cfg)
{
    return RT_EOK;
}

static rt_err_t _swo_ctrl(struct rt_serial_device *serial, int cmd, void *arg)
{
    return RT_EOK;
}
static int _swo_putc(struct rt_serial_device *serial, char c)
{
    if (((ITM->TCR & ITM_TCR_ITMENA_Msk) != 0UL) &&      /* ITM enabled */
            ((ITM->TER & (1<<SWO_PORT_NUMBER)         ) != 0UL)   )     /* ITM Port #0 enabled */
    {
        while (ITM->PORT[SWO_PORT_NUMBER].u32 == 0)
        {
            __NOP();
        }
        ITM->PORT[SWO_PORT_NUMBER].u8 = (uint8_t)c;
    }
    return RT_EOK;
}

static int _swo_getc(struct rt_serial_device *serial)
{
    return ITM_ReceiveChar();
}

static struct rt_uart_ops _swo_ops =
{
    _swo_cfg,
    _swo_ctrl,
    _swo_putc,
    _swo_getc
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
    _serial_swo.ops = &_swo_ops;
    _serial_swo.config = config;
    m_swo_cfg.serial = &_serial_swo;
    rt_hw_serial_register(&_serial_swo, "SWO", \
                          RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX,  &m_swo_cfg);
    return 0;
}
//INIT_BOARD_EXPORT(rt_hw_swo_init);

