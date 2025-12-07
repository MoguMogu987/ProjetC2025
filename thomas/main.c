#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BASE 2

typedef struct{
    int *Tdigits;
    int Taille;
    int Signe;
} BigBinary;

BigBinary createBigBinary(int size){
    BigBinary bb;
    bb.Tdigits=malloc(sizeof(int)*size);
    bb.Taille=size;
    bb.Signe=0;
    return bb;
}

BigBinary initBigBinary (int taille,int signe){
    BigBinary nb;
    nb.Taille=taille;
    nb.Signe=signe;
    nb.Tdigits=malloc(sizeof(int)*taille);
    for (int i=0;i<taille;i++){
        nb.Tdigits[i]=0;
    }
    return nb;
}

void afficheBigBinary(BigBinary nb){
    if (nb.Signe==-1) printf("-") ;
    if (nb.Signe==0||nb.Taille==0) {
        printf("0\n");
        return;
    }
    for (int i=0;i < nb.Taille;i++){
        printf("%d",nb.Tdigits[i]);
    }
    printf("\n");
}

void libereBigBinary(BigBinary *nb){
    free(nb->Tdigits);
    nb->Tdigits=NULL;
    nb->Taille=0;
    nb->Signe=0;
}

BigBinary setBB(int a){
    if (a==0) return initBigBinary(1,0);
    int temp=abs(a);
    int t=0;
    while (temp!=0) {
        t+=1;
        temp/=2;
    }

    BigBinary A = createBigBinary(t);
    if (a == 0) {
        return A;
    }
    if (a < 0) {
        A.Signe = -1;
    } else {
        A.Signe = 1;
    }

    temp = abs(a);
    int i = A.Taille;
    while (i > 0) {
        A.Tdigits[--t] = temp%2;
        temp /= 2;
        i--;
    }
    return A;
}

void normalizeBB(BigBinary *bb){
    if (bb->Taille==0){
        bb->Signe=0;
        return;
    }
    int index=-1;
    for (int i=0;i<bb->Taille;i++){
        if (bb->Tdigits[i]==1){
            index=i;
            break;
        }
    }
    if (index == -1) {
        if (bb->Tdigits != NULL) free(bb->Tdigits);
        bb->Tdigits = (int *)malloc(sizeof(int));
        bb->Tdigits[0] = 0;
        bb->Taille = 1;
        bb->Signe = 0;
    } else if (index > 0) {
        int new_taille = bb->Taille - index;
        int *new_digits = (int *)malloc(sizeof(int) * new_taille);
        if (new_digits == NULL) {
             fprintf(stderr, "Erreur d'allocation mémoire.\n");
             exit(EXIT_FAILURE);
        }

        for (int i = 0; i < new_taille; ++i) {
            new_digits[i] = bb->Tdigits[index + i];
        }
        free(bb->Tdigits);
        bb->Tdigits = new_digits;
        bb->Taille = new_taille;
        if (bb->Signe == 0) {
            bb->Signe = 1;
        }
    }
}

BigBinary copieBigBinary(BigBinary src) {
    BigBinary res = initBigBinary(src.Taille, src.Signe);
    if (src.Taille > 0) {
        memcpy(res.Tdigits, src.Tdigits, src.Taille * sizeof(int));
    }
    return res;
}

BigBinary BB_Add(BigBinary a, BigBinary b) {//addition normale
    int max_len;
    if (a.Taille > b.Taille) max_len=a.Taille;
    else max_len= b.Taille;
    int res_len = max_len + 1;
    BigBinary res = initBigBinary(res_len, 1); 
    int r = 0; //retenu

    for (int i = 1; i <= max_len; ++i) {
        int a_bit,b_bit;
        if (i <= a.Taille) a_bit= a.Tdigits[a.Taille - i];
        else a_bit = 0;
        if (i <= b.Taille) b_bit= b.Tdigits[b.Taille - i];
        else b_bit = 0;
        int sum = a_bit + b_bit + r;
        res.Tdigits[res_len - i] = sum % BASE;
        r = sum / BASE;
    }
    res.Tdigits[0] = r;
    normalizeBB(&res);
    if (res.Signe != 0) res.Signe = 1;
    return res;
}

