static int globalfifo_probe(struct platform_device *pdev)
{
	int ret;
	dev_t devno = MKDEV(globalfifo_major,0);

	if(globalfifo_major)
		ret = register_chrdev_region(devno,1,"globalfifo");
	else{
		ret = register_chrdev_region(devno,1,"globalfifo");
		globalfifo_major = MAJOR(devno);
	}

	if(ret < 0)
		return ret;

	globalfifo_devp = devm_kazlloc(&pdev->dev,sizeof(*glabalfifo_devp),
		GEP_KERNEL);
	if(!globalfifo_devp){
		ret = -ENOMEM;
		goto fail_malloc;
	}

	globalfifo_setup_cdev(glabalfifo_devp,0);

	mutex_init(&glabalfifo_devp->mutex);
	init_waitqueue_head(&globalfifo_devp->r_wait);
	init_waitqueue_head(&glabalfifo_devp->w_wait);

	return 0;

fail_malloc:
	unregister_chrdev_region(devno,1);
	return ret;
}


static int globalfifo_remove(struct platform_device * pdev)
{
	cdev_del(&glabalfifo_devp->cdev);
	unregister_chrdev_region(MKDEV(globafifio_maior,0),1);

	return 0;
}


static struct platform_driver globalfifo_driver ={
	.driver ={
		.name = "globalfifo",
		.owner = THIS_MODULE,
	},
	.probe = globalfifo_probe,
	.remove = globalfifo_remove,
};

module_platform_driver(globalfifo_driver);


