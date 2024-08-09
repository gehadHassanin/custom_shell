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
#include <sys/wait.h>
#include <sys/types.h>

#include "../inc/utils.h"
#include "../inc/cmd.h"

FlagState_t out_flag;
FlagState_t inp_flag;
FlagState_t err_flag;

extern ProcessInfo_t Info;
extern ProcessHist_t History;

extern uint8_t *echoMsg;

uint8_t *__InternalCmd[] = { 
        "exit", 
        "mypwd",
        "help", 
        "mycat",
        "mycp", 
        "mymv",
        "myecho"  , 
        "myrealpath",
        "myrmdir", 
        "myrm",
        "myclear",
        "cd", 
        "envir",
        "type",
        "phist",
	    "myuptime",
        "myfree"
};

void ExecuteCmd(InputInfo_t* pInputInfo) {
	if (Is_InternalCmd(pInputInfo->__token[0]) == 0) {
        ExecuteInternalCmd(pInputInfo);
	} else if (Is_ExternalCmd(pInputInfo->__token[0]) == 0) {
        ExecuteExternalCmd(pInputInfo);
	} else {
		printf("Command '%s' not found, but can be used help command:\n", pInputInfo->__token[0]);
		printf("  to know about my supported command\n"); 
    }
}

void ExecuteExternalCmd(InputInfo_t* pInputInfo) {
    pid_t ret_pid = fork();
    uint8_t buf[BUFF_SIZE];
    if (ret_pid == 0) {
        if (pInputInfo->redir_num != 0) {
            Redirection(pInputInfo, buf);
        }
        // CHILD
        pInputInfo->token_num++;
	    pInputInfo->__token[pInputInfo->token_num - 1] = NULL;
	    execvp (pInputInfo->__token[0], pInputInfo->__token);
	    printf ("Exec failed\n");
        exit(EXIT_FAILURE);
    } else if (ret_pid > 0) { 
        // PARENT
        int32_t wstatus;
        if (wait(&wstatus) == -1) {
            perror("wait");
        }
	    Info.status__ = WIFEXITED(wstatus);
	    Info.pid__ = ret_pid;
	    strcpy(Info.__cmd, pInputInfo->__token[0]);
	    Push_Process(&History, &Info);	
    } else {
		printf("Failed to create child process\n");
    }
}

void ExecuteInternalCmd(InputInfo_t* pInputInfo) {
    uint8_t buf[BUFF_SIZE];
    if (pInputInfo->redir_num != 0) {
        Redirection(pInputInfo, buf);
    }
    if (!strcmp(pInputInfo->__token[0], "exit")) {
        write(STDOUT, "Goodbye\n", strlen("Goodbye\n"));
        exit(EXIT_FAILURE);
    } else if (!strcmp(pInputInfo->__token[0], "mypwd")) {
		ImplementPwdCommand(pInputInfo->token_num);
	} else if (!strcmp(pInputInfo->__token[0], "help")) {
		pInputInfo->__token[1] = "myhelp.txt";
		ImplementCatCommand(2, pInputInfo->__token);
	} else if (!strcmp(pInputInfo->__token[0], "mycat")) {
		ImplementCatCommand(pInputInfo->token_num, pInputInfo->__token);
	} else if (!strcmp(pInputInfo->__token[0], "mycp")) {
		ImplementCpCommand(pInputInfo->token_num, pInputInfo->__token);
	} else if (!strcmp(pInputInfo->__token[0], "mymv")) {
		ImplementMvCommand(pInputInfo->token_num, pInputInfo->__token);
	} else if (!strcmp(pInputInfo->__token[0], "myecho")) {
		pInputInfo->token_num == 1 ? printf("\n") : 
		ImplementEchoCommand(echoMsg);
		free(echoMsg);
	} else if (!strcmp(pInputInfo->__token[0], "myrealpath")) {
        uint8_t pathBuf[PATH_MAX];
		if (pInputInfo->token_num == 2) {    
            FindRealPath(pInputInfo->__token[1], pathBuf);
			printf("%s\n", pathBuf);
		} else {
			perror("realpath: Invalid argument");
		}
	} else if (!strcmp(pInputInfo->__token[0], "myrmdir")) {
		for(uint8_t iterator = 1; iterator < pInputInfo->token_num; iterator++) {
			RemoveDirectory(pInputInfo->__token[iterator]);	
		}
	} else if (!strcmp(pInputInfo->__token[0], "myrm")) {
		for(uint8_t iterator = 1; iterator < pInputInfo->token_num; iterator++) { 
			RemoveFile(pInputInfo->__token[iterator]);
		}
	} else if (!strcmp(pInputInfo->__token[0], "cd")) {
		ChangeCurrentDirectory(pInputInfo->token_num, pInputInfo->__token);
	} else if (!strcmp(pInputInfo->__token[0], "envir")) {

		ImplementEnvCommand(pInputInfo->token_num, pInputInfo->__token);

	} else if (!strcmp(pInputInfo->__token[0], "type")) {
		ImplementTypeCommand(pInputInfo->token_num, pInputInfo->__token);
	} else if (!strcmp(pInputInfo->__token[0], "myclear")) {
        clear_screen();
	} else if (!strcmp(pInputInfo->__token[0], "phist")) {
		printf("%-8s %-10s %s\n", "PID", "Status", "Command");
		if (Get_Size(&History) < NUM_PROCESS) {
			Traverse_History(&History, Get_Size(&History));
		} else {
			Traverse_History(&History, NUM_PROCESS);
		}
	} else if (!strcmp(pInputInfo->__token[0], "myuptime")) {
		ImplementUptimeCommand();
	} else if (!strcmp(pInputInfo->__token[0], "myfree")) {
		PrintMemAndSwapInfo();
	}
    // RETURN SYMBOLIC LINK (STDOUT, STDIN, STDERR) TO THE DEFAULT
    int32_t fd = open(buf, O_RDWR, S_IRWXU);
    if (IS_FLAG_ON(out_flag)) {
        RedirectFile(fd, STDOUT);
    } 
    if (IS_FLAG_ON(inp_flag)) {
        RedirectFile(fd, STDIN);
    }
    if (IS_FLAG_ON(err_flag)) {
        RedirectFile(fd, STDERR);
    }
}

