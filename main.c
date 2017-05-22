
#include "libraries.h"
#include "constants.h"

int sh_cd(char **argv);
int sh_list(char **argv);
int sh_wd(char **argv);
int sh_create(char **argv);
int sh_quit(char **argv);

char *builtin_str[] = {
    "cd",
    "list",
    "wd",
    "create",
    "quit"
};

int (*builtin_func[]) (char **) = {
    &sh_cd,
    &sh_list,
    &sh_wd,
    &sh_create,
    &sh_quit
};

int sh_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}


int sh_cd(char **argv)
{
    if (argv[1] == NULL)
    {
        fprintf(stderr, "expected argument to \"cd\"\n");
    }
    else
    {
        if (chdir(argv[1]) != 0)
        {
            perror("sh");
        }
    }
    
    return(1);
}
 
int sh_create(char **argv)
{
    char destpath[MAX_LEN];
    int newfile = 0;
    int newdir = 0;
    
    if (argv[1] == NULL)
    {
        fprintf(stderr, "sh: expected argument to \"create\"\n");
    }
    else
    {
        if(sizeof(argv) == 3)
        {
            if(strcmp(argv[1], "-f") == 0)
            {
                chdir(argv[2]);
                printf("changed to %s path\n", argv[2]);
            
                if(newfile = open(argv[2], 0640) != NULL)
                {
                    printf("Successfully created new file\n");
                }
                else
                {
                    fprintf(stderr, "Error in creating file\n");
                }
            }
            else if(strcmp(argv[1], "-d") == 0)
            {
                chdir(argv[2]);
                printf("changed to %s path\n", argv[2]);
            
                if(newdir = mkdir(argv[2], 0750) != NULL)
                {
                    printf("successfuly created new directory\n");
                }
                else
                {
                    fprintf(stderr, "Error in creating directory\n");
                }
            }
            else
            {
                fprintf(stderr,"there was an error\n");
            }
        }
        else if(sizeof(argv) == 4)
        {
            char oldname[MAX_LEN];
            char linkname[MAX_LEN];
        
            if(strcmp(argv[1],"-h") == 0)
            {
                strcpy(oldname, argv[2]);
                strcpy(linkname, argv[3]);
            
                if(link(oldname, linkname) == 0)
                {
                    printf("created a hard link\n");
                }
                else
                {
                    fprintf(stderr, "could not create a hard link\n");
                    perror("sh");
                }
            }
            else if(strcmp(argv[1], "-s") == 0)
            {
                strcpy(oldname, argv[2]);
                strcpy(linkname, argv[3]);
                
                if(symlink(oldname, linkname) == 0)
                {
                    printf("created a symbol link\n");
                }
                else
                {
                    fprintf(stderr, "Could not create a symbol link\n");
                    perror("sh");
                }
                
            }
        }
        else
        {
            fprintf(stderr, "incorrect input\n");
        }
    }
    
    return(1);
}

int sh_wd(char **argv)
{
    char wd[MAX_LEN];
    getcwd(wd, sizeof(wd));
    printf("Working directory is %s\n", wd);
    
    return(1);
}


