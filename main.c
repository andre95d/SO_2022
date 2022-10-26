#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>

void recursive_f(char* fullPath, int w){
    bool isEmpty = true;
    int basePtrId = strlen(fullPath);
    DIR* dir = opendir(fullPath);
    struct dirent *dp; struct stat st;
    while((dp = readdir(dir)) != NULL){
        if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name,"..")==0) continue;
        isEmpty = false;
        strcat(fullPath, "/"); strcat(fullPath,dp->d_name);
        stat(fullPath, &st);
        for(int i = 0; i<w ;i++) printf("-");
        printf("Name: %s; Type: %s\n",
               dp->d_name, (S_ISDIR(st.st_mode) ? "Directory" : "File or special block"));
        if(S_ISDIR(st.st_mode)) {
            for(int i = 0; i<w ;i++) printf("-");
            printf("Subdir content: ->\n");
            recursive_f(fullPath, w+1);
            for(int i = 0; i<w ;i++) printf("-");
            printf("End of subdir %s.\n", dp->d_name);
        }
        fullPath[basePtrId] = '\0';
    }
    closedir(dir);
    for(int i = 0; i<w ;i++) printf("-");
    if(isEmpty) printf("Empty folder.\n");
}

int main(int argc, char** argv) {
    char str[PATH_MAX]; str[0] = '\0';
    //I don't know why but argc is <=1 even if I choose to not pass any argument
    //if(argc < 1) return -1;
    if(argv[1] == 0x0) {printf("No.\n");return -1;}

    // Possible cases: absolute: "/foo", "/.foo"; relative: "foo"; ".foo"; "../foo"; "./foo";
    if(argv[1][0] != '/') {
        int spn = strspn(argv[1],"./"); //treating the latter two cases
        getwd(str);
        printf("Got a relative path. \n"
               "CWD: %s\n"
               "Relative path: %s\n\n", str, argv[1]);

        //treating the case "../foo"
        if(spn == 3)
            for(int i =strlen(str)-2; i>spn-1; i--)
                if (str[i] == '/') {str[i] ='\0'; break;}

        strcat(str, "/");
        strcat(str, argv[1] + (spn == 2 || spn == 3 ? spn : 0));
    } else {
        strcpy(str, argv[1]);
        printf("Got an absolute path: %s\n\n", str);
    }
    if(str[strlen(str)-1] == '/') str[strlen(str)-1] = '\0';
    recursive_f(str, 0);
    printf("End. Writing this cost me 2 days.\n");
    return 0;
}
