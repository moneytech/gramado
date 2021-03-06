/*
 * File shellui.c 
 *
 * Shell User Interface.
 *     Suporte às rotinas gráficas do aplicativo shell.
 *     #importante: 
 *     A intenção é usar esse módulo do aplicativo shell 
 *     para construir e testar os recursos gráicos do sistema.
 *     Aqui é um campo de testes. 
 * 
 * 2017 - Fred Nora.
 */


#include "gdeshell.h"


extern int ShellFlag;


#define SHELLFLAG_NULL 0
#define SHELLFLAG_COMMANDLINE 1
#define SHELLFLAG_SCRIPT 2
#define SHELLFLAG_HELP 3
#define SHELLFLAG_VERSION 4
#define SHELLFLAG_USAGE 5
#define SHELLFLAG_TOPBAR 6
//...


// Credits: L.
void shellui_fntos (char *name)
{
    int  i, ns = 0;
    char ext[4];
    //const char ext[4];

    //transforma em maiúscula
    while ( *name && *name != '.' )
    {
        if ( *name >= 'a' && *name <= 'z' )
            *name -= 0x20;

        name++;
        ns++;

        // ##bugbug: 
        // E se não encontrarmos o ponto??
    };

    // aqui name[0] é o ponto.
	// então constroi a extensão.
    for ( i=0; i < 3 && name[i+1]; i++ )
    {
        if ( name[i+1] >= 'a' && name[i+1] <= 'z' )
            name[i+1] -= 0x20;

        ext[i] = name[i+1];
    };

    while (ns < 8)
    {
        *name++ = ' ';
        ns++;
    };

    for ( i=0; i < 3; i++ ){
        *name++ = ext[i];
    };

    *name = '\0';
}


/*
 ***********************************************
 * shellTopbarProcedure:
 *     Procedimento de janela.
 *     LOCAL
 */