void RedirectFile(int old_fd, int new_fd) {
    int32_t fd = dup2(old_fd, new_fd);  
    if (fd == -1) 
    {
            perror("dup2");
    }
    close(old_fd);
}

void CollectMemAndSwapInfo(uint8_t **buf, SwapInfo_t *pSwapInfo, MemInfo_t *pMemInfo) {
    if (!strcmp(buf[0], "MemTotal:")) {
        pMemInfo->memTotal__ = atoi(buf[1]); 
    } else if (!strcmp(buf[0], "SwapTotal:")) {
        pSwapInfo->swapTotal__ = atoi(buf[1]);
    } else if (!strcmp(buf[0], "MemFree:")) {
        pMemInfo->memFree__ = atoi(buf[1]);
    } else if (!strcmp(buf[0], "SwapFree:")) {
        pSwapInfo->swapFree__ = atoi(buf[1]);
    } else if (!strcmp(buf[0], "Buffers:")) {
        pMemInfo->memBuffer__ = atoi(buf[1]);
    } else if (!strcmp(buf[0], "Cached:")) {
        pMemInfo->memCached__ = atoi(buf[1]);  
    } else if (!strcmp(buf[0], "SwapCached:")) {
        pSwapInfo->swapCached__ = atoi(buf[1]); 
    }
    pMemInfo->memUsed__ = 
    (pMemInfo->memTotal__ - pMemInfo->memFree__ - pMemInfo->memBuffer__ - pMemInfo->memCached__);
    pSwapInfo->swapUsed__ = (pSwapInfo->swapTotal__ - pSwapInfo->swapFree__);
}

int8_t Is_File(const uint8_t* pathName) {
	struct stat statBuf;
	if(stat(pathName, &statBuf) == -1) {
		return -1;
    }
	if ((S_ISREG(statBuf.st_mode))) {
		return 1;
	} else {
		return 0;
	}
}

int8_t Is_Directory(const uint8_t *pathName) {
    struct stat statBuf;
    if(stat(pathName, &statBuf) == -1) {
        return -1;
    }
    if ((S_ISDIR(statBuf.st_mode))) {
        return 1;
    } else {
        return 0;
    }
}

const uint8_t* Get_FileName(const uint8_t *path) {
    const uint8_t *file_name = strrchr(path, '/');
    if (file_name) {
        file_name++;
    } else {
        file_name = path;
    }
    return file_name;
}


