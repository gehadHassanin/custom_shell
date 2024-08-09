#ifndef CMD_H_
#define CMD_H_



/**
 * @brief  : This function is used to clear screen
 * @param  : Nothing
 * @return : Nothing
 */
void clear_screen(void) ;



/**
 * @brief  : This function is used to print memory and swap information
 * @param  : Nothing
 * @return : Nothing
 */
void PrintMemAndSwapInfo(void);



/**
 * @brief  : This function is used to print the uptime for the system 
 *           and the time spent in the idle process
 * @param  : Nothing
 * @return : Nothing
 */
void ImplementUptimeCommand(void);



/**
 * @brief      : This function is used to print the current working directory 
 * @param argc : The number of arguments to the command
 * @return     : Nothing
 */
void ImplementPwdCommand(uint8_t argc);



/**
 * @brief          : This function is used to remove file
 * @param pathname : !Pointer to the variable from @uint8_t data type 
 *                   !Pointer to the file path name 
 * @return         : Nothing
 */
void RemoveFile(const uint8_t *pathname);



/**
 * @brief          : This function is used to remove directory 
 * @param pathname : !Pointer to the variable from @uint8_t data type 
 *                   !Pointer to the file path name 
 * @return         : Nothing
 */
void RemoveDirectory(const uint8_t *pathname);



/**
 * @brief      : This function is used to print 'echo' message 
 * @param buff : !Pointer to the variable from @uint8_t data type 
 *               !Pointer to the buffer that the message stored in it 
 * @return     : Nothing
 */
void ImplementEchoCommand(uint8_t *buff);



/**
 * @brief      : This function is used to print the resolved absolute path name
 * @param buff : !Pointer to the path of file or directory  
 *               !Pointer to the buffer that the real path will store in it 
 * @return     : Nothing
 */
void FindRealPath(uint8_t* file, uint8_t* pathBuf);



/**
 * @brief      : This function is used to move file from certain path to another path 
 * @param argc : The number of arguments to the command
 * @param argv : !Array of pointer to the variable from @uint8_t data type 
 *               that the arguments of the command stored in it
 * @return     : Nothing
 */
void ImplementMvCommand(uint8_t argc, uint8_t** argv);



/**
 * @brief      : This function is used to copy file from one to another
 * @param argc : The number of arguments to the command
 * @param argv : !Array of pointer to the variable from @uint8_t data type 
 *               that the arguments of the command stored in it
 * @return     : Nothing
 */
void ImplementCpCommand(uint8_t argc, uint8_t** argv);



/**
 * @brief      : This function is used to print environment variables
 * @param argc : The number of arguments to the command
 * @param argv : !Array of pointer to the variable from @uint8_t data type 
 *               that the arguments of the command stored in it
 * @return     : Nothing
 */
void ImplementEnvCommand(uint8_t argc, uint8_t** argv);



/**
 * @brief      : This function is used to concatenate files and print on the standard output
 * @param argc : The number of arguments to the command
 * @param argv : !Array of pointer to the variable from @uint8_t data type 
 *               that the arguments of the command stored in it
 * @return     : Nothing
 */
void ImplementCatCommand(uint8_t argc, uint8_t** argv);



/**
 * @brief      : This function is used to display information about command type
 * @param argc : The number of arguments to the command
 * @param argv : !Array of pointer to the variable from @uint8_t data type 
 *               that the arguments of the command stored in it
 * @return     : Nothing
 */
void ImplementTypeCommand(uint8_t argc, uint8_t** argv);



/**
 * @brief      : This function is used to change the current working directory
 * @param argc : The number of arguments to the command
 * @param argv : !Array of pointer to the variable from @uint8_t data type 
 *               that the arguments of the command stored in it
 * @return     : Nothing
 */
void ChangeCurrentDirectory(uint8_t argc, uint8_t** argv);



/**
 * @brief            : This function is used to implement [input, output, error] redirection.
 * @param pInputInfo : Pointer to the variable from @InputInfo_t data type
 * @param buf        : !Pointer to the variable from @uint8_t data type 
 *                     !Used to return in it the default file of [STDIN, STDOUT, STDERR]
 * @return           : Nothing
 */
void Redirection(InputInfo_t* pInputInfo, uint8_t *buf);



/**
 * @brief          : This function is used to list history of the last ten processes 
 * @param pHistory : Pointer to the variable from @ProcessHist_t data type
 * @param size     : !Pointer to the variable from @uint8_t data type 
 *                   !Used to return in it the default file of [STDIN, STDOUT, STDERR]
 * @return         : Nothing
 */
void Traverse_History(ProcessHist_t *pHistory, uint32_t size);



/**
 * @brief        : This function is used to redirect the file 
 * @param old_fd : The old file descriptor
 * @param new_fd : The new file descriptor
 * @return       : Nothing
 */
void RedirectFile(int old_fd, int new_fd);
#endif  // CMD_H_

