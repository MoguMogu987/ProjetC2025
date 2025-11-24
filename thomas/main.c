#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BASE 2

typedef struct {
    int * Tdigits ; // Tableau de bits : Tdigits [0] = bit de poids fort ,Tdigits [ Taille - 1] = bit de poids faible
    int Taille ; // Nombre de bits significatifs
    int Signe ; // +1 pour positif , -1 pour n é gatif , 0 pour nul
} BigBinary ;

BigBinary createBigBinary ( int size ) {
    BigBinary bb ;
    bb.Tdigits = malloc ( sizeof (int) * size ) ;
    bb.Taille = size ;
    bb.Signe = 0; // Par d é faut nul
    return bb ;
}

BigBinary initBigBinary ( int taille , int signe ) {
    BigBinary nb ;
    nb.Taille = taille ;
    nb.Signe = signe ;
    nb.Tdigits = malloc ( sizeof (int) * taille ) ;
    for (int i = 0; i < taille ; ++ i ) {
        nb . Tdigits [ i ] = 0;
    }
    return nb;
}

void afficheBigBinary ( BigBinary nb ) {
    if ( nb.Signe == -1) printf ( " -" ) ;
    if ( nb.Signe == 0 || nb . Taille == 0) {
        printf ( " 0\n " ) ;
        return ;
    }
    for (int i = 0; i < nb . Taille ; ++ i ) {
        printf ( " % d " , nb . Tdigits [ i ]) ;
    }
    printf ( " \n " ) ;
}

void libereBigBinary ( BigBinary * nb ) {
    free ( nb -> Tdigits ) ;
    nb -> Tdigits = NULL ;
    nb -> Taille = 0;
    nb -> Signe = 0;
}

