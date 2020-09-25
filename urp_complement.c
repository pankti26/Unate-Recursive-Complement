#include <stdlib.h>
#include <stdio.h>

unsigned int *** complement(unsigned int a, unsigned int b, unsigned int c, unsigned int F[][b][c], unsigned int *arr_s);
unsigned int split(unsigned int a, unsigned int b, unsigned int c, unsigned int F[][b][c]);
void PosCofactor(unsigned int a, unsigned int b, unsigned int c, unsigned int F[][b][c], unsigned int x);
void NegCofactor(unsigned int a, unsigned int b, unsigned int c, unsigned int F[][b][c], unsigned int x);
void andp(unsigned int a, unsigned int b, unsigned int c, unsigned int F[][b][c], unsigned int x);
void andn(unsigned int a, unsigned int b, unsigned int c, unsigned int F[][b][c], unsigned int x);
unsigned int *** or(unsigned int ap, unsigned int an, unsigned int b, unsigned int c, unsigned int P[][b][c], unsigned int N[][b][c]);


void main(){
    char f[200][64] ={0};
    unsigned int a; //no of cubes
    unsigned int b; //no of var
    unsigned int c = 2;
    int i,j;
    //read file and store in array
    FILE *fptr;
    fptr = fopen("Honours Assignment 1b/part5.pcn", "r");
    printf("File Open\n");
    if (fptr != NULL) {
        char line[64];
        i=0;
        while (fgets(line, sizeof line, fptr) != NULL) {
            char *start = line;
            int field;
            int n;
            j=0;
            while (sscanf(start, "%d%n", &field, &n) == 1) {
                //printf("%d ", field);
                start += n;
                f[i][j] = field;
                //printf("%d ", f[i][j]);
                j++;
            }
            //printf("\n");
            i++;
        }
    }
    printf("File Read\n");
    b = abs(f[0][0]);
    a = abs(f[1][0]);
    printf("a %d b %d", a, b);
    unsigned int M[a][b][c];
    //Initialize M elements to 11
    for(int i=0; i<a; i++){
        for(int j=0; j<b; j++){
            M[i][j][0] = 1;
            M[i][j][1] = 1;
        }
    }
    //Store values in M
    for(i=0; i<a; i++){
        int g=f[2+i][0];
        for(j=0; j<g; j++){
            if(f[2+i][1+j]>0){
                M[i][abs(f[2+i][1+j])-1][0] = 0;
                M[i][abs(f[2+i][1+j])-1][1] = 1;
            }
            else{
                M[i][abs(f[2+i][1+j])-1][0] = 1;
                M[i][abs(f[2+i][1+j])-1][1] = 0;
            }
        }
    }
    printf("M array is\n");
    //print M
    for(i=0; i<a; i++){
        for(j=0; j<b; j++){
            printf("%d%d ", M[i][j][0], M[i][j][1]);
        }
        printf("\n");
    }
    fclose(fptr);

    unsigned int ***F;
    unsigned int arr_s;
    F = complement(a,b,c,M,&arr_s);
    int num=0;
    for(i=0; i<arr_s; i++){
        if(F[i][0][0]==1 || F[i][0][1]==1) num++;
    }
    unsigned int temp[num];
    for(i=0; i<num; i++){
        temp[i]=0;
    }
    int k=0;
    for(i=0; i<arr_s; i++){
        if(F[i][0][0]==1 || F[i][0][1]==1){
            for(j=0; j<b; j++){
                if(F[i][j][0]==0 && F[i][j][1]==1) temp[k]++;
                else if(F[i][j][0]==1 && F[i][j][1]==0) temp[k]++;
            }
            k++;
        }
    }
    FILE *wptr;
    wptr = fopen("part5_ans.txt","w");
    fprintf(wptr,"%d",b);
    fprintf(wptr,"\n");
    fprintf(wptr,"%d",num);
    fprintf(wptr,"\n");
    printf("Final Complement\n");
    k=0;
    for(i=0;i<arr_s;i++){   //print
        if(F[i][0][0]==1 || F[i][0][1]==1){
            fprintf(wptr,"%d ",temp[k]);
            for(j=0;j<b;j++){
                printf("%d%d ", F[i][j][0], F[i][j][1]);
                if(F[i][j][0]==0 && F[i][j][1]==1){
                    fprintf(wptr,"%d ",j+1);
                }
                else if(F[i][j][0]==1 && F[i][j][1]==0){
                    fprintf(wptr,"-%d ",j+1);
                }
            }
            fprintf(wptr,"\n");
            printf("\n");
            k++;
        }
    }
    fclose(wptr);

    free(F);
}

