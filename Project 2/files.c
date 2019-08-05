#include <stdio.h>
#include <dirent.h>
#include <string.h>

int main(void)
{
    struct dirent *de; // Pointer for directory entry
    char name[64];
    char contents[1000];

    // opendir() returns a pointer of DIR type.
    DIR *dr = opendir(".");

    if (dr == NULL) // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory");
        return 0;
    }

    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html for readdir()
    while ((de = readdir(dr)) != NULL)
    {
        if(!(strcmp(de->d_name, "..") == 0 || strcmp(de->d_name, ".") == 0))
        {
            sprintf(name, "%s\n", de->d_name);
            strcat(contents, name);
        }
    }
    printf("%s", contents);

    closedir(dr);
    return 0;
}