bool Inferieur_Taille(BigBinary A, BigBinary B) { //compare la taille est pas la valeur
    if (A.Taille < B.Taille) return true;
    if (A.Taille > B.Taille) return false;

    for (int i = 0; i < A.Taille; ++i) {
        if (A.Tdigits[i] < B.Tdigits[i]) return true;
        if (A.Tdigits[i] > B.Tdigits[i]) return false;
    }
    return false;
}

BigBinary BB_Add2(BigBinary a, BigBinary b) { //addition pour les signe diff
    if (Inferieur_Taille(a, b)) return initBigBinary(1, 0); 
    
    int max_len = a.Taille;
    BigBinary res = initBigBinary(max_len, 1);
    int r = 0; 
    bool is_null=true;

    for (int i = 1; i <= max_len; ++i) {
        int a_bit = a.Tdigits[a.Taille - i];
        int b_bit;
        if (i <= b.Taille) b_bit = b.Tdigits[b.Taille - i];
        else b_bit = 0;
        int diff = a_bit-b_bit-r;
        if (diff < 0) {
            res.Tdigits[max_len-i] = diff+BASE;
            r = 1;
        } else {
            res.Tdigits[max_len-i] = diff;
            r = 0;
        }
        if (res.Tdigits[max_len-i]==1) is_null=false;
    }
    if(is_null){
        libereBigBinary(&res);
        return initBigBinary(1,0);
    }
    normalizeBB(&res);
    return res;
}

BigBinary Addition(BigBinary a, BigBinary b) {
    if (a.Signe == b.Signe) { //meme signe
        BigBinary res = BB_Add(a, b);
        res.Signe = a.Signe;
        return res;
    }
    BigBinary res;
    int sign_result;

    if (Inferieur_Taille(a, b)) { //signe diff
        res = BB_Add2(b, a);
        sign_result = b.Signe;
    } else {
        res = BB_Add2(a, b);
        sign_result = a.Signe;
    }
    
    if (res.Signe == 0) return res; 
    else {
        res.Signe = sign_result;
        return res;
    }
}

BigBinary Soustraction(BigBinary a, BigBinary b) {// A - B == A + (-B)
    BigBinary B = copieBigBinary(b);
    if (B.Signe != 0) B.Signe *= -1;
    BigBinary res = Addition(a,B);
    libereBigBinary(&B);
    return res;
}

bool Egal(BigBinary a,BigBinary b){
    if ((a.Signe!=b.Signe||a.Taille!=b.Taille))return false;
    for (int i=0;i<a.Taille;i++){
        if(a.Tdigits[i]!=b.Tdigits[i])return false;
    }
    return true;
}

bool Inferieur(BigBinary a,BigBinary b){
    if (a.Signe!=b.Signe) return a.Signe<b.Signe;
    if (a.Signe==0)return false;
    if (a.Signe==1) return Inferieur_Taille(a,b);
    return Inferieur_Taille(b,a);
}

BigBinary Multiplication(BigBinary a,int n){ //tres lent ne pas utiliser plutot utiliser Shift surtout pour les *2
    BigBinary res = initBigBinary(a.Taille,a.Signe);
    for (int i=0;i<n;i++) {
        BigBinary tmp=res;
        res=Addition(tmp,a);
        libereBigBinary(&tmp);
    }
    return res;
}

//renvoie des copie car a créer des probleme pdt les test
BigBinary minBB(BigBinary a, BigBinary b){
    if (Inferieur(a,b)) return copieBigBinary(a);
    return copieBigBinary(b);
}

BigBinary maxBB(BigBinary a,BigBinary b){
    if (Inferieur(a,b))return copieBigBinary(b);
    return copieBigBinary(a);
}

BigBinary PGCD(BigBinary a,BigBinary b){
    if (a.Signe<=0||b.Signe<=0) return initBigBinary(1,0);
    BigBinary u=copieBigBinary(a);
    BigBinary v=copieBigBinary(b);
    while (!Egal(u,v)){
        BigBinary res;
        if(Inferieur(v,u)){
            res = Soustraction(u,v);
            libereBigBinary(&u);
            u=res;
        }
        else {
            res = Soustraction(v,u);
            libereBigBinary(&v);
            v=res;
        }
    }
    libereBigBinary(&v);
    return u;
}