unsigned long 
shellTopbarProcedure ( 
    struct window_d *window, 
    int msg, 
    unsigned long long1, 
    unsigned long long2 )
{

    unsigned long input_ret=0;
    unsigned long compare_return=0;


    switch (msg)
    {
 
        case MSG_KEYDOWN:
            switch (long1)
            {
				// Null key.
				case 0:
				    return (unsigned long) 0;
				    break;
				
				// Enter.
				// Finaliza a string e compara.
				//case VK_RETURN:
				//    input('\0'); 
				//	shellCompare(window);
				//	goto done;
                //    break; 					
                              
                // Mensagens de digitação.
				// Texto. Envia o caractere.
                // Imprime os caracteres normais na janela com o foco de 
				// entrada.
				// Enfilera os caracteres na string 'prompt[]' para depois 
				// ser comparada com outras strings.
                default:			   
				    
					// Coloca no stdin.
					//input( (unsigned long) long1);      
                    
					// Coloca no stdout.
					//shellInsertNextChar((char) long1);  
					//
					// #importante: IMPRIMINDO.
					// Funcionando bem.
					//printf("%c", (char) long1); 					
					//goto done;
                    break;               
            };
        break;

        //case MSG_KEYUP: 
            //break;

		//Não interceptaremos mensagens do sistema por enquanto.
		//As mensagens do sistema são interceptadas primeiro pelo procedimento 
		//do sistema.

		case MSG_SYSKEYDOWN:
		    switch(long1)
			{
				
		        // queremos atuar sobre os botões da topbar.
				
				//help
				case VK_F1:
				    //APISetFocus(i1Window);
					//APIredraw_window(i1Window);
					gde_message_box ( 3, 
					    "Gramado Core - Shell Topbar", 
					    "F1: HELP");
					break;
				
                //full screen
                //colocar em full screen somente a área de cliente. 				
		        case VK_F2:
				    //APISetFocus(i2Window);
					//APIredraw_window(i2Window);				
				    gde_message_box ( 3, 
				        "Gramado Core - Shell Topbar",
				        "F2: ");
					//ShellFlag = SHELLFLAG_COMMANDLINE;
					break;
					
				case VK_F3:
				    printf("F3: Saindo da top bar e voltando ao shell...\n");
				    ShellFlag = SHELLFLAG_COMMANDLINE;
				    break;
					
				//...

			};
			break;
		
		//Obs:
		//essas teclas são tratadas pelo procedimento do sistema.
		//mas alguma tecla personalizada pode ser  tratada pelo aplicativo,
		//como o context menu [Application Key]
		case MSG_SYSKEYUP:
            switch(long1){
				
				//O MENU APPLICATION É O CONTEXT MENU.
				//
				case VK_APPS:
				    gde_message_box ( 3, 
				        "Gramado Core Shell Topbar:", 
				        "Context Menu");
					break;
			}		
		    break;
		

        // Commands.		
		case MSG_COMMAND:
            switch(long1)
			{
				// Null.
				//case 0:
				//    gde_message_box( 1, "Shell test Topbar","Testing MSG_COMMAND.NULL.");
				//    break;
				
				// About.
				// Abre uma janela e oferece informações sobre o aplicativo.
				case CMD_ABOUT:
				    // Test.
				    gde_message_box ( 3, 
				        "Shell test Topbar", 
				        "Testing MSG_COMMAND.CMD_ABOUT.");
				    break;
				
				//clicaram no botão
				case BN_CLICKED:
				    //if(window == i1Window){
					     //@todo: abre o menu de aplicativos
					//}
				   // if(window == i2Window){
					   //@todo: abre o interpretador de comandos.
					//}
					//#debug
					printf("  Topbar ** BN_CLICKED  **  \n");
				break;
				//...
				
				//default:
				//break;
			}
		    break; 		
		
		//Essa mensagem pode ser acionada clidando um botão.
		case MSG_CLOSE:
		    //??
		    //isso deve fechar qualquer janela que esteja usando esse procedimento.
			//pode ser uma janela filha ou ainda uma janela de dialogo criada pelo sistema.
			//??
		    printf("SHELL.BIN: Topbar MSG_CLOSE\n");
			
			//@todo: Criar essa função na api.
			//apiExitProcess(0);
			break;
		
		//Essa mensagem pode ser acionada clidando um botão.
		case MSG_DESTROY:
		    printf("SHELL.BIN: Topbar MSG_DESTROY\n");
		    break;
		
		//Quando a aplicativo em user mode chama o kernel para 
		//que o kernel crie uma janela, depois que o kernel criar a janela,
		//ele faz uma chamada ao procedimento de janela do aplicativo com a mensagem 
        //MSG_CREATE, se o aplicativo retornar -1, então a rotina em kernel mode que 
        //esta criando a janela, cancela a janela que está criando e retorn NULL.
        //		
		case MSG_CREATE:
		    printf("SHELL.BIN:Topbar  MSG_CREATE\n");
		    break;


        //case MSG_SETFOCUS:  break;
        //case MSG_KILLFOCUS: break;
        //case MSG_PAINT:     break;


		//#importante
		// os caracteres de controle encontrados na tabela ascii.
		//@todo: Essa mensagem precis ser criada, tanto no kernel 
		//quanto na API.
		//case MSG_ASCII_CONTROL_KEYS:
		//   switch(long1)
		//	{
	
		        //^A STX
                //case 1:
		        //    printf("^A");
                //    break;  		
	
		        //^B SOT
                //case 2:
		        //    printf("^B");
                //   break;  		
			
		        //^C ETX
                //case 3:
		        //    printf("^C");
		        //	shellPrompt();
                //    break;

		        //^D EOT
                //case 4:
		        //   printf("^D");
                //   break;

		        //^E ENQ
                //case 5:
		        //    printf("^E");
                //    break;  
			
		        //^F ACK
                //case 6:
		        //    printf("^F");
                //    break;  					


		        //^G BEL
                //case 7:
		        //   printf("^G");
                //    break;  					
			
			
		        //^H BS
                //case 8:
                //    break;  								
			
		        //^I HT horizontal tab
                //case 9:
                //    break;  	

		        //^J LF
                //case 10:
                //    break;  			
			
						
		        //^K VT vertical tab
                //case 11:
                //    break;  	

		        //^L FF form feed
                //case 12:
                //    break;  	
			
			
		        //^M CR
                //case 13:
                //    break;  	
			

		        //^N SO
                //case 14:
                //    break;  				
		

		        //^O SI
                //case 15:
                //    break;


		        //^P DLE DATA LINK ESCAPE
                //case 16:
                //    break;  			
		
		
		        //^Q DC1 DEVICE CONTROL 1
                //case 17:
                //    break;  	
  			
			
		        //^R DC2 DEVICE CONTROL 2
                //case 18:
                //    break;

		        //^S DC3 DEVICE CONTROL 3
                //case 19:
                //    break;  			

		        //^T DC3 DEVICE CONTROL 4
                //case 20:
                //    break;  			
		
		
		        //^U NAK NEGATIVE ACKNOLEDGE
                //case 21:
                //    break;  			

		        //^V SYN SYNCHRONOUS IDLE
                //case 22:
                //    break;  			
		
		        //^W ETB END OF TRANSMISSION BLOCK
                //case 23:
                //    break;  			
		
		        //^X CAN CANCEL
                //case 24:
                //    break;  			

		
		        //^Y EM END OF MEDIUM
                //case 25:
                //    break;  			
		
		        //^Z SUB SUBSTITUTE
                //case 26:
                //    break;  			
		
		
		        //^[ ESC ESCAPE
                //case 27:
                //    break;  			
		
		        //^\ FS FILE SEPARATOR
                //case 28:
                //    break;  			
		
		        //^] GS GROUP SEPARATOR
                //case 29:
                //    break;  			
		
		        //^ RS RECORD SEPARATOR
                //case 30:
                //    break;  			
		
		
		        //_ US UNIT SEPARATOR
                //case 31:
                //    break;  			
		
		
		        //DELETE
                //case 127:
                //    break; 

        //    };		
		//    break;
			


		//Mensagem desconhecida.
		default:
		    //printf("shell procedure: mensagem desconhecida\n");
		    goto done;
		    break;
    };

    // Nothing for now !

//printf("Shell procedure\n");
done:
    //@todo: variavel que indica que estamos em modo gráfico.
	//if(VideoBlock.useGui == 1)
	//{
	    //Debug.
		//refresh_screen(); 
		//Obs: #bugbug perceba que o procedimento de janela do 
		//sistema também tem um refresh screen.
	//};	
	
	//return (unsigned long) apiDefDialog(window,msg,long1,long2);
    return (unsigned long) 0;	
}


