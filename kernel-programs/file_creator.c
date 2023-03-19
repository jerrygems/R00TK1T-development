#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/uaccess.h>
#include <asm/segment.h>
#include <linux/slab.h>

MODULE_AUTHOR("j3rry");
MODULE_DESCRIPTION("A kernel module to create a file by writing some texts inside it");
MODULE_LICENSE("GPL");

char *filename = "hello.txt";
char *data = "texts are here";


int file_creator(char *filename,char *data){
        struct file *file;//a pointer to point for the file
        umode_t flags = O_CREAT | O_WRONLY | O_TRUNC;//flags required to create the file
        umode_t permbits = 0644;//file permissions required

        //declared some variables for later use here
        int len = strlen(data);//size of string
        loff_t pos = 0;//starting position for reading or writting
          ssize_t written;//declared for later use

        //now we are opening file for writing
        file = filp_open(filename, flags, permbits);//now we're opening the file using filp_open()
        if(IS_ERR(file)){//if there is any error in opening the file then print failed and return -1
                printk(KERN_INFO "failed in opening file : %s\n", filename);
                return -1;
        }
         
        //we've opened the file now we will write in file
        written = kernel_write(file,data,len,&pos);//kernel_write() is used to write data to the file
        if(written != len){//if whatever we've written in file the length of that string is not equal to error then we know that something went wrong obviously
                printk("failed to write data to the file ");
                filp_close(file,NULL);//after failing we've to close the file so that this process doesn't go further
                return -1;
        }
         
        filp_close(file,NULL);//and if previous condition is false then our task is done and we've to close the file after operation
        
        printk("the file has been created successfully");
        return 0;
}

static int __init initiate(void){
        printk(KERN_INFO "kernel module has been inserted\n");
        file_creator(filename,data);
        printk(KERN_INFO "whatever you wanted is done now");
        return 0;
}
static void __exit exitting(void){
        printk(KERN_INFO "kernel module has been removed");
}

module_init(initiate);
module_exit(exitting);
