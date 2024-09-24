#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ALPHABET_SIZE 26
double englishFreq[ALPHABET_SIZE] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966,
    0.153, 0.772, 4.025, 2.406, 6.749, 7.507, 1.929, 0.095, 5.987,
    6.327, 9.056, 2.758, 0.978, 2.360, 0.150, 1.974, 0.074
};
void calculateFrequency(char *ciphertext, double freq[ALPHABET_SIZE]) {
    int len = strlen(ciphertext);
    int letterCount[ALPHABET_SIZE] = {0};
    for (int i = 0; i < len; i++) {
        if (ciphertext[i] >= 'A' && ciphertext[i] <= 'Z') {
            letterCount[ciphertext[i] - 'A']++;
        } else if (ciphertext[i] >= 'a' && ciphertext[i] <= 'z') {
            letterCount[ciphertext[i] - 'a']++;
        }
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        freq[i] = (double)letterCount[i] / len * 100;
    }
}
void decrypt(char *ciphertext, int shift, char *plaintext) {
    int len = strlen(ciphertext);
    for (int i = 0; i < len; i++) {
        if (ciphertext[i] >= 'A' && ciphertext[i] <= 'Z') {
            plaintext[i] = ((ciphertext[i] - 'A' - shift + ALPHABET_SIZE) % ALPHABET_SIZE) + 'A';
        } else if (ciphertext[i] >= 'a' && ciphertext[i] <= 'z') {
            plaintext[i] = ((ciphertext[i] - 'a' - shift + ALPHABET_SIZE) % ALPHABET_SIZE) + 'a';
        } else {
            plaintext[i] = ciphertext[i]; 
        }
    }
    plaintext[len] = '\0';
}
double chiSquared(double observed[ALPHABET_SIZE], double expected[ALPHABET_SIZE]) {
    double chiSq = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (expected[i] != 0) {
            chiSq += ((observed[i] - expected[i]) * (observed[i] - expected[i])) / expected[i];
        }
    }
    return chiSq;
}
void performAttack(char *ciphertext, int topN) {
    double cipherFreq[ALPHABET_SIZE];
    calculateFrequency(ciphertext, cipherFreq);
    double chiSq[ALPHABET_SIZE];
    char plaintext[1000];
    char possiblePlaintexts[ALPHABET_SIZE][1000];  
    for (int shift = 0; shift < ALPHABET_SIZE; shift++) {
        decrypt(ciphertext, shift, plaintext);
        calculateFrequency(plaintext, cipherFreq);
        chiSq[shift] = chiSquared(cipherFreq, englishFreq);
        strcpy(possiblePlaintexts[shift], plaintext);
    }
    for (int i = 0; i < ALPHABET_SIZE - 1; i++) {
        for (int j = i + 1; j < ALPHABET_SIZE; j++) {
            if (chiSq[i] > chiSq[j]) {
                double temp = chiSq[i];
                chiSq[i] = chiSq[j];
                chiSq[j] = temp;
                char tempText[1000];
                strcpy(tempText, possiblePlaintexts[i]);
                strcpy(possiblePlaintexts[i], possiblePlaintexts[j]);
                strcpy(possiblePlaintexts[j], tempText);
            }
        }
    }
    printf("Top %d possible plaintexts:\n", topN);
    for (int i = 0; i < topN && i < ALPHABET_SIZE; i++) {
        printf("%d: %s\n", i + 1, possiblePlaintexts[i]);
    }
}
int main() {
    char ciphertext[1000];
    int topN;
    printf("Enter the ciphertext: ");
    fgets(ciphertext, sizeof(ciphertext), stdin);
    ciphertext[strcspn(ciphertext, "\n")] = '\0'; 
    printf("Enter the number of top possible plaintexts to display: ");
    scanf("%d", &topN);
    performAttack(ciphertext, topN);
    return 0;
}

