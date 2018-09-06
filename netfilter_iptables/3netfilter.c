#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>//for ip header
#include <linux/tcp.h>//for tcp header

MODULE_LICENSE("Dual BSD/GPL");

static struct nf_hook_ops nfho;

static unsigned char *drop_port = "\x00\x5a";//port 90

//钩子函数，注意参数格式与开发环境源码树保持一致
unsigned int hook_func(const struct nf_hook_ops *ops, 
        struct sk_buff *skb,
        const struct net_device *in,
        const struct net_device *out,
        int (*okfn)(struct sk_buff *))
{
    struct iphdr *ip = ip_hdr(skb);//获取ip首部
    struct tcphdr *tcp = tcp_hdr(skb);//获取tcp首部

    if(!skb)
        return NF_ACCEPT;
    if(!tcp)
        return NF_ACCEPT;
    if(ip->protocol != IPPROTO_TCP)//不是tcp协议的放行
        return NF_ACCEPT;

    //检查目的端口,也就是本机上的某个应用程序
    if(tcp->dest == *(unsigned short *)drop_port)
    {
        printk("Dropped packet from %d\n", tcp->dest);
        return NF_DROP;
    }
    else
    {
        printk("Allowed packet from %d\n", tcp->dest);
        return NF_ACCEPT;
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