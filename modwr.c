#include <linux/init.h>           // Macros used to mark up functions e.g. __init __exit
#include <linux/module.h>         // Core header for loading LKMs into the kernel
#include <linux/device.h>         // Header to support the kernel Driver Model
#include <linux/kernel.h>         // Contains types, macros, functions for the kernel
#include <linux/fs.h>             // Header for the Linux file system support
#include <linux/uaccess.h>        // Required for the copy to user function
#include <linux/mutex.h>	  // Import the mutext lock 

#include <linux/string.h>

#define  DEVICE_NAME "modwr"    //< The device will appear at /dev/group19 using this value
#define  CLASS_NAME  "mod19wr"        ///< The device class -- this is a character device driver
#define  MAX_SIZE 1024
 
MODULE_LICENSE("GPL");            //< The license type -- this affects available functionality
MODULE_AUTHOR("Group 19");        //< The author -- visible when you use modinfo
MODULE_DESCRIPTION("A simple Linux char driver based on Derrick Malloy's Code");  ///< The description -- see modinfo
MODULE_VERSION("1.0");            //< A version number to inform users
 
static int    majorNumber;                  ///< Stores the device number -- determined automatically

char   message[MAX_SIZE] = {0};      ///< Memory for the string that is passed from userspace
EXPORT_SYMBOL(message);
static DEFINE_MUTEX(group19_mutex);
EXPORT_SYMBOL(group19_mutex);
static short  size_of_message;              ///< Used to remember the size of the string stored
static int    numberOpens = 0;              ///< Counts the number of times the device is opened
static int    size_msg = 0;
EXPORT_SYMBOL(size_msg);
static struct class*  group19Class  = NULL; ///< The device-driver class struct pointer
static struct device* group19Device = NULL; ///< The device-driver device struct pointer
static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);
 
static struct file_operations fops =
{
   .open = dev_open,
   .write = dev_write,
   .release = dev_release,
};
 
static int __init group19_init(void){
   printk(KERN_INFO "Write-Module: Initializing the Write-Module Driver LKM\n");

	mutex_init(&group19_mutex);
//pthread_mutex_init(&group19_mutex, NULL);
 	printk(KERN_INFO "Write-Module: Mutex initiated\n");

   // Try to dynamically allocate a major number for the device -- more difficult but worth it
   majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
   if (majorNumber<0){
      printk(KERN_INFO "Module failed to register a major number\n");
      return majorNumber;
   }
   printk(KERN_INFO "Write-Module: registered correctly with major number %d\n", majorNumber);
 
   // Register the device class
   group19Class = class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(group19Class)){                // Check for error and clean up if there is
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_INFO "Failed to register device class\n");
      return PTR_ERR(group19Class);          // Correct way to return an error on a pointer
   }
   printk(KERN_INFO "Write-Module: device class registered correctly\n");
 
   // Register the device driver
   group19Device = device_create(group19Class, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
   if (IS_ERR(group19Device)){               // Clean up if there is an error
      class_destroy(group19Class);           // Repeated code but the alternative is goto statements
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_INFO "Failed to create the device\n");
      return PTR_ERR(group19Device);
   }
   printk(KERN_INFO "Write-Module: device class created correctly\n"); // Made it! device was initialized
   return 0;
}
 
static void __exit group19_exit(void){
	mutex_destroy(&group19_mutex);
	printk(KERN_INFO "Write-Module: Mutex destroyed\n");
   device_destroy(group19Class, MKDEV(majorNumber, 0));     // remove the device
   class_unregister(group19Class);                          // unregister the device class
   class_destroy(group19Class);                             // remove the device class
   unregister_chrdev(majorNumber, DEVICE_NAME);             // unregister the major number
   printk(KERN_INFO "Write-Module: Goodbye from the LKM!\n");
}
 
