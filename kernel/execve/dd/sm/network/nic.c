/*
 * File: nic.c   
 *
 * Descrição:
 *     Network interface controller
 *     Network card driver.
 *     Algum gerenciamento de adaptador de network.
 *     Esse é o header do driver da placa de rede.
 *
  * 8086/100e network controller encontrado na oracle virtual box.
 * History:
 *     2016 - Created by Fred Nora.
 *
 */
 
 
// >> The register at offset 0x00 is the "IOADDR" window. 
// >> The register at offset 0x04 is the "IODATA" window. 

 /*
   wikipedia - NIC
   A network interface controller 
   (NIC, also known as a network interface card, network adapter, 
   LAN adapter or physical network interface,[1] and by similar terms) 
   is a computer hardware component that connects a computer to a computer network.[2]
  
Connects to Motherboard via one of:
    Integrated, PCI Connector, ISA Connector, PCI-E, FireWire, USB, Thunderbolt.

Network via one of:
    Ethernet, Wi-Fi, Fibre Channel, ATM, FDDI, Token ring.

Speeds:
    10 Mbit/s, 100 Mbit/s, 1 Gbit/s, 10 Gbit/s, up to 160 Gbit/s.

Common manufacturers:
    Intel, Realtek, Broadcom, Marvell Technology Group, QLogic, Mellanox.
 
techniques:
    Polling, Interrupt-driven I/O, Programmed input/output, Direct memory access.
 
 */
 
#include <kernel.h>

/*
void nicHandler();
void nicHandler()
{
	return;
}
*/


/*
 * init_network:
 *     Inicializa o módulo gerenciador de rede.
 *     @todo: deletar: Usar nicInit()
 *
 * ## bugbug: problemas no mapeamento do endereço encontrado em BAR0
 */
 
int init_nic (){
	
	
	//pci info.
	uint32_t data; 
	unsigned char bus;
	unsigned char dev;
	unsigned char fun;			


	
	printf("\n");
	printf("probing pci ...\n");
					
	data = (uint32_t) diskPCIScanDevice ( PCI_CLASSCODE_NETWORK);
	
	//#test: testando encontrar placa de rede.
	if( data == -1 )
	{
		printf("**fail**\n");
		
		return (int) 1;
		//refresh_screen();
	}
					
	bus = ( data >> 8 &0xff );
    dev = ( data >> 3 &31 );
    fun = ( data &7 );
					
	data = (uint32_t) diskReadPCIConfigAddr( bus, dev, fun, 0 );
					
					
	//#todo: salvar na estrutura de pci device.
	//printf("Vendor=%x \n", (data & 0xffff) );
	//printf("Device=%x \n", (data >> 16 &0xffff) );
	
    //
	//  PCI
	//	
	
	struct pci_device_d *pci_device;
	
	pci_device = (void *) malloc ( sizeof( struct pci_device_d  ) );
	
	if ( (void *) pci_device ==  NULL )
    {
		return (int) 1;
	}else{
		
		pci_device->deviceUsed = 1;
		pci_device->deviceMagic = 1234;
		
		pci_device->bus = (unsigned char) bus;
		pci_device->dev = (unsigned char) dev;
		pci_device->func = (unsigned char) fun;
		
		pci_device->Vendor = (unsigned short) (data & 0xffff);
		pci_device->Device = (unsigned short) (data >> 16 &0xffff);
		
		pci_device->BAR0 = (unsigned long) diskReadPCIConfigAddr( bus, dev, fun, 0x10 );
		pci_device->BAR1 = (unsigned long) diskReadPCIConfigAddr( bus, dev, fun, 0x14 ); 
		pci_device->BAR2 = (unsigned long) diskReadPCIConfigAddr( bus, dev, fun, 0x18 );
		pci_device->BAR3 = (unsigned long) diskReadPCIConfigAddr( bus, dev, fun, 0x1C );
		pci_device->BAR4 = (unsigned long) diskReadPCIConfigAddr( bus, dev, fun, 0x20 );
		pci_device->BAR5 = (unsigned long) diskReadPCIConfigAddr( bus, dev, fun, 0x24 );
		
		//...
	};
	
	

    //##importante:
	//##todo tem que mapear esse endereço.
	
	//########### todo
	//## bugbug: problemas no mapeamento do endereço encontrado em BAR0
	
	unsigned long phy_address = (pci_device->BAR0 & 0xFFFFFFF0);
	
	//mapeando para obter o endereço virtual que o kernel pode manipular.
	unsigned long virt_address = mapping_nic0_device_address( phy_address );
	
	//endereço base.
	unsigned char *base_address = (unsigned char *) virt_address;
	unsigned long *base_address32 = (unsigned long *) virt_address;

	//se for 0 temos que pegar de outro jeito.
	//if ( base_address[0x5400 + 0] == 0 &&
	//     base_address[0x5400 + 1] == 0 &&
	//	 base_address[0x5400 + 2] == 0 &&
    //     base_address[0x5400 + 3] == 0 &&
    //     base_address[0x5400 + 4] == 0 &&
    //     base_address[0x5400 + 5] == 0  )
	//{
		//pegar em outro lugar.	 
	//}  		 
	
	//ok, os endereços estão certos.
	//printf("phy_address = %x\n", phy_address );
	//printf("virt_address = %x\n", virt_address );
	
	//int z;
	
	//for( z=0; z<50; z++ )
	//{
	//    printf("%x \n",base_address32[z]);		
	//}
	
	//printf("%x ",base_address32[0]);
	//printf("%x ",base_address32[0]);
	//printf("%x ",base_address32[0]);
	//printf("%x ",base_address32[0]);
	//printf("%x ",base_address32[0]);
	//printf("%x ",base_address32[0]);
		
	//refresh_screen();
	//while(1){}
	
    //
	//  NIC
	//
	
	currentNIC = (void *) malloc ( sizeof( struct nic_info_d ) );
	
	if ( (void *) currentNIC ==  NULL )
	{
	    return (int) 1;	
	}else{
		
		currentNIC->used = 1;
		currentNIC->magic = 1234;
		
		currentNIC->pci = (struct pci_device_d *) pci_device;
		
		//salvando o endereço para outras rotinas usarem.
		currentNIC->registers_base_address = (unsigned long) &base_address[0];
		
		
		//
		// #### Get Info ####
		//
		
		//device status
		currentNIC->DeviceStatus = base_address[ 0x8];
		
		
		//mac
		currentNIC->mac0 = base_address[ 0x5400 + 0 ];
		currentNIC->mac1 = base_address[ 0x5400 + 1 ];
		currentNIC->mac2 = base_address[ 0x5400 + 2 ];
		currentNIC->mac3 = base_address[ 0x5400 + 3 ];
		currentNIC->mac4 = base_address[ 0x5400 + 4 ];
		currentNIC->mac5 = base_address[ 0x5400 + 5 ];
		
		//...
		
	};	
	
	printf("done\n");		
	

    return (int) 0;	
};



