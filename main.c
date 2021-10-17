/***************************************************************************//**

  @file         main.c

  @author       Stephen Brennan

  @date         Thursday,  8 January 2015

  @brief        LSH (Libstephen SHell)

*******************************************************************************/
#include <libgen.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>

#define MAX_PATH 260
#define PATH_SIZE 512
/*
  Function Declarations for builtin shell commands:
 */
int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);
int my_mv(char **args);
int my_ls(char **args);
void file_permissions(struct stat sb,char *str);

int fid = 0;

char *month[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sept","Oct","Nov","Dec"};
/*
  List of builtin commands, followed by their corresponding functions.
 */

 void  INThandler(int sig)
{
   
}

char *builtin_str[] = {
  "cd",
  "help",
  "exit",
  "mv",
  "ls"
};

int (*builtin_func[]) (char **) = {
  &lsh_cd,
  &lsh_help,
  &lsh_exit,
  &my_mv,
  &my_ls
};

int calblocksize(int n){
    int num=1;
    if(n == 0){
        return 0;
    }
    while(1){
        if(4000*num+100 > n){
            
            return (4000*num)/1000;
        }
        num += 1;
    }
}
int ccountnum(int n){
    int count=0;
    while(n >= 10){
        count += 1;
        n = n/10;
    }
    return count+2;
}

int my_ls(char **args){  
    int len=0,max=0;
    long int total=0;
    char a[20]={' '};
    struct passwd *user_pw;
    struct group *user_gr;
    struct stat buf,dbuf;
    struct tm *d;
    char currentdir[MAX_PATH];
    DIR *dp = NULL;
    struct dirent *entry = NULL;
    memset(a,0,sizeof(char)*20);
    if(args[1] == NULL){
        if((dp = opendir(".")) == NULL)
        {
            fprintf(stderr,"fail to open current directory");
            return 1;
        }
        while((entry = readdir(dp)) != NULL){
           
            if(entry->d_name[0] == '.'){
                continue;
            }
            len += strlen(entry->d_name);
            fprintf(stdout,"%s ",entry->d_name);
            if(len >= 90){
                fprintf(stdout,"\n");
                len = 0;
            }
        }
         fprintf(stdout,"\n");
    }else if(args[2] == NULL){
        if((dp = opendir(".")) == NULL)
        {
            fprintf(stderr,"fail to open current directory");
            return 1;
        }
        
        if(!strcmp(args[1],"-l")){
            
            while((entry = readdir(dp)) != NULL){
                if (stat(entry->d_name, &dbuf) == -1) 
            { fprintf(stderr,"stat"); 
            return 1; }
             if(entry->d_name[0] == '.'){
                continue;
            }      
                total += calblocksize(dbuf.st_size);
                if(dbuf.st_size > max)
                    max = dbuf.st_size;            
            }
            fprintf(stdout,"total %ld\n",total);        
             if((dp = opendir(".")) == NULL)
            {
            fprintf(stderr,"fail to open current directory");
            return 1;
            }
            while((entry = readdir(dp)) != NULL){
                 if(entry->d_name[0] == '.'){
                continue;
            }          
            if (stat(entry->d_name, &buf) == -1) 
            { fprintf(stderr,"stat"); 
            return 1; }
            user_gr = getgrgid(buf.st_gid);
            user_pw = getpwuid(buf.st_uid);
            file_permissions(buf,a);
            d = localtime(&buf.st_ctime);
            fprintf(stdout,"%s %ld %s %s %*lld %.3s %2d %.2d:%.2d %s\n",a,(long) buf.st_nlink,user_pw->pw_name,user_gr->gr_name,\
            ccountnum(max),(long long) buf.st_size,month[d->tm_mon],d->tm_mday,d->tm_min,d->tm_sec,entry->d_name);
            memset(a,0,sizeof(char)*20);
            }
        }else if(!strcmp(args[1],"-la")){
             
            while((entry = readdir(dp)) != NULL){
                if (stat(entry->d_name, &dbuf) == -1) 
            { fprintf(stderr,"stat"); 
            return 1; }
         
                total += calblocksize(dbuf.st_size);
                if(dbuf.st_size > max)
                    max = dbuf.st_size;            
            }
            fprintf(stdout,"total %ld\n",total);        
             if((dp = opendir(".")) == NULL)
            {
            fprintf(stderr,"fail to open current directory");
            return 1;
            }
            while((entry = readdir(dp)) != NULL){
                
            if (stat(entry->d_name, &buf) == -1) 
            { fprintf(stderr,"stat"); 
            return 1; }
            user_gr = getgrgid(buf.st_gid);
            user_pw = getpwuid(buf.st_uid);
            file_permissions(buf,a);
            d = localtime(&buf.st_ctime);
            fprintf(stdout,"%s %ld %s %s %*lld %.3s %2d %.2d:%.2d %s\n",a,(long) buf.st_nlink,user_pw->pw_name,user_gr->gr_name,\
            ccountnum(max),(long long) buf.st_size,month[d->tm_mon],d->tm_mday,d->tm_min,d->tm_sec,entry->d_name);
            memset(a,0,sizeof(char)*20);
            }
        }else if(!strcmp(args[1],"-help")){
            printf("my_ls is show the file or directory list\n");
            printf("-l is more detail, -la is more more detail\n");
            printf("enjoy!\n");
        }
    }

return 1;
}


int lsh_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

/*
  Builtin function implementations.
*/

/**
   @brief Bultin command: change directory.
   @param args List of args.  args[0] is "cd".  args[1] is the directory.
   @return Always returns 1, to continue executing.
 */
int lsh_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "lsh: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("lsh");
    }
  }
  return 1;
}
long file_size(int fd){
    long filesize = 0;
    filesize = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    return filesize;
}
void file_permissions(struct stat sb,char *str){
    int root=0,group=0,other=0,sid=0; 
    switch (sb.st_mode & S_IFMT) { 
        case S_IFBLK: strcat(str,"b"); break; 
        case S_IFCHR: strcat(str,"c"); break; 
        case S_IFDIR: strcat(str,"d");break; 
        case S_IFIFO: strcat(str,"p");break; 
        case S_IFLNK: strcat(str,"l"); break; 
        case S_IFREG: strcat(str,"-"); break; 
        case S_IFSOCK: strcat(str,"s"); break; 
        default: strcat(str,"?"); break; }


    if( sb.st_mode & S_IRUSR )
        strcat(str,"r");
     else
        strcat(str,"-");
    if( sb.st_mode & S_IWUSR )
        strcat(str,"w");
     else
        strcat(str,"-");
    if( sb.st_mode & S_IXUSR )
        strcat(str,"x");
    else if( sb.st_mode & S_ISUID )
        strcat(str,"s");
     else
        strcat(str,"-");


    if( sb.st_mode & S_IRGRP )
        strcat(str,"r");
     else
        strcat(str,"-");
    if( sb.st_mode & S_IWGRP )
        strcat(str,"w");
     else
        strcat(str,"-");
    if( sb.st_mode & S_IXGRP )
        strcat(str,"x");
    else if( sb.st_mode & S_ISGID )
        strcat(str,"s");
     else
        strcat(str,"-");
 

    if( sb.st_mode & S_IROTH )
        strcat(str,"r");
     else
        strcat(str,"-");
    if( sb.st_mode & S_IWOTH )
        strcat(str,"w");
     else
        strcat(str,"-");
    if( sb.st_mode & S_IXOTH )
        strcat(str,"x");
    else if( sb.st_mode & S_ISVTX )
        strcat(str,"t");
     else
        strcat(str,"-");
    
}

