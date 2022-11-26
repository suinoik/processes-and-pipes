// C program to demonstrate use of fork() and pipe() 
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
  
int main() 
{ 
    // We use two pipes 
    // First pipe to send input string from parent 
    // Second pipe to send concatenated string from child 
  
    int fd1[2];  // Used to store two ends of first pipe 
    int fd2[2];  // Used to store two ends of second pipe 
  
    char fixed_str[] = "howard.edu"; 
    char input_str[100]; 
    pid_t p; 
  
    if (pipe(fd1)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
    if (pipe(fd2)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
  
    printf("Enter a string to concatenate:");
    scanf("%s", input_str); 
    p = fork(); 
  
    if (p < 0) 
    { 
        fprintf(stderr, "fork Failed" ); 
        return 1; 
    } 
  
    // Parent process 
    else if (p > 0) {
    close(fd1[0]); // Close reading end of Parent pipe
    close(fd2[1]); // Close writing end of Child pipe

    // concatenate fixed_str to input_str
    int k = strlen(input_str);
    int i;
    for (i = 0; i < strlen(fixed_str); i++)
      input_str[k++] = fixed_str[i];
    input_str[k] = '\0'; // string ends with '\0'

    // Write input string (after concatenation) to Parent pipe.
    write(fd1[1], input_str, strlen(input_str) + 1);

    // Wait for child to send a string
    wait(NULL);

    // Read new string from Child pipe.
    char concat_str2[100]; //= "gobison.org";
    read(fd2[0], concat_str2, 100);

    // Concatenate string from Child pipe to it.
    int j = strlen(input_str);
    int l;
    for (l = 0; l < strlen(concat_str2); l++) {
      input_str[j++] = concat_str2[l];
    }
    input_str[j] = '\0'; // string ends with '\0'
    printf("Second Concatenated string: %s\n", input_str);

    // Close writing end of Parent pipe and reading end of Child pipe.
    close(fd1[1]);
    close(fd2[0]);
  }
  // child process
  else {
    close(fd1[1]); // Close writing end of Parent pipe.
    close(fd2[0]); // Close reading end of Child pipe.
    char input_str2[100];
    // Read a string from the Parent pipe.
    char concat_str[100];
    read(fd1[0], concat_str, 100);

    // print the string from the parent
    printf("First Concatenated string: %s\n", concat_str);

    // Write input string to Child pipe.
    printf("Enter another string to concatenate: ");
    scanf("%s", input_str2);
    write(fd2[1], input_str2, strlen(input_str2) + 1);

    // Close writing end of Child pipe and reading end of Parent pipe.
    close(fd1[0]);
    close(fd2[1]);
    exit(0);
  }
} 