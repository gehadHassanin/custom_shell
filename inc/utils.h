#ifndef UTILS_H_
#define UTILS_H_


#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

#define SIZE_HISTORY (1000)
#define SIZE_BUFF    (100)
#define MSG_SIZE     (100)
#define COMMAND_SIZE (100)
#define BUFF_SIZE    (10)

#define STDIN  (0)
#define STDOUT (1)
#define STDERR (2)

#define TOK_NUM      (20)
#define NUM_PROCESS  (10)

#define COLON_DELIMETER (":")
#define SPACE_DELIMETER (" ")

#define IS_FLAG_ON(flag)        (flag == kOn)
#define IS_REDIR_SYMBOL(token)  (!strcmp(token, ">")  ||\
                                 !strcmp(token, "<")  ||\
                                 !strcmp(token, "2>") ||\
                                 !strcmp(token, ">>"))


typedef struct {
    pid_t pid__;
    int32_t status__;
    uint8_t __cmd[SIZE_BUFF];
}ProcessInfo_t;

typedef struct {
    ProcessInfo_t __info[SIZE_HISTORY];
    uint32_t top__;
}ProcessHist_t;

typedef struct {
    uint64_t memTotal__;
    uint64_t memFree__;
    uint64_t memBuffer__;
    uint64_t memCached__;
    uint64_t memUsed__;
}MemInfo_t;

typedef struct {
    uint64_t swapTotal__;
    uint64_t swapFree__;
    uint64_t swapCached__;
    uint64_t swapUsed__;
}SwapInfo_t;

typedef struct {
    uint8_t __command[COMMAND_SIZE];
    uint8_t *__token[TOK_NUM];
    uint8_t *__redir[TOK_NUM];
    uint8_t token_num;   
    uint8_t redir_num;
}InputInfo_t;

typedef enum {
    kOff,
    kOn,
}FlagState_t;



/**
 * @brief            : This function is used to execute the user command 
 * @param pInputInfo : Pointer to the variable from @InputInfo_t data type
 * @return           : Nothing
 */
void ExecuteCmd(InputInfo_t* pInputInfo);



/**
 * @brief            : This function is used to execute the external command
 * @param pInputInfo : Pointer to the variable from @InputInfo_t data type
 * @return           : Nothing
 */
void ExecuteExternalCmd(InputInfo_t* pInputInfo);



/**
 * @brief            : This function is used to execute the internal command
 * @param pInputInfo : Pointer to the variable from @InputInfo_t data type
 * @return           : Nothing
 */
void ExecuteInternalCmd(InputInfo_t* pInputInfo);



/**
 * @brief      : This function is used to clear the buffer to store another command
 * @param buf  : !Pointer to the variable from @uint8_t data type
 *               !Pointer to The buffer that stores the command 
 * @param size : The size of the buffer
 * @return     : Nothing
 */
void clearBuffer(uint8_t* buf, int32_t size);



/**
 * @brief         : This function is used to store the message of the 'echo' command correctly
 * @param command : !Pointer to the variable from @uint8_t data type
 *                  !Pointer to the 'echo' command 
 * @param echoMsg : !Pointer to the variable from @uint8_t data type
 *                  !Pointer to the buffer that it will store the message
 * @return        : Nothing
 */
void StoreMsg(uint8_t command[], uint8_t *echoMsg);



/**
 * @brief  : This function is used to print prompt message
 * @param  : Nothing
 * @return : Nothing
 */
void PrintPromptMessage(void);



/**
 * @brief            : This function is used to parse the input command from the user
 * @param pInputInfo : Pointer to the variable from @InputInfo_t data type
 * @return           : Nothing
 */
void ParsingInputInfo(InputInfo_t* pInputInfo);



/**
 * @brief          : This function is used to push the process to the buffer
 * @param pHistory : Pointer to the variable from @ProcessHist_t data type
 * @param pInfo    : Pointer to the variable from @ProcessInfo_t data type 
 * @return         : Nothing
 */
