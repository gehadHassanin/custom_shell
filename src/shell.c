#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

#include "../inc/utils.h"
#include "../inc/cmd.h"

ProcessInfo_t Info;
ProcessHist_t History;

uint8_t *echoMsg;

int main() {
    InputInfo_t inputInfo;
	History.top__ = 0;
	while (1) {
        echoMsg = (uint8_t *) malloc(COMMAND_SIZE); 
        // PRINT THE PROMPT MESSAGE 
	    PrintPromptMessage();
        clearBuffer(inputInfo.__command, COMMAND_SIZE);
        // READ COMMAND FROM THE USER 
        if (read(STDIN, inputInfo.__command, 100) == -1) {
            perror("read");
        }
        // REMOVE THE NEWLINE CHARACTER IF PRESENT 
        inputInfo.__command[strlen(inputInfo.__command) - 1] = '\0';
        // SKIP TO THE NEXT ITERATION TO PRINT THE PROMPT AGAIN 
        if (strlen(inputInfo.__command) == 0) {
			continue;
        }
	    // STORE THE REST OF THE INPUT AFTER THE "echo" COMMAND	
	    StoreMsg(inputInfo.__command, echoMsg);
        // PARSE THE USER COMMAND
        ParsingInputInfo(&inputInfo);     
        // EXECUTE THE USER COMMAND
        ExecuteCmd(&inputInfo);
    }
    return 0;
}