unsigned int *** complement(unsigned int a, unsigned int b, unsigned int c, unsigned int F[][b][c], unsigned int *arr_s){
    //check if F is simple
    int flag=0;
    int index=0;
    int all=0;
    unsigned int arr_size=0;
    unsigned int temp[b];
    int i,j;
    for(j=0; j<b; j++){ //initialize temp to 0
        temp[j]=0;
    }
    for(i=0; i<a; i++){ //determine if simple or not
        if(F[i][0][0]==1 || F[i][0][1]==1) {
            flag++;
            index = i;  //store the index of cube list for simple cube
            if(F[i][0][0]==1 && F[i][0][1]==1 && all==0){   //check for cube list with all elements 11
                all=1;
                for(j=1; j<b; j++){
                    if(F[i][j][0]==1 && F[i][j][1]==1 && all==1) all=1;
                    else all=0;
                }
            }
        }
    }
    if(all==1) flag=1; //there is a cube list with all elements 11
    if(flag<2){ //if F simple
        printf("Simple \n");
        for(i=0;i<a;i++){
            for(j=0;j<b;j++){
                printf("%d%d ", F[i][j][0], F[i][j][1]);
            }
            printf("\n");
        }
        //printf("Index %d\n", index);
        int k=0;
        for(j=0; j<b; j++){ //determine the matrix size
            if((F[index][j][0]==0 && F[index][j][1]==1) || (F[index][j][0]==1 && F[index][j][1]==0)){
                arr_size++;
                temp[k]=j;
                k++;
            }
        }
        //printf("Array Size %d\n", arr_size);
        //create matrix M of arr_size and set all elements to 11
        unsigned int ***M;
        if(arr_size==0 || arr_size==1){
            M = malloc(sizeof(unsigned int*) *1);
            M[0] = malloc(sizeof(unsigned int*) *b);
            for(j=0; j<b; j++){
                M[0][j] = malloc(sizeof(unsigned int*) *c);
            }
            for(j=0; j<b; j++){
                M[0][j][0]=1;
                M[0][j][1]=1;
            }
            /*printf("Created M of Array size 1\n");
            for(j=0; j<b; j++){
                printf("%d%d ", M[0][j][0], M[0][j][1]);
            }
            printf("\n");*/
        }
        else{
            M = malloc(sizeof(unsigned int*) *arr_size);
            for(i=0; i<arr_size; i++){
                M[i] = malloc(sizeof(unsigned int*) *b);
            }
            for(i=0; i<arr_size; i++){
                for(j=0; j<b; j++){
                    M[i][j] = malloc(sizeof(unsigned int*) *c);
                }
            }
            for(i=0; i<arr_size; i++){
                for(j=0; j<b; j++){
                    M[i][j][0]=1;
                    M[i][j][1]=1;
                }
            }
            //printf("Created M\n");
        }
        //printf("Flag %d \n", flag);
        if(flag==0){    //if cube list empty
            //do nothing, matrix elements already 11
        }
        else{
            if(all==1) {   //if cube list not empty and all 11
                for(j=0; j<b; j++){
                    M[0][j][0]=0;
                    M[0][j][1]=0;
                }
            }
            else{   //if cube list not empty and not all 11
                k=0;
                //printf("Cube list not empty\n");
                for(i=0; i<arr_size; i++){
                    //printf("Index %d temp[k] %d\n",index, temp[k]);
                    if(F[index][temp[k]][0]==0 && F[index][temp[k]][1]==1){
                        M[i][temp[k]][0]=1;
                        M[i][temp[k]][1]=0;
                    }
                    else{
                        M[i][temp[k]][0]=0;
                        M[i][temp[k]][1]=1;
                    }
                    k++;
                }
            }
            /*for(i=0; i<arr_size; i++){
                for(j=0; j<b; j++){
                    printf("%d%d ", M[i][j][0], M[i][j][1]);
                }
                printf("\n");
            }*/
        }
        if(arr_size==0) *arr_s=1;
        else *arr_s=arr_size;
        return(M);
    }
    else {
        printf("Not Simple \n");
        unsigned int a_sp, a_sn;
        unsigned int x = split(a,b,c,F);
        printf("Split Index %d \n", x);
        unsigned int F_copy[a][b][c];
        //printf("F before splitting\n");
        for(i=0; i<a; i++){ //create copy of F
            for(j=0; j<b; j++){
                F_copy[i][j][0]=F[i][j][0];
                F_copy[i][j][1]=F[i][j][1];
                //printf("%d%d ", F[i][j][0],F[i][j][1]);
            }
            //printf("\n");
        }
        PosCofactor(a,b,c,F,x);
        NegCofactor(a,b,c,F_copy,x);
        unsigned int ***P = complement(a,b,c,F,&a_sp);
        unsigned int ***N = complement(a,b,c,F_copy, &a_sn);
        /*printf("P complement \n");
        for(i=0;i<a_sp;i++){
            for(j=0;j<b;j++){
                printf("%d%d ", P[i][j][0], P[i][j][1]);
            }
            printf("\n");
        }
        printf("N complement \n");
        for(i=0;i<a_sn;i++){
            for(j=0;j<b;j++){
                printf("%d%d ", N[i][j][0], N[i][j][1]);
            }
            printf("\n");
        }*/
        unsigned int P_copy[a_sp][b][c];
        unsigned int N_copy[a_sn][b][c];
        for(i=0; i<a_sp; i++){ //create copy of P
            for(j=0; j<b; j++){
                P_copy[i][j][0]=P[i][j][0];
                P_copy[i][j][1]=P[i][j][1];
            }
        }
        for(i=0; i<a_sn; i++){ //create copy of N
            for(j=0; j<b; j++){
                N_copy[i][j][0]=N[i][j][0];
                N_copy[i][j][1]=N[i][j][1];
            }
        }
        //Free P
        free(P);
        //Free N
        free(N);

        andp(a_sp,b,c,P_copy,x);
        andn(a_sn,b,c,N_copy,x);
        *arr_s=a_sp+a_sn;
        return(or(a_sp,a_sn,b,c,P_copy,N_copy));
    }
}

