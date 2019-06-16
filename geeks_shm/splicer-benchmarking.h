// 8888888b.  8888888888 8888888888       .d8888b.           8888888 888b    888  .d8888b.  888     888     888 8888888b.  8888888888 
// 888  "Y88b 888        888             d88P  "88b            888   8888b   888 d88P  Y88b 888     888     888 888  "Y88b 888        
// 888    888 888        888             Y88b. d88P            888   88888b  888 888    888 888     888     888 888    888 888        
// 888    888 8888888    8888888          "Y8888P"             888   888Y88b 888 888        888     888     888 888    888 8888888    
// 888    888 888        888             .d88P88K.d88P         888   888 Y88b888 888        888     888     888 888    888 888        
// 888    888 888        888             888"  Y888P"          888   888  Y88888 888    888 888     888     888 888    888 888        
// 888  .d88P 888        888             Y88b .d8888b          888   888   Y8888 Y88b  d88P 888     Y88b. .d88P 888  .d88P 888        
// 8888888P"  8888888888 888              "Y8888P" Y88b      8888888 888    Y888  "Y8888P"  88888888 "Y88888P"  8888888P"  8888888888 

#ifndef SPLICE_H
#define SPLICE_H
#endif


#include <errno.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <linux/unistd.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>    // time()



#define SPLICE_SIZE  1024
#define K_MULTIPLY   40000 


#if defined(__i386__)
#define __NR_sys_splice		        313
#define __NR_sys_tee		        315
#define __NR_sys_vmsplice	        316
#elif defined(__x86_64__)
#define __NR_sys_splice		        275
#define __NR_sys_tee		        276
#define __NR_sys_vmsplice	        278
#define __NR_sys_process_vm_readv	310
#define __NR_sys_process_vm_writev	311
#elif defined(__powerpc__) || defined(__powerpc64__)
#define __NR_sys_splice		        283
#define __NR_sys_tee		        284
#define __NR_sys_vmsplice	        285
#elif defined(__ia64__)
#define __NR_sys_splice		        1297
#define __NR_sys_tee		        1301
#define __NR_sys_vmsplice	        1302
#define __NR_sys_process_vm_readv	1332
#define __NR_sys_process_vm_readv	1333
#elif defined(__arm__)
#define __NR_sys_splice	            340
#define __NR_sys_tee		        342
#define __NR_sys_vmsplice	        343
#else
#error unsupported arch
#endif

#ifndef SPLICE_F_MOVE

#define SPLICE_F_MOVE	(0x01)	/* move pages instead of copying */
#define SPLICE_F_NONBLOCK (0x02) /* don't block on the pipe splicing (but */
				 /* we may still block on the fd we splice */
				 /* from/to, of course */
#define SPLICE_F_MORE	(0x04)	/* expect more data */
#define SPLICE_F_GIFT   (0x08)  /* pages passed in are a gift */

#endif /* SPLICE_F_MOVE defined */

/*
 * SPLICE_F_UNMAP was introduced later, so check for that seperately
 */
#ifndef SPLICE_F_UNMAP
#define SPLICE_F_UNMAP	(0x10)	/* undo vmsplice map */
#endif

static int clock_counter;
clock_t start, end;


//  .d8888b.   .d88888b.  888b     d888 888b     d888  .d88888b.  888b    888  .d8888b.  
// d88P  Y88b d88P" "Y88b 8888b   d8888 8888b   d8888 d88P" "Y88b 8888b   888 d88P  Y88b 
// 888    888 888     888 88888b.d88888 88888b.d88888 888     888 88888b  888 Y88b.      
// 888        888     888 888Y88888P888 888Y88888P888 888     888 888Y88b 888  "Y888b.   
// 888        888     888 888 Y888P 888 888 Y888P 888 888     888 888 Y88b888     "Y88b. 
// 888    888 888     888 888  Y8P  888 888  Y8P  888 888     888 888  Y88888       "888 
// Y88b  d88P Y88b. .d88P 888   "   888 888   "   888 Y88b. .d88P 888   Y8888 Y88b  d88P 
//  "Y8888P"   "Y88888P"  888       888 888       888  "Y88888P"  888    Y888  "Y8888P"  
                                                                                      


static inline int error(const char *n)
{
	perror(n);
	return -1;
}

static int __check_pipe(int pfd)
{
	struct stat sb;

	if (fstat(pfd, &sb) < 0)
		return error("stat");
	if (!S_ISFIFO(sb.st_mode))
		return 1;

	return 0;
}

static inline int check_input_pipe(void)
{
	if (!__check_pipe(STDIN_FILENO))
		return 0;

	fprintf(stderr, "stdin must be a pipe\n");
	return 1;
}

static inline int check_output_pipe(void)
{
	if (!__check_pipe(STDOUT_FILENO))
		return 0;

	fprintf(stderr, "stdout must be a pipe\n");
	return 1;
}

static inline void test_string_askii()
{
    int a = 'a';
    int A = 'A';
    int z = 'z';
    int Z = 'Z';
    printf("a = %d, A = %d, z = %d, Z = %d\n", a,A,z,Z);
    return;
}

