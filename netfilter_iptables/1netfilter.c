/*安装一个丢弃所有进入我们指定接口的数据包的 netfilter hook 函数*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>

MODULE_LICENSE("Dual BSD/GPL");

static struct nf_hook_ops nfho;

static char *drop_if = "wlan0";//以太网（无线）

//钩子函数，注意参数格式与开发环境源码树保持一致
unsigned int hook_func(const struct nf_hook_ops *ops, 
        struct sk_buff *skb,
        const struct net_device *in,
        const struct net_device *out,
        int (*okfn)(struct sk_buff *))
{
    if(0 == strcmp(in->name, drop_if))//如果数据包是来源这个接口
    {
        printk("Dropped packet on %s ...\n", drop_if);
        return NF_DROP;//丢弃数据包
    }
    else
    {
        printk("Allow packet pass...\n");
        return NF_ACCEPT;//允许数据包通过
    }
}

static int __init hook_init(void)
{
    nfho.hook = hook_func;//关联对应处理函数
    nfho.hooknum = NF_INET_PRE_ROUTING;//ipv4的第一个hook
    nfho.pf = PF_INET;//ipv4，所以用这个
    nfho.priority = NF_IP_PRI_FIRST;//优先级，第一顺位

    nf_register_hook(&nfho);//注册

    return 0;
}
static void __exit hook_exit(void)
{
    nf_unregister_hook(&nfho);//注销
}

module_init(hook_init);
module_exit(hook_exit);