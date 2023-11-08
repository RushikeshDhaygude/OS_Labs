#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
int main() {
int fd1;
char *myfifo1 = "/tmp/myfifo1";
char *myfifo2 = "/tmp/myfifo2";
mkfifo(myfifo1, 0666);
mkfifo(myfifo2, 0666);
char str1[80], str2[80];
while (1) {
fd1 = open(myfifo1, O_RDONLY);
read(fd1, str1, 80);
printf("User1: %s\n", str1);
close(fd1);
FILE *outputFile = fopen("output.txt", "w");
int charCount = 0, wordCount = 0, lineCount = 0;
for (int i = 0; str1[i] != '\0'; i++) {
charCount++;
if (str1[i] == ' ' || str1[i] == '\n') {
wordCount++;
}
if (str1[i] == '\n') {
lineCount++;
}
}
fprintf(outputFile, "Character Count: %d\nWord Count: %d\nLine Count: %d\n", charCount,
wordCount, lineCount);
fclose(outputFile);
fd1 = open(myfifo2, O_WRONLY);
fgets(str2, 80, stdin);
write(fd1, str2, strlen(str2) + 1);
close(fd1);
}
return 0;
}
