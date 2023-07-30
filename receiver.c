#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdbool.h>
#include <string.h>

#define MAKS_TEKST 100
#define PERMISIJE 0666
#define FIFO 0

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

       	// Ukoliko nije uspesno povezivanje sa redom poruka
        if ((msgq_id = msgget(key, PERMISIJE)) == -1) {
                perror("msgget");
                exit(EXIT_FAILURE);
        }

       	printf("Red je spreman za citanje poruka!\n");
	int brojac = 1;
	while (true) {
		// Ukoliko poruka nije uspesno procitana
		if (msgrcv(msgq_id, &poruka, MAKS_TEKST, FIFO, 0) == -1) {
			perror("msgrcv");
			exit(EXIT_FAILURE);
		}
		if (strncmp(poruka.tekst, "KRAJ", 4) == 0) {
      			printf("Poruke su uspesno procitane!\n");
			break;
		}
		printf("%d. poruka: %s\n", brojac++, poruka.tekst);
        }
	
	// Ukoliko red sa porukama nije uspesno zatvoren
	if (msgctl(msgq_id, IPC_RMID, 0) == -1) {
		perror("msgctl");
		exit(EXIT_FAILURE);
	}
        exit(EXIT_SUCCESS);
}
