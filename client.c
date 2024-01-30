#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

char* StringToBinary(const char* text) {
    size_t textLength = strlen(text);
    char* binary = (char*)malloc((textLength * 8 + 1) * sizeof(char));

    size_t binaryIndex = 0;
    for (size_t i = 0; i < textLength; ++i) {
        unsigned char character = text[i];
        for (int j = 7; j >= 0; --j) {
            binary[binaryIndex++] = ((character >> j) & 1) ? '1' : '0';
        }
    }

    binary[binaryIndex] = '\0';

    return binary;
}

void sendSignal(char bit, char* targetPID) {
    int pid = atoi(targetPID);

    if (bit == '1') {
        kill(pid, SIGUSR1);
    } else {
        kill(pid, SIGUSR2);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Erreur : Nombre incorrect d'arguments\n");
        printf("Usage : %s <PID du serveur> <message>\n", argv[0]);
        return 1;
    }

    int pid = atoi(argv[1]);
    const char *message = argv[2];

    printf("Envoi au serveur (PID %d)...\n", pid);

    // char startHeader = 2;
    // sendSignal(startHeader, argv[1]);
    // usleep(2000);

    char *binaryMessage = StringToBinary(message);
  
    for (size_t i = 0; i < strlen(binaryMessage); ++i) {
        sendSignal(binaryMessage[i], argv[1]);
        usleep(800);  
    }

    kill(pid, SIGTRAP);

    printf("Done.\n");

    free(binaryMessage);

    return 0;
}