int8_t CheckFileInPath(const uint8_t* filePath) {
    return access(filePath, F_OK) != -1 ? 0 : -1;
}

int8_t Is_InternalCmd(uint8_t *cmd) {
    for (int32_t Iterator = 0; __InternalCmd[Iterator] != NULL; Iterator++) {
        if (!strcmp(__InternalCmd[Iterator], cmd)) {
            return 0;
        }
    }
    return -1;
}

void Parsing(uint8_t __command[], uint8_t *__token[], uint8_t *token_num) {    
    *(token_num) = 0;   
    for (int32_t Iterator = 0; Iterator < SIZE_BUFF; Iterator++) {
        __token[Iterator] = NULL;
    }
    uint8_t* token = strtok(__command, COLON_DELIMETER);
    while (token != NULL) {
        __token[(*(token_num))++] = token;
        token = strtok(NULL, COLON_DELIMETER);
    }
}

int8_t Is_ExternalCmd(const uint8_t *cmd) {
        uint8_t *env_path = getenv("PATH");
        uint8_t *__token[TOK_NUM];
        uint8_t token_num;
        uint8_t full_path[PATH_MAX];
        if (env_path == NULL)
        {
                return -1;
        }
        char *path_dub = strdup(env_path);
        if (path_dub == NULL) {
            return -1; 
        }
        Parsing(path_dub, __token, &token_num);
        for (int32_t Iterator = 0; Iterator < token_num; Iterator++) {
            // CONSTRUCT THE FULL PATH TO THE COMMAND 
            snprintf(full_path, sizeof(full_path), "%s/%s", __token[Iterator], cmd); 
            if (CheckFileInPath(full_path) == 0) {
                return 0;
            }
        } 
        return -1;     
}

uint32_t Get_Size(ProcessHist_t *pHistory) {
    return pHistory->top__;
}

void Display_History(ProcessInfo_t *pInfo) {
	printf("%-8d %-10d %s\n", pInfo->pid__, pInfo->status__, pInfo->__cmd);
}

void Push_Process(ProcessHist_t *pHistory, ProcessInfo_t *pInfo) {
    pHistory->__info[pHistory->top__++] = *pInfo;
}

void ParsingInputInfo(InputInfo_t* pInputInfo) {
        pInputInfo->token_num = 0;
        pInputInfo->redir_num = 0;
        for (int32_t Iterator = 0; Iterator < BUFF_SIZE; Iterator++) {
            pInputInfo-> __token[Iterator] = NULL;
            pInputInfo-> __redir[Iterator] = NULL;
        }
        uint8_t* token = strtok(pInputInfo->__command, SPACE_DELIMETER);
        while (token != NULL && !(IS_REDIR_SYMBOL(token))) {
            pInputInfo->__token[(pInputInfo->token_num)++] = token;
            token = strtok(NULL, SPACE_DELIMETER);
        }
        if (token != NULL) {
            while (token != NULL) {
                    pInputInfo->__redir[(pInputInfo->redir_num)++] = token;
                    token = strtok(NULL, SPACE_DELIMETER);
            }
        }
}

void clearBuffer(uint8_t* buf, int32_t size) {
    for (uint8_t i = 0; i < size; i++) {
        buf[i] = '\0';
    }
}

void StoreMsg(uint8_t command[], uint8_t *echoMsg) {
	uint8_t i = 7;
	uint8_t iterator = 0;
	clearBuffer(echoMsg, COMMAND_SIZE);
	while (command[i] != '\0') {
		if (command[i] == '"' && command[i + 1] != '\0') {
			echoMsg[iterator++] = command[++i];
			i++;		
		} else if (i == strlen(command) - 1 && command[strlen(command) - 1] == '"') {
			break;
		} else {
			echoMsg[iterator++] = command[i++];
		}
	}
	echoMsg[iterator] = '\0';
}

void PrintPromptMessage(void) {
	uint8_t buff[PATH_MAX];
    if (write(STDOUT, "gehad@gehad:", strlen("gehad@gehad:")) == -1) {
        perror("write");
    }	
	getcwd(buff, PATH_MAX);
	if (write(STDOUT, buff, strlen(buff)) == -1) {
		perror("write");
	}
    if (write(STDOUT, "$ ", strlen("$ ")) == -1) {
        perror("write");
    }
}
