#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/mman.h>

int createFile(char* name){
    FILE* fd = fopen(name, "w");
    if(fd == NULL){
        /* Если файл открыть не удалось, выдаем сообщение об ошибке и завершаем работу */
        printf("File open failed!\n");
       return 1;
    }
    fputs("input_text", fd);
    fclose(fd);
    return 0;
}

// Получаем указатель на название файла
char* getFileName(int i, int withTemp){
    char* filename = malloc(sizeof("100"));

    if(withTemp == 0){
        sprintf(filename, "temp/%03d", i);
    }
    else{
        sprintf(filename, "%03d", i);
    }

    return filename;
}


void mFree(char* f1, char* f2){
    free(f1);
    free(f2);
}



int main(int argc, char* argv[]){
    char* directory_name = "temp";

    printf("Looking at the dir: %s\n", directory_name);
    
    DIR *dir = opendir(directory_name);


    if(dir != NULL){

       system("rm -r temp");
    }


    system("mkdir temp");

    char* firstName = getFileName(0, 0);
    if(createFile(firstName)){
        /* Если файл открыть не удалось, выдаем сообщение об ошибке и завершаем работу */
        free(firstName);
        printf("File open failed!\n");
        exit(1);
    }

    free(firstName);

    int counter = 1;

    while(1){
        char* prevFile = getFileName(counter - 1, 1);
        char* nextFile = getFileName(counter, 0);

        if(symlink(prevFile, nextFile)) {
            mFree(prevFile, nextFile);

            printf("Symlink error\n");
            exit(-1);
        }

        

        FILE* newFile = fopen(nextFile, "r");
        if(newFile == NULL){
            printf("%d\n", counter - 1);

            mFree(prevFile, nextFile);

            exit(1);
         }

        fclose(newFile);
        mFree(prevFile, nextFile);

        counter++;
    }

    if (closedir(dir) != 0) {
        printf("Error closing dir!\n");
        return(-1);
    }

    return 0;
}
