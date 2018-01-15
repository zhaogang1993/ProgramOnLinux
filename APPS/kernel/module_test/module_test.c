//必要的头文件    
#include <linux/module.h>    
#include <linux/kernel.h>    
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/workqueue.h>
#include <linux/moduleparam.h>
#include <linux/time.h>
#include <linux/timer.h>
#include <asm/atomic.h>

#include "../../../PUBILIC/include/basetype.h"

UINT g_uiExportInt = 0;
static struct timer_list stTimer1;

void TimerCallBack(unsigned long arg)
{
    mod_timer(&stTimer1, jiffies + HZ ); // 设置超时时间 1s 
    g_uiExportInt++;
    printk("timer-0 loops: %u\n", g_uiExportInt);
    return;
}

/* 内核函数无参数的话需要加void */
static ULONG hello_kTimerInit(void)
{
    init_timer(&stTimer1);
    stTimer1.function= TimerCallBack;
    add_timer(&stTimer1);
    mod_timer(&stTimer1, jiffies + HZ); // 设置超时时间 1s 
    return ERROR_SUCCESS;
}
static void hello_kTimerFini(void)
{
    del_timer(&stTimer1);
    return;
}
static void hello_ExportFunc(void)
{
    printk(KERN_ALERT "g_iExportInt = %d\n", g_uiExportInt);
    return;
}

//模块加载函数（必须）    
static int hello_init(void)    
{  
    printk(KERN_ALERT "Hello World enter.\n");

    /* 导出符号测试 */
    hello_ExportFunc();

    /* 内核定时器测试 */
    hello_kTimerInit();
    return 0;    
}    
//模块卸载函数（必须）    
static void hello_exit(void)    
{    
    printk(KERN_ALERT "Hello World exit.\n");

    /* 内核定时器销毁 */
    hello_kTimerFini();
    return;  
}

EXPORT_SYMBOL(hello_ExportFunc);

//模块的注册    
module_init(hello_init);    
module_exit(hello_exit);    

//模块许可证声明（必须）    
MODULE_LICENSE("Dual BSD/GPL");
//声明模块的作者（可选）    
MODULE_AUTHOR("XXX");    
//声明模块的描述（可选）    
MODULE_DESCRIPTION("This is a simple example!/n");    
//声明模块的别名（可选）    
MODULE_ALIAS("A simplest example");    