static inline double time_calc(clock_t en, clock_t st, char* str){
    double cpu_time_used = ((double) (en - st)) / CLOCKS_PER_SEC;
    printf("in %s, time_calc: %f\n", str, cpu_time_used);
    return cpu_time_used;
}

// static inline void clock_init(){
//     start = (clock_t*) malloc(1);
//     end = (clock_t*) malloc(1);
//     clock_counter = (int*) malloc(1);
// }

static inline clock_t clocker(int cnt, char* str)
{   
    if(cnt == 0){
        start = clock();
        printf("in %s: started: %f\n", str, (double)start);
        return start;
    }
    else{
        end = clock();
        printf("in %s:finished: %f\n", str, (double)end);
        return end;
    }
}

static inline void size_printer(char* str){
    printf("int %s, size of data is: %d*%d\n", str, K_MULTIPLY, SPLICE_SIZE);
	printf("--------------------------------------------------------------------\n");
}

/**
 * TO DO...
*/
static inline void file(){
	FILE *fptr;
	int num;

	fptr = fopen("./result.txt","w");

	if(fptr == NULL)
	{
		printf("Error!");   
		exit(1);             
	}

	printf("Enter num: ");
	scanf("%d",&num);

	fseek(fptr, 0, SEEK_END);
	fprintf(fptr,"--------------------------------------\n %d\n",num);
	
	fclose(fptr);
}



// 8888888b.        d8888 88888888888     d8888      8888888888 888     888 888b    888  .d8888b.   .d8888b.  
// 888  "Y88b      d88888     888        d88888      888        888     888 8888b   888 d88P  Y88b d88P  Y88b 
// 888    888     d88P888     888       d88P888      888        888     888 88888b  888 888    888 Y88b.      
// 888    888    d88P 888     888      d88P 888      8888888    888     888 888Y88b 888 888         "Y888b.   
// 888    888   d88P  888     888     d88P  888      888        888     888 888 Y88b888 888            "Y88b. 
// 888    888  d88P   888     888    d88P   888      888        888     888 888  Y88888 888    888       "888 
// 888  .d88P d8888888888     888   d8888888888      888        Y88b. .d88P 888   Y8888 Y88b  d88P Y88b  d88P 
// 8888888P" d88P     888     888  d88P     888      888         "Y88888P"  888    Y888  "Y8888P"   "Y8888P"  
                                                                                                                                                                                                                                                                                           
/**
 * Benchmarking Data
*/
void random_char_selector(char* ch)
{
    time_t t;
    srand((unsigned) time(&t));
    int chars [58]= {};

    for (int i = 0; i < 58; i++){
        chars[i] = (rand() % 58) + 65;
        ch[i] = chars[i];
        // printf("[%d, %c], ", chars[i], ch[i]);
    }
    return;
}

void k_generator(char* chars){
    char ch[58] = {};
    random_char_selector(ch);

    for(int i = 0; i < 1024; i++){
        chars[i] = ch[(rand() % 58)];
    }
    return;
}

char** empty_allocator(){
    char** buf =  malloc(sizeof(char*) * K_MULTIPLY);
    for(int i = 0; i < K_MULTIPLY ;i++)
        buf[i] = malloc(sizeof(char) * SPLICE_SIZE);
    return buf;
}

void free_allocator(char** mem){
    for(int i = 0; i < K_MULTIPLY; i++){
        mem[i];
    }
    free(mem);
    return;
}

void fake_data_generator(char** container){
    char ch[1024] = {};
    for(int i = 0; i < K_MULTIPLY; i++){
        k_generator(ch);
        container[i] = ch;
        // printf("container[%d]: %s\n", i, container[i]);
    }
}

char** initializer(){
    char** buf = empty_allocator();
    fake_data_generator(buf);
	return buf;
}

static int usage(char *name)
{
	fprintf(stderr, "%s: [-u(unalign)] [-g(gift)]| ...\n", name);
	return 1;
}




//  .d8888b. Y88b   d88P  .d8888b.        .d8888b.        d8888 888      888      .d8888b.  
// d88P  Y88b Y88b d88P  d88P  Y88b      d88P  Y88b      d88888 888      888     d88P  Y88b 
// Y88b.       Y88o88P   Y88b.           888    888     d88P888 888      888     Y88b.      
//  "Y888b.     Y888P     "Y888b.        888           d88P 888 888      888      "Y888b.   
//     "Y88b.    888         "Y88b.      888          d88P  888 888      888         "Y88b. 
//       "888    888           "888      888    888  d88P   888 888      888           "888 
// Y88b  d88P    888     Y88b  d88P      Y88b  d88P d8888888888 888      888     Y88b  d88P 
//  "Y8888P"     888      "Y8888P"        "Y8888P" d88P     888 88888888 88888888 "Y8888P"  
                                                                                         
/**
 * VMSPLICE
*/
static inline int svmsplice(int fd, const struct iovec *iov,
			    unsigned long nr_segs, unsigned int flags)
{
	return syscall(__NR_sys_vmsplice, fd, iov, nr_segs, flags);
}