// Criando edit box
// o tipo dois funciona.
// testando colocar o ponteiro no edit box. setar foco.

void shellCreateEditBox (void)
{
    //Deprecated!
}


/*
 ******************** 
 * shellCreateMainWindow:
 *
 *     CRIANDO A TOP BAR.
 *     Obs: Essa é uma janela filha.
 *     @todo: ?? e o procedimento de janela ?? e as mensagens ??
 *     Obs: É uma janela simples e limpa, feita para dispositivos IOT 
 * com resolução 800x600.
 *
 */
 
// #todo
// Vamos criar a main window do tamanho da tela. get system metrics. 
 
struct window_d *shellCreateMainWindow ( int status ){

    struct window_d *w;

     //#bugbug
     //shellShell já inicializou isso com a métrica do sistema.
      	
    //
    // Limits.
    //	
	
	if ( wpWindowLeft > 50 )
	    wpWindowLeft = 50;
	
	if ( wpWindowTop > 50 )
	    wpWindowTop = 50;	

	if ( wsWindowWidth > 2048 )
	    wsWindowWidth = 2048;

	if ( wsWindowHeight > 2048 )
	    wsWindowHeight = 2048;
	    
    //no zeros.
    	    
	if ( wsWindowWidth == 0 )
	    wsWindowWidth = 100;