int file_permission(struct stat sb){
    int root=0,group=0,other=0,sid=0;
    if( sb.st_mode & S_ISUID )
        sid += 4;
    if( sb.st_mode & S_ISGID )
        sid += 2;
    if( sb.st_mode & S_ISVTX )
        sid += 1;

    if( sb.st_mode & S_IRUSR )
        root += 4;
    if( sb.st_mode & S_IWUSR )
        root += 2;
    if( sb.st_mode & S_IXUSR )
        root += 1;

    if( sb.st_mode & S_IRGRP )
        group += 4;
    if( sb.st_mode & S_IWGRP )
        group += 2;
    if( sb.st_mode & S_IXGRP )
        group += 1;
 
    if( sb.st_mode & S_IROTH )
        other += 4;
    if( sb.st_mode & S_IWOTH )
        other += 2;
    if( sb.st_mode & S_IXOTH )
        other += 1;
    return (512*sid)+(64*root)+(8*group)+(1*other);
}

int my_mv(char **args){
    struct stat src_sb,dest_sb;
    int src,dest,src_size,rlen;
    char *src_value;
    char tmp_path[PATH_SIZE];
    if (stat(args[1], &src_sb) == -1)  { fprintf(stderr,"stat1"); 
    return 1; 
    };
    //--------------------------------
    int len=0,max=0;
    char a[20]={' '};
    struct stat buf;
    struct tm *d;
    char currentdir[MAX_PATH];
    DIR *dp = NULL;
    struct dirent *entry = NULL;
    //--------------------------------
   

    if (args[1] == NULL){
        fprintf(stderr,"my_mv: missing file operand");
        return 1;
    }else if(args[2] == NULL){
        fprintf(stderr,"my_mv: missing destination file operand after '%s'",args[1]);
        return 1;
    }else{
        src = open(args[1],O_RDWR);
        if(!S_ISDIR(src_sb.st_mode))
        {
        src_size = file_size(src);
        src_value = (char *)malloc(sizeof(char)*src_size);
        rlen  = read(src, src_value, src_size);
        dest = open(args[2],  O_RDWR | O_CREAT,file_permission(src_sb));
    
        if (stat(args[2], &dest_sb) == -1)
        { 
            fprintf(stdout,"args[2] : %s\n",args[2]);
            fprintf(stderr,"stat3%s\n",strerror(errno)); 
            return 1; 
        }
        if(S_ISDIR(dest_sb.st_mode))
        {
            char *newname;
            close(dest);
            newname = (char *)malloc(strlen(args[2])+strlen(args[1])+2);
            strcpy(newname,args[2]);
            strcat(newname,"/");
            
            dest = open(strcat(newname,args[1]),  O_RDWR | O_CREAT,file_permission(src_sb));
            free(newname);
        }
        write(dest, src_value, src_size);
        free(src_value);
        close(src);
        close(dest);
       
        remove(args[1]);
        rmdir(args[1]);  
        }else{

        strcpy(tmp_path,args[2]);
         strcat(tmp_path,"/");
        char *t1 = strdup(args[1]);
        strcat(tmp_path,basename(t1));
        if((dp = opendir(args[1])) == NULL)
        {
            fprintf(stderr,"fail to open directory");
            return 1;
        }
           if(fid != 0){
         strcpy(tmp_path,args[2]);
          
        strcat(tmp_path,"/");
        char *t1 = strdup(args[1]);

        strcat(tmp_path,basename(t1));
           }else{
                strcpy(tmp_path,args[2]);
              
           }
        mkdir(tmp_path,file_permission(src_sb));
        strcpy(args[2],tmp_path);
          while((entry = readdir(dp)) != NULL){
              
                 
            if(!strcmp(entry->d_name,".") || !strcmp(entry->d_name,"..")){
                continue;
            }     
         
            strcpy(tmp_path,args[1]);
            strcat(tmp_path,"/");
            strcat(tmp_path,entry->d_name);
        
            
            if (stat(tmp_path, &buf) == -1) 
            { 
                fprintf(stderr,"stat3%s\n",strerror(errno)); 
            return 1; }
           
            if(S_ISDIR(buf.st_mode)){
                fid = fork();
                
                if(fid == 0){
                    char *argv[3];
                  
                    strcpy(tmp_path,args[1]);
                    strcat(tmp_path,"/");
                    strcat(tmp_path,entry->d_name);
                    argv[1] = (char *)malloc(sizeof(char) * strlen(tmp_path)+1);
                    strcpy(argv[1],tmp_path);
                    strcpy(argv[0],args[1]);
                    strcpy(tmp_path,args[2]);
                    strcat(tmp_path,"/");
                    strcat(tmp_path,entry->d_name);
    
                    argv[2] = (char *)malloc(sizeof(char) * strlen(tmp_path)+1);
                    strcpy(argv[2],tmp_path);
              
                    my_mv(argv);
                   
                    rmdir(argv[0]); 
                    free(argv[1]);
                    free(argv[2]);
                    exit(0);
                }
            }else{
            char *argv[3];
            strcpy(tmp_path,args[1]);
            strcat(tmp_path,"/");
            strcat(tmp_path,entry->d_name);
            argv[1] = (char *)malloc(sizeof(char) * strlen(tmp_path)+1);
            strcpy(argv[1],tmp_path);
            strcpy(tmp_path,args[2]);
            strcat(tmp_path,"/");
            strcat(tmp_path,entry->d_name);
            argv[2] = (char *)malloc(sizeof(char) * strlen(tmp_path)+1);
            strcpy(argv[2],tmp_path);
            my_mv(argv);
            }
           
           
    }  
        }
    }
   
    rmdir(args[1]);      
    return 1;
}