static int dev_open(struct inode *inodep, struct file *filep){
   numberOpens++;
   printk(KERN_INFO "Write-Module: Device has been opened %d time(s)\n", numberOpens);
   return 0;
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){

mutex_lock(&group19_mutex); // Turn on the mutex at the beginning of the write function

int x, size_msg, size_buff, exp_msg_len;
bool quit = false;
bool quit2 = false;
char *buffer_hol;
char buffer_temp[MAX_SIZE];
char buffer_temp1[MAX_SIZE];
char buffer_temp2[MAX_SIZE];
char *expanded_message = "Undefeated 2018 National Champions UCF";

x = 0;
size_msg = strlen(message);
size_buff = strlen(buffer);
exp_msg_len = strlen(expanded_message);
memset(buffer_temp, 0, strlen(buffer_temp));

printk(KERN_INFO "Write-Module: Characters in memory before processing are: %s : with length %d\n", message, size_msg);

if(size_msg > 0 && ((size_msg >= 1 && message[size_msg - 1] == 'U' && size_buff >=2 && buffer[0] == 'C' && buffer[1] == 'F') || (size_msg >= 2 && message[size_msg - 2] == 'U' && message[size_msg-1] == 'C' && size_buff >=1 && buffer[0] == 'F')))
{
	if(message[size_msg - 1] == 'U' && buffer[0] == 'C' && buffer[1] == 'F')
	{
		message[size_msg - 1] = '\0';
		memmove(buffer_temp, buffer + 2, size_buff);
	}
	else if(message[size_msg - 2] == 'U' && message[size_msg-1] == 'C' && buffer[0] == 'F')
	{
		message[size_msg - 1] = '\0';
		message[size_msg - 2] = '\0';
		memmove(buffer_temp, buffer + 1, size_buff);
	}

	if(size_msg + exp_msg_len > MAX_SIZE)
	{
		strncat(message, expanded_message, MAX_SIZE - size_msg);
		quit = true;
	}
	else if(quit == false) {
		strncat(message, expanded_message, exp_msg_len);
	}
}
else if(size_buff >= MAX_SIZE) memmove(buffer_temp, buffer, (2 * MAX_SIZE) - size_buff);
else memmove(buffer_temp, buffer, size_buff);

for(x=0;x<=MAX_SIZE;x++)
{
	bool msg = true;
	bool tmp = true;
	if(tmp == true & buffer_temp[x] == '\n')
	{
		buffer_temp[x] = '\0';
		tmp = false;
		continue;
	}
	if(msg == true & message[x] == '\n')
	{
		message[x] = '\0';
		msg = false;
		continue;
	}
}

printk(KERN_INFO "Write-Module: Characters sent in through buffer are: %s : with length %d\n", buffer_temp, strlen(buffer_temp));

for(x = 0 ; x < MAX_SIZE && quit == false && quit2 == false ; x++)
{
	if(x <= (strlen(buffer_temp) - 3) && buffer_temp[x] == 'U' && buffer_temp[x+1] == 'C' && buffer_temp[x+2] == 'F')
	{
		buffer_hol = NULL;
		memset(buffer_temp1, 0, strlen(buffer_temp1));
		memset(buffer_temp2, 0, strlen(buffer_temp2));

		strcpy(buffer_temp1,buffer_temp);
		buffer_temp1[x] = '\0';
		buffer_hol = buffer_temp;
		buffer_hol = buffer_hol + x + 3;
		strcpy(buffer_temp2, buffer_hol);
		
		if(strlen(buffer_temp1) + exp_msg_len > MAX_SIZE && quit2 == false)
		{
			strncat(buffer_temp1, expanded_message, MAX_SIZE - strlen(buffer_temp1));
			strcpy(buffer_temp, buffer_temp1);
			quit2 = true;
			x = MAX_SIZE;
			break;
		}
		else if(quit2 == false)
		{
			strncat(buffer_temp1, expanded_message, exp_msg_len);
			x += (exp_msg_len - 1);
		}

		if(strlen(buffer_temp1) + strlen(buffer_temp2) > MAX_SIZE && quit2 == false)
		{
			strncat(buffer_temp1, buffer_temp2, MAX_SIZE - strlen(buffer_temp1));
			strcpy(buffer_temp, buffer_temp1);
			quit2 = true;
			x = MAX_SIZE;
			break;
		}
		else if(quit2 == false) strncat(buffer_temp1, buffer_temp2, strlen(buffer_temp2));
		
		memset(buffer_temp, 0, size_buff);

		strcpy(buffer_temp,buffer_temp1);
		
		buffer_hol = NULL;
		memset(buffer_temp1, 0, strlen(buffer_temp1));
		memset(buffer_temp2, 0, strlen(buffer_temp2));
	}
}

if(size_msg + strlen(buffer_temp) > MAX_SIZE) strncat(message, buffer_temp, MAX_SIZE - size_msg);
else strncat(message, buffer_temp, strlen(buffer_temp));

memset(buffer_temp, 0, strlen(buffer_temp));

size_msg = strlen(message);

for(x=0;x<=MAX_SIZE;x++)
{
	bool msg = true;
	bool tmp = true;
	if(tmp == true & buffer_temp[x] == '\n')
	{
		buffer_temp[x] = '\0';
		tmp = false;
		continue;
	}
	if(msg == true & message[x] == '\n')
	{
		message[x] = '\0';
		msg = false;
		continue;
	}
}


printk(KERN_INFO "Write-Module: Characters in memory after processing are: %s : with length %d\n", message, size_msg);

mutex_unlock(&group19_mutex); // Turn off the mutex at the end of the write function

return size_msg;
}

 
static int dev_release(struct inode *inodep, struct file *filep){
   printk(KERN_INFO "Write-Module: Device successfully closed\n");
   return 0;
}
 
module_init(group19_init);
module_exit(group19_exit);