int puissance(int base,int e){// pas pour les BB
    int res=1;
    for (int i=0;i<e;i++) res*=base;
    return res;
}

int BBtoInt(BigBinary a){
    if (a.Signe==0) return 0;
    int res=0;
    for (int i=a.Taille-1;i>=0;i--){
        if (a.Tdigits[i]==1) res+=puissance(2,a.Taille-i-1);
    }
    if (a.Signe==1) return res;
    return -res;
}

BigBinary Shift(BigBinary a){//décalle vers la gauche soit *2 en binaire
    if (a.Signe==0)return copieBigBinary(a);

    BigBinary res = initBigBinary(a.Taille+1,a.Signe);
    for (int i=0;i<a.Taille;i++) res.Tdigits[i]=a.Tdigits[i];
    res.Tdigits[a.Taille]=0;
    return res;
}

BigBinary Modulo(BigBinary a,BigBinary b){
    //a et b positif
    if (a.Signe <= 0|| b.Signe<=0)return initBigBinary(1,0);
    BigBinary res = copieBigBinary(a);
    BigBinary next;
    while (Inferieur_Taille(b,res)||Egal(res,b)){
        BigBinary tmp = copieBigBinary(b);
        BigBinary next;
        while (true) {
            next = Shift(tmp);
            if (Inferieur(next, res) || Egal(next, res)) {
                libereBigBinary(&tmp);
                tmp = next;
            } else {
                libereBigBinary(&next);
                break;
            }
        }
        BigBinary tmp2 = res;
        res = Soustraction(tmp2,tmp); // res = res-b*2^k
        libereBigBinary(&tmp2);
        libereBigBinary(&tmp);
    }
    normalizeBB(&res);
    return res;
}

BigBinary MultiplicationEgyptienne(BigBinary a,BigBinary b){//marche pas utilliser BigBinary_Egypt
    int signe;
    if (a.Signe==0||b.Signe==0) signe=0;
    else signe = a.Signe*b.Signe;
    BigBinary A= copieBigBinary(a);
    BigBinary B = copieBigBinary(b);
    A.Signe=1;
    B.Signe=1;
    BigBinary somme = initBigBinary(A.Taille+B.Taille,0);
    BigBinary tmp = copieBigBinary(A);
    for (int i=B.Taille-1;i>=0;i--){
        if (B.Tdigits[i]==1){
            BigBinary tmp2 = somme;
            somme = Addition(tmp2,tmp);
            libereBigBinary(&tmp2);
        }
        BigBinary tmp3 = copieBigBinary(tmp);
        //tmp = Multiplication(tmp3,2);
        tmp=Shift(tmp3);
        libereBigBinary(&tmp3);
    }
    libereBigBinary(&tmp);
    libereBigBinary(&A);
    libereBigBinary(&B);
    somme.Signe=signe;
    return somme;
}

BigBinary BigBinary_Egypt(BigBinary A, BigBinary B) {
    BigBinary Somme = initBigBinary(1,0);
    BigBinary tempM = copieBigBinary(A);                        // Valeur locale de A
    if (A.Signe == 0 || B.Signe == 0) {
        libereBigBinary(&tempM);
        return Somme;
    }
    for (int i = B.Taille-1; i>=0; i--) {
        if (B.Tdigits[i] == 1) {
            BigBinary tmpSomme = Somme;
            Somme = Addition(tmpSomme, tempM);
            libereBigBinary(&tmpSomme);
        }
        BigBinary old_tempM = tempM;
        BigBinary tmp2 = Shift(old_tempM);
        libereBigBinary(&old_tempM);
        tempM=tmp2;
        
    }
    libereBigBinary(&tempM);
    Somme.Signe=A.Signe*B.Signe;
    return Somme;
}

