#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <dirent.h>
#include "arquivo.c"
#define FILE 8
#define ENOTENOUGHARGS 256
int createDir(char *path);
unsigned long long totalBytes = 0;
unsigned int totalFiles = 0, totalDirs = 0;


int treeCopy(char *originalPath, char *destinyPath){
    DIR *origem = opendir(originalPath);
    
    if(origem){
        // abriu o diretório de origem


        struct dirent *child = readdir(origem);
        // printf("Path: %s\n", originalPath);
        while( child != NULL){
            
            // enquanto o diretório tiver arquivo ou pasta, continue executando.
            
                char newPathOriginal[500];
                char newPathDestiny[500];
                strcpy(newPathOriginal, originalPath);
                strcat(newPathOriginal, "/");
                strcat(newPathOriginal, child->d_name);
                

                strcpy(newPathDestiny, destinyPath);
                strcat(newPathDestiny, "/");
                strcat(newPathDestiny, child->d_name);

            if(child->d_type == FILE){
                totalFiles++;
                totalBytes += fileCopy(newPathOriginal, newPathDestiny); 
            }else if(strcmp(child->d_name, ".") !=0 && strcmp(child->d_name, "..")  != 0){
                totalDirs++;
                //Não pode ser um arquivo nem a pasta . e nem a pasta .. 
                createDir(newPathDestiny);
                
                treeCopy(newPathOriginal, newPathDestiny);
            }
            child = readdir(origem);
        }
    }

    return 1;
}

int createDir(char *path){
    return mkdir(path, 0777); 
}

int main(int argc, char *argv[]){
    if (argc != 3){
        errno = ENOTENOUGHARGS;
        return erro(-1,-1,"","");
        //  esses argumentos para a função erro() sinalizam que nenhum arquivo
        //  foi aberto.
    }
    char * pastaOrigem = argv[1];
    char * pastaDestino = argv[2];

    if(!createDir(pastaDestino)){
        treeCopy(pastaOrigem, pastaDestino);
        printf("treecopy: foram copiados %i diretórios, %i arquivos e %llu bytes de %s para %s\n", totalDirs, totalFiles, totalBytes, pastaOrigem, pastaDestino);
        
    }
    return (0);
}