BigBinary setBB(int liste[]){
    int taille = sizeof(liste)/sizeof(liste[0]) ;

    BigBinary nb = initBigBinary(taille ,+1);
    for (int i = 0; i < taille ; ++ i ) {
    nb.Tdigits[i] = liste[i];
    }
    return nb;
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

BigBinary Addition(BigBinary a,BigBinary b){
    int taille;
    if(a.Taille>b.Taille)taille=a.Taille;
    else taille=b.Taille;
    BigBinary res=createBigBinary(taille+1);
    int r=0; //retenu

    for (int i=taille;i>=0;i--){
        if (a.Tdigits[i]==0 && b.Tdigits[i]==0){
            res.Tdigits[i]=r;
            r=0;
        } else if (a.Tdigits[i]==1 && b.Tdigits[i]==1){
            res.Tdigits[i]=r;
            r=1;
        } else if (a.Tdigits[i]==1 || b.Tdigits[i]==1){
            if(r==1){
                res.Tdigits[i]=0;
            } else {
                res.Tdigits[i]=1;
            }
        } else res.Tdigits[i]=0;
    }
    if (res.Tdigits[0]>0) res.Signe=1;
    else if (res.Tdigits[0]<0) res.Signe=-1;
    else res.Signe=0;

    normalizeBB(&res);
    return res;
}

BigBinary Soustraction(BigBinary a,BigBinary b){
    if(b.Signe!=0 && b.Tdigits[0]==1) b.Tdigits[0]=0;
    else if(b.Signe!=0 && b.Tdigits[0]==0) b.Tdigits[0]=1;
    b.Signe==b.Signe*-1;
    return Addition(a,b);// a-b == a+(-b)
}

bool Egal(BigBinary a,BigBinary b){
    if ((a.Signe!=b.Signe)&&(a.Taille!=b.Taille))return false;
    for (int i=0;i<a.Taille;i++){
        if(a.Tdigits[i]!=b.Tdigits[i])return false;
    }
    return true;
}

bool Inferieur(BigBinary a,BigBinary b){
    if (a.Signe!=b.Signe||((a.Signe==0)&&(b.Signe==0))) return a.Signe<b.Signe;
    if (a.Taille!=b.Taille) return a.Taille<b.Taille; //il faut normaliser avant
    for (int i=1; i<a.Taille;i++){
        if(a.Tdigits[i]!=b.Tdigits[i]) return a.Tdigits!=1; //si a=1 alors b=0 donc a>b
    }
    return false;//==
}

BigBinary Multiplication(BigBinary a,int n){
    BigBinary res = initBigBinary(a.Taille,a.Signe);
    for (int i=0;i<n;i++) res=Addition(res,a);
    return res;
}

BigBinary minBB(BigBinary a, BigBinary b){
    if (Inferieur(a,b)) return a;
    return b;
}

BigBinary maxBB(BigBinary a,BigBinary b){
    if (Inferieur(a,b))return b;
    return a;
}

BigBinary PGCD(BigBinary a,BigBinary b){
    BigBinary u=maxBB(a,b);
    BigBinary v=minBB(a,b);
    while (!Egal(u,v)){
        if(Inferieur(v,u))u=Soustraction(u,v);
        else v=Soustraction(v,u);
    }
    return u;
}

int puissance(int a,int b){// pas pour les BB
    for (int i=0;i<b;i++) a+=a;
    return a;
}

int BBtoInt(BigBinary a){
    int res=0;
    for (int i=a.Taille-1;i>0;i--){
        if(a.Tdigits[i]==1)res+=puissance(2,a.Taille-i);
    }
    return res;
}

BigBinary Modulo(BigBinary a,BigBinary b){
    //a et b positif
    if (Egal(a,b)) return initBigBinary(0,0);
    if (!Inferieur(a,b))return Modulo(Soustraction(a,b),b);
    BigBinary p = PGCD(a,b);
    if (!Inferieur(a,Multiplication(b,2)))return Modulo(Soustraction(a,Multiplication(b,2)),b);
    if (!Inferieur(a,Multiplication(b,3)))return Modulo(Soustraction(a,Multiplication(b,3)),b);
    if (Inferieur(a,b))return a;
}

BigBinary MultiplicationEgyptienne(BigBinary a,BigBinary b){
    BigBinary somme = initBigBinary(a.Taille,0);
    BigBinary tmp = a;
    for (int i=a.Taille-1;i>0;i++){
        if (b.Tdigits[i]==1){
            Addition(somme,tmp);
        }
        Multiplication(tmp,2);
    }
    return somme;
}

int main () {
    // Création des BigBinary à partir de chaînes binaires
    // 83 = 1010011
    int a[]={1,0,1,0,0,1,1};
    BigBinary A = setBB(a);
    // 57 = 111001
    int b[]={1,1,1,0,0,1};
    BigBinary B = setBB(b);;
    // 26 = 11010
    int c[]={1,1,0,1,0};
    BigBinary C = setBB(c);;
    // 83 = 1010011
    int d[]={1,0,1,0,0,1,1};
    BigBinary D = setBB(d);;
    // 0
    int z[]={0};
    BigBinary Z = setBB(z);;
    // 140 = 10001100
    int e[]={1,0,0,0,1,1,0,0};
    BigBinary E = setBB(e);;

    printf("--- Phase 1: Opérations de Base ---\n\n");
    
    // 1. Tests de Comparaison
    printf("1. Tests de Comparaison:\n");
    printf("A (83) : "); afficheBigBinary(A);
    printf("B (57) : "); afficheBigBinary(B);
    printf("C (26) : "); afficheBigBinary(C);
    printf("D (83) : "); afficheBigBinary(D);

    printf("Egal(A, D) (83 == 83) : %s\n", Egal(A, D) ? "true" : "false"); // true
    printf("Egal(A, B) (83 == 57) : %s\n", Egal(A, B) ? "true" : "false"); // false
    printf("Inferieur(B, A) (57 < 83) : %s\n", Inferieur(B, A) ? "true" : "false"); // true
    printf("Inferieur(A, D) (83 < 83) : %s\n", Inferieur(A, D) ? "true" : "false"); // false
    printf("Inferieur(Z, A) (0 < 83) : %s\n", Inferieur(Z, A) ? "true" : "false"); // true
    
    // 2. Test d'Addition (A + B)
    printf("\n2. Test d'Addition (A + B):\n");
    // A (83) + B (57) = 140 (10001100)
    BigBinary Sum = Addition(A, B);
    printf("A + B (83 + 57 = 140): "); afficheBigBinary(Sum); // 10001100
    libereBigBinary(&Sum);

    // Test d'Addition avec retenue
    // A (83) + D (83) = 166 (10100110)
    BigBinary Sum2 = Addition(A, D);
    printf("A + D (83 + 83 = 166): "); afficheBigBinary(Sum2); // 10100110
    libereBigBinary(&Sum2);

    // 3. Test de Soustraction (A - B)
    printf("\n3. Test de Soustraction (A - B):\n");
    // A (83) - B (57) = 26 (11010)
    BigBinary Diff = Soustraction(A, B);
    printf("A - B (83 - 57 = 26): "); afficheBigBinary(Diff); // 11010
    libereBigBinary(&Diff);

    // Test de Soustraction (A - A)
    BigBinary Diff2 = Soustraction(A, D);
    printf("A - D (83 - 83 = 0): "); afficheBigBinary(Diff2); // 0
    libereBigBinary(&Diff2);

    // Libération de la mémoire
    libereBigBinary(&A);
    libereBigBinary(&B);
    libereBigBinary(&C);
    libereBigBinary(&D);
    libereBigBinary(&Z);
    libereBigBinary(&E);

    return 0;
}