	if ( wsWindowHeight == 0 )
	    wsWindowHeight = 100;
	    
  
       /*
        //#debug ok funcionou;
        printf("l={%d} t={%d} w={%d} h={%d}\n", 
	        wpWindowLeft, 
			wpWindowTop,
		    wsWindowWidth, 
			wsWindowHeight );    		
	  while (1){ asm ("pause"); }
	  */
    
    
    //
    // Draw.
    //

    w = (void *) gde_create_window ( 1, 1, 1, 
                    "gdeshell-main",     
                     wpWindowLeft, wpWindowTop, 
                     wsWindowWidth, wsWindowHeight,    
                     0, 0, xCOLOR_GRAY1, xCOLOR_GRAY1 );

	if ( (void *) w == NULL )
	{
		// #bugbug
		// Precisamos resolver a sessão crítica caso 
		// quem chamaou tenha entrado.
		printf ("shellCreateMainWindow: taskbar Window fail *breakpoint");
		while (1){ asm ("pause"); }
	    return NULL;
	}

    return (struct window_d *) w;
}


void testCreateWindow (void){

    struct window_d *hWindow;	

    //++
	gde_begin_paint ();
	
	hWindow = (void *) gde_create_window ( WT_OVERLAPPED, 1, 1, 
	                       "XXX-test",     
                           10, 10, 200, 200,
                           0, 0, COLOR_RED, COLOR_YELLOW ); //xCOLOR_GRAY1, xCOLOR_GRAY1 );
	
	if ( (void *) hWindow == NULL )
	{
		printf ("testCreateWindow: FAIL! *breakpoint");
		gde_end_paint ();
		while(1){}
		
		//die ("shell.bin: hWindow fail");
	}
	
	//Registrar e mostrar.

    gde_register_window (hWindow);
    gde_show_window (hWindow);	

    gde_end_paint ();
    //--

    printf ("ok\n");
}


/*
 *************************************
 * shellDisplayBMP:
 *     Carrega um arquivo .bmp na memória e decodifica, mostrando na tela.
 */

int shellDisplayBMP (char *file_name){	
	
	// #bugbug @todo: Aumenta o tamanho do heap do processo.
	// Esse heap é gerenciando nas bibliotecas ou na API.
	//Obs: 32Kb é alem do limite.
	
	// testando malloc.
	void *b = (void *) malloc (1024*30); 	
    
	if ( (void *) b == NULL )
	{
		printf("shellTestDisplayBMP: allocation fail\n");
		//while(1){}
		return -1;
	}
	
	//Carregando o arquivo.
loadFile:

    shellui_fntos ( (char *) file_name );

    //@todo: Usar alguma rotina da API específica para carregar arquivo.
	// na verdade tem que fazer essas rotinas na API.
	
	system_call ( SYSTEMCALL_READ_FILE, (unsigned long) file_name, 
		(unsigned long) b, (unsigned long) b);	
	 
	//Usando a API para exibir o bmp carregado. 
	gde_display_bmp ( (char *) b, 10, 00 ); 
 
    //
	//Mostrando informações sobre o arquivo.
	//
	
	//base do bmp carregado na memória
	//unsigned char *bmp = (unsigned char *) b;
	
	
	//@todo: encontrando o magic
	
	//if( bmp[0] != 0x42 )
	//{
	//	printf("~Sig fail\n");
	//    printf("magic0 %c\n", bmp[0]);	
	//    printf("magic1 %c\n", bmp[1]);			
	//	printf("buffer %x\n",bmp); //Ok
    //    printf("buffer %x\n",b);   //Ok
		//printf("width %d \n", bmp[0x12]);
		//printf("height %d \n", bmp[0x16]);
	//}
	
	//
	//Mostrando características do bmp.
	
	//printf("magic0 %c\n", bmp[0]);	
	//printf("magic1 %c\n", bmp[1]);
	//printf("data area begin %c %c %c \n",bmp[base] ,bmp[base+1] ,bmp[base+2]);	
	//printf("buffer %x \n",bmp);
	//printf("data area address %x \n",&bmp[base]);
	
	return 0;
}


