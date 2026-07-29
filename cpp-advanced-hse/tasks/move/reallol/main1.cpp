#include <cstdio>

int main(){
    FILE* file = fopen("input.txt", "w");
    fputs("1\n",file );
    fputs("usr1\n", file);
    fputs("1\n",file );
    fputs("usr2\n", file);
    fputs("4\n",file );
    fputs("usr2\n", file);
    fputs("usr1\n",file );
    for(int i = 0; i <16; i++){
        fputs("2\n",file);
        fputs("usr1\n", file);
        fputs("1 1\n", file);
    }
    fputs("3\n",file);
    fputs("usr2\n", file);
    fputs("2\n", file);
    fputs("usr1\n", file);
    fputs("1 1\n",file);
    fputs("3\nusr2\n", file);
}