/**
   @brief Builtin command: print help.
   @param args List of args.  Not examined.
   @return Always returns 1, to continue executing.
 */
int lsh_help(char **args)
{
  int i;
  printf("Stephen Brennan's LSH\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");

  for (i = 0; i < lsh_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }

  printf("Use the man command for information on other programs.\n");
  return 1;
}

/**
   @brief Builtin command: exit.
   @param args List of args.  Not examined.
   @return Always returns 0, to terminate execution.
 */
int lsh_exit(char **args)
{
  return 0;
}

/**
  @brief Launch a program and wait for it to terminate.
  @param args Null terminated list of arguments (including program).
  @return Always returns 1, to continue execution.
 */
int lsh_launch(char **args)
{
  pid_t pid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("lsh");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("lsh");
  } else {
    // Parent process
    do {
      waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

/**
   @brief Execute shell built-in or launch program.
   @param args Null terminated list of arguments.
   @return 1 if the shell should continue running, 0 if it should terminate
 */
int lsh_execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < lsh_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return lsh_launch(args);
}

/**
   @brief Read a line of input from stdin.
   @return The line from stdin.
 */
char *lsh_read_line(void)
{
#ifdef LSH_USE_STD_GETLINE
  char *line = NULL;
  ssize_t bufsize = 0; // have getline allocate a buffer for us
  if (getline(&line, &bufsize, stdin) == -1) {
    if (feof(stdin)) {
      exit(EXIT_SUCCESS);  // We received an EOF
    } else  {
      perror("lsh: getline\n");
      exit(EXIT_FAILURE);
    }
  }
  return line;
#else
#define LSH_RL_BUFSIZE 1024
  int bufsize = LSH_RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    // Read a character
    c = getchar();

    if (c == EOF) {
      exit(EXIT_SUCCESS);
    } else if (c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize) {
      bufsize += LSH_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
#endif
}

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
/**
   @brief Split a line into tokens (very naively).
   @param line The line.
   @return Null-terminated array of tokens.
 */
char **lsh_split_line(char *line)
{
  int bufsize = LSH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token, **tokens_backup;

  if (!tokens) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, LSH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += LSH_TOK_BUFSIZE;
      tokens_backup = tokens;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
		free(tokens_backup);
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, LSH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

/**
   @brief Loop getting input and executing it.
 */
void lsh_loop(void)
{
  char *line;
  char **args;
  int status;

  do {
    printf("> ");
    line = lsh_read_line();
    args = lsh_split_line(line);
    status = lsh_execute(args);

    free(line);
    free(args);
  } while (status);
}

/**
   @brief Main entry point.
   @param argc Argument count.
   @param argv Argument vector.
   @return status code
 */
int main(int argc, char **argv)
{
  // Load config files, if any.
signal(SIGINT, INThandler);
  // Run command loop.
  lsh_loop();

  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}