unsigned int split(unsigned int a, unsigned int b, unsigned int c, unsigned int F[][b][c]){
    unsigned int unate[b][c];
    unsigned int dontcare[b];   //number of dont cares for resp var
    unsigned int binate[b];     //list of binate var (starting from 1)
    unsigned int binate_l[b];   //list of binate var with same number of dont cares
    int binate_tc[b];  //value of |T-C| for each var in binate_l list
    //Initialize arrays to 0
    int i, j;
    for(i=0; i<b; i++){
        dontcare[i] = 0;
        binate[i] = 0;
        binate_l[i] = 0;
        binate_tc[i] = -1;
        for(j=0; j<c; j++){
            unate[i][j] = 1;
        }
    }
    //Check unate-ness, if element = 00, that var is unate
    for(j=0; j<b; j++){
        for(i=0; i<a; i++){
            if(F[i][j][0]==0 && F[i][j][1]==0){ //do nothing
            }
            else{
                if(F[i][j][0]==1 && unate[j][0]==1) unate[j][0] = 1;
                else unate[j][0] = 0;
                if(F[i][j][1]==F[0][j][1] && unate[j][1]==1) unate[j][1] = 1;
                else unate[j][1] = 0;
            }
        }
    }
    //Check number of dont cares for each var
    for(j=0; j<b; j++){
        for(i=0; i<a; i++){
            if(F[i][j][0]==1 && F[i][j][1]==1) dontcare[j]++;
        }
    }
    //Check binate
    unsigned int x = 0;
    for(i=0; i<b; i++){
        if(unate[i][0]==0 && unate[i][1]==0) {
            binate[x]=i+1;
            x++;
        }
    }
    /*for(i=0; i<b; i++){
        printf("unate: %d%d ", unate[i][0], unate[i][1]);
    }
    printf("\n");
    for(i=0; i<b; i++){
        printf("dontcare: %d ", dontcare[i]);
    }
    printf("\n");

    for(i=0; i<b; i++){
        printf("binate: %d ", binate[i]);
    }
    printf("\n");*/
    unsigned int y=1;
    x=0;    //index
    if(binate[0]!=0){ //it is binate
        x = binate[0];
        //find the binate in most cubes
        while(binate[y]>0 && y<b){
            if(dontcare[binate[y]-1] < dontcare[x-1]) x=binate[y];
            y++;
            //printf("I ");
        }
        //printf("Most binate %d\n", x);
        y=0;
        unsigned int z=0;
        binate_l[z]=x;
        z++;
        while(binate[y]!=x) y++;
        y++;
        while(binate[y]>0 && y<b){
            if(dontcare[binate[y]-1] == dontcare[x-1]){
                binate_l[z]=binate[y];
                z++;
            }
            y++;
        }
        /*for(i=0; i<b; i++){
            printf("binate_l: %d ", binate_l[i]);
        }
        printf("\n");*/
        int Tf, Cf;
        if(binate_l[1]!=0){
            for(j=0; j<z; j++){
                Tf = 0;
                Cf = 0;
                for(i=0; i<a; i++){
                    if(F[i][binate_l[j]-1][0]==0 && F[i][binate_l[j]-1][1]==1) Tf++;
                    else if(F[i][binate_l[j]-1][0]==1 && F[i][binate_l[j]-1][1]==0) Cf++;
                }
                binate_tc[j] = abs(Tf-Cf);
            }
            /*for(i=0; i<b; i++){
                printf("binate_tc: %d ", binate_tc[i]);
            }
            printf("\n");*/
            y = binate_tc[0];
            for(j=1; j<z; j++){
                if(binate_tc[j]<y){
                    x=binate_l[j];
                }
            }
        }
        return(x);
    }
    else {  //if unate
       for(i=1; i<b; i++){
            if(dontcare[i]<dontcare[x]) x=i;
       }
       return(x+1);
    }
}

