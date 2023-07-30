#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdbool.h>

#define MAKS_TEKST 100
#define PERMISIJE 0666

typedef struct moja_poruka {
   long tip;
   char tekst[MAKS_TEKST];
} PORUKA;

int main() {
	PORUKA poruka;
	int msgq_id; 
	key_t key;
	
	// Ukoliko kljuc nije uspesno generisan
	if ((key = ftok("sender.c", 0)) == -1) {
		perror("ftok");
		exit(EXIT_FAILURE);
	}
	
	// Ukoliko red sa porukama nije uspesno generisan
	if ((msgq_id = msgget(key, PERMISIJE | IPC_CREAT)) == -1) {
		perror("msgget");
		exit(EXIT_FAILURE);
	}
	
	printf("Red je spreman za prijem novih poruka!\n");
	bool flag = true;
	while (flag) {
		printf("Unesite vasu poruku(unesite \"KRAJ\" za prekid unosa):\n");
		poruka.tip = 1;
		char buf[MAKS_TEKST];
		fgets(buf, MAKS_TEKST, stdin);
		if (strncmp(buf, "KRAJ", 4) == 0) {
			flag = false;
		}
		strncpy(poruka.tekst, buf, MAKS_TEKST);
		// Ukoliko poruka nije uspesno poslata
		if (msgsnd(msgq_id, &poruka, MAKS_TEKST, 0) == -1) {
			perror("msgsnd");
		}
	}
	
	exit(EXIT_SUCCESS);
}