//testando controlador ... 
//encontrando o controlador e identificando vendor e device.
void nic_test1 (){
	
/*	
	//pci info.
	uint32_t data; 
	unsigned char bus;
	unsigned char dev;
	unsigned char fun;		
	
	printf("\n");
	printf("probing pci ...\n");
					
	data = (uint32_t) diskPCIScanDevice ( PCI_CLASSCODE_NETWORK);
	
	//#test: testando encontrar placa de rede.
	if( data == -1 )
	{
		printf("**fail**\n");
		refresh_screen();
	}
					
	bus = ( data >> 8 &0xff );
    dev = ( data >> 3 &31 );
    fun = ( data &7 );
					
	data = (uint32_t) diskReadPCIConfigAddr( bus, dev, fun, 0 );
					
	printf("Vendor=%x \n", (data & 0xffff) );
	printf("Device=%x \n", (data >> 16 &0xffff) );
	printf("done\n");	
*/

};



void show_current_nic_info (){

	if ( (void *) currentNIC ==  NULL )
	{
		printf("show_current_nic_info: struct fail\n");
	    return;	
	}else{
		
		if ( currentNIC->used != 1 || currentNIC->magic != 1234 )
		{
		    printf("show_current_nic_info: validation fail\n");
	        return;				
		}
		
		if ( (void *) currentNIC->pci == NULL )
		{
		    printf("show_current_nic_info: pci struct fail\n");
	        return;				
		}

        //messages  		
		printf("NIC device info:\n");
		printf("Vendor %x Device %x \n", 
		    currentNIC->pci->Vendor, currentNIC->pci->Device );
			
			
		//bars	
		printf("BAR0 %x\n",currentNIC->pci->BAR0);
		printf("BAR1 %x\n",currentNIC->pci->BAR1);
		printf("BAR2 %x\n",currentNIC->pci->BAR2);
		printf("BAR3 %x\n",currentNIC->pci->BAR3);
		printf("BAR4 %x\n",currentNIC->pci->BAR4);
		printf("BAR5 %x\n \n",currentNIC->pci->BAR5);
		
		
		
		printf("Device status %x \n", currentNIC->DeviceStatus );
		
		if (currentNIC->DeviceStatus & 1){
			printf("Full duplex \n");
		}	
		
		
		if (currentNIC->DeviceStatus & 0x80){
			printf("1000Mbs\n");
		}	
		
		printf("MAC %x ", currentNIC->mac0 );
		printf("%x ", currentNIC->mac1 );
		printf("%x ", currentNIC->mac2 );
		printf("%x ", currentNIC->mac3 );
		printf("%x ", currentNIC->mac4 );
		printf("%x \n", currentNIC->mac5 );
		
		//...
		
	};	    
	
};