int sh_list(char **argv)
{
    short octarray[SIZE] = {0400, 0200, 0100, 0040, 0020, 0010, 0004, 0002, 0001};
    char perms[SIZE + 1] = "rwxrwxrwx";
    char description[SIZE + 1];
    struct stat statbuf;
    int j = 0;
    
    DIR *dirptr;
    struct dirent *dp;
    
    //list contents of current directory
    if (argv[1] == NULL)
    {
        dirptr = opendir(".");
        if(dirptr)
        {
            while((dp = readdir(dirptr)) != NULL)
            {
                printf("%s\t", dp->d_name);
                
                for(j=0; j < SIZE; j++)
                {
                    if(statbuf.st_mode & octarray[j])
                        description[j] = perms[j];
                    else
                        description[j] = '-';
                }
                description[SIZE] = '\0';
                
                printf("%ld bytes\t", statbuf.st_size);
                printf("User Id: %d, Group Id: %d\t", statbuf.st_uid, statbuf.st_size);
                printf("permissions: %s\n", description);
            }
        }
    }
    else if(sizeof(argv)==2)
    {
        if(strcmp(argv[1], "-i") == 0)
        {
            if((dirptr = opendir(argv[2])) == NULL)
            {
                fprintf(stderr, "could not open directory\n");
                perror("sh");
            }
            else
            {
                while((dp = readdir(dirptr)) != NULL)
                {
                    printf("%s\n", dp->d_name);
                    
                    for(j=0; j < SIZE; j++)
                    {
                        if(statbuf.st_mode & octarray[j])
                            description[j] = perms[j];
                        else
                            description[j] = '-';
                    }
                    description[SIZE] = '\0';
                    
                    printf("%ld bytes\t", statbuf.st_size);
                    printf("User Id: %d, Group Id: %d\t", statbuf.st_uid, statbuf.st_size);
                    printf("permissions: %s\n", description);

                }
            }
        }
        else if(strcmp(argv[1], "-h") == 0)
        {
            if((dirptr = opendir(argv[2])) == NULL)
            {
                fprintf(stderr, "could not open directory\n");
                perror("sh");
            }
            else
            {
                while((dp = readdir(dirptr)) != NULL)
                {
                    if(strcmp(dp->d_name, ".") || strcmp(dp->d_name, "..") == 0)
                    {
                        printf("%s\n", dp->d_name);
                    }
                    else
                    {
                        printf("no hidden files\n");
                    }
                }
            }
        }
        else
        {
            if((dirptr = opendir(argv[2])) == NULL)
            {
                fprintf(stderr, "could not open directory %s\n", argv[2]);
                perror("sh");
            }
            else
            {
                while((dp = readdir(dirptr)) != NULL)
                {
                    printf("%s\n", dp->d_name);
                }
            }
        }
    }
    else //3 arguments
    {
        if((dirptr = opendir(argv[2])) == NULL)
        {
            fprintf(stderr, "could not open directory %s\n", argv[2]);
            perror("sh");
        }
        while((dp = readdir(dirptr)) != NULL)
        {
            printf("%s\n", dp->d_name);
        }
    }
    closedir(dirptr);
    return(1);
}

int sh_quit(char **argv)
{
    printf("Goodbye\n");
    return(0);
}

int sh_launch(char **argv)
{
    pid_t pid;
    int status;
    
    pid = fork();
    if (pid == 0)
    {
        if (execvp(argv[0], argv) == -1)
        {
            perror("sh");
        }
        
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        perror("sh");
    }
    else
    {
        do
        {
            waitpid(pid, &status, WUNTRACED);
        }
        while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    
    return(1);
}

int sh_execute(char **argv)
{
    int i;
    
    if (argv[0] == NULL)
    {

        return(1);
    }
    
    for (i = 0; i < sh_num_builtins(); i++)
    {
        if (strcmp(argv[0], builtin_str[i]) == 0)
        {
            return (*builtin_func[i])(argv);
        }
    }
    
    return sh_launch(argv);
}

char *sh_read_line(void)
{
    int bufsize = BUFSIZE;
    int pos = 0;
    char *buf = malloc(sizeof(char) * bufsize);
    int c;
    
    if (!buf)
    {
        fprintf(stderr, "sh: allocation error\n");
        exit(EXIT_FAILURE);
    }
    
    while (1)
    {
        c = getchar();
        
        if (c == EOF)
        {
            exit(EXIT_SUCCESS);
        }
        else if (c == '\n')
        {
            buf[pos] = '\0';
            return buf;
        }
        else
        {
            buf[pos] = c;
        }
        pos++;
        
        if (pos >= bufsize)
        {
            bufsize += BUFSIZE;
            buf = realloc(buf, bufsize);
            
            if (!buf)
            {
                fprintf(stderr, "sh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}


char **sh_split_line(char *line)
{
    int bufsize = TOK_BUFSIZE;
    int pos = 0;
    char **tokes = malloc(bufsize * sizeof(char*));
    char *toke;
    char **tokens;
    
    if (!tokes)
    {
        fprintf(stderr, "sh: allocation error\n");
        exit(EXIT_FAILURE);
    }
    
    toke = strtok(line, TOK_DELIM);
    while (toke != NULL)
    {
        tokes[pos] = toke;
        pos++;
        
        if (pos >= bufsize)
        {
            bufsize += TOK_BUFSIZE;
            tokens = tokes;
            tokes = realloc(tokes, bufsize * sizeof(char*));
            if (!tokens)
            {
                free(tokens);
                fprintf(stderr, "sh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        
        toke = strtok(NULL, TOK_DELIM);
    }
    tokes[pos] = NULL;
    return tokes;
}

void sh_loop(void)
{
    char *line;
    char **argv;
    int status;
    
    do
    {
        printf("> ");
        line = sh_read_line();
        argv = sh_split_line(line);
        status = sh_execute(argv);
        
        free(line);
        free(argv);
    }
    
    while (status);
}

int main(int argc, char **argv)
{

    sh_loop();
    
    
    return EXIT_SUCCESS;
}

