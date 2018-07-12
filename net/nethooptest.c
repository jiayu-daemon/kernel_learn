#include <linux/moudle>
#include <linux/kernel>
#include <skbuff.h>
//此处省略一些头文件

static unsigned int my_hook(unsigned int hooknum,
                   struct sk_buff *skb,
                   const struct net_device *in,
                   const struct net_device *out,
                   int (*okfn)(struct sk_buff *));
{
    struct iphdr *ip;
    struct udphdr *udp;

    if(!skb)
        return NF_ACCEPT;
    if(skb->protocol != htons(0x0800)) //抓ip包（排除arp包）
        return NF_ACCEPT
    ip = ip_hdr(skb);
    if(ip->protocol != 17)// 抓取udp数据包
        return NFACCEPT;

    //udp = udp_hdr(skb); //这个函数虽然有，却不一定能得到正确的结果，这取决于钩子函数的挂载点和优先级
    udp = (struct udphdr *)(ip+1);
    if(ntohs(udp->dest) != 53) //过滤udp目的端口为53端口的数据
        return NFACCEPT; 

    return NF_DROP;
}

struct nf_hook_ops my_ops = {
    .list = {NULL, NULL},
    .hook = my_hook,
    .pf = PF_INET,
    .hooknum = NF_INET_PRE_ROUTING,
    .priority = NF_IPPRI_FIRST+1
}          

static int __init m_init(void)
{
    nf_register_hook(&my_ops);
}

static void __exit m_exit(void)
{
    nf_unregister_hook(&my_ops);
}

module_init(m_exit);
module_exit(m_exit);