BigBinary BBexpMod(BigBinary M,int E,BigBinary N){
    BigBinary Base = Modulo(M,N);
    BigBinary res=setBB(1);
    if (E==0) return res;
    while (E>0){
        if (E%2==1){
            BigBinary old_res = res;
            BigBinary tmp = BigBinary_Egypt(old_res,Base);
            BigBinary tmp2=Modulo(tmp,N);
            res=tmp2;
            libereBigBinary(&old_res);
            libereBigBinary(&tmp);
            //libereBigBinary(&tmp2);
        }
        if (E>1){ //changement de la base car on passe au bit suivant
            BigBinary tmp =  copieBigBinary(Base);
            BigBinary tmp2 = BigBinary_Egypt(tmp,tmp);
            BigBinary old_Base = Base;
            Base=Modulo(tmp2,N);
            libereBigBinary(&old_Base);
            libereBigBinary(&tmp);
            libereBigBinary(&tmp2);
        }
        E=E/2;
    }
    libereBigBinary(&Base);
    return res;
}

BigBinary RSA_Encrypt(BigBinary M, int E, BigBinary N){
    return BBexpMod(M,E,N);
}

BigBinary RSA_Decrypt(BigBinary C, int D, BigBinary N){
    return BBexpMod(C,D,N);
}

//fonction de test
void testAfficheBigBinary(){
    BigBinary A = setBB(83);
    BigBinary B = setBB(57);
    BigBinary C = setBB(26);
    BigBinary D = setBB(83);
    BigBinary Z = setBB(0);
    BigBinary E = setBB(140);
    BigBinary N = setBB(-13);
    BigBinary M = setBB(100);

    printf("A (83) : "); printf("(1010011)" );afficheBigBinary(A);
    printf("B (57) : "); printf("(111001)" );afficheBigBinary(B);
    printf("C (26) : "); printf("(11010)" );afficheBigBinary(C);
    printf("D (83) : "); printf("(1010011)" );afficheBigBinary(D);
    printf("Z (0) : "); printf("(0)" );afficheBigBinary(Z);
    printf("E (140) : "); printf("(10001100)" );afficheBigBinary(E);
    printf("N (-13) : "); printf("(-1101)" );afficheBigBinary(N);
    printf("M (100) : "); printf("(1100100)" );afficheBigBinary(M);

    libereBigBinary(&A);
    libereBigBinary(&B);
    libereBigBinary(&C);
    libereBigBinary(&D);
    libereBigBinary(&N);
    libereBigBinary(&M);
    libereBigBinary(&Z);
    libereBigBinary(&E);
}

void testEgalInf(){
    BigBinary A = setBB(83);
    BigBinary B = setBB(57);
    BigBinary C = setBB(26);
    BigBinary D = setBB(83);
    BigBinary Z = setBB(0);
    BigBinary E = setBB(140);
    BigBinary N = setBB(-13);
    BigBinary M = setBB(100);

    printf("Egal(A, D) (83 == 83) : %s\n", Egal(A, D) ? "true" : "false"); // true
    printf("Egal(A, B) (83 == 57) : %s\n", Egal(A, B) ? "true" : "false"); // false
    printf("Inferieur(B, A) (57 < 83) : %s\n", Inferieur(B, A) ? "true" : "false"); // true
    printf("Inferieur(A, D) (83 < 83) : %s\n", Inferieur(A, D) ? "true" : "false"); // false
    printf("Inferieur(Z, A) (0 < 83) : %s\n", Inferieur(Z, A) ? "true" : "false"); // true
    printf("Inferieur(N, A) (-13 < 83) : %s\n", Inferieur(N, A) ? "true" : "false"); // true

    libereBigBinary(&A);
    libereBigBinary(&B);
    libereBigBinary(&C);
    libereBigBinary(&D);
    libereBigBinary(&N);
    libereBigBinary(&M);
    libereBigBinary(&Z);
    libereBigBinary(&E);
}

void testAddSub(){
    BigBinary A = setBB(83);
    BigBinary B = setBB(57);
    BigBinary C = setBB(26);
    BigBinary D = setBB(83);
    BigBinary Z = setBB(0);
    BigBinary E = setBB(140);
    BigBinary N = setBB(-13);
    BigBinary M = setBB(100);

    BigBinary Sum = Addition(A, B);
    printf("A + B (83 + 57 = 140): "); printf("(10001100) "); afficheBigBinary(Sum);
    libereBigBinary(&Sum);

    BigBinary Sum2 = Addition(A, D);
    printf("A + D (83 + 83 = 166): "); printf("(10100110) ");afficheBigBinary(Sum2);
    libereBigBinary(&Sum2);

    BigBinary Diff = Soustraction(A, B);
    printf("A - B (83 - 57 = 26): ");printf("(11010) ");afficheBigBinary(Diff);
    libereBigBinary(&Diff);

    BigBinary Diff2 = Soustraction(A, D);
    printf("A - D (83 - 83 = 0): ");printf("(0) "); afficheBigBinary(Diff2); // 0
    libereBigBinary(&Diff2);

    libereBigBinary(&A);
    libereBigBinary(&B);
    libereBigBinary(&C);
    libereBigBinary(&D);
    libereBigBinary(&N);
    libereBigBinary(&M);
    libereBigBinary(&Z);
    libereBigBinary(&E);
}