void nic_i8254x_reset(){
	
	
	printf("nic_i8254x_reset: \n");

	//#todo: precisamos checar a validade dessa estrutura e do endereço.
	
	//endereço base.
	unsigned char *base_address = (unsigned char *) currentNIC->registers_base_address;
	unsigned long *base_address32 = (unsigned long *) currentNIC->registers_base_address;	
	
	
	//; Disable all interrupt causes
	//; Interrupt Mask Clear Register
	base_address32[0x00D8] = 0xFFFFFFFF;
	
	
	//Disable interrupt throttling logic
	//Interrupt Throttling Register
	base_address32[0x00C4] = 0; 
	
	
	//PBA: set the RX buffer size to 48KB (TX buffer is calculated as 64-RX buffer)
	//Transmit Configuration Word
	base_address32[0x0178] = 0x00000030; 
	
	
	//#todo:
	// CTRL: clear LRST, set SLU and ASDE, clear RSTPHY, VME, and ILOS
	//0x0000
	//limpar alguns bit so Control
	
	
	
	
	//; MTA: reset
	//; Multicast Table Array
	//0x5200
	base_address32[0x5200] = 0xFFFFFFFF;
	
	
	//configurar a recepção.
	
	
	// TDBAL
	//; Transmit Descriptor Base Address Low
	base_address32[0x3800] = 0x80000;
	
	//TX Descriptor Length
	base_address32[0x3808] = (32 * 8);	
	
	//TDH - Transmit Descriptor Head
	base_address32[0x3810] = 0;
	
	//TDL - Transmit Descriptor Tail
    base_address32[0x3818] = 1;
	
	//; Enabled, Pad Short Packets, 15 retries, 64-byte COLD, Re-transmit on Late Collision
	//; Transmit Control Register	
	base_address32[0x0400] = 0x010400FA;
	
	
	//; IPGT 10, IPGR1 8, IPGR2 6
	//; Transmit IPG Register
	//; Transmit Inter Packet Gap
	//0x0060200A	0x0410
	base_address32[0x0410] = 0x0060200A;
	
	
	//; Clear the Receive Delay Timer Register 0x2820 RX Delay Timer Register
	// Clear the Receive Interrupt Absolute Delay Timer 0x282C RX Int. Absolute Delay Timer
	// Clear the Receive Small Packet Detect Interrupt 0x2C00  RX Small Packet Detect Interrupt
	base_address32[0x2820] = 0; 
	base_address32[0x282C] = 0; 
	base_address32[0x2C00] = 0; 
	
	
	
	//#bugbug: essa parte pode ser complicada.
	//; Temp enable all interrupt types
	//; Enable interrupt types
	//Interrupt Mask Set/Read Register 
	base_address32[0x00D0] = 0x1FFFF; 
	
	
	printf("nic_i8254x_reset: done\n");
	refresh_screen();
};



void nic_i8254x_transmit(){
	
	
	printf("nic_i8254x_transmit: \n");

	//#todo: precisamos checar a validade dessa estrutura e do endereço.
	
	//endereço base.
	unsigned char *base_address = (unsigned char *) currentNIC->registers_base_address;
	unsigned long *base_address32 = (unsigned long *) currentNIC->registers_base_address;	
	
	// TDBAL
	base_address32[0x3800] = 0x80000;
	
	// TDH - Transmit Descriptor Head
	base_address32[0x3810] = 0;
	
	// TDL - Transmit Descriptor Tail
    base_address32[0x3818] = 1;
	
	
	printf("nic_i8254x_transmit: done\n");
	refresh_screen();
};

/*
int nicInit()
{};
*/

//
// End.
//