/*
 *************************************
 * shellDisplayBMP:
 *     Carrega um arquivo .bmp na memória e decodifica, mostrando na tela.
 *  tamanho dado em kb
 */

int shellDisplayBMPEx (char *file_name, int size ){	
	
	
    //naõ pode ser igual a zero, nem menor que zero, nem maior que 3mb.	
	if ( size <= 0 || size > (1024*3) )
	{
	    return 1;	
	}
	
	// testando malloc.
	void *b = (void *) malloc (1024*size); 	
    
	if ( (void *) b == NULL )
	{
		printf("shellTestDisplayBMP: allocation fail\n");
		//while(1){}
		return -1;
	}
	
	//Carregando o arquivo.
loadFile:

    shellui_fntos ( (char *) file_name );

    //@todo: Usar alguma rotina da API específica para carregar arquivo.
	// na verdade tem que fazer essas rotinas na API.
	
	system_call ( SYSTEMCALL_READ_FILE, (unsigned long) file_name, 
		(unsigned long) b, (unsigned long) b);	
	 
	//Usando a API para exibir o bmp carregado. 
	gde_display_bmp ( (char *) b, 0, 0 ); 
	 
    //
	//Mostrando informações sobre o arquivo.
	//
	
	//base do bmp carregado na memória
	//unsigned char *bmp = (unsigned char *) b;
	
	
	//@todo: encontrando o magic
	
	//if( bmp[0] != 0x42 )
	//{
	//	printf("~Sig fail\n");
	//    printf("magic0 %c\n", bmp[0]);	
	//    printf("magic1 %c\n", bmp[1]);			
	//	printf("buffer %x\n",bmp); //Ok
    //    printf("buffer %x\n",b);   //Ok
		//printf("width %d \n", bmp[0x12]);
		//printf("height %d \n", bmp[0x16]);
	//}
	
	//
	//Mostrando características do bmp.
	
	//printf("magic0 %c\n", bmp[0]);	
	//printf("magic1 %c\n", bmp[1]);
	//printf("data area begin %c %c %c \n",bmp[base] ,bmp[base+1] ,bmp[base+2]);	
	//printf("buffer %x \n",bmp);
	//printf("data area address %x \n",&bmp[base]);
	
	return 0;
}



/*
 * shellTestDisplayBMP:
 *     Carrega um arquivo .bmp na memória e decodifica, mostrando na tela.
 *     Devemos usar a função oferecida pela api.
 */

