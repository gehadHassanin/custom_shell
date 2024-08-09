#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "../inc/utils.h"
#include "../inc/cmd.h"


extern uint8_t **environ;

extern FlagState_t out_flag;
extern FlagState_t inp_flag;
extern FlagState_t err_flag;


void ImplementPwdCommand(uint8_t argc) {
    uint8_t buff[PATH_MAX];
    ssize_t bytes = 0;
    if (argc == 1 && getcwd(buff, PATH_MAX) != NULL) {
        bytes = write(STDOUT, buff, strlen(buff));
        bytes = write(STDOUT, "\n", 1);
        if (bytes == -1) {
            perror("pwd: write");
        } else if (getcwd(buff, PATH_MAX) == NULL) {
            perror("pwd: error");
        } else if (argc > 1) {
            perror("pwd: argc");
        }
    }
}

void ImplementEchoCommand(uint8_t *buff) {
    ssize_t bytes = 0;
    bytes = write(STDOUT, buff, strlen(buff));
	bytes = write(STDOUT, "\n", 1);
    if (bytes == -1) {
        perror("pwd");
    }
}

void ImplementCatCommand(uint8_t argc, uint8_t** argv) {
        uint8_t buff[SIZE_BUFF];
        ssize_t bytesW = 0;
        ssize_t bytesR = 0;
        uint8_t pathBuf[PATH_MAX];
        realpath(argv[1], pathBuf);
        if (argc == 2 && CheckFileInPath(pathBuf) != -1) {
            int32_t fd = open(argv[1], O_RDONLY);
            if (fd == -1) {
                perror("cat");
                close(fd);
            }
            do {
                bytesR = read(fd, buff, 100);
                bytesW = write(STDOUT, buff, bytesR);
                if (fd != -1 && bytesW == -1) {
                    perror("cat");
                    close(fd);
                    break;
                }        
            } while (bytesR != 0);
            close(fd);
        } else {
            perror("cat");
        }
}

void ImplementCpCommand(uint8_t argc, uint8_t** argv) {
    ssize_t bytes = 0;
    uint8_t buff[100];
    ssize_t size = 0;
	int32_t desFlags = 0;
	int32_t fd_S = 0;
	int32_t fd_D = 0;
	uint8_t sourcePath[PATH_MAX];
	uint8_t distinationPath[PATH_MAX];
    desFlags = (argc == 3) ? (O_CREAT | O_WRONLY) : 
		                     (O_CREAT | O_APPEND | O_WRONLY);       
	if (argc == 3 || (argc == 4 && !strcmp(argv[1], "-a"))) {
        FindRealPath(argv[argc - 2], sourcePath);
        realpath(argv[argc - 1], distinationPath);
        if (!strcmp(distinationPath, sourcePath)) {
            fprintf(stderr, "cp: '%s' and '%s' are the same file\n", argv[argc - 2], argv[argc - 1]);
        } else if (!Is_File(distinationPath)) {
			strcat(distinationPath, "/");
			strcat(distinationPath, Get_FileName(sourcePath));
		}
		fd_S = open(sourcePath, O_RDONLY);
		fd_D = open(distinationPath, desFlags, S_IRWXU);
        if (fd_S == -1) {
            perror("cp: open source");
            close(fd_D);
            close(fd_S);
		}
        if (fd_D == -1) {
            perror("cp: open destination");
            close(fd_D);
            close(fd_S);
        }
        do {
            size = read(fd_S, buff, 100);
            if (size == -1) {
                perror("cp: read");
                close(fd_S);
                close(fd_D);
                break;
            }
            bytes = write(fd_D, buff, size);
            if (bytes == -1) {
                perror("cp: write");
                close(fd_S);
                close(fd_D);
                break;
            }
        } while (size != 0);
        close(fd_S);
        close(fd_D);
	} else {
		perror("cp: Invalid argument");
	}
}


void ImplementMvCommand(uint8_t argc, uint8_t** argv) {
    uint8_t sourcePath[PATH_MAX];
    uint8_t distinationPath[PATH_MAX];
    if (argc == 3 || (argc == 4 && !strcmp(argv[1], "-f"))) {
		FindRealPath(argv[argc - 2], sourcePath);
        realpath(argv[argc - 1], distinationPath);
		if (!strcmp(distinationPath, sourcePath)) {
            fprintf(stderr, "cp: '%s' and '%s' are the same file\n", argv[argc - 2], argv[argc - 1]);
        } else if (Is_File(distinationPath) == 0 && Is_Directory(distinationPath) == 1) {
            strcat(distinationPath, "/");
            strcat(distinationPath, Get_FileName(sourcePath));
		}	
		if (argc == 3 && CheckFileInPath(distinationPath) == 0 && strcmp(distinationPath, sourcePath) != 0) {
			perror("mv");
		} else {
            if (rename(sourcePath, distinationPath) == -1) {
               	perror("mv");
			}
        }
	} else {
		perror("mv: Invalid argument");
	}
}

void ChangeCurrentDirectory(uint8_t argc, uint8_t** argv) {
	if (argc == 2) {
        if (chdir(argv[1]) == -1) {
			perror("cd");
        }
	} else {
		perror("argc");
	}
}

void FindRealPath(uint8_t* file, uint8_t* pathBuf) {
	if(realpath(file, pathBuf) == NULL) {
		perror("realpath");
	}
}