void PosCofactor(unsigned int a, unsigned int b, unsigned int c, unsigned int F[][b][c], unsigned int x){
    int i, j;
    for(i=0; i<a; i++){
            if(F[i][x-1][0]==0 && F[i][x-1][1]==1){
                F[i][x-1][0]=1;
                //printf("1 \n");
            }
            else if(F[i][x-1][0]==1 && F[i][x-1][1]==0){
                for(j=0; j<b; j++){
                    F[i][j][0]=0;
                    F[i][j][1]=0;
                }
                //printf("2 \n");
            }
            else{
                //do nothing
                //printf("3 \n");
            }
    }
}

void NegCofactor(unsigned int a, unsigned int b, unsigned int c, unsigned int F[][b][c], unsigned int x){
    int i, j;
    for(i=0; i<a; i++){
            if(F[i][x-1][0]==1 && F[i][x-1][1]==0){
                F[i][x-1][1]=1;
                //printf("1 \n");
            }
            else if(F[i][x-1][0]==0 && F[i][x-1][1]==1){
                for(j=0; j<b; j++){
                    F[i][j][0]=0;
                    F[i][j][1]=0;
                }
                //printf("2 \n");
            }
            else{
                //do nothing
                //printf("3 \n");
            }
    }
}

void andp(unsigned int a, unsigned int b, unsigned int c, unsigned int F[][b][c], unsigned int x){
    unsigned int flag=0;
    int i;
    for(i=0; i<a; i++){
        if(F[i][0][0]==1 || F[i][0][1]==1) flag=1;  //check if cube list isnt empty
        if(flag==1){    //if cube list not empty, insert x
            F[i][x-1][0]=0;
            F[i][x-1][1]=1;
        }
        flag=0;
    }

}

void andn(unsigned int a, unsigned int b, unsigned int c, unsigned int F[][b][c], unsigned int x){
    unsigned int flag=0;
    int i;
    for(i=0; i<a; i++){
        if(F[i][0][0]==1 || F[i][0][1]==1) flag=1;  //check if cube list isnt empty
        if(flag==1){    //if cube list not empty, insert !x
            F[i][x-1][0]=1;
            F[i][x-1][1]=0;
        }
        flag=0;
    }

}

unsigned int *** or(unsigned int ap, unsigned int an, unsigned int b, unsigned int c, unsigned int P[][b][c], unsigned int N[][b][c]){
    int i,j;
    unsigned int ***F;
    F = malloc(sizeof(unsigned int*) *(ap+an));

    for(i=0; i<(ap+an); i++){
        F[i] = malloc(sizeof(unsigned int*) *b);
    }
    for(i=0; i<(ap+an); i++){
        for(j=0; j<b; j++){
            F[i][j] = malloc(sizeof(unsigned int*) *c);
        }
    }

    for(i=0; i<(ap+an); i++){
        for(j=0; j<b; j++){
            if(i<ap){
                F[i][j][0] = P[i][j][0];
                F[i][j][1] = P[i][j][1];
            }
            else{
                F[i][j][0] = N[i-ap][j][0];
                F[i][j][1] = N[i-ap][j][1];
            }
        }
    }
    return(F);
}

