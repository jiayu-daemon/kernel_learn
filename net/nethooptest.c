#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
 
 
static struct nf_hook_ops nfho;
 
 
static unsigned int hook_func(unsigned int hooknum,
                       struct sk_buff *skb,
                       const struct net_device *in,
                       const struct net_device *out,
                       int (*okfn)(struct sk_buff *))
{
    return NF_ACCEPT;
}
 
 
static int nf_init(void)
{
    nfho.hook = hook_func;
    nfho.hooknum  = NF_INET_PRE_ROUTING; 
    nfho.pf       = PF_INET;
    nfho.priority = NF_IP_PRI_FIRST;
 
 
    nf_register_hook(&nfho);
 
 
    return 0;
}
 
 
static void  nf_cleanup(void)
{
    nf_unregister_hook(&nfho);
}
 
 
module_init(nf_init);
module_exit(nf_cleanup);
MODULE_AUTHOR("djwow");
MODULE_LICENSE("GPL");