void testMinMax(){
    BigBinary A = setBB(83);
    BigBinary B = setBB(57);
    BigBinary C = setBB(26);
    BigBinary D = setBB(83);
    BigBinary Z = setBB(0);
    BigBinary E = setBB(140);
    BigBinary N = setBB(-13);
    BigBinary M = setBB(100);

    BigBinary min_ab = minBB(A, B);
    BigBinary max_ab = maxBB(A, B);
    BigBinary min_an = minBB(A, N); // min(83, -13) = -13
    
    printf("min(A, B) (min(83, 57) = 57): "); afficheBigBinary(min_ab);
    printf("max(A, B) (max(83, 57) = 83): "); afficheBigBinary(max_ab);
    printf("min(A, N) (min(83, -13) = -13): "); afficheBigBinary(min_an);
    
    libereBigBinary(&min_ab);
    libereBigBinary(&max_ab);
    libereBigBinary(&min_an);

    libereBigBinary(&A);
    libereBigBinary(&B);
    libereBigBinary(&C);
    libereBigBinary(&D);
    libereBigBinary(&N);
    libereBigBinary(&M);
    libereBigBinary(&Z);
    libereBigBinary(&E);
}

void testPGCD(){
    BigBinary A = setBB(83);
    BigBinary B = setBB(57);
    BigBinary C = setBB(26);
    BigBinary D = setBB(83);
    BigBinary Z = setBB(0);
    BigBinary E = setBB(140);
    BigBinary N = setBB(-13);
    BigBinary M = setBB(100);

    // PGCD(83, 57) = 1
    BigBinary pgcd_ab = PGCD(A, B);
    printf("PGCD(A, B) (PGCD(83, 57) = 1): "); afficheBigBinary(pgcd_ab); 
    libereBigBinary(&pgcd_ab);

    // PGCD(100, 26) = 2
    BigBinary pgcd_mc = PGCD(M, C);
    printf("PGCD(M, C) (PGCD(100, 26) = 2): "); afficheBigBinary(pgcd_mc); // c'est normal le résultat est en binaire
    libereBigBinary(&pgcd_mc);
    
    // PGCD(140, 26) = 1
    BigBinary pgcd_dc = PGCD(D, C);
    printf("PGCD(D, C) (PGCD(83, 26) = 1): "); afficheBigBinary(pgcd_dc);
    libereBigBinary(&pgcd_dc);

    libereBigBinary(&A);
    libereBigBinary(&B);
    libereBigBinary(&C);
    libereBigBinary(&D);
    libereBigBinary(&N);
    libereBigBinary(&M);
    libereBigBinary(&Z);
    libereBigBinary(&E);
}

void testBBtoInt(){
    BigBinary A = setBB(83);
    BigBinary B = setBB(57);
    BigBinary C = setBB(26);
    BigBinary D = setBB(83);
    BigBinary Z = setBB(0);
    BigBinary E = setBB(140);
    BigBinary N = setBB(-13);
    BigBinary M = setBB(100);

     int int_a = BBtoInt(A);
    int int_b = BBtoInt(B);
    int int_n = BBtoInt(N);
    int int_z = BBtoInt(Z);

    printf("BBtoInt(A) (83) : %d\n", int_a);
    printf("BBtoInt(B) (57) : %d\n", int_b);
    printf("BBtoInt(N) (-13) : %d\n", int_n);
    printf("BBtoInt(Z) (0) : %d\n", int_z);

    libereBigBinary(&A);
    libereBigBinary(&B);
    libereBigBinary(&C);
    libereBigBinary(&D);
    libereBigBinary(&N);
    libereBigBinary(&M);
    libereBigBinary(&Z);
    libereBigBinary(&E);
}

