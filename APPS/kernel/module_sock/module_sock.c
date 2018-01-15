#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/netfilter_ipv4.h> /* for struct nf_sockopt_ops */
#include <asm/uaccess.h>/* for copy_from_user */

#define SOCKET_OPS_BASE          128  
#define SOCKET_OPS_SET       (SOCKET_OPS_BASE)  
#define SOCKET_OPS_GET      (SOCKET_OPS_BASE)  
#define SOCKET_OPS_MAX       (SOCKET_OPS_BASE + 1) 

static struct nf_sockopt_ops g_stSockOpt_Self;


MODULE_LICENSE("GPL");
MODULE_AUTHOR("zhaogang");  
MODULE_DESCRIPTION("This is a simple example!/n");  
MODULE_ALIAS("A simplest example");  

static int sock_self_Recv(struct sock *sk, int optval, void __user *user, unsigned int len)
{
    int iRet = 0;  
    char szMsg[1000] = {0, };
    printk(KERN_INFO "sockopt: sock_self_Recv()\n"); 
    
    if (optval == SOCKET_OPS_SET)  
    {     
        /* 从用户空间拷贝 */
        iRet = copy_from_user(szMsg, user, sizeof(szMsg));  
        printk("sock_self_Recv: szMsg = %s. ret = %d. success\n", szMsg, iRet);  
    }     
    return iRet;  
}
static int sock_self_Send(struct sock *sk, int optval, void __user *user, unsigned int len)
{
    int iRet = 0;  
    printk(KERN_INFO "sockopt: send_msg()\n");

    char szSendBuf[1000] = "This is kernel msg";
    if (optval == SOCKET_OPS_GET)  
    {     
        iRet = copy_to_user(user, szSendBuf, strlen(szSendBuf));  
        printk("sock_self_Send: szSendBuf = %s. iRet = %d. success\n", szSendBuf, iRet);  
    }     
    return 0;   
}

static void sock_Self_Init(struct nf_sockopt_ops *pstSock)
{
    memset(pstSock, 0, sizeof(struct nf_sockopt_ops));
    pstSock->pf = PF_INET;
    pstSock->set_optmax = SOCKET_OPS_MAX;
    pstSock->set_optmin = SOCKET_OPS_SET;
    pstSock->set = sock_self_Recv;
    pstSock->get_optmax = SOCKET_OPS_MAX;
    pstSock->get_optmin = SOCKET_OPS_SET;
    pstSock->get = sock_self_Send;
    pstSock->owner = THIS_MODULE;

    return;
}

static int __init test_init(void)
{
    int iRet = 0;
    printk(KERN_ALERT "Linux cai ji.------start----\n");
    sock_Self_Init(&g_stSockOpt_Self);
    
    iRet = nf_register_sockopt(&g_stSockOpt_Self);
    return iRet;
}
static void __exit test_fini(void)
{
    printk(KERN_ALERT "Linux cai ji.------end----\n");

    nf_unregister_sockopt(&g_stSockOpt_Self);  
    return;
}
module_init(test_init);
module_exit(test_fini);