void shellTestDisplayBMP (void){

	
	// #bugbug @todo: Aumenta o tamanho do heap do processo.
	// Esse heap é gerenciando nas bibliotecas ou na API.
	// Obs: 32Kb é alem do limite.
	
	void *b = (void *) malloc (1024*30); 	// testando malloc.
    
	if ( (void *) b == NULL )
	{
		printf("shellTestDisplayBMP: allocation fail\n");
		//while(1){}
	}
	
	//Carregando o arquivo.
loadFile:
    //@todo: Usar alguma rotina da API específica para carregar arquivo.
	// na verdade tem que fazer essas rotinas na API.
	system_call ( SYSTEMCALL_READ_FILE, (unsigned long) bmp1_file_name, 
		(unsigned long) b, (unsigned long) b);	
	

	gde_display_bmp ( (char *) b, 10, 450 );  
	 
    //
	//Mostrando informações sobre o arquivo.
	//
	
	//base do bmp carregado na memória
	//unsigned char *bmp = (unsigned char *) b;
	
	
	//@todo: encontrando o magic
	
	//if( bmp[0] != 0x42 )
	//{
	//	printf("~Sig fail\n");
	//    printf("magic0 %c\n", bmp[0]);	
	//    printf("magic1 %c\n", bmp[1]);			
	//	printf("buffer %x\n",bmp); //Ok
    //    printf("buffer %x\n",b);   //Ok
		//printf("width %d \n", bmp[0x12]);
		//printf("height %d \n", bmp[0x16]);
	//}
	
	//
	//Mostrando características do bmp.
	
	//printf("magic0 %c\n", bmp[0]);	
	//printf("magic1 %c\n", bmp[1]);
	//printf("data area begin %c %c %c \n",bmp[base] ,bmp[base+1] ,bmp[base+2]);	
	//printf("buffer %x \n",bmp);
	//printf("data area address %x \n",&bmp[base]);		
}


/*
 * bmpDisplayBMP:
 *     Mostra na tela uma imagem .bmp carregada na memória.
 *     #todo: No kernel ou na api tem rotina mais aprimorada que essa.
 */

void 
bmpDisplayBMP ( 
    void *address, 
    unsigned long x, 
    unsigned long y, 
    int width, 
    int height )
{
    gde_display_bmp ( (char *) address, x, y ); 
}


//testando botão.
//quando clicamos no menu da barra de tarefas.
//todo mudar o nome da função.

int shellTestButtons (void)
{

	// Tamanho da tela.	
    unsigned long ScreenWidth = gde_get_system_metrics (1);
    unsigned long ScreenHeight = gde_get_system_metrics (2); 

	unsigned long app1Left = ((ScreenWidth/8) * 2);
	unsigned long app2Left = ((ScreenWidth/8) * 3);
	unsigned long app3Left = ((ScreenWidth/8) * 4);
	unsigned long app4Left = ((ScreenWidth/8) * 5);
	
	unsigned long app1Top = (ScreenHeight - ((ScreenHeight/16)*2) ); //( (ScreenHeight/10) * 8 ); 
	unsigned long app2Top = app1Top; 
	unsigned long app3Top = app1Top; 
	unsigned long app4Top = app1Top;
	
	
    
	//
	// botão de reboot
	//
	
    //em shell.h está o ponteiro.	
	app1_button = (void *) gde_create_window ( WT_BUTTON, 1, 1, 
	                            "(CIMA)",     
                                app1Left, app1Top, 80, 24,    
                                0, 0, xCOLOR_GRAY1, xCOLOR_GRAY1 );

    gde_register_window (app1_button);


	//
	// botão de close
	//
	
    //em shell.h está o ponteiro.	
	app2_button = (void *) gde_create_window ( WT_BUTTON, 1, 1, 
	                            "(BAIXO)",     
                                app2Left, app2Top, 80, 24,    
                                0, 0, xCOLOR_GRAY1, xCOLOR_GRAY1 );
								
    gde_register_window (app2_button);
	
	
	
	//
	// botão de close
	//
	
    //em shell.h está o ponteiro.	
	app3_button = (void *) gde_create_window ( WT_BUTTON, 1, 1, 
	                            " APP3 ",     
                                app3Left, app3Top, 80, 24,    
                                0, 0, xCOLOR_GRAY1, xCOLOR_GRAY1 );
								
    gde_register_window (app3_button);




	//
	// botão de close
	//
	
	
    //em shell.h está o ponteiro.	
	app4_button = (void *) gde_create_window ( WT_BUTTON, 1, 1, 
	                            " APP4 ",     
                                app4Left, app4Top, 80, 24,    
                                0, 0, xCOLOR_GRAY1, xCOLOR_GRAY1 );
								
    gde_register_window (app4_button);	
    

    return 0;	
}




