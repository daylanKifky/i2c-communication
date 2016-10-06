// Ejemplo de interfaz userspace con /dev/i2c-N
// Original en: http://blog.davidegrayson.com/2012/06/using-ic-on-raspberry-pi.html
// Mas info en: http://git.kernel.org/cgit/linux/kernel/git/torvalds/linux.git/tree/Documentation/i2c/dev-interface

#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#define CHIP_ADDR 0x52

int main(){

	int file;
	//El adapter se puede encontrar haciendo: 
	//>>ls /dev/*i2c*
	//o
	//>> i2cdetect -l
	int adapter_nr = 1;
	char filename[20];
	char buf[10];

	//abir el device i2c, y elevar un error si no se encuentra
	snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);
	file = open(filename, O_RDWR);
	if (file < 0) {
		perror(filename);
		exit(1);
	}

	//Especificar la address del chip i2c (EEPROM en este caso)
	if (ioctl(file, I2C_SLAVE, CHIP_ADDR) < 0){
		perror("No se encuentra el Slave device");
		exit(1);
	}

	//escribir en el dispositivo
	buf[0] = 0x00;
	buf[1] = 0xe3;
	if ( write(file, buf, 2) != 2){
		perror("No se pudo escribir en el dispositivo");
		exit(1);
	} else {
		printf("Escrito el valor 0x%X en 0x%X\n", buf[1], CHIP_ADDR );

	}


	 if ( write(file, buf, 1) != 1){
                  perror("No se pudo asignar ADDR en el dispositivo\n");
              exit(1);
	}

	if ( read(file, buf, 1) !=1 ){
		perror("No se pudo leer del dispositivo");
		exit(1);
	}

	printf("Leido el valor 0x%X en 0x%X\n", buf[0], CHIP_ADDR);

	return 0;

}



