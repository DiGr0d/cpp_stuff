#include <iostream>
#include <unistd.h>
#include <fcntl.h>

int main(){
    char filepath[256];
    char cur;
    int i = 0;
    

    while(i < 255){
      int val = read(0, &cur, 1); 
      if(val == 0 || val == -1){break;}
       if(cur == ' ' || cur == '\n' || cur == '\t'){break;}
       *(filepath + i) = cur;
       i++;
    }

    *(filepath+i) = '\0';
    printf("%s", filepath);
    char input[300];
    scanf("%s", input);
   
    int err = open(filepath, O_WRONLY);
    if(err != -1){
        int i = 0;
        while(*(input + i) != '\0')
        {
            write(err, input+i, 1);
            i++;
        }
        
    }
}