void testModulo(){
    BigBinary A = setBB(83);
    BigBinary B = setBB(57);
    BigBinary C = setBB(26);
    BigBinary D = setBB(83);
    BigBinary Z = setBB(0);
    BigBinary E = setBB(140);
    BigBinary N = setBB(-13);
    BigBinary M = setBB(100);

    // Modulo(83, 57) = 26
    BigBinary mod_ab = Modulo(A, B);
    printf("Modulo(A, B) (83 %% 57 = 26): "); afficheBigBinary(mod_ab);
    libereBigBinary(&mod_ab);
    
    // Modulo(100, 26) = 22
    BigBinary mod_mc = Modulo(M, C);
    printf("Modulo(M, C) (100 %% 26 = 22): "); afficheBigBinary(mod_mc);
    libereBigBinary(&mod_mc);

    // Modulo(26, 83) = 26
    BigBinary mod_ca = Modulo(C, A);
    printf("Modulo(C, A) (26 %% 83 = 26): "); afficheBigBinary(mod_ca);
    libereBigBinary(&mod_ca);

    libereBigBinary(&A);
    libereBigBinary(&B);
    libereBigBinary(&C);
    libereBigBinary(&D);
    libereBigBinary(&N);
    libereBigBinary(&M);
    libereBigBinary(&Z);
    libereBigBinary(&E);
}

void testEgypt(){
    BigBinary A = setBB(83);
    BigBinary B = setBB(57);
    BigBinary C = setBB(26);
    BigBinary D = setBB(83);
    BigBinary Z = setBB(0);
    BigBinary E = setBB(140);
    BigBinary N = setBB(-13);
    BigBinary M = setBB(100);

    // 5 * 13 = 65
    BigBinary T13 = setBB(13);
    BigBinary T5 = setBB(5);
    //BigBinary mult_13_5 = MultiplicationEgyptienne(T13, T5);
    BigBinary mult_13_5 = BigBinary_Egypt(T13, T5);
    printf("13 * 5 = 65: "); afficheBigBinary(mult_13_5);
    libereBigBinary(&mult_13_5);
    
    // 83 * 57 = 4731
    //BigBinary mult_ab = MultiplicationEgyptienne(A, B);
    BigBinary mult_ab = BigBinary_Egypt(A, B);
    printf("A * B (83 * 57 = 4731): "); afficheBigBinary(mult_ab);
    libereBigBinary(&mult_ab);

    // -13 * 5 = -65
    //BigBinary mult_n_5 = MultiplicationEgyptienne(N, T5);
    BigBinary mult_n_5 = BigBinary_Egypt(N, T5);
    printf("N * 5 (-13 * 5 = -65): "); afficheBigBinary(mult_n_5);
    libereBigBinary(&mult_n_5);

    libereBigBinary(&T13);
    libereBigBinary(&T5);
    libereBigBinary(&A);
    libereBigBinary(&B);
    libereBigBinary(&C);
    libereBigBinary(&D);
    libereBigBinary(&N);
    libereBigBinary(&M);
    libereBigBinary(&Z);
    libereBigBinary(&E);
}

void testRSA(){
    BigBinary N = setBB(323); 
    int e = 17; //e = d il faut qu'il ait la meme clé de décriptage
    int d = 17;

    BigBinary M = setBB(123); 
    printf("Message M (123): "); afficheBigBinary(M);
    
    BigBinary C = RSA_Encrypt(M, e, N);
    printf("Chiffre C (M^e mod N): "); afficheBigBinary(C);

    BigBinary M_prime = RSA_Decrypt(C, d, N);
    printf("Dechiffre M' (C^d mod N): "); afficheBigBinary(M_prime);

    printf("Chiffrement/Déchiffrement réussi : %s\n", Egal(M, M_prime) ? "true" : "false");

    libereBigBinary(&N);
    libereBigBinary(&M);
    libereBigBinary(&C);
    libereBigBinary(&M_prime);
}

int main () {

    //testAfficheBigBinary();
    //testEgalInf();
    //testAddSub();
    //testMinMax();
    //testPGCD();
    //testBBtoInt();
    //testModulo();
    testEgypt();
    testRSA();

    return 0;
}