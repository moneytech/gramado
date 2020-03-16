/*
 * File: ns.h 
 *
 *    Network Server.
 *
 * //(network) Server process support. network support
 * Descrição:
 *     Header do módulo server do sistema.
 *
 * O módulo server é a parte do kernel que faz a conexão e envia mensagens aos
 * servers.
 * Os servers podem estar em (user mode) ou em (kernel mode, 
 * como modulos esternos) ou no (kernel base, como modulos internos).
 *
 * Versão 1.0, 2016.
 */

 
 
struct process_d *serverSP;  //Processo Cliente.

FILE *server_stdio;
FILE *server_stdout;
FILE *server_stderr;

//...  
 
//
//fim.
//