void Push_Process(ProcessHist_t *pHistory, ProcessInfo_t *pInfo);



/**
 * @brief          : This function is used to get the number of processes that pushed it into the buffer
 * @param pHistory : Pointer to the variable from @ProcessHist_t data type
 * @return         : Nothing
 */
uint32_t Get_Size(ProcessHist_t *pHistory);



/**
 * @brief       : This function is used to print a history of the process
 * @param pInfo : Pointer to the variable from @ProcessInfo_t data type
 * @return      : Nothing
 */
void Display_History(ProcessInfo_t *pInfo);



/**
 * @brief     : This function is used to check if command is internal or not
 * @param cmd : !Pointer to the variable from @uint8_t data type
 *              !Pointer to the command
 * @return    : Nothing
 */
int8_t Is_InternalCmd(uint8_t *cmd);



/**
 * @brief     : This function is used to check if command is external or not
 * @param cmd : !Pointer to the variable from @uint8_t data type
 *              !Pointer to the command
 * @return    : Nothing
 */
int8_t Is_ExternalCmd(const uint8_t *cmd);



/**
 * @brief          : This function is used to check if a path is a path to a file or not
 * @param pathName : !Pointer to the variable from @uint8_t data type
 *                   !Pointer to the path name 
 * @return         : Nothing
 */
int8_t Is_File(const uint8_t* pathName);



/**
 * @brief          : This function is used to check if a path is a path to a directory or not
 * @param pathName : !Pointer to the variable from @uint8_t data type
 *                   !Pointer to the path name 
 * @return         : Nothing
 */
int8_t Is_Directory(const uint8_t* pathName);



/**
 * @brief          : This function is used to check if the file is found on this path or not
 * @param filePath : !Pointer to the variable from @uint8_t data type 
 *                   !Pointer to the file path 
 * @return         : Nothing
 */
int8_t CheckFileInPath(const uint8_t* filePath);



/**
 * @brief      : This function is used to extract the file name from this path
 * @param path : !Pointer to the variable from @uint8_t data type
 *               !Pointer to the path
 * @return     : Nothing
 */
const uint8_t* Get_FileName(const uint8_t *path);



/**
 * @brief           : This function is used to parse the $PATH [environment variable] 
 * @param command   : !Pointer to the variable from @uint8_t data type
 *                    !Pointer to the value of the $PATH [environment variable] 
 * @param __token   : !Array of pointer to the variable from @uint8_t data type 
 *                    !Used to store the dierctories names found on the $PATH [environment variable]
 * @param token_num : !Pointer to the variable from @uint8_t data type 
 *                    !Used to return in it the number of dierctories found on the $PATH [environment variable]
 * @return          : Nothing
 */
void Parsing(uint8_t command[], uint8_t *__token[], uint8_t *token_num);



/**
 * @brief           : This function is used to implement the free command 
 * @param pSwapInfo : !Pointers to the variable from @SwapInfo_t data type 
 *                    !Used to store swap information
 * @param pMemInfo  : !Pointers to the variable from @MemInfo_t data type 
 *                    !Used to store memory information
 * @return          : Nothing
 */
void ImplementFreeCommand(SwapInfo_t *pSwapInfo, MemInfo_t *pMemInfo);



/**
 * @brief           : This function is used to collect memory and swap information
 * @param buf       : !Array of pointer to the variable from @uint8_t data type,
 *                    that is present in it information about memory and swap
 * @param pSwapInfo : !Pointers to the variable from @SwapInfo_t data type 
 *                    !Used to store swap information
 * @param pMemInfo  : !Pointers to the variable from @MemInfo_t data type 
 *                    !Used to store memory information
 * @return          : Nothing
 */
void CollectMemAndSwapInfo(uint8_t **buf, SwapInfo_t *pSwapInfo, MemInfo_t *pMemInfo);
#endif  // UTILS_H_