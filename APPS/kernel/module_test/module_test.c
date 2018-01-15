//��Ҫ��ͷ�ļ�    
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
    mod_timer(&stTimer1, jiffies + HZ ); // ���ó�ʱʱ�� 1s 
    g_uiExportInt++;
    printk("timer-0 loops: %u\n", g_uiExportInt);
    return;
}

/* �ں˺����޲����Ļ���Ҫ��void */
static ULONG hello_kTimerInit(void)
{
    init_timer(&stTimer1);
    stTimer1.function= TimerCallBack;
    add_timer(&stTimer1);
    mod_timer(&stTimer1, jiffies + HZ); // ���ó�ʱʱ�� 1s 
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

//ģ����غ��������룩    
static int hello_init(void)    
{  
    printk(KERN_ALERT "Hello World enter.\n");

    /* �������Ų��� */
    hello_ExportFunc();

    /* �ں˶�ʱ������ */
    hello_kTimerInit();
    return 0;    
}    
//ģ��ж�غ��������룩    
static void hello_exit(void)    
{    
    printk(KERN_ALERT "Hello World exit.\n");

    /* �ں˶�ʱ������ */
    hello_kTimerFini();
    return;  
}

EXPORT_SYMBOL(hello_ExportFunc);

//ģ���ע��    
module_init(hello_init);    
module_exit(hello_exit);    

//ģ�����֤���������룩    
MODULE_LICENSE("Dual BSD/GPL");
//����ģ������ߣ���ѡ��    
MODULE_AUTHOR("XXX");    
//����ģ�����������ѡ��    
MODULE_DESCRIPTION("This is a simple example!/n");    
//����ģ��ı�������ѡ��    
MODULE_ALIAS("A simplest example");    