void RemoveFile(const uint8_t *pathname) {
    if (Is_Directory(pathname) == 1 && Is_File(pathname) == 0) {
        fprintf(stderr, "rm: cannot remove '%s': Is a directory\n", pathname);
    } else if (Is_Directory(pathname) != 1 && Is_File(pathname) != 1) {
        fprintf(stderr, "rm: cannot remove '%s': No such file or directory\n", pathname);
    } else if (unlink(pathname) == -1) {
        perror("rm");
    }
}

void RemoveDirectory(const uint8_t *pathname) {
    if (Is_Directory(pathname) == 0 && Is_File(pathname) == 1) {
        fprintf(stderr, "rmdir: failed to remove '%s': Not a directory\n", pathname);
    } else if (Is_Directory(pathname) != 1 && Is_File(pathname) != 1) {
        fprintf(stderr, "rmdir: cannot remove '%s': No such file or directory\n", pathname);
    } else if (rmdir(pathname) == -1) {
        perror("rmdir");
    }
}

void ImplementEnvCommand(uint8_t argc, uint8_t** argv) {
    if (argc == 1) {
        for (int32_t Iterator = 0; environ[Iterator] != NULL; Iterator++) {
            printf("%s\n", environ[Iterator]);
        }
    } else if (getenv(argv[1]) != NULL) {
        printf("%s\n", getenv(argv[1]));
    } else {
        printf("env: ‘%s’: No such file or directory\n", argv[1]);
    }
}

void ImplementTypeCommand(uint8_t argc, uint8_t** argv) {
    for (int32_t Iterator = 1; Iterator < argc; Iterator++) {
        if (Is_InternalCmd(argv[Iterator]) == 0) {
            printf("%s is a shell builtin\n", argv[Iterator]);
        } else if (Is_ExternalCmd(argv[Iterator]) == 0) {
            printf("%s is a external command\n", argv[Iterator]);
        } else {
            printf("%s is a unsupported command\n", argv[Iterator]);
        }
    }
}

void ImplementUptimeCommand(void) {
	uint8_t *argv[] = {"myuptime", "/proc/uptime"};
	ImplementCatCommand(2, argv);
}

void PrintMemAndSwapInfo(void) {
        SwapInfo_t swapInfo;
        MemInfo_t  memInfo;
        ImplementFreeCommand(&swapInfo, &memInfo);
        // PRINT THE HEADER
        printf("%-14s %-6s %9s %10s\n", 
           "", "total", "used", "free");
        // PRINT THE MEMORY INFORMATION 
        printf("%-12s %-6lu %10lu %10lu\n", 
               "Mem:", memInfo.memTotal__, memInfo.memUsed__, memInfo.memFree__);
        // PRINT THE SWAP INFORMATION 
        printf("%-12s %-6lu %10lu %10lu\n", 
               "Swap:", swapInfo.swapTotal__, swapInfo.swapUsed__, swapInfo.swapFree__);
}


void ImplementFreeCommand(SwapInfo_t *pSwapInfo, MemInfo_t *pMemInfo) {
    InputInfo_t inputInfo;
    FILE *file = fopen("/proc/meminfo", "r");
	while (fgets(inputInfo.__command, SIZE_BUFF, file) != NULL) {    
        ParsingInputInfo(&inputInfo);  
        CollectMemAndSwapInfo(inputInfo.__token, pSwapInfo, pMemInfo);
	}
}

void clear_screen(void) {
    if(system("clear") == -1) {
        perror("clear");
    }
}

void Traverse_History(ProcessHist_t *pHistory, uint32_t size) {
    int32_t index = size == NUM_PROCESS ? (pHistory->top__ - NUM_PROCESS) : 0;
    for (int32_t iterator = 0; iterator < size; iterator++) {
        Display_History(&pHistory->__info[index++]);
    }
}


void Redirection(InputInfo_t* pInputInfo, uint8_t *buf) {
    uint8_t idx = 0;
    int32_t fd;
    out_flag = kOff;
    inp_flag = kOff;
    err_flag = kOff;
    ssize_t ret = readlink("/proc/self/fd/0", buf, BUFF_SIZE);
    while (idx < pInputInfo->redir_num) {
        if (!strcmp(pInputInfo->__redir[idx], ">")) {
            out_flag = kOn;
            fd = open(pInputInfo->__redir[++idx], O_WRONLY | O_CREAT, S_IRWXU);
            if (fd == -1) {
                perror("open");
            } else {
                RedirectFile(fd, STDOUT);
            }       
        } else if (!strcmp(pInputInfo->__redir[idx], "<")) { 
            inp_flag = kOn;
            fd = open(pInputInfo->__redir[++idx], O_RDONLY | O_CREAT, S_IRWXU);
            if (fd == -1) {
                perror("open");
            } else {
                RedirectFile(fd, STDIN);
            }
        } else if (!strcmp(pInputInfo->__redir[idx], "2>")) {
            err_flag = kOn;
            fd = open(pInputInfo->__redir[++idx], O_WRONLY | O_CREAT, S_IRWXU);
            if (fd == -1) {
                perror("open");
            } else {
                RedirectFile(fd, STDERR);
            }
        }
        ++idx;
    }
}

