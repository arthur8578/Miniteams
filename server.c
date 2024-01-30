#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

int is_new = 1;

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
void printTimestampedMessage() {
    // Obtenir l'horodatage actuel
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%H:%M:%S", tm_info);

    // Afficher uniquement l'horodatage
    printf("[%s] ", timestamp);
}

void handler(int signum) {
    static char buffer[9];
    static int index = 0;
    if (signum == SIGUSR1 || signum == SIGUSR2) {

        if (is_new == 1){
            printTimestampedMessage();
            is_new = 0;
        }

        buffer[index++] = (signum == SIGUSR1) ? '1' : '0';
        

        if (index == 8) {
            buffer[index] = '\0';
            char asciiChar = strtol(buffer, NULL, 2);
            
          // FILE *file = fopen("conversations.log", "a");
          // if (file != NULL) {
          //     fprintf(file, "%c", asciiChar);
          //     fclose(file);
          // }

            printf(&asciiChar, 1);

            index = 0;            
        }
        
    }

    if (signum == SIGTRAP) {
        printf("\n");
        is_new = 1;
        
    }
}

int main() {
    pid_t pid = getpid();
    printf("Serveur démarré avec PID : %d\n", pid);

    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGUSR1, &sa, NULL) == -1 || sigaction(SIGUSR2, &sa, NULL) == -1 || sigaction(SIGTRAP, &sa, NULL) == -1) {
        return 1;
    }

    printf("En attente de messages...\n");

    while (1) {
        pause(); 
    }


    return 0;
}