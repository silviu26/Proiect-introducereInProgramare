#include <iostream>
#include <graphics.h>
#include <cstring>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>


#define raza 15 // modifica la trapez si la start/stop, dreptunghi|dreptunghi-inaltime,lungime|trapez-inaltime,baza mare|triunghi-inaltimea,baza|start/stop-arc,inaltime
#define in_plus 6 // mereu lungimea
#define ecran_W 970
#define ecran_H 600
bool exista_start=false;
unsigned int indx=0;
unsigned int i_1=0;
unsigned int i_2=0;
unsigned int i_3=0;
unsigned int i_s=0;
unsigned int i_d=0;
bool avem_xy=false;
const char* Forma_format_iesire= "(%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %s )\n";
const char* Forma_format_intrare="(%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %s )";

int top1,top2; // varfurile celor doua stive
const int max_stiva=100;

const double pi=3.1415926;
double Opd[max_stiva]; // stiva operanzilor
char Op[max_stiva]; // stiva operatorilor

char OperatiiBinare[200]="+-*/^<>=![]";
char OperatiiUnare[200]="scareltg";
char Operatii[200]="+-*/^<>=![]scareltg";

//dimensiune ecran ppartial
#define incepe_ecran_x 150
#define sfarsit_ecran_x 720
#define incepe_ecran_y 20
#define sfarsit_ecran_y 500

#define infinit INT_MAX
#define epsi 0.0001
#define  MAX 100
#define MAX1 20

 using namespace std;

 struct variabile
{
    char alph[27];
    double vl[27];
    int declar[27];
}var;

struct functie
{
    char expresie[300];
    char vect[MAX+1][MAX1];
    // vectorul cuprinzand �cuvintele�ce formeaza expresia}
    int lung; // lungimea efectiva a vectorului
    double a,b; // intervalul de evaluare a functiei
    int n; //numarul de puncte de evaluare
};

struct forma
{
    int x,y;
    int lungime; //lungimea textului in pixeli
    unsigned int denumire; //1-start, 2-stop, 3-if, 4-citire, 5-scriere, 6-calcul
    char text[21];
    int lungime_obiect_pe_2;
    int inaltime_obiect_pe_2;
    int salvare_x;
    int salvare_y;
    unsigned int index;
    unsigned int c1=0;
    unsigned int c2=0;
    bool miscare=true;
   // unsigned int intrare_1[100]={0};
    unsigned int intrare_2=0;

    bool vizitat=false;
    unsigned int pt_if_cod[100]={0};

    forma* urm;//toate formrle, aici salvez oridinea in care sunt adaugate

};

struct legatura
{
    unsigned int tip;
    unsigned int index;
    legatura *urm;
    legatura *urmst, *urmdr;
    char text[21];

}*leg=new legatura;

struct lista_index
{
    unsigned int index;
    lista_index* urm;
    bool vizitat=false;
    bool w=false;

}*prim_index=new lista_index;

string to_String(int n)
{
    int m = n;
    char s[21];
    char ss[21];
    int i=0,j=0;
    if (n==0) {ss[0]='0'; j=1;}
    if (n < 0)// Handling negative numbers
    {
        m = 0 - m;
        j = 1;
        ss[0] = '-';
    }
    while (m>0)
    {
        s[i++] = m % 10 + '0';
        m /= 10;
    }
    s[i] = '\0';
    i = i - 1;
    while (i >= 0)
    {
        ss[j++] = s[i--];
    }
    ss[j] = '\0';
    return ss;
}

int Prioritate(char c)  //Bianca
{
    if(c=='(' || c==')')
        return 0;
    if(c=='=' || c=='!' || c=='<' || c=='>' || c=='[' || c==']')
        return 1;
    if(c=='+' || c=='-')
        return 2;
    if(c=='*' || c=='/')
        return 3;
    if(c=='^')
        return 4;
    if(c=='c' || c=='s' || c=='l' || c=='e' || c=='t' || c=='a' || c=='r' || c=='t' || c=='g')
        return 5;
    return 6;
}

bool DifInf(double x) //Bianca
{
    return fabs(infinit-fabs(x)) > infinit / 2.0;
}

double Logaritm(double x) //Bianca
{
    if (x>epsi && DifInf(x))
        return log(x);
    else
        return infinit;
}

double Exponential(double x) //Bianca
{
    if (DifInf(x)) return exp(x);
    else return infinit;
}

double Inmultit(double x, double y) //Bianca
{
    if (fabs(x < epsi) || fabs(y) < epsi) return 0;
        else if (DifInf(x) && DifInf(y)) return x*y;
            else return infinit;
}

double Putere(double x, double y) //Bianca
{
    // float p;
    if (x==0) return 0;
    else if (y==0) return 1;
    else if (x==infinit || y==infinit) return infinit;
    else
        return pow(x,y);
}

bool Egal(double x, double y) //Bianca
{
    return x==y;
}

double Diferit(double x, double y) //Bianca
{
    if (x==y) return 0;
    else return 1;
}

double MaiMic(double x, double y) //Bianca
{
    return x < y;
}

bool MaiMare(double x, double y) //Bianca
{
    return x > y;
}

double MaiMicEgal(double x, double y) //Bianca
{
    return x <= y;
}

bool MaiMareEgal(double x, double y) //Bianca
{
    return x >= y;
}

double Plus(double x, double y) //Bianca
{
    if (DifInf(x) && DifInf(y))  return x+y;
    else return infinit;
}

double Minus(double x, double y) //Bianca
{
    if (DifInf(x) && DifInf(y))  return x-y;
    else return infinit;
}

double Impartit(double x, double y) //Bianca
{
    if (fabs(y)>epsi) return x/y;
    else return infinit;
}

double Sinus(double x) //Bianca
{
    if (DifInf(x))  return sin(x);
    else return infinit;
}

double Cosinus(double x) //Bianca
{
    if (DifInf(x))  return cos(x);
    else return infinit;
}

double Tangenta(double x) //Bianca
{
    if (DifInf(x) && Cosinus(x)!=0) return Sinus(x)/Cosinus(x);
    else return infinit;
}

double Cotangenta(double x) //Bianca
{
    if (DifInf(x) && sin(x)!=0)  return cos(x)/sin(x);
    else return infinit;
}

double Modul(double x) //Bianca
{
    if (DifInf(x)) return fabs(x);
    else return infinit;
}

double Radical(double x) //Bianca
{
    if (DifInf(x) && (x>epsi)) return sqrt(x);
    else return infinit;
}

bool EsteNumar(char sir[MAX1]) //Bianca
{
    return (strchr("0123456789",sir[0]));
}

void depanare() //Bianca
{
    printf("Stiva operanzilor: ");
    for (int i=1; i<=top1; i++)
        printf("%f,",Opd[i]);
    printf("\nStiva operatorilor: ");
    for (int i=1; i<=top2; i++)
        printf("%c,",Op[i]);
    printf("\n");
}

double ValoareFunctie(functie E) //Bianca
{
    int i;
    // returneaza valoarea functiei E in punctul x
    double valo,x_1,x_2;
    //calculul expresiei
    for (i=1; i<=max_stiva; i++)
    {
        Opd[i]=0;
        Op[i]='@';
    }
    top1=0;
    top2=1;
    Op[top2]='(';
    i=0;
    while (i<=E.lung && top2>0)
    {
        i++;
        if (EsteNumar(E.vect[i]))
        {
            top1++;
            char aux[100];
            int m=0;
            if (E.expresie[i-1]=='_') i--;
            while ((E.expresie[i]>='0' && E.expresie[i]<='9') || E.expresie[i]=='.' || E.expresie[i]=='_')
            {
                aux[m++]=E.expresie[i];
                aux[m]='\0';
                i++;
            }
            i--;
            if (aux[0]=='_') aux[0]='-';
            valo=atof(aux);
            Opd[top1]=valo;
            //printf ("%s", aux);
            //printf("%f",valo);
             //depanare();
        }
        else


            switch (E.vect[i][0]) {
            /* constanta pi=3.1415926 se da sub forma literei q */
            case 'q': top1++; Opd[top1]=pi; break;
            case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':  top1++; Opd[top1]=var.vl[E.vect[i][0]-'A'];
                 break;
            case '(': /* inceput de bloc */ top2++; Op[top2]='('; break;
            default:
                /* operatii binare si unare */
               while ((top2>0) && !(strchr("()",Op[top2])) &&
                   Prioritate(Op[top2])>=Prioritate(E.vect[i][0]))
                {
                    if (top1>1) x_1=Opd[top1-1];
                    x_2=Opd[top1];
                     //depanare();
                    /* functii scrise in OPERATII */
                    switch (Op[top2]) {
                        case '=': valo=Egal(x_1,x_2); break;
                        case '!': valo=Diferit(x_1,x_2); break;
                        case '<': valo=MaiMic(x_1,x_2); break;
                        case '>': valo=MaiMare(x_1,x_2); break;
                        case '[': valo=MaiMicEgal(x_1,x_2); break;
                        case ']': valo=MaiMareEgal(x_1,x_2); break;
                        case '+': valo=Plus(x_1,x_2); break;
                        case '-': valo=Minus(x_1,x_2); break;
                        case '*': valo=Inmultit(x_1,x_2); break;
                        case '/': valo=Impartit(x_1,x_2); break;
                        case '^': valo=Putere(x_1,x_2); break;
                        case 's': valo=Sinus(x_2); break;
                        case 'c': valo=Cosinus(x_2); break;
                        case 'l': valo=Logaritm(x_2); break;
                        case 'e': valo=Exponential(x_2); break;
                        case 'a': valo=Modul(x_2); break;
                        case 'r': valo=Radical(x_2); break;
                        case 't': valo=Tangenta(x_2); break;
                        case 'g': valo=Cotangenta(x_2); break;
                    }
                    if (strchr(OperatiiBinare,Op[top2])) top1--;
                    if (strchr(Operatii,Op[top2])) Opd[top1]=valo;
                    top2--;
                }
             //depanare();
            if (top2>0)
                {if ((Op[top2]!='(') || (strcmp(E.vect[i],")")))
                    {
                        top2++; Op[top2] = E.vect[i][0];
                    }
                else top2--;
                }
            }

    if(Op[top2]==')' && Op[top2-1]=='(') top2=top2-2;
    }
    if (top2==0) return Opd[1];
    else return infinit;
}

void desen_trapez_baza_jos(forma* f,int culoare) //Silviu
{
    setcolor(WHITE);
    line(f->x-f->lungime/2-in_plus,f->y-raza,f->x+f->lungime/2+in_plus,f->y-raza);
    line(f->x-f->lungime/2-in_plus,f->y-raza,f->x-f->lungime/2-raza-in_plus,f->y+raza);
    line(f->x+f->lungime/2+raza+in_plus,f->y+raza,f->x+f->lungime/2+in_plus,f->y-raza);
    line(f->x+f->lungime/2+raza+in_plus,f->y+raza,f->x-f->lungime/2-raza-in_plus,f->y+raza);
    setfillstyle(SOLID_FILL,culoare);
    floodfill(f->x,f->y,WHITE);

    setcolor(MAGENTA);
    bgiout<<f->text;
    setbkcolor(culoare);
    outstreamxy(f->x-f->lungime/2,f->y-16/2);

    setcolor(LIGHTCYAN);
    circle(f->x,f->y-raza,4);
    setfillstyle(SOLID_FILL,LIGHTCYAN);
    floodfill(f->x,f->y-raza,LIGHTCYAN);

    setcolor(GREEN);
    circle(f->x,f->y+raza,4);
    setfillstyle(SOLID_FILL,GREEN);
    floodfill(f->x,f->y+raza,GREEN);
}

void desen_dreptunghi(forma* f,int culoare) //Silviu
{
    setcolor(WHITE);
    rectangle(f->x-f->lungime/2-in_plus-raza,f->y-raza,f->x+f->lungime/2+in_plus+raza,f->y+raza);
    setfillstyle(SOLID_FILL,culoare);
    floodfill(f->x,f->y,WHITE);

    setcolor(MAGENTA);
    bgiout<<f->text;
    setbkcolor(culoare);
    outstreamxy(f->x-f->lungime/2,f->y-16/2);

    setcolor(LIGHTCYAN);
    circle(f->x,f->y-raza,4);
    setfillstyle(SOLID_FILL,LIGHTCYAN);
    floodfill(f->x,f->y-raza,LIGHTCYAN);

    setcolor(GREEN);
    circle(f->x,f->y+raza,4);
    setfillstyle(SOLID_FILL,GREEN);
    floodfill(f->x,f->y+raza,GREEN);
}

void desen_trapez_baza_sus(forma* f,int culoare) //Silviu
{
    setcolor(WHITE);
    line(f->x-f->lungime/2-in_plus-raza,f->y-raza,f->x+f->lungime/2+in_plus+raza,f->y-raza);
    line(f->x-f->lungime/2-in_plus,f->y+raza,f->x-f->lungime/2-raza-in_plus,f->y-raza);
    line(f->x+f->lungime/2+raza+in_plus,f->y-raza,f->x+f->lungime/2+in_plus,f->y+raza);
    line(f->x-f->lungime/2-in_plus,f->y+raza,f->x+f->lungime/2+in_plus,f->y+raza);
    setfillstyle(SOLID_FILL,culoare);
    floodfill(f->x,f->y,WHITE);

    setcolor(MAGENTA);
    bgiout<<f->text;
    setbkcolor(culoare);
    outstreamxy(f->x-f->lungime/2,f->y-16/2);

    setcolor(LIGHTCYAN);
    circle(f->x,f->y-raza,4);
    setfillstyle(SOLID_FILL,LIGHTCYAN);
    floodfill(f->x,f->y-raza,LIGHTCYAN);

    setcolor(GREEN);
    circle(f->x,f->y+raza,4);
    setfillstyle(SOLID_FILL,GREEN);
    floodfill(f->x,f->y+raza,GREEN);
}

void desen_triunghi(forma* f,int culoare) //Silviu
{

    setcolor(WHITE);
    line(f->x-f->lungime/2*2-in_plus-raza,f->y+2*raza,f->x+f->lungime/2*2+in_plus+raza,f->y+2*raza);
    line(f->x-f->lungime/2*2-in_plus-raza,f->y+2*raza,f->x,f->y-2*raza);
    line(f->x-f->lungime/2*2-in_plus-raza-1,f->y+2*raza,f->x,f->y-2*raza);
    line(f->x+f->lungime/2*2+in_plus+raza,f->y+2*raza,f->x,f->y-2*raza);
    line(f->x+f->lungime/2*2+in_plus+raza+1,f->y+2*raza,f->x,f->y-2*raza);


    setfillstyle(SOLID_FILL,culoare);
    floodfill(f->x,f->y,WHITE);

    setcolor(MAGENTA);
    bgiout<<f->text;
    setbkcolor(culoare);
    outstreamxy(f->x-f->lungime/2,f->y+4);

    setcolor(LIGHTCYAN);
    circle(f->x,f->y-2*raza,4);
    setfillstyle(SOLID_FILL,LIGHTCYAN);
    floodfill(f->x,f->y-2*raza,LIGHTCYAN);

    setcolor(GREEN);
    circle(f->x-f->lungime/2*2-in_plus-raza,f->y+2*raza,4);
    setfillstyle(SOLID_FILL,GREEN);
    floodfill(f->x-f->lungime/2*2-in_plus-raza,f->y+2*raza,GREEN);

    setcolor(GREEN);
    circle(f->x+f->lungime/2*2+in_plus+raza,f->y+2*raza,4);
    setfillstyle(SOLID_FILL,GREEN);
    floodfill(f->x+f->lungime/2*2+in_plus+raza,f->y+2*raza,GREEN);



}

void desen_start_stop(forma* f,int culoare) //Silviu
{
    setcolor(WHITE);
    line(f->x-f->lungime/2-in_plus,f->y-raza,f->x+f->lungime/2+in_plus,f->y-raza);
    line(f->x-f->lungime/2-in_plus,f->y+raza,f->x+f->lungime/2+in_plus,f->y+raza);
    arc(f->x-f->lungime/2-in_plus,f->y,90,270,raza);
    arc(f->x+f->lungime/2+in_plus,f->y,270,90,raza);
    setfillstyle(SOLID_FILL,culoare);
    floodfill(f->x,f->y,WHITE);

    setcolor(MAGENTA);
    bgiout<<f->text;
    setbkcolor(culoare);
    outstreamxy(f->x-f->lungime/2,f->y-16/2);

    if(strcmp(f->text,"stop")==0)
    {
        setcolor(LIGHTCYAN);
        circle(f->x,f->y-raza,4);
        setfillstyle(SOLID_FILL,LIGHTCYAN);
        floodfill(f->x,f->y-raza,LIGHTCYAN);
    }

    if(strcmp(f->text,"start")==0)
    {
        setcolor(GREEN);
        circle(f->x,f->y+raza,4);
        setfillstyle(SOLID_FILL,GREEN);
        floodfill(f->x,f->y+raza,GREEN);
    }

}

void desen_forma(forma* f,int culoare) //Silviu
{
    switch(f->denumire%7)
    {
    case 1:
        desen_start_stop(f,culoare);
        break;
    case 2:
        desen_start_stop(f,culoare);
        break;
    case 3:
        desen_triunghi(f,culoare);
        break;
    case 4:
        desen_trapez_baza_sus(f,culoare);
        break;
    case 5:
        desen_trapez_baza_jos(f,culoare);
        break;
    case 6:
        desen_dreptunghi(f,culoare);
        break;
    }

}

void desen_partea_stanga() //Silviu
{
    forma f;
    int culoare=COLOR(135,123,164);
    f.denumire=1;
    strcpy(f.text,"start");
    f.lungime=textwidth(f.text);
    f.x=incepe_ecran_x/2;
    f.y=ecran_H/6-ecran_H/12;
    desen_forma(&f,culoare);

    f.denumire=2;
    strcpy(f.text,"stop");
    f.lungime=textwidth(f.text);
    f.x=incepe_ecran_x/2;
    f.y=2*ecran_H/6-ecran_H/12;
    desen_forma(&f,culoare);

    f.denumire=3;
    strcpy(f.text,"decizie");
    f.lungime=textwidth(f.text);
    f.x=incepe_ecran_x/2;
    f.y=3*ecran_H/6-ecran_H/12;
    f.lungime_obiect_pe_2=f.lungime/2*2+in_plus+raza+4;
    desen_forma(&f,culoare);

    f.denumire=4;
    strcpy(f.text,"in");
    f.lungime=textwidth(f.text);
    f.x=incepe_ecran_x/2;
    f.y=4*ecran_H/6-ecran_H/12;
    desen_forma(&f,culoare);

    f.denumire=5;
    strcpy(f.text,"out");
    f.lungime=textwidth(f.text);
    f.x=incepe_ecran_x/2;
    f.y=5*ecran_H/6-ecran_H/12;
    desen_forma(&f,culoare);

    f.denumire=6;
    strcpy(f.text,"calcul");
    f.lungime=textwidth(f.text);
    f.x=incepe_ecran_x/2;
    f.y=6*ecran_H/6-ecran_H/12;
    desen_forma(&f,culoare);
}

void desen_sus() //Silviu
{

    unsigned int a;
    setcolor(COLOR(137,249,163));
    a=(sfarsit_ecran_x-incepe_ecran_x)/4;
    line(incepe_ecran_x+a*1,0,incepe_ecran_x+a*1,incepe_ecran_y-1);
    line(incepe_ecran_x+a*2,0,incepe_ecran_x+a*2,incepe_ecran_y-1);
    line(incepe_ecran_x+a*3,0,incepe_ecran_x+a*3,incepe_ecran_y-1);

    char text[31];
    strcpy(text,"SAVE");
    setcolor(BLACK);
    bgiout<<text;
    setbkcolor(COLOR(135,154,103));
    outstreamxy((incepe_ecran_x+a*1-a/2)-textwidth(text)/2,incepe_ecran_y/2-16/2);

    strcpy(text,"LOAD");
    setcolor(BLACK);
    bgiout<<text;
    setbkcolor(COLOR(135,154,103));
    outstreamxy((incepe_ecran_x+a*2-a/2)-textwidth(text)/2,incepe_ecran_y/2-16/2);

    strcpy(text,"BUILD");
    setcolor(BLACK);
    bgiout<<text;
    setbkcolor(COLOR(135,154,103));
    outstreamxy((incepe_ecran_x+a*3-a/2)-textwidth(text)/2,incepe_ecran_y/2-16/2);

    strcpy(text,"CODE");
    setcolor(BLACK);
    bgiout<<text;
    setbkcolor(COLOR(135,154,103));
    outstreamxy((incepe_ecran_x+a*4-a/2)-textwidth(text)/2,incepe_ecran_y/2-16/2);



}

void desen_butoane_jos() //Silviu
{
    unsigned int a;
    setcolor(COLOR(137,249,163));
    a=(sfarsit_ecran_x-incepe_ecran_x)/2;
    line(incepe_ecran_x+a*1,sfarsit_ecran_y,incepe_ecran_x+a*1,sfarsit_ecran_y+20-1);
    line(incepe_ecran_x,sfarsit_ecran_y+20,sfarsit_ecran_x,sfarsit_ecran_y+20);

    char text[31];
    strcpy(text,"CLEAN");
    setcolor(BLACK);
    bgiout<<text;
    setbkcolor(COLOR(135,154,103));
    outstreamxy((incepe_ecran_x+a-a/2)-textwidth(text)/2,sfarsit_ecran_y+16/2/2);


    strcpy(text,"REMOVE");
    setcolor(BLACK);
    bgiout<<text;
    setbkcolor(COLOR(135,154,103));
    outstreamxy((incepe_ecran_x+a*2-a/2)-textwidth(text)/2,sfarsit_ecran_y+16/2/2);

}

void desen_legaturi(forma* f, forma* primul,int culoare) //Silviu
{
    if(f!=primul)
    {if(primul->denumire!=3 && f->index==primul->c1)
    {


            if(primul->x>=f->x-f->lungime_obiect_pe_2-primul->lungime_obiect_pe_2 && primul->x<=f->x+f->lungime_obiect_pe_2+primul->lungime_obiect_pe_2 && primul->y<f->y)
            {
                setcolor(culoare);
                unsigned int j=(primul->y+f->y)/2;
                line(primul->x,primul->y+primul->inaltime_obiect_pe_2,primul->x,j);
                line(primul->x,j,f->x,j);
                line(f->x,j,f->x,f->y-f->inaltime_obiect_pe_2);
            }
            else
            {
            if(primul->x>=f->x)
            {
                setcolor(culoare);
                line(primul->x,primul->y+primul->inaltime_obiect_pe_2,primul->x,primul->y+primul->inaltime_obiect_pe_2+4);

                line(primul->x,primul->y+primul->inaltime_obiect_pe_2+4,f->x+f->lungime_obiect_pe_2+4,primul->y+primul->inaltime_obiect_pe_2+4);
                line(f->x+f->lungime_obiect_pe_2+4,primul->y+primul->inaltime_obiect_pe_2+4,f->x+f->lungime_obiect_pe_2+4,f->y-f->inaltime_obiect_pe_2-4);
                line(f->x+f->lungime_obiect_pe_2+4,f->y-f->inaltime_obiect_pe_2-4,f->x,f->y-f->inaltime_obiect_pe_2-4);
                line(f->x,f->y-f->inaltime_obiect_pe_2-4,f->x,f->y-f->inaltime_obiect_pe_2);
            }
            if(primul->x<f->x)
            {
                setcolor(culoare);
                line(primul->x,primul->y+primul->inaltime_obiect_pe_2,primul->x,primul->y+primul->inaltime_obiect_pe_2+4);

                line(primul->x,primul->y+primul->inaltime_obiect_pe_2+4,f->x-f->lungime_obiect_pe_2-4,primul->y+primul->inaltime_obiect_pe_2+4);
                line(f->x-f->lungime_obiect_pe_2-4,primul->y+primul->inaltime_obiect_pe_2+4,f->x-f->lungime_obiect_pe_2-4,f->y-f->inaltime_obiect_pe_2-4);
                line(f->x-f->lungime_obiect_pe_2-4,f->y-f->inaltime_obiect_pe_2-4,f->x,f->y-f->inaltime_obiect_pe_2-4);
                line(f->x,f->y-f->inaltime_obiect_pe_2-4,f->x,f->y-f->inaltime_obiect_pe_2);
            }
            }


    }

    if(primul->denumire==3 )
    {

            if(primul->c1==f->index)
            {
                if(primul->x>=f->x-f->lungime_obiect_pe_2-primul->lungime_obiect_pe_2 && primul->x<=f->x+f->lungime_obiect_pe_2+primul->lungime_obiect_pe_2 && primul->y<f->y)
                {
                    setcolor(culoare);
                    unsigned int j=(primul->y+f->y)/2;
                    line(primul->x-primul->lungime_obiect_pe_2+4,primul->y+primul->inaltime_obiect_pe_2,primul->x-primul->lungime_obiect_pe_2+4,j);
                    line(primul->x-primul->lungime_obiect_pe_2+4,j,f->x,j);
                    line(f->x,j,f->x,f->y-f->inaltime_obiect_pe_2);
                }
                else
                {
                if(primul->x>= f->x)
                {
                    setcolor(culoare);
                    line(primul->x-primul->lungime_obiect_pe_2+4,primul->y+primul->inaltime_obiect_pe_2,primul->x-primul->lungime_obiect_pe_2+4,primul->y+primul->inaltime_obiect_pe_2+4);

                    line(primul->x-primul->lungime_obiect_pe_2+4,primul->y+primul->inaltime_obiect_pe_2+4,f->x+f->lungime_obiect_pe_2+4,primul->y+primul->inaltime_obiect_pe_2+4);
                    line(f->x+f->lungime_obiect_pe_2+4,primul->y+primul->inaltime_obiect_pe_2+4,f->x+f->lungime_obiect_pe_2+4,f->y-f->inaltime_obiect_pe_2-4);
                    line(f->x+f->lungime_obiect_pe_2+4,f->y-f->inaltime_obiect_pe_2-4,f->x,f->y-f->inaltime_obiect_pe_2-4);
                    line(f->x,f->y-f->inaltime_obiect_pe_2-4,f->x,f->y-f->inaltime_obiect_pe_2);
                }
                if(primul->x<f->x)
                {
                    setcolor(culoare);
                    line(primul->x-primul->lungime_obiect_pe_2+4,primul->y+primul->inaltime_obiect_pe_2,primul->x-primul->lungime_obiect_pe_2+4,primul->y+primul->inaltime_obiect_pe_2+4);

                    line(primul->x-primul->lungime_obiect_pe_2+4,primul->y+primul->inaltime_obiect_pe_2+4,f->x-f->lungime_obiect_pe_2-4,primul->y+primul->inaltime_obiect_pe_2+4);
                    line(f->x-f->lungime_obiect_pe_2-4,primul->y+primul->inaltime_obiect_pe_2+4,f->x-f->lungime_obiect_pe_2-4,f->y-f->inaltime_obiect_pe_2-4);
                    line(f->x-f->lungime_obiect_pe_2-4,f->y-f->inaltime_obiect_pe_2-4,f->x,f->y-f->inaltime_obiect_pe_2-4);
                    line(f->x,f->y-f->inaltime_obiect_pe_2-4,f->x,f->y-f->inaltime_obiect_pe_2);
                }
                }
            }

            if(primul->c2==f->index)
            {
                if(primul->x>=f->x-f->lungime_obiect_pe_2-primul->lungime_obiect_pe_2 && primul->x<=f->x+f->lungime_obiect_pe_2+primul->lungime_obiect_pe_2 && primul->y<f->y)
                {
                    setcolor(culoare);
                    unsigned int j=(primul->y+f->y)/2;
                    line(primul->x+primul->lungime_obiect_pe_2-4,primul->y+primul->inaltime_obiect_pe_2,primul->x+primul->lungime_obiect_pe_2-4,j);
                    line(primul->x+primul->lungime_obiect_pe_2-4,j,f->x,j);
                    line(f->x,j,f->x,f->y-f->inaltime_obiect_pe_2);
                }
                else
                {
                if(primul->x>=f->x)
                {
                    setcolor(culoare);
                    line(primul->x+primul->lungime_obiect_pe_2-4,primul->y+primul->inaltime_obiect_pe_2,primul->x+primul->lungime_obiect_pe_2-4,primul->y+primul->inaltime_obiect_pe_2+4);

                    line(primul->x+primul->lungime_obiect_pe_2-4,primul->y+primul->inaltime_obiect_pe_2+4,f->x+f->lungime_obiect_pe_2+4,primul->y+primul->inaltime_obiect_pe_2+4);
                    line(f->x+f->lungime_obiect_pe_2+4,primul->y+primul->inaltime_obiect_pe_2+4,f->x+f->lungime_obiect_pe_2+4,f->y-f->inaltime_obiect_pe_2-4);
                    line(f->x+f->lungime_obiect_pe_2+4,f->y-f->inaltime_obiect_pe_2-4,f->x,f->y-f->inaltime_obiect_pe_2-4);
                    line(f->x,f->y-f->inaltime_obiect_pe_2-4,f->x,f->y-f->inaltime_obiect_pe_2);
                }
                    if(primul->x<f->x)
                {
                    setcolor(culoare);
                    line(primul->x+primul->lungime_obiect_pe_2-4,primul->y+primul->inaltime_obiect_pe_2,primul->x+primul->lungime_obiect_pe_2-4,primul->y+primul->inaltime_obiect_pe_2+4);

                    line(primul->x+primul->lungime_obiect_pe_2-4,primul->y+primul->inaltime_obiect_pe_2+4,f->x-f->lungime_obiect_pe_2-4,primul->y+primul->inaltime_obiect_pe_2+4);
                    line(f->x-f->lungime_obiect_pe_2-4,primul->y+primul->inaltime_obiect_pe_2+4,f->x-f->lungime_obiect_pe_2-4,f->y-f->inaltime_obiect_pe_2-4);
                    line(f->x-f->lungime_obiect_pe_2-4,f->y-f->inaltime_obiect_pe_2-4,f->x,f->y-f->inaltime_obiect_pe_2-4);
                    line(f->x,f->y-f->inaltime_obiect_pe_2-4,f->x,f->y-f->inaltime_obiect_pe_2);
                }
                }
            }


        /*if(primul->index==f->intrare_2)
        {
            if(primul->c1==f->index)
            {
                setcolor(culoare);
                line(primul->x-primul->lungime_obiect_pe_2+4,primul->y+primul->inaltime_obiect_pe_2,primul->x-primul->lungime_obiect_pe_2+4,primul->y+primul->inaltime_obiect_pe_2+4);

                line(primul->x-primul->lungime_obiect_pe_2+4,primul->y+primul->inaltime_obiect_pe_2+4,f->x-f->lungime_obiect_pe_2,primul->y+primul->inaltime_obiect_pe_2+4);
                line(f->x-f->lungime_obiect_pe_2,primul->y+primul->inaltime_obiect_pe_2+4,f->x-f->lungime_obiect_pe_2,f->y);
                line(f->x-f->lungime_obiect_pe_2,f->y,f->x-f->lungime_obiect_pe_2/2-4,f->y);

            }
            if(primul->c2==f->index)
            {
                setcolor(culoare);
                line(primul->x+primul->lungime_obiect_pe_2-4,primul->y+primul->inaltime_obiect_pe_2,primul->x+primul->lungime_obiect_pe_2-4,primul->y+primul->inaltime_obiect_pe_2+4);

                line(primul->x+primul->lungime_obiect_pe_2-4,primul->y+primul->inaltime_obiect_pe_2+4,f->x-f->lungime_obiect_pe_2,primul->y+primul->inaltime_obiect_pe_2+4);
                line(f->x-f->lungime_obiect_pe_2,primul->y+primul->inaltime_obiect_pe_2+4,f->x-f->lungime_obiect_pe_2,f->y);
                line(f->x-f->lungime_obiect_pe_2,f->y,f->x-f->lungime_obiect_pe_2/2-4,f->y);

            }
            */
        }
    }
    if(f!=primul)
    {


    if(f->denumire!=3 && f->c1==primul->index )
    {
        if(f->x>=primul->x-primul->lungime_obiect_pe_2-f->lungime_obiect_pe_2 && f->x<=primul->x+primul->lungime_obiect_pe_2+f->lungime_obiect_pe_2 && primul->y>f->y)
            {
                setcolor(culoare);
                unsigned int j=(primul->y+f->y)/2;
                line(f->x,f->y+f->inaltime_obiect_pe_2,f->x,j);
                line(f->x,j,primul->x,j);
                line(primul->x,j,primul->x,primul->y-primul->inaltime_obiect_pe_2);
            }
            else
            {

            if(f->x>= primul->x)
            {
                setcolor(culoare);
                line(f->x,f->y+f->inaltime_obiect_pe_2,f->x,f->y+f->inaltime_obiect_pe_2+4);

                line(f->x,f->y+f->inaltime_obiect_pe_2+4,primul->x+primul->lungime_obiect_pe_2+4,f->y+f->inaltime_obiect_pe_2+4);
                line(primul->x+primul->lungime_obiect_pe_2+4,f->y+f->inaltime_obiect_pe_2+4,primul->x+primul->lungime_obiect_pe_2+4,primul->y-primul->inaltime_obiect_pe_2-4);
                line(primul->x+primul->lungime_obiect_pe_2+4,primul->y-primul->inaltime_obiect_pe_2-4,primul->x,primul->y-primul->inaltime_obiect_pe_2-4);
                line(primul->x,primul->y-primul->inaltime_obiect_pe_2-4,primul->x,primul->y-primul->inaltime_obiect_pe_2);
            }
            if(f->x<primul->x)
            {
                setcolor(culoare);
                line(f->x,f->y+f->inaltime_obiect_pe_2,f->x,f->y+f->inaltime_obiect_pe_2+4);

                line(f->x,f->y+f->inaltime_obiect_pe_2+4,primul->x-primul->lungime_obiect_pe_2-4,f->y+f->inaltime_obiect_pe_2+4);
                line(primul->x-primul->lungime_obiect_pe_2-4,f->y+f->inaltime_obiect_pe_2+4,primul->x-primul->lungime_obiect_pe_2-4,primul->y-primul->inaltime_obiect_pe_2-4);
                line(primul->x-primul->lungime_obiect_pe_2-4,primul->y-primul->inaltime_obiect_pe_2-4,primul->x,primul->y-primul->inaltime_obiect_pe_2-4);
                line(primul->x,primul->y-primul->inaltime_obiect_pe_2-4,primul->x,primul->y-primul->inaltime_obiect_pe_2);

            }
            }


        /* if(f->index==primul->intrare_2)
        {
            setcolor(culoare);
            line(f->x,f->y+f->inaltime_obiect_pe_2,f->x,f->y+f->inaltime_obiect_pe_2+4);

            line(f->x,f->y+f->inaltime_obiect_pe_2+4,primul->x-primul->lungime_obiect_pe_2,f->y+f->inaltime_obiect_pe_2+4);
            line(primul->x-primul->lungime_obiect_pe_2,f->y+f->inaltime_obiect_pe_2+4,primul->x-primul->lungime_obiect_pe_2,primul->y);
            line(primul->x-primul->lungime_obiect_pe_2,primul->y,primul->x-primul->lungime_obiect_pe_2/2-4,primul->y);
        }*/
    }

    if(f->denumire==3 )
    {


            if(f->c1==primul->index)
            {
                if(f->x>=primul->x-primul->lungime_obiect_pe_2-f->lungime_obiect_pe_2 && f->x<=primul->x+primul->lungime_obiect_pe_2+f->lungime_obiect_pe_2 && primul->y>f->y)
                {
                    setcolor(culoare);
                    unsigned int j=(primul->y+f->y)/2;
                    line(f->x-f->lungime_obiect_pe_2+4,f->y+f->inaltime_obiect_pe_2,f->x-f->lungime_obiect_pe_2+4,j);
                    line(f->x-f->lungime_obiect_pe_2+4,j,primul->x,j);
                    line(primul->x,j,primul->x,primul->y-primul->inaltime_obiect_pe_2);
                }
                else
                {
                if(f->x>= primul->x)
                {
                    setcolor(culoare);
                    line(f->x-f->lungime_obiect_pe_2+4,f->y+f->inaltime_obiect_pe_2,f->x-f->lungime_obiect_pe_2+4,f->y+f->inaltime_obiect_pe_2+4);

                    line(f->x-f->lungime_obiect_pe_2+4,f->y+f->inaltime_obiect_pe_2+4,primul->x+primul->lungime_obiect_pe_2+4,f->y+f->inaltime_obiect_pe_2+4);
                    line(primul->x+primul->lungime_obiect_pe_2+4,f->y+f->inaltime_obiect_pe_2+4,primul->x+primul->lungime_obiect_pe_2+4,primul->y-primul->inaltime_obiect_pe_2-4);
                    line(primul->x+primul->lungime_obiect_pe_2+4,primul->y-primul->inaltime_obiect_pe_2-4,primul->x,primul->y-primul->inaltime_obiect_pe_2-4);
                    line(primul->x,primul->y-primul->inaltime_obiect_pe_2-4,primul->x,primul->y-primul->inaltime_obiect_pe_2);
                }
                if(f->x<primul->x)
                {
                    setcolor(culoare);
                    line(f->x-f->lungime_obiect_pe_2+4,f->y+f->inaltime_obiect_pe_2,f->x-f->lungime_obiect_pe_2+4,f->y+f->inaltime_obiect_pe_2+4);

                    line(f->x-f->lungime_obiect_pe_2+4,f->y+f->inaltime_obiect_pe_2+4,primul->x-primul->lungime_obiect_pe_2-4,f->y+f->inaltime_obiect_pe_2+4);
                    line(primul->x-primul->lungime_obiect_pe_2-4,f->y+f->inaltime_obiect_pe_2+4,primul->x-primul->lungime_obiect_pe_2-4,primul->y-primul->inaltime_obiect_pe_2-4);
                    line(primul->x-primul->lungime_obiect_pe_2-4,primul->y-primul->inaltime_obiect_pe_2-4,primul->x,primul->y-primul->inaltime_obiect_pe_2-4);
                    line(primul->x,primul->y-primul->inaltime_obiect_pe_2-4,primul->x,primul->y-primul->inaltime_obiect_pe_2);
                }
                }
            }
            if(f->c2==primul->index)
            {
                if(f->x>=primul->x-primul->lungime_obiect_pe_2-f->lungime_obiect_pe_2 && f->x<=primul->x+primul->lungime_obiect_pe_2+f->lungime_obiect_pe_2 && primul->y>f->y)
                {
                    setcolor(culoare);
                    unsigned int j=(primul->y+f->y)/2;
                    line(f->x+f->lungime_obiect_pe_2-4,f->y+f->inaltime_obiect_pe_2,f->x+f->lungime_obiect_pe_2-4,j);
                    line(f->x+f->lungime_obiect_pe_2-4,j,primul->x,j);
                    line(primul->x,j,primul->x,primul->y-primul->inaltime_obiect_pe_2);
                }
                else
                {
                if(f->x>=primul->x)
                {
                    setcolor(culoare);
                    line(f->x+f->lungime_obiect_pe_2-4,f->y+f->inaltime_obiect_pe_2,f->x+f->lungime_obiect_pe_2-4,f->y+f->inaltime_obiect_pe_2+4);

                    line(f->x+f->lungime_obiect_pe_2-4,f->y+f->inaltime_obiect_pe_2+4,primul->x+primul->lungime_obiect_pe_2+4,f->y+f->inaltime_obiect_pe_2+4);
                    line(primul->x+primul->lungime_obiect_pe_2+4,f->y+f->inaltime_obiect_pe_2+4,primul->x+primul->lungime_obiect_pe_2+4,primul->y-primul->inaltime_obiect_pe_2-4);
                    line(primul->x+primul->lungime_obiect_pe_2+4,primul->y-primul->inaltime_obiect_pe_2-4,primul->x,primul->y-primul->inaltime_obiect_pe_2-4);
                    line(primul->x,primul->y-primul->inaltime_obiect_pe_2-4,primul->x,primul->y-primul->inaltime_obiect_pe_2);
                }
                    if(f->x<primul->x)
                {
                    setcolor(culoare);
                    line(f->x+f->lungime_obiect_pe_2-4,f->y+f->inaltime_obiect_pe_2,f->x+f->lungime_obiect_pe_2-4,f->y+f->inaltime_obiect_pe_2+4);

                    line(f->x+f->lungime_obiect_pe_2-4,f->y+f->inaltime_obiect_pe_2+4,primul->x-primul->lungime_obiect_pe_2-4,f->y+f->inaltime_obiect_pe_2+4);
                    line(primul->x-primul->lungime_obiect_pe_2-4,f->y+f->inaltime_obiect_pe_2+4,primul->x-primul->lungime_obiect_pe_2-4,primul->y-primul->inaltime_obiect_pe_2-4);
                    line(primul->x-primul->lungime_obiect_pe_2-4,primul->y-primul->inaltime_obiect_pe_2-4,primul->x,primul->y-primul->inaltime_obiect_pe_2-4);
                    line(primul->x,primul->y-primul->inaltime_obiect_pe_2-4,primul->x,primul->y-primul->inaltime_obiect_pe_2);
                }
                }

            }

        /*if(primul->intrare_2==f->index)
        {
            if(primul->index==f->c1)
            {
                setcolor(culoare);
                line(f->x-f->lungime_obiect_pe_2+4,f->y+f->inaltime_obiect_pe_2,f->x-f->lungime_obiect_pe_2+4,f->y+f->inaltime_obiect_pe_2+4);

                line(f->x-f->lungime_obiect_pe_2+4,f->y+f->inaltime_obiect_pe_2+4,primul->x-primul->lungime_obiect_pe_2,f->y+f->inaltime_obiect_pe_2+4);
                line(primul->x-primul->lungime_obiect_pe_2,f->y+f->inaltime_obiect_pe_2+4,primul->x-primul->lungime_obiect_pe_2,primul->y);
                line(primul->x-primul->lungime_obiect_pe_2,primul->y,primul->x-primul->lungime_obiect_pe_2/2-4,primul->y);

            }
            if(primul->index==f->c2)
            {
                setcolor(culoare);
                line(f->x+f->lungime_obiect_pe_2-4,f->y+f->inaltime_obiect_pe_2,f->x+f->lungime_obiect_pe_2-4,f->y+f->inaltime_obiect_pe_2+4);

                line(f->x+f->lungime_obiect_pe_2-4,f->y+f->inaltime_obiect_pe_2+4,primul->x-primul->lungime_obiect_pe_2,f->y+f->inaltime_obiect_pe_2+4);
                line(primul->x-primul->lungime_obiect_pe_2,f->y+f->inaltime_obiect_pe_2+4,primul->x-primul->lungime_obiect_pe_2,primul->y);
                line(primul->x-primul->lungime_obiect_pe_2,primul->y,primul->x-primul->lungime_obiect_pe_2/2-4,primul->y);

            }*/
        }
    }
}

void doar_parcurgere(forma* f,forma* primul,int culoare) //Silviu
{
    desen_legaturi(f,primul,culoare);
    if(primul->urm!=NULL)
        doar_parcurgere(f,primul->urm,culoare);
}

void miscare_obiecte( forma* f,forma* primul) //Silviu
{
    if(ismouseclick(WM_LBUTTONDOWN))
     {
         int a=mousex();
         int b=mousey();
         if((a>f->x-f->lungime_obiect_pe_2 && a<f->x+f->lungime_obiect_pe_2) && (b>f->y-f->inaltime_obiect_pe_2 && b<f->y+f->inaltime_obiect_pe_2))
         {
             if(ismouseclick(WM_MOUSEMOVE))
             {
                 f->miscare=true;
                 if((f->x-f->lungime_obiect_pe_2>incepe_ecran_x && f->x+f->lungime_obiect_pe_2<sfarsit_ecran_x)&&(f->y-f->inaltime_obiect_pe_2>incepe_ecran_y+4 && f->y+f->inaltime_obiect_pe_2<sfarsit_ecran_y-4)&&(getpixel(mousex(),mousey())==COLOR(135,123,164) || getpixel(mousex(),mousey())==MAGENTA ))//
                 {
                     setfillstyle(SOLID_FILL,COLOR(135,154,103));
                     bar(f->x-f->lungime_obiect_pe_2,f->y-f->inaltime_obiect_pe_2,f->x+f->lungime_obiect_pe_2,f->y+f->inaltime_obiect_pe_2);


                     f->x=a;
                     f->y=b;

                     while(primul!=NULL)
                     {

                        if(f->index!=primul->index && f->x+2>= primul->x-primul->lungime_obiect_pe_2-f->lungime_obiect_pe_2 && f->x-2<=primul->x+primul->lungime_obiect_pe_2+f->lungime_obiect_pe_2 &&f->y+2>=primul->y-primul->inaltime_obiect_pe_2-f->inaltime_obiect_pe_2 &&
                           f->y-2<=primul->y+primul->inaltime_obiect_pe_2+f->inaltime_obiect_pe_2)
                            {
                               // cout<<"7";
                                f->x=f->salvare_x;
                                f->y=f->salvare_y;
                            }

                        primul=primul->urm;
                     }

                     f->salvare_x=f->x;
                     f->salvare_y=f->y;

                 }



                if(f->x-f->lungime_obiect_pe_2<=incepe_ecran_x)
                {
                    f->x=incepe_ecran_x+f->lungime_obiect_pe_2+1;;
                    f->salvare_x=f->x;
                    setfillstyle(SOLID_FILL,COLOR(135,154,103));
                     bar(f->x-f->lungime_obiect_pe_2,f->y-f->inaltime_obiect_pe_2,f->x+f->lungime_obiect_pe_2,f->y+f->inaltime_obiect_pe_2);
                }
                if(f->x+f->lungime_obiect_pe_2>=sfarsit_ecran_x)
                {
                    f->x=sfarsit_ecran_x-f->lungime_obiect_pe_2-1;
                    f->salvare_x=f->x;
                    setfillstyle(SOLID_FILL,COLOR(135,154,103));
                     bar(f->x-f->lungime_obiect_pe_2,f->y-f->inaltime_obiect_pe_2,f->x+f->lungime_obiect_pe_2,f->y+f->inaltime_obiect_pe_2);
                }
                if(f->y-f->inaltime_obiect_pe_2<=incepe_ecran_y+4)
                {
                    f->y=incepe_ecran_y+f->inaltime_obiect_pe_2+1+4;
                    f->salvare_y=f->y;
                    setfillstyle(SOLID_FILL,COLOR(135,154,103));
                     bar(f->x-f->lungime_obiect_pe_2,f->y-f->inaltime_obiect_pe_2,f->x+f->lungime_obiect_pe_2,f->y+f->inaltime_obiect_pe_2);
                }
                if(f->y+f->inaltime_obiect_pe_2>=sfarsit_ecran_y-4)
                {
                    f->y=sfarsit_ecran_y-f->inaltime_obiect_pe_2-1-4;
                    f->salvare_y=f->y;
                    setfillstyle(SOLID_FILL,COLOR(135,154,103));
                     bar(f->x-f->lungime_obiect_pe_2,f->y-f->inaltime_obiect_pe_2,f->x+f->lungime_obiect_pe_2,f->y+f->inaltime_obiect_pe_2);
                }


             }
        }

      }

    if(ismouseclick(WM_LBUTTONUP))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            clearmouseclick(WM_MOUSEMOVE);
        }
         clearmouseclick(WM_LBUTTONUP);

}

void determinare_xy_obiect(forma* f, forma* prima) //Silviu
{

    if(ismouseclick(WM_LBUTTONDOWN))
    {

        if(mousex()>incepe_ecran_x+f->lungime_obiect_pe_2 && mousex()<sfarsit_ecran_x-f->lungime_obiect_pe_2 && mousey()>incepe_ecran_y+f->inaltime_obiect_pe_2 && mousey()<sfarsit_ecran_y-f->inaltime_obiect_pe_2)
        {
            bool ok=true;
            forma* a=prima;
            while(a!=f)
            {
                if(mousex()>= a->x-a->lungime_obiect_pe_2-f->lungime_obiect_pe_2 && mousex()<=a->x+a->lungime_obiect_pe_2+f->lungime_obiect_pe_2 && mousey()>=a->y-a->inaltime_obiect_pe_2-f->inaltime_obiect_pe_2 &&
                mousey()<=a->y+a->inaltime_obiect_pe_2+f->inaltime_obiect_pe_2)
                {
                    ok=false;
                    break;
                }
                a=a->urm;
            }


            if(ok==true)
            {
                avem_xy=true;
                f->x=mousex();
                f->y=mousey();
                clearmouseclick(WM_LBUTTONDOWN);

            }
        }
    }

    if(ismouseclick(WM_LBUTTONUP))
        clearmouseclick(WM_LBUTTONDOWN);

    clearmouseclick(WM_LBUTTONUP);
}

void adaugare_obiect(forma* anterior,forma* prima) //Silviu
{
    avem_xy=false;
     if(ismouseclick(WM_LBUTTONUP))
        clearmouseclick(WM_LBUTTONDOWN);
    int y=ecran_H/6-ecran_H/12;
    int x=incepe_ecran_x/2;
    char a[100];
    strcpy(a,"start");
    int lungime=textwidth(a);
    if(exista_start==false  &&  mousex()>=x-lungime/2-in_plus-raza && mousex()<=x+lungime/2+in_plus+raza  &&  mousey()>=y-raza && mousey()<=y+raza  &&  (getpixel(mousex(),mousey())==COLOR(135,123,164) || getpixel(mousex(),mousey())==MAGENTA ))
    {
        if(ismouseclick(WM_LBUTTONDOWN))
         {
             clearmouseclick(WM_LBUTTONDOWN);
             avem_xy=false;
             indx++;

             forma* f=new forma;
             anterior->urm=f;
             f->index=indx;
             strcpy(f->text,"start");
             f->lungime=lungime;
             f->denumire=1;
             f->urm=NULL;
             f->lungime_obiect_pe_2=lungime/2+in_plus+raza;
             f->inaltime_obiect_pe_2=raza+4;// +4 este de la raza cercul mic
             Beep(400,100);

             while(!avem_xy)
                determinare_xy_obiect(f,prima);

             f->salvare_x=f->x;
             f->salvare_y=f->y;

             exista_start=true;

             clearmouseclick(WM_LBUTTONDOWN);
         }

    }

    y=2*ecran_H/6-ecran_H/12;
    x=incepe_ecran_x/2;
    strcpy(a,"stop");
    lungime=textwidth(a);
    if(mousex()>=x-lungime/2-in_plus-raza && mousex()<=x+lungime/2+in_plus+raza  &&  mousey()>=y-raza && mousey()<=y+raza  &&  (getpixel(mousex(),mousey())==COLOR(135,123,164) || getpixel(mousex(),mousey())==MAGENTA ))
    {
        if(ismouseclick(WM_LBUTTONDOWN))
         {
             clearmouseclick(WM_LBUTTONDOWN);
             avem_xy=false;
             indx++;

             forma* f=new forma;
             anterior->urm=f;
             f->index=indx;
             strcpy(f->text,"stop");
             f->lungime=lungime;
             f->denumire=2;
             f->urm=NULL;
             f->lungime_obiect_pe_2=lungime/2+in_plus+raza;
             f->inaltime_obiect_pe_2=raza+4;// +4 este de la raza cercul mic
             Beep(400,100);

             while(!avem_xy)
                determinare_xy_obiect(f,prima);

             f->salvare_x=f->x;
             f->salvare_y=f->y;

             clearmouseclick(WM_LBUTTONDOWN);
         }

    }

    y=3*ecran_H/6-ecran_H/12;
    x=incepe_ecran_x/2;
    strcpy(a,"???");
    lungime=textwidth(a);
    if(mousex()>=x-lungime/2*2-in_plus-raza && mousex()<=x+lungime/2*2+in_plus+raza  &&  mousey()>=y-2*raza && mousey()<=y+2*raza  &&  (getpixel(mousex(),mousey())==COLOR(135,123,164) || getpixel(mousex(),mousey())==MAGENTA ))
    {
        if(ismouseclick(WM_LBUTTONDOWN))
         {
             clearmouseclick(WM_LBUTTONDOWN);
             avem_xy=false;
             indx++;

             forma* f=new forma;
             anterior->urm=f;
             f->index=indx;
             strcpy(f->text,"???");
             f->lungime=lungime;
             f->denumire=3;
             f->urm=NULL;
             f->lungime_obiect_pe_2=lungime/2*2+in_plus+raza+4;// +4 este de la raza cercul mic
             f->inaltime_obiect_pe_2=2*raza+4;// +4 este de la raza cercul mic
             Beep(400,100);

             while(!avem_xy)
                determinare_xy_obiect(f,prima);

             f->salvare_x=f->x;
             f->salvare_y=f->y;

             clearmouseclick(WM_LBUTTONDOWN);
         }

    }

    y=4*ecran_H/6-ecran_H/12;
    x=incepe_ecran_x/2;
    strcpy(a,"?");
    lungime=textwidth(a);
    if(mousex()>=x-lungime/2-in_plus-raza && mousex()<=x+lungime/2+in_plus+raza  &&  mousey()>=y-raza && mousey()<=y+raza  &&  (getpixel(mousex(),mousey())==COLOR(135,123,164) || getpixel(mousex(),mousey())==MAGENTA ))
    {
        if(ismouseclick(WM_LBUTTONDOWN))
         {
             clearmouseclick(WM_LBUTTONDOWN);
             avem_xy=false;
             indx++;

             forma* f=new forma;
             anterior->urm=f;
             f->index=indx;
             strcpy(f->text,"?");
             f->lungime=lungime;
             f->denumire=4;
             f->urm=NULL;
             f->lungime_obiect_pe_2=lungime/2+in_plus+raza+1;
             f->inaltime_obiect_pe_2=raza+4;// +4 este de la raza cercul mic
             Beep(400,100);

             while(!avem_xy)
                determinare_xy_obiect(f,prima);

             f->salvare_x=f->x;
             f->salvare_y=f->y;

             clearmouseclick(WM_LBUTTONDOWN);
         }

    }

    y=5*ecran_H/6-ecran_H/12;
    x=incepe_ecran_x/2;
    strcpy(a,"?");
    lungime=textwidth(a);
    if(mousex()>=x-lungime/2-in_plus-raza && mousex()<=x+lungime/2+in_plus+raza  &&  mousey()>=y-raza && mousey()<=y+raza  &&  (getpixel(mousex(),mousey())==COLOR(135,123,164) || getpixel(mousex(),mousey())==MAGENTA ))
    {
        if(ismouseclick(WM_LBUTTONDOWN))
         {
             clearmouseclick(WM_LBUTTONDOWN);
             avem_xy=false;
             indx++;

             forma* f=new forma;
             anterior->urm=f;
             f->index=indx;
             strcpy(f->text,"?");
             f->lungime=lungime;
             f->denumire=5;
             f->urm=NULL;
             f->lungime_obiect_pe_2=lungime/2+in_plus+raza+1;
             f->inaltime_obiect_pe_2=raza+4;// +4 este de la raza cercul mic
             Beep(400,100);

             while(!avem_xy)
                determinare_xy_obiect(f,prima);

             f->salvare_x=f->x;
             f->salvare_y=f->y;

             clearmouseclick(WM_LBUTTONDOWN);
         }

    }

    y=6*ecran_H/6-ecran_H/12;
    x=incepe_ecran_x/2;
    strcpy(a,"???");
    lungime=textwidth(a);
    if(mousex()>=x-lungime/2-in_plus-raza && mousex()<=x+lungime/2+in_plus+raza  &&  mousey()>=y-raza && mousey()<=y+raza  &&  (getpixel(mousex(),mousey())==COLOR(135,123,164) || getpixel(mousex(),mousey())==MAGENTA ))
    {
        if(ismouseclick(WM_LBUTTONDOWN))
         {
             clearmouseclick(WM_LBUTTONDOWN);
             avem_xy=false;
             indx++;

             forma* f=new forma;
             anterior->urm=f;
             f->index=indx;
             strcpy(f->text,"???");
             f->lungime=lungime;
             f->denumire=6;
             f->urm=NULL;
             f->lungime_obiect_pe_2=lungime/2+in_plus+raza+1;
             f->inaltime_obiect_pe_2=raza+4;// +4 este de la raza cercul mic
             Beep(400,100);

             while(!avem_xy)
                determinare_xy_obiect(f,prima);

             f->salvare_x=f->x;
             f->salvare_y=f->y;

             clearmouseclick(WM_LBUTTONDOWN);
         }

    }
}

void determinare_xy_obiect(forma* f) //Silviu
{

    if(ismouseclick(WM_LBUTTONDOWN) && mousex()>incepe_ecran_x+f->lungime_obiect_pe_2 && mousex()<sfarsit_ecran_x-f->lungime_obiect_pe_2 && mousey()>incepe_ecran_y+f->inaltime_obiect_pe_2 && mousey()<sfarsit_ecran_y-f->inaltime_obiect_pe_2)
    {

        avem_xy=true;
        f->x=mousex();
        f->y=mousey();

    }
    if(ismouseclick(WM_LBUTTONUP))
    {
        clearmouseclick(WM_LBUTTONDOWN);
    }
    clearmouseclick(WM_LBUTTONUP);
}

void adaugare_obiect(forma* anterior) //Silviu
{
    avem_xy=false;
     if(ismouseclick(WM_LBUTTONUP))
        clearmouseclick(WM_LBUTTONDOWN);
    int y=ecran_H/6-ecran_H/12;
    int x=incepe_ecran_x/2;
    char a[100];
    strcpy(a,"start");
    int lungime=textwidth(a);
    if(exista_start==false  &&  mousex()>=x-lungime/2-in_plus-raza && mousex()<=x+lungime/2+in_plus+raza  &&  mousey()>=y-raza && mousey()<=y+raza  &&  (getpixel(mousex(),mousey())==COLOR(135,123,164) || getpixel(mousex(),mousey())==MAGENTA ))
    {
        if(ismouseclick(WM_LBUTTONDOWN))
         {
             clearmouseclick(WM_LBUTTONDOWN);
             avem_xy=false;
             indx++;

             forma* f=new forma;
             anterior->urm=f;
             f->index=indx;
             strcpy(f->text,"start");
             f->lungime=lungime;
             f->denumire=1;
             f->urm=NULL;
             f->lungime_obiect_pe_2=lungime/2+in_plus+raza;
             f->inaltime_obiect_pe_2=raza+4;// +4 este de la raza cercul mic
             Beep(400,100);

             while(!avem_xy)
                determinare_xy_obiect(f);

             f->salvare_x=f->x;
             f->salvare_y=f->y;

             exista_start=true;

             clearmouseclick(WM_LBUTTONDOWN);
         }

    }

    y=2*ecran_H/6-ecran_H/12;
    x=incepe_ecran_x/2;
    strcpy(a,"stop");
    lungime=textwidth(a);
    if(mousex()>=x-lungime/2-in_plus-raza && mousex()<=x+lungime/2+in_plus+raza  &&  mousey()>=y-raza && mousey()<=y+raza  &&  (getpixel(mousex(),mousey())==COLOR(135,123,164) || getpixel(mousex(),mousey())==MAGENTA ))
    {
        if(ismouseclick(WM_LBUTTONDOWN))
         {
             clearmouseclick(WM_LBUTTONDOWN);
             avem_xy=false;
             indx++;

             forma* f=new forma;
             anterior->urm=f;
             f->index=indx;
             strcpy(f->text,"stop");
             f->lungime=lungime;
             f->denumire=2;
             f->urm=NULL;
             f->lungime_obiect_pe_2=lungime/2+in_plus+raza;
             f->inaltime_obiect_pe_2=raza+4;// +4 este de la raza cercul mic
             Beep(400,100);

             while(!avem_xy)
                determinare_xy_obiect(f);

             f->salvare_x=f->x;
             f->salvare_y=f->y;

             clearmouseclick(WM_LBUTTONDOWN);
         }

    }

    y=3*ecran_H/6-ecran_H/12;
    x=incepe_ecran_x/2;
    strcpy(a,"???");
    lungime=textwidth(a);
    if(mousex()>=x-lungime/2*2-in_plus-raza && mousex()<=x+lungime/2*2+in_plus+raza  &&  mousey()>=y-2*raza && mousey()<=y+2*raza  &&  (getpixel(mousex(),mousey())==COLOR(135,123,164) || getpixel(mousex(),mousey())==MAGENTA ))
    {
        if(ismouseclick(WM_LBUTTONDOWN))
         {
             clearmouseclick(WM_LBUTTONDOWN);
             avem_xy=false;
             indx++;

             forma* f=new forma;
             anterior->urm=f;
             f->index=indx;
             strcpy(f->text,"???");
             f->lungime=lungime;
             f->denumire=3;
             f->urm=NULL;
             f->lungime_obiect_pe_2=lungime/2*2+in_plus+raza+4;// +4 este de la raza cercul mic
             f->inaltime_obiect_pe_2=2*raza+4;// +4 este de la raza cercul mic
             Beep(400,100);

             while(!avem_xy)
                determinare_xy_obiect(f);

             f->salvare_x=f->x;
             f->salvare_y=f->y;

             clearmouseclick(WM_LBUTTONDOWN);
         }

    }

    y=4*ecran_H/6-ecran_H/12;
    x=incepe_ecran_x/2;
    strcpy(a,"?");
    lungime=textwidth(a);
    if(mousex()>=x-lungime/2-in_plus-raza && mousex()<=x+lungime/2+in_plus+raza  &&  mousey()>=y-raza && mousey()<=y+raza  &&  (getpixel(mousex(),mousey())==COLOR(135,123,164) || getpixel(mousex(),mousey())==MAGENTA ))
    {
        if(ismouseclick(WM_LBUTTONDOWN))
         {
             clearmouseclick(WM_LBUTTONDOWN);
             avem_xy=false;
             indx++;

             forma* f=new forma;
             anterior->urm=f;
             f->index=indx;
             strcpy(f->text,"?");
             f->lungime=lungime;
             f->denumire=4;
             f->urm=NULL;
             f->lungime_obiect_pe_2=lungime/2+in_plus+raza+1;
             f->inaltime_obiect_pe_2=raza+4;// +4 este de la raza cercul mic
             Beep(400,100);

             while(!avem_xy)
                determinare_xy_obiect(f);

             f->salvare_x=f->x;
             f->salvare_y=f->y;

             clearmouseclick(WM_LBUTTONDOWN);
         }

    }

    y=5*ecran_H/6-ecran_H/12;
    x=incepe_ecran_x/2;
    strcpy(a,"?");
    lungime=textwidth(a);
    if(mousex()>=x-lungime/2-in_plus-raza && mousex()<=x+lungime/2+in_plus+raza  &&  mousey()>=y-raza && mousey()<=y+raza  &&  (getpixel(mousex(),mousey())==COLOR(135,123,164) || getpixel(mousex(),mousey())==MAGENTA ))
    {
        if(ismouseclick(WM_LBUTTONDOWN))
         {
             clearmouseclick(WM_LBUTTONDOWN);
             avem_xy=false;
             indx++;

             forma* f=new forma;
             anterior->urm=f;
             f->index=indx;
             strcpy(f->text,"?");
             f->lungime=lungime;
             f->denumire=5;
             f->urm=NULL;
             f->lungime_obiect_pe_2=lungime/2+in_plus+raza+1;
             f->inaltime_obiect_pe_2=raza+4;// +4 este de la raza cercul mic
             Beep(400,100);

             while(!avem_xy)
                determinare_xy_obiect(f);

             f->salvare_x=f->x;
             f->salvare_y=f->y;

             clearmouseclick(WM_LBUTTONDOWN);
         }

    }

    y=6*ecran_H/6-ecran_H/12;
    x=incepe_ecran_x/2;
    strcpy(a,"???");
    lungime=textwidth(a);
    if(mousex()>=x-lungime/2-in_plus-raza && mousex()<=x+lungime/2+in_plus+raza  &&  mousey()>=y-raza && mousey()<=y+raza  &&  (getpixel(mousex(),mousey())==COLOR(135,123,164) || getpixel(mousex(),mousey())==MAGENTA ))
    {
        if(ismouseclick(WM_LBUTTONDOWN))
         {
             clearmouseclick(WM_LBUTTONDOWN);
             avem_xy=false;
             indx++;

             forma* f=new forma;
             anterior->urm=f;
             f->index=indx;
             strcpy(f->text,"???");
             f->lungime=lungime;
             f->denumire=6;
             f->urm=NULL;
             f->lungime_obiect_pe_2=lungime/2+in_plus+raza+1;
             f->inaltime_obiect_pe_2=raza+4;// +4 este de la raza cercul mic
             Beep(400,100);

             while(!avem_xy)
                determinare_xy_obiect(f);

             f->salvare_x=f->x;
             f->salvare_y=f->y;

             clearmouseclick(WM_LBUTTONDOWN);
         }

    }
}

void evaluare_text(char text[21],double &valoare,bool &eroare) //Bianca
{


    functie F;
    strcpy(F.expresie,text);

    for (unsigned int i=0; i<strlen(F.expresie); i++)
        if (F.expresie[i]==' ')
            {strcpy(F.expresie+i, F.expresie+1+i); i--;}
    for (unsigned int i=0; i<strlen(F.expresie); i++)
    {
        if (F.expresie[i]=='s' && F.expresie[i+1]=='i' && F.expresie[i+2]=='n')
           strcpy(F.expresie+i+1, F.expresie+3+i);
        else if (F.expresie[i]=='c' && F.expresie[i+1]=='o' && F.expresie[i+2]=='s')
           strcpy(F.expresie+i+1, F.expresie+3+i);
        else if (F.expresie[i]=='l' && F.expresie[i+1]=='o' && F.expresie[i+2]=='g')
           strcpy(F.expresie+i+1, F.expresie+3+i);
        else if (F.expresie[i]=='a' && F.expresie[i+1]=='b' && F.expresie[i+2]=='s')
           strcpy(F.expresie+i+1, F.expresie+3+i);
        else if (F.expresie[i]=='r' && F.expresie[i+1]=='a' && F.expresie[i+2]=='d')
           strcpy(F.expresie+i+1, F.expresie+3+i);
        else if (F.expresie[i]=='e' && F.expresie[i+1]=='x' && F.expresie[i+2]=='p')
           strcpy(F.expresie+i+1, F.expresie+3+i);
        else if (F.expresie[i]=='t' && F.expresie[i+1]=='g')
           strcpy(F.expresie+i+1, F.expresie+2+i);
        else if (F.expresie[i]=='c' && F.expresie[i+1]=='t' && F.expresie[i+2]=='g')
           {strcpy(F.expresie+i+1, F.expresie+3+i); F.expresie[i]='g';}
        else if (F.expresie[i]=='!' && F.expresie[i+1]=='=')
           strcpy(F.expresie+i+1, F.expresie+2+i);
        else if (F.expresie[i]=='=' && F.expresie[i+1]=='=')
           strcpy(F.expresie+i+1, F.expresie+2+i);
        else if (F.expresie[i]=='<' && F.expresie[i+1]=='=')
           {strcpy(F.expresie+i+1, F.expresie+2+i); F.expresie[i]='[';}
        else if (F.expresie[i]=='>' && F.expresie[i+1]=='=')
           {strcpy(F.expresie+i+1, F.expresie+2+i); F.expresie[i]=']';}
        else if (F.expresie[i]=='p' && F.expresie[i+1]=='i')
           {strcpy(F.expresie+i+1, F.expresie+2+i); F.expresie[i]='q';}
        else if (F.expresie[i]=='-' && (strchr(OperatiiBinare, F.expresie[i-1]) || strchr("()", F.expresie[i-1])))
            F.expresie[i]='_';
    }
    for (unsigned int i=0; i<strlen(F.expresie); i++)
        F.vect[i][0]=F.expresie[i];
    if (strstr(F.expresie, "l()") || strstr(F.expresie, "r()") || strstr(F.expresie, "e()") || strstr(F.expresie, "s()") || strstr(F.expresie, "c()") || strstr(F.expresie, "t()") || strstr(F.expresie, "g()") || strstr(F.expresie, "a()"))
    {
        printf ("%s", "EROARE!");
        //cout<<"&1 ";
        eroare=true;
        return ;
    }
    for (unsigned int i=0; i<strlen(F.expresie)-1; i++)
        if (F.expresie[i]=='(' && strchr(OperatiiBinare, F.expresie[i+1]) && F.expresie[i+1]!='-')
        {
            printf ("%s", "EROARE!");
            //cout<<"&2 ";
            eroare=true;
            return ;
        }
    for (unsigned int i=1; i<strlen(F.expresie); i++)
        if (F.expresie[i]==')' && strchr(OperatiiBinare, F.expresie[i-1]))
        {
            printf ("%s", "EROARE!");
            //cout<<"&3 ";
            eroare=true;
            return ;
        }
    F.lung=strlen(F.expresie);
    //printf("%s", F.expresie);
    double a=ValoareFunctie(F);
    if (a==infinit)
    {
        //printf ("%s", "EROARE!");
        //cout<<"&4 ";
        eroare=true;
        return ;
    }
    else
    {
       // printf("%f",a);

        valoare=a;
    }


}

void editare_text(forma* f,forma* prima) //Silviu
{
    setfillstyle(SOLID_FILL,COLOR(135,154,103));
    bar(sfarsit_ecran_x+1,ecran_H-40+1,ecran_W,ecran_H);
    int a=mousex();
    int b=mousey();
    int culoare=COLOR(135,123,164);
    if((a>f->x-f->lungime_obiect_pe_2 && a<f->x+f->lungime_obiect_pe_2) && (b>f->y-f->inaltime_obiect_pe_2 && b<f->y+f->inaltime_obiect_pe_2) && (getpixel(a,b)==COLOR(135,123,164) || getpixel(a,b)==MAGENTA))
    {
        if(ismouseclick(WM_RBUTTONDOWN))
        {
            if(f->denumire==3)
            {
                clearmouseclick(WM_RBUTTONDOWN);
                char ch[21];
                Beep(400,100);
                int i=0;
                ch[0]='(';
                ch[1]=')';
                i++;
                i++;
                setcolor(BLACK);
                ch[i]='\0';
                i--;
                bgiout<<ch;
                setbkcolor(COLOR(135,154,103));
                outstreamxy((sfarsit_ecran_x+ecran_W)/2-textwidth(ch)/2,(ecran_H-40/2)-16/2);
                char c='`';
                while(c!=13 && i<=20)
                {
                    if (kbhit())
                    {

                        c = getch();
                        if((c>='0'&&c<='9')||(c>='A'&&c<='Z')||c=='+'||c=='-'||c=='*'||c=='/'||c=='%'||c=='^'||c=='('||c==')'||c==' '||c=='s'||c=='i'||c=='n'||c=='c'||c=='o'||c=='t'
                           ||c=='g'||c=='a'||c=='b'||c=='r'||c=='d'||c=='l'||c=='>'||c=='<'||c=='!'||c=='='||c=='.')
                        {
                            ch[i]=c;
                            i++;
                            setfillstyle(SOLID_FILL,COLOR(135,154,103));
                            bar(sfarsit_ecran_x+1,ecran_H-40+1,ecran_W,ecran_H);
                            ch[i]=')';
                            i++;
                            setcolor(BLACK);
                            ch[i]='\0';
                            i--;
                            bgiout<<ch;
                            setbkcolor(COLOR(135,154,103));
                            outstreamxy((sfarsit_ecran_x+ecran_W)/2-textwidth(ch)/2,(ecran_H-40/2)-16/2);

                        }

                    }
                }
                Beep(400,100);
                clearmouseclick(WM_RBUTTONDOWN);


                setfillstyle(SOLID_FILL,COLOR(135,154,103));
                bar(sfarsit_ecran_x+1,ecran_H-40+1,ecran_W,ecran_H);


                i++;

                if(i<21)
                    ch[i]='\0';

                strcpy(f->text,ch);
                f->lungime=textwidth(f->text);

                setfillstyle(SOLID_FILL,COLOR(135,154,103));
                bar(f->x-f->lungime_obiect_pe_2,f->y-f->inaltime_obiect_pe_2,f->x+f->lungime_obiect_pe_2,f->y+f->inaltime_obiect_pe_2);
                while(prima!=NULL)
                {
                    if(prima!=f)
                        desen_legaturi(f,prima,COLOR(135,154,103));
                    prima=prima->urm;
                }


                f->lungime_obiect_pe_2=f->lungime/2*2+in_plus+raza+4;

                desen_forma(f,culoare);
                char t[21];
                strcpy(t,f->text);
                double d;
                bool error=false;
                evaluare_text(t,d,error);

                clearmouseclick(WM_RBUTTONDOWN);
            }
            if(f->denumire==4)
            {
                clearmouseclick(WM_RBUTTONDOWN);

                char ch[21];
                Beep(400,100);
                char c='~';
                while(c!=13)
                {
                    if (kbhit())
                    {

                        c = getch();
                        if((c>='A'&&c<='Z'))
                        {
                            ch[0]=c;
                            setfillstyle(SOLID_FILL,COLOR(135,154,103));
                            bar(sfarsit_ecran_x+1,ecran_H-40+1,ecran_W,ecran_H);

                            setcolor(BLACK);
                            ch[1]='\0';
                            bgiout<<ch;
                            setbkcolor(COLOR(135,154,103));
                            outstreamxy((sfarsit_ecran_x+ecran_W)/2-textwidth(ch)/2,(ecran_H-40/2)-16/2);
                        }
                    }
                }
                Beep(400,100);
                ch[1]='\0';
                strcpy(f->text,ch);
                f->lungime=textwidth(ch);


                setfillstyle(SOLID_FILL,COLOR(135,154,103));
                bar(f->x-f->lungime_obiect_pe_2,f->y-f->inaltime_obiect_pe_2,f->x+f->lungime_obiect_pe_2,f->y+f->inaltime_obiect_pe_2);
                while(prima!=NULL)
                {
                    if(prima!=f)
                        desen_legaturi(f,prima,COLOR(135,154,103));
                    prima=prima->urm;
                }
                f->lungime_obiect_pe_2=f->lungime/2+in_plus+raza+1;

                desen_forma(f,culoare);

                clearmouseclick(WM_RBUTTONDOWN);
            }
            if(f->denumire==5)
            {
                clearmouseclick(WM_RBUTTONDOWN);

                char ch[21];
                ch[0]='`';
                Beep(400,100);
                char c='`';
                while(c!=13)
                {
                    if (kbhit())
                    {

                        c = getch();
                        if((c>='A'&&c<='Z'))
                        {
                            ch[0]=c;
                            setfillstyle(SOLID_FILL,COLOR(135,154,103));
                            bar(sfarsit_ecran_x+1,ecran_H-40+1,ecran_W,ecran_H);

                            setcolor(BLACK);
                            ch[1]='\0';
                            bgiout<<ch;
                            setbkcolor(COLOR(135,154,103));
                            outstreamxy((sfarsit_ecran_x+ecran_W)/2-textwidth(ch)/2,(ecran_H-40/2)-16/2);
                        }
                    }
                }
                Beep(400,100);
                ch[1]='\0';
                strcpy(f->text,ch);
                f->lungime=textwidth(ch);


                setfillstyle(SOLID_FILL,COLOR(135,154,103));
                bar(f->x-f->lungime_obiect_pe_2,f->y-f->inaltime_obiect_pe_2,f->x+f->lungime_obiect_pe_2,f->y+f->inaltime_obiect_pe_2);
                while(prima!=NULL)
                {
                    if(prima!=f)
                        desen_legaturi(f,prima,COLOR(135,154,103));
                    prima=prima->urm;
                }
                f->lungime_obiect_pe_2=f->lungime/2+in_plus+raza+1;
                desen_forma(f,culoare);

                clearmouseclick(WM_RBUTTONDOWN);
            }
            if(f->denumire==6)
            {
                clearmouseclick(WM_RBUTTONDOWN);
                char ch[21];
                Beep(400,100);
                int i=0;
                char c='`';
                while(c!=13 || i<=0)
                {
                    if (kbhit())
                    {

                        c = getch();
                        if((c>='A'&&c<='Z'))
                        {
                            i=0;
                            ch[i]=c;
                            i=1;
                            setfillstyle(SOLID_FILL,COLOR(135,154,103));
                            bar(sfarsit_ecran_x+1,ecran_H-40+1,ecran_W,ecran_H);

                            setcolor(BLACK);
                            ch[i]='\0';
                            bgiout<<ch;
                            setbkcolor(COLOR(135,154,103));
                            outstreamxy((sfarsit_ecran_x+ecran_W)/2-textwidth(ch)/2,(ecran_H-40/2)-16/2);
                        }
                    }
                }
                Beep(400,100);
                clearmouseclick(WM_RBUTTONDOWN);

                ch[i]='<';
                i++;
                ch[i]='-';
                i++;
                ch[i]='(';
                i++;
                ch[i]=')';
                i++;
                setcolor(BLACK);
                ch[i]='\0';
                i--;
                bgiout<<ch;
                setbkcolor(COLOR(135,154,103));
                outstreamxy((sfarsit_ecran_x+ecran_W)/2-textwidth(ch)/2,(ecran_H-40/2)-16/2);
                int salvare_i=i;
                c='`';
                while((c!=13|| i==salvare_i)&& i<=19)
                {
                    if (kbhit())
                    {

                        c = getch();
                        if((c>='0'&&c<='9')||(c>='A'&&c<='Z')||c=='+'||c=='-'||c=='*'||c=='/'||c=='%'||c=='^'||c=='('||c==')'||c==' '||c=='s'||c=='i'||c=='n'||c=='c'||c=='o'||c=='t'||c=='g'||c=='a'||c=='b'||c=='r'||c=='d'||c=='l'||c=='.')
                        {
                            ch[i]=c;
                            i++;
                            ch[i]=')';
                            i++;
                            setfillstyle(SOLID_FILL,COLOR(135,154,103));
                            bar(sfarsit_ecran_x+1,ecran_H-40+1,ecran_W,ecran_H);

                            setcolor(BLACK);
                            ch[i]='\0';
                            i--;
                            bgiout<<ch;
                            setbkcolor(COLOR(135,154,103));
                            outstreamxy((sfarsit_ecran_x+ecran_W)/2-textwidth(ch)/2,(ecran_H-40/2)-16/2);
                        }
                    }
                }
                i++;
                Beep(400,100);

                if(i<21)
                    ch[i]='\0';

                strcpy(f->text,ch);
                f->lungime=textwidth(ch);

                setfillstyle(SOLID_FILL,COLOR(135,154,103));
                bar(f->x-f->lungime_obiect_pe_2,f->y-f->inaltime_obiect_pe_2,f->x+f->lungime_obiect_pe_2,f->y+f->inaltime_obiect_pe_2);
                 while(prima!=NULL)
                {
                    if(prima!=f)
                        desen_legaturi(f,prima,COLOR(135,154,103));
                    prima=prima->urm;
                }
                f->lungime_obiect_pe_2=f->lungime/2+in_plus+raza+4;
                desen_forma(f,culoare);

                char t[21];
                strcpy(t,f->text+salvare_i-1);
                double d;
                bool error=false;
                evaluare_text(t,d,error);


                clearmouseclick(WM_RBUTTONDOWN);
            }
        }
    }
    if(ismouseclick(WM_RBUTTONUP))
       clearmouseclick(WM_RBUTTONDOWN);
    clearmouseclick(WM_RBUTTONUP);
}

void parcurgere(forma* f,forma* prima) //Silviu
{
    int s_x=f->x;
    int s_y=f->y;
    int culoare=COLOR(135,123,164);
    miscare_obiecte(f,prima);

    if(f->miscare==true && f->x-f->lungime_obiect_pe_2>=incepe_ecran_x && f->y-f->inaltime_obiect_pe_2>=incepe_ecran_y+4 && f->x+f->lungime_obiect_pe_2<=sfarsit_ecran_x && f->y+f->inaltime_obiect_pe_2<=sfarsit_ecran_y-4)
    {
        int j=f->x;
        int k=f->y;
        f->x=s_x;
        f->y=s_y;
        doar_parcurgere(f,prima,COLOR(135,154,103));
        f->miscare=false;
        f->x=j;
        f->y=k;

        desen_forma(f,culoare);

    }
    doar_parcurgere(f,prima,LIGHTMAGENTA);
    editare_text(f,prima);

    if(f->urm!=NULL)
        parcurgere(f->urm,prima);
    if(f->urm==NULL)
        adaugare_obiect(f,prima);

}

forma* salvare_pt_leg;

void legaturi(forma* f,forma* prima) //Silviu
{
    if(f->denumire!=2 && f->denumire!=3)
    {
        if(mousex()>=f->x-4 && mousex()<= f->x+4 && mousey()>=f->y+f->inaltime_obiect_pe_2-4-4 && mousey()<=f->y+f->inaltime_obiect_pe_2-4+4)
        {
            if(ismouseclick(WM_LBUTTONDOWN))
            {
                Beep(200,30);
                i_1=f->index;
                salvare_pt_leg=f;

            }
            clearmouseclick(WM_LBUTTONDOWN);
        }
    }
    if(f->denumire!=1 )
    {
        if(mousex()>=f->x-4 && mousex()<= f->x+4 && mousey()>=f->y-f->inaltime_obiect_pe_2+4-4 && mousey()<=f->y-f->inaltime_obiect_pe_2+4+4)
        {
            if(ismouseclick(WM_LBUTTONDOWN))
            {
                if(i_1!=0)
                    if(salvare_pt_leg->index!=f->index)
                    {
                        Beep(200,30);
                        i_2=f->index;
                    }
                clearmouseclick(WM_LBUTTONDOWN);
            }
        }

    }
    if(f->denumire==3 )
    {
        if(mousex()>=f->x-f->lungime_obiect_pe_2+4-4 && mousex()<=f->x-f->lungime_obiect_pe_2+4+4 && mousey()>=f->y+f->inaltime_obiect_pe_2-4-4 && mousey()<=f->y+f->inaltime_obiect_pe_2-4+4)
        {
            if(ismouseclick(WM_LBUTTONDOWN))
            {
                Beep(200,30);
                i_1=f->index;
                i_s=i_1;
                salvare_pt_leg=f;
            }
            clearmouseclick(WM_LBUTTONDOWN);
        }
        if(mousex()>=f->x+f->lungime_obiect_pe_2-4-4 && mousex()<=f->x+f->lungime_obiect_pe_2-4+4 && mousey()>=f->y+f->inaltime_obiect_pe_2-4-4 && mousey()<=f->y+f->inaltime_obiect_pe_2-4+4)
        {
            if(ismouseclick(WM_LBUTTONDOWN))
            {
                Beep(200,30);
                i_1=f->index;
                i_d=i_1;
                salvare_pt_leg=f;
            }
            clearmouseclick(WM_LBUTTONDOWN);
        }
    }

    if(i_1!=0 && i_2!=0)
    {
        if(salvare_pt_leg->denumire!=3)
        {if(salvare_pt_leg->c1!=0 )
        {

           while(prima!=NULL)
           {
               desen_legaturi(salvare_pt_leg,prima,COLOR(135,154,103));

                prima=prima->urm;
           }
        }

        if(salvare_pt_leg->c2!=0)
        {
            while(prima!=NULL)
           {
               desen_legaturi(salvare_pt_leg,prima,COLOR(135,154,103));

                prima=prima->urm;
           }
           }
        }

        if(salvare_pt_leg->denumire==3)
        {
            if(i_s!=0)
            {
                while(prima!=NULL)
                {
                    desen_legaturi(salvare_pt_leg,prima,COLOR(135,154,103));

                    prima=prima->urm;
                }
                salvare_pt_leg->c1=i_2;

            }
            if(i_d!=0)
            {
                while(prima!=NULL)
                {
                    desen_legaturi(salvare_pt_leg,prima,COLOR(135,154,103));

                    prima=prima->urm;
                }
                salvare_pt_leg->c2=i_2;

            }
            desen_legaturi(f,salvare_pt_leg,BLACK);
            i_s=0;
            i_d=0;

        }
        else
        {
            salvare_pt_leg->c1=i_2;

        }
        //cout<<i_1<<" "<<i_2<<" ";
        i_1=0;
        i_2=0;
        i_3=0;

    }



    if(ismouseclick(WM_LBUTTONUP))
        clearmouseclick(WM_LBUTTONDOWN);


    if(f->urm!=NULL && prima!=NULL)
        legaturi(f->urm,prima);
}

void stergere_legaturi(forma* f,forma* prima) //Silviu
{

    if(f->denumire!=2 && f->denumire!=3)
    {
        if(mousex()>=f->x-4 && mousex()<= f->x+4 && mousey()>=f->y+f->inaltime_obiect_pe_2-4-4 && mousey()<=f->y+f->inaltime_obiect_pe_2-4+4)
        {
            if(ismouseclick(WM_RBUTTONDOWN))
            {
                Beep(200,30);
                while(prima!=NULL)
                {
                    if(prima->index==f->c1)
                    {
                        desen_legaturi(f,prima,COLOR(135,154,103));

                    }
                    prima=prima->urm;
                }
                if(f->c1!=0)
                    f->c1=0;
                clearmouseclick(WM_RBUTTONDOWN);
            }

        }
    }

    if(f->denumire==3)
    {
        if(mousex()>=f->x-f->lungime_obiect_pe_2+4-4 && mousex()<=f->x-f->lungime_obiect_pe_2+4+4 && mousey()>=f->y+f->inaltime_obiect_pe_2-4-4 && mousey()<=f->y+f->inaltime_obiect_pe_2-4+4)
        {
            if(ismouseclick(WM_RBUTTONDOWN))
            {
                Beep(200,30);
                while(prima!=NULL)
                {
                    if(prima->index==f->c1)
                    {
                        desen_legaturi(f,prima,COLOR(135,154,103));

                    }
                    prima=prima->urm;
                }
                if(f->c1!=0)
                    f->c1=0;
                clearmouseclick(WM_RBUTTONDOWN);
            }

        }
        if(mousex()>=f->x+f->lungime_obiect_pe_2-4-4 && mousex()<=f->x+f->lungime_obiect_pe_2-4+4 && mousey()>=f->y+f->inaltime_obiect_pe_2-4-4 && mousey()<=f->y+f->inaltime_obiect_pe_2-4+4)
        {
            if(ismouseclick(WM_RBUTTONDOWN))
            {
                Beep(200,30);
                while(prima!=NULL)
                {
                    if(prima->index==f->c2)
                    {
                        desen_legaturi(f,prima,COLOR(135,154,103));

                    }
                    prima=prima->urm;
                }
                if(f->c2!=0)
                    f->c2=0;
                clearmouseclick(WM_RBUTTONDOWN);
            }

        }

    }

    if(f->urm!=NULL && prima!=NULL)
        stergere_legaturi(f->urm,prima);
    if(ismouseclick(WM_RBUTTONUP))
       clearmouseclick(WM_RBUTTONDOWN);
    clearmouseclick(WM_RBUTTONUP);
}

void salvare_proiect(forma* prima) //Silviu
{
    setfillstyle(SOLID_FILL,COLOR(135,154,103));
    bar(sfarsit_ecran_x+1,ecran_H-40+1,ecran_W,ecran_H);

    FILE* fis;
    clearmouseclick(WM_RBUTTONDOWN);
    char text_fisier[51];

    int i=0;
    char ch='`';
    while(ch!=13 && i<47)
    {
        if (kbhit())
        {

            ch = getch();
            if((ch>='0'&&ch<='9')||(ch>='A'&&ch<='Z')||(ch>='a'&&ch<='z'))
            {
                text_fisier[i]=ch;
                i++;
            }
            text_fisier[i]='\0';
            int a=textwidth(text_fisier);

            setfillstyle(SOLID_FILL,COLOR(135,154,103));
            bar(sfarsit_ecran_x+1,ecran_H-40+1,ecran_W,ecran_H);

            setcolor(BLACK);
            bgiout<<text_fisier;
            setbkcolor(COLOR(135,154,103));
            outstreamxy((sfarsit_ecran_x+ecran_W)/2-a/2,(ecran_H-40/2)-16/2);
        }
    }
    text_fisier[i]='.';
    i++;
    text_fisier[i]='s';
    i++;
    text_fisier[i]='s';
    i++;
    text_fisier[i]='s';
    i++;
    if(i<51)
        text_fisier[i]='\0';
    Beep(200,20);

    clearmouseclick(WM_RBUTTONDOWN);

    fis=fopen(text_fisier,"w");
    fseek(fis,0,SEEK_SET);


    while(prima!=NULL)
    {
        fprintf(fis,Forma_format_iesire,prima->x,prima->y,prima->lungime,prima->denumire,prima->lungime_obiect_pe_2,
            prima->inaltime_obiect_pe_2,prima->salvare_x,prima->salvare_y,prima->index,prima->c1,prima->c2,prima->text);
        prima=prima->urm;
    }

    fclose(fis);
    setfillstyle(SOLID_FILL,COLOR(135,154,103));
    bar(incepe_ecran_x+1,incepe_ecran_y+1,sfarsit_ecran_x,sfarsit_ecran_y);
    setfillstyle(SOLID_FILL,COLOR(135,154,103));
    bar(sfarsit_ecran_x+1,ecran_H-40+1,ecran_W,ecran_H);

}

void incarcare_proiect(forma* prima, bool &ok) //Silviu
{
    setfillstyle(SOLID_FILL,COLOR(135,154,103));
    bar(sfarsit_ecran_x+1,ecran_H-40+1,ecran_W,ecran_H);

    setfillstyle(SOLID_FILL,COLOR(135,154,103));
    bar(incepe_ecran_x+1,incepe_ecran_y+1,sfarsit_ecran_x,sfarsit_ecran_y);

    clearmouseclick(WM_RBUTTONDOWN);
    char text_fisier[51];

    int i=0;
    char ch;
    while(ch!=13 && i<47)
    {
        if (kbhit())
        {

            ch = getch();
            if((ch>='0'&&ch<='9')||(ch>='A'&&ch<='Z')||(ch>='a'&&ch<='z'))
            {
                text_fisier[i]=ch;
                i++;
            }
            text_fisier[i]='\0';
            int a=textwidth(text_fisier);

            setfillstyle(SOLID_FILL,COLOR(135,154,103));
            bar(sfarsit_ecran_x+1,ecran_H-40+1,ecran_W,ecran_H);

            setcolor(BLACK);
            bgiout<<text_fisier;
            setbkcolor(COLOR(135,154,103));
            outstreamxy((sfarsit_ecran_x+ecran_W)/2-a/2,(ecran_H-40/2)-16/2);

        }
    }
    text_fisier[i]='.';
    i++;
    text_fisier[i]='s';
    i++;
    text_fisier[i]='s';
    i++;
    text_fisier[i]='s';
    i++;
    if(i<51)
        text_fisier[i]='\0';
    Beep(200,20);

    clearmouseclick(WM_RBUTTONDOWN);
    FILE* fis;
    fis=fopen(text_fisier,"r");
    fseek(fis,0,SEEK_SET);
    int x,y;
    int lungime;
    unsigned int denumire;
    char text[21];
    int lungime_obiect_pe_2;
    int inaltime_obiect_pe_2;
    int salvare_x;
    int salvare_y;
    unsigned int index;
    unsigned int c1;
    unsigned int c2;




    if(fis!=NULL)
    {

        char buffer[200];
        fgets(buffer, 200, fis);
        while(!feof(fis) && buffer!=NULL)
        {
            indx++;


            sscanf(buffer,Forma_format_intrare,&x,&y,&lungime,&denumire,&lungime_obiect_pe_2,&inaltime_obiect_pe_2,&salvare_x,&salvare_y,&index,&c1,&c2,text);
            fgets(buffer, 200, fis);

            forma* f=new forma;

            f->x=x;
            f->y=y;
            f->lungime=lungime;
            f->denumire=denumire;
            strcpy(f->text,text);
            f->lungime_obiect_pe_2=lungime_obiect_pe_2;
            f->inaltime_obiect_pe_2=inaltime_obiect_pe_2;
            f->salvare_x=salvare_x;
            f->salvare_y=salvare_y;
            f->index=index;
            f->c1=c1;
            f->c2=c2;
            f->miscare=true;

            f->urm=NULL;
            if(f->denumire==1)
                exista_start=true;
            prima->urm=f;

            prima=prima->urm;
            Beep(200,20);
        }

    }
    else
    {
        cout<<"nu exista fisierul";
        ok=false;
    }


    fclose(fis);
    setfillstyle(SOLID_FILL,COLOR(135,154,103));
    bar(sfarsit_ecran_x+1,ecran_H-40+1,ecran_W,ecran_H);
}

void apasat_buton_sus_jos(int &buton) //Silviu
{
    unsigned int a=mousex();
    unsigned int b=mousey();
    unsigned int c;
    c=(sfarsit_ecran_x-incepe_ecran_x)/4;
    if(a>=incepe_ecran_x+1 && a<=incepe_ecran_x+c*1-1 && b>=0+1 && b<=incepe_ecran_y-1)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            buton=1;
            clearmouseclick(WM_LBUTTONDOWN);
        }
    }
    if(a>=incepe_ecran_x+c*1+1 && a<=incepe_ecran_x+c*2-1 && b>=0+1 && b<=incepe_ecran_y-1)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            buton=2;
            clearmouseclick(WM_LBUTTONDOWN);
        }
    }
    if(a>=incepe_ecran_x+c*2+1 && a<=incepe_ecran_x+c*3-1 && b>=0+1 && b<=incepe_ecran_y-1)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            buton=3;
            clearmouseclick(WM_LBUTTONDOWN);
        }
    }
    if(a>=incepe_ecran_x+c*3+1 && a<=incepe_ecran_x+c*4-1 && b>=0+1 && b<=incepe_ecran_y-1)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            buton=4;
            clearmouseclick(WM_LBUTTONDOWN);
        }
    }

    c=(sfarsit_ecran_x-incepe_ecran_x)/2;

    if(a>=incepe_ecran_x+1 && a<=incepe_ecran_x+c-1 && b>=sfarsit_ecran_y+1 && b<=sfarsit_ecran_y+20-1)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            buton=5;
            clearmouseclick(WM_LBUTTONDOWN);
        }
    }

    if(a>=incepe_ecran_x+c+1 && a<=incepe_ecran_x+2*c-1 && b>=sfarsit_ecran_y+1 && b<=sfarsit_ecran_y+20-1)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            buton=6;
            clearmouseclick(WM_LBUTTONDOWN);
        }
    }


    if(ismouseclick(WM_LBUTTONUP))
        clearmouseclick(WM_LBUTTONDOWN);
    clearmouseclick(WM_LBUTTONUP);

}

void parc_leg (legatura* l,unsigned int index) //Silviu
{
    if(l->index==index)
    {
        l->tip=7;
        return;
    }

    if(l->urm!=NULL)
        parc_leg(l->urm,index);
    if(l->urmst!=NULL)
        parc_leg(l->urmst,index);
    if(l->urmdr!=NULL)
        parc_leg(l->urmdr,index);
}

void det_urm(forma *prima,unsigned int c1,unsigned int c2,unsigned int denumire,unsigned int index1,legatura* l_1, bool& ok_4) //Silviu, Bianca
{

    forma* f=prima;
    bool ok=false;
    if(c1!=0 && denumire!=3)
        ok=true;
    if(denumire==3)
    {

        if(c2!=0 && c1!=0)
            ok=true;
    }


    if(ok==false)
    {

        if(denumire!=2)
        {
            cout<<" incorect";
            ok_4=false;
            return;
        }
    }
    f=prima;
    if(denumire!=2)
    {

    if(denumire!=3)
    {
            f=prima;
            while(f!=NULL)
            {

                if(f->index==c1)
                {
                    if(f->denumire==3)
                    {

                        bool ok=false;
                        lista_index* parcurg=prim_index->urm;
                        lista_index* salvare_pt_while=parcurg;

                        while(parcurg!=NULL)
                        {
                            if(parcurg->w && !parcurg->vizitat)
                            {
                                salvare_pt_while=parcurg;
                            }

                            parcurg=parcurg->urm;
                        }


                        parcurg=prim_index->urm;
                        while(parcurg!=salvare_pt_while)
                        {
                            if(f->index==parcurg->index)
                            {
                                cout<<"incorect";
                                ok_4=false;
                                return;
                            }
                            parcurg=parcurg->urm;
                        }


                        parcurg=prim_index->urm;
                        while(parcurg!=NULL)
                        {
                            if(f->index==parcurg->index)
                            {
                                if(parcurg->vizitat )
                                {
                                    cout<<"incorect";
                                    ok_4=false;
                                    return;
                                }
                                ok=true;
                                parcurg->w=true;
                                parc_leg(leg,parcurg->index);
                                break;
                            }
                            parcurg=parcurg->urm;
                        }
                        if(ok)
                        {

                            if(parcurg!=NULL)
                                parcurg=parcurg->urm;
                            while(parcurg!=NULL)
                            {
                                parcurg->vizitat=true;
                                parcurg=parcurg->urm;
                            }

                            return;
                        }
                        else
                        {


                            parcurg=prim_index;
                            lista_index* h=new lista_index;
                            h->index=f->index;
                            h->vizitat=false;
                            h->urm=NULL;

                            while(parcurg->urm!=NULL)
                                parcurg=parcurg->urm;
                            parcurg->urm=h;
                        }


                    }





                    legatura* l_2=new legatura;
                    strcpy(l_2->text,f->text);
                    l_2->tip=f->denumire;
                    l_2->index=f->index;
                    l_2->urm=NULL;
                    l_2->urmdr=NULL;
                    l_2->urmst=NULL;
                    l_1->urm=l_2;

                    det_urm(prima,f->c1,f->c2,f->denumire,f->index,l_2,ok_4);
                }

                f=f->urm;
            }

    }
    if(denumire==3)
    {
            f=prima;
            while(f!=NULL)
            {
                if(f->index==c1)
                {
                    if(f->denumire==3)
                    {
                         bool ok=false;
                        lista_index* parcurg=prim_index->urm;
                        lista_index* salvare_pt_while=parcurg;

                        while(parcurg!=NULL)
                        {
                            if(parcurg->w && !parcurg->vizitat)
                            {
                                salvare_pt_while=parcurg;
                            }

                            parcurg=parcurg->urm;
                        }

                        if(salvare_pt_while->w==true)
                        {
                        parcurg=prim_index->urm;
                        while(parcurg!=salvare_pt_while)
                        {
                            if(f->index==parcurg->index)
                            {
                                cout<<"incorect";
                                ok_4=false;
                                return;
                            }
                            parcurg=parcurg->urm;
                        }
                        }

                        parcurg=prim_index->urm;
                        while(parcurg!=NULL)
                        {
                            if(f->index==parcurg->index)
                            {
                                if(parcurg->vizitat )
                                {
                                    //cout<<f->index<<" ";
                                    cout<<"incorect";
                                    ok_4=false;
                                    return;
                                }
                                ok=true;
                                parcurg->w=true;
                                parc_leg(leg,parcurg->index);
                                break;
                            }
                            parcurg=parcurg->urm;
                        }
                        if(ok)
                        {
                            //cout<<"1";
                            if(parcurg!=NULL)
                                parcurg=parcurg->urm;
                            while(parcurg!=NULL)
                            {
                                parcurg->vizitat=true;
                                parcurg=parcurg->urm;
                            }
                            break;
                        }
                        else
                        {
                            parcurg=prim_index;
                            lista_index* h=new lista_index;
                            h->index=f->index;
                            h->vizitat=false;
                            h->urm=NULL;

                            while(parcurg->urm!=NULL)
                                parcurg=parcurg->urm;
                            parcurg->urm=h;
                        }



                    }


                    legatura* l_2=new legatura;
                    strcpy(l_2->text,f->text);
                    l_2->tip=f->denumire;
                    l_2->index=f->index;
                    l_2->urm=NULL;
                    l_2->urmdr=NULL;
                    l_2->urmst=NULL;
                    l_1->urmst=l_2;

                    det_urm(prima,f->c1,f->c2,f->denumire,f->index,l_2,ok_4);
                }

                f=f->urm;
            }


            lista_index* parcurg=prim_index->urm;


            while(parcurg!=NULL)
            {
                if(parcurg->index==index1)
                {

                    parcurg->vizitat=true;

                }

                parcurg=parcurg->urm;
            }

            f=prima;
            while(f!=NULL)
            {
                if(f->index==c2)
                {
                    if(f->denumire==3)
                    {

                        lista_index* parcurg=prim_index->urm;
                        lista_index* salvare_pt_while=parcurg;

                        while(parcurg!=NULL)
                        {
                            if(parcurg->w && !parcurg->vizitat)
                            {
                                salvare_pt_while=parcurg;
                            }

                            parcurg=parcurg->urm;
                        }

                        if(salvare_pt_while->w==true)
                        {
                        parcurg=prim_index->urm;
                        while(parcurg!=salvare_pt_while)
                        {
                            if(f->index==parcurg->index)
                            {
                                cout<<"incorect";
                                ok_4=false;
                                return;
                            }
                            parcurg=parcurg->urm;
                        }
                        }
                        parcurg=prim_index->urm;
                        while(parcurg!=NULL)
                        {
                            if(f->index==parcurg->index)
                            {
                                if(parcurg->vizitat )
                                {
                                    cout<<"incorect";
                                    ok_4=false;
                                    return;
                                }
                                return;
                            }
                            parcurg=parcurg->urm;
                        }

                            parcurg=prim_index;
                            lista_index* h=new lista_index;
                            h->index=f->index;
                            h->urm=NULL;

                            while(parcurg->urm!=NULL)
                                parcurg=parcurg->urm;
                            parcurg->urm=h;

                    }


                    legatura* l_2=new legatura;
                    strcpy(l_2->text,f->text);
                    l_2->tip=f->denumire;
                    l_2->index=f->index;
                    l_2->urm=NULL;
                    l_2->urmdr=NULL;
                    l_2->urmst=NULL;
                    l_1->urmdr=l_2;


                    det_urm(prima,f->c1,f->c2,f->denumire,f->index,l_2,ok_4);
                }

                f=f->urm;
            }

    }
    }

}

void det_cod(forma *prima,bool& ok) //Silviu, Bianca
{

    forma* f=prima;

    prim_index->urm=NULL;

    ok=false;
    while(f!=NULL)
    {
        if(f->denumire==1)
        {
            strcpy(leg->text,f->text);
            leg->tip=f->denumire;
            leg->index=f->index;
            leg->urm=NULL;
            leg->urmdr=NULL;
            leg->urmst=NULL;
            unsigned int c1=f->c1;
            unsigned int c2=f->c2;
            unsigned int denumire=1;
            unsigned int index1=f->index;
            ok=true;
            det_urm(prima,c1,c2,denumire,index1,leg,ok);

        }

        f=f->urm;
    }
    //cout<<"2";


}

void parcurger_cod(forma* prima) //Silviu, Bianca
{

    if(prima->denumire==4 || prima->denumire==6)
    {
        var.declar[prima->text[0]-'A']=1;

    }
    if(prima->urm!=NULL)
        parcurger_cod(prima->urm);

}

void restul_codului(legatura* l,int nr, unsigned int& k) //Silviu, Bianca
{

    char ty[100]="";
    char t='\n';
    if(l->tip!=3 && l->tip!=7)
    {
        if(l->tip==2)
        {
            for(int i=1;i<=nr;i++)
                strcat(ty,"   ");
            strcat(ty,"return 0;");

            bgiout<<ty;
            outstreamxy(sfarsit_ecran_x+2,k*16/2);

            k++;
            bgiout<<t;
            outstreamxy(sfarsit_ecran_x+2,k*16/2);

            k++;
        }
        if(l->tip==4)
        {
            for(int i=1;i<=nr;i++)
                strcat(ty,"   ");

            strcat(ty,"cin>>");
            strcat(ty,l->text);


            while(l!=NULL  &&l->urm!=NULL&& l->urm->tip==4)
            {
                strcat(ty,">>");
                strcat(ty,l->urm->text);

                l=l->urm;
            }
            strcat(ty,";");
            bgiout<<ty;
            outstreamxy(sfarsit_ecran_x+2,k*16/2);

            k++;
            bgiout<<t;
            outstreamxy(sfarsit_ecran_x+2,k*16/2);

            k++;
        }
        if(l->tip==5)
        {
            for(int i=1;i<=nr;i++)
                strcat(ty,"   ");
            strcat(ty,"cout<<");
            strcat(ty,l->text);
            while(l!=NULL &&l->urm!=NULL&& l->urm->tip==5)
            {
                strcat(ty,"<<");
                strcat(ty,l->urm->text);
                l=l->urm;
            }
            strcat(ty,";");
            bgiout<<ty;
            outstreamxy(sfarsit_ecran_x+2,k*16/2);

            k++;
            bgiout<<t;
            outstreamxy(sfarsit_ecran_x+2,k*16/2);

            k++;
        }
        if(l->tip==6)
        {
            for(int i=1;i<=nr;i++)
                strcat(ty,"   ");
            unsigned int j=strlen(ty);
            ty[j]=l->text[0];
            ty[j+1]='\0';
            strcat(ty,"=");
            for(unsigned int i=4;i<strlen(l->text)-1;i++)
            {
                j=strlen(ty);
                ty[j]=l->text[i];
                ty[j+1]='\0';

            }
            strcat(ty,";");
            bgiout<<ty;
            outstreamxy(sfarsit_ecran_x+2,k*16/2);

            k++;
            bgiout<<t;
            outstreamxy(sfarsit_ecran_x+2,k*16/2);

            k++;
        }
        if(l->urm!=NULL && l!=NULL)
            restul_codului(l->urm,nr,k);
    }
    else
    {
        if(l->tip==3)
        {
            for(int i=1;i<=nr;i++)
                strcat(ty,"   ");
            strcat(ty,"if (");
            for(unsigned int i=1;i<=strlen(l->text)-2;i++)
                {
                    unsigned int j;
                    j=strlen(ty);
                    ty[j]=l->text[i];
                    ty[j+1]='\0';
                }
            strcat(ty,")");
            bgiout<<ty;
            outstreamxy(sfarsit_ecran_x+2,k*16/2);

            k++;
            bgiout<<t;
            outstreamxy(sfarsit_ecran_x+2,k*16/2);

            k++;

            ty[0]='\0';
            for(int i=1;i<=nr;i++)
                strcat(ty,"   ");

            strcat(ty,"{");
            bgiout<<ty;
            outstreamxy(sfarsit_ecran_x+2,k*16/2);

            k++;
            bgiout<<t;
            outstreamxy(sfarsit_ecran_x+2,k*16/2);

            k++;
        }
        if(l->tip==7)
        {
            for(int i=1;i<=nr;i++)
                strcat(ty,"   ");
            strcat(ty,"while (");
            for(unsigned int i=1;i<=strlen(l->text)-2;i++)
                {
                    unsigned int j;
                    j=strlen(ty);
                    ty[j]=l->text[i];
                    ty[j+1]='\0';
                }
            strcat(ty,")");
            bgiout<<ty;
            outstreamxy(sfarsit_ecran_x+2,k*16/2);

            k++;
            bgiout<<t;
            outstreamxy(sfarsit_ecran_x+2,k*16/2);

            k++;

            ty[0]='\0';
            for(int i=1;i<=nr;i++)
                strcat(ty,"   ");

            strcat(ty,"{");
            bgiout<<ty;
            outstreamxy(sfarsit_ecran_x+2,k*16/2);

            k++;
            bgiout<<t;
            outstreamxy(sfarsit_ecran_x+2,k*16/2);

            k++;
        }
        if(l->urmst!=NULL)
            restul_codului(l->urmst,nr+1,k);
        ty[0]='\0';
        for(int i=1;i<=nr;i++)
                strcat(ty,"   ");
        strcat(ty,"}");
        bgiout<<ty;
        outstreamxy(sfarsit_ecran_x+2,k*16/2);

        k++;
        bgiout<<t;
        outstreamxy(sfarsit_ecran_x+2,k*16/2);

        k++;

        if(l->tip==3)
        {
            ty[0]='\0';
            for(int i=1;i<=nr;i++)
                strcat(ty,"   ");

            strcat(ty,"else");
            bgiout<<ty;
            outstreamxy(sfarsit_ecran_x+2,k*16/2);

            k++;
            bgiout<<t;
            outstreamxy(sfarsit_ecran_x+2,k*16/2);

            k++;
            ty[0]='\0';
            for(int i=1;i<=nr;i++)
                strcat(ty,"   ");
            strcat(ty,"{");
            bgiout<<ty;
            outstreamxy(sfarsit_ecran_x+2,k*16/2);

            k++;
            bgiout<<t;
            outstreamxy(sfarsit_ecran_x+2,k*16/2);

            k++;
        }
        if(l->urmdr!=NULL)
        {
            if(l->tip==3)
                restul_codului(l->urmdr,nr+1,k);
            if(l->tip==7)
                restul_codului(l->urmdr,nr,k);
        }
        if(l->tip==3)
        {
            ty[0]='\0';
            for(int i=1;i<=nr;i++)
                strcat(ty,"   ");
            strcat(ty,"}");
            bgiout<<ty;
            outstreamxy(sfarsit_ecran_x+2,k*16/2);

            k++;
            bgiout<<t;
            outstreamxy(sfarsit_ecran_x+2,k*16/2);

            k++;
        }

    }

}


void sterge_forma(forma* prima) //Silviu
{
    unsigned int i=0;
    forma* f=prima->urm;
    while(i==0)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {

            int a=mousex();
            int b=mousey();
            f=prima->urm;
            while(f!=NULL)
            {
                if((a>f->x-f->lungime_obiect_pe_2 && a<f->x+f->lungime_obiect_pe_2) && (b>f->y-f->inaltime_obiect_pe_2 && b<f->y+f->inaltime_obiect_pe_2))
                {
                    i=f->index;


                }
                f=f->urm;
            }
            clearmouseclick(WM_LBUTTONDOWN);
        }
    }

    if(prima->urm->index==i)
    {

        f=prima->urm;
        if(f->denumire==1)
            exista_start=false;
        doar_parcurgere(f,prima->urm,COLOR(135,154,103));
        setfillstyle(SOLID_FILL,COLOR(135,154,103));
        bar(f->x-f->lungime_obiect_pe_2,f->y-f->inaltime_obiect_pe_2,f->x+f->lungime_obiect_pe_2,f->y+f->inaltime_obiect_pe_2);

        prima->urm=prima->urm->urm;

        f=prima->urm;

        while(f!=NULL)
        {

            f->index--;
            if(f->c1==i)
            {
                f->c1=0;
            }
            if(f->c2==i)
            {
                f->c2=0;
            }
            if(f->c1!=0)
            {
                f->c1--;
            }
            if(f->c2!=0)
            {
                f->c2--;
            }
            f=f->urm;
        }

    }
    else
    {

        f=prima->urm->urm;
        forma* anterior=prima->urm;
        while(f!=NULL)
        {
            if(f->index==i)
            {
                if(f->denumire==1)
                    exista_start=false;
                doar_parcurgere(f,prima->urm,COLOR(135,154,103));
                setfillstyle(SOLID_FILL,COLOR(135,154,103));
                bar(f->x-f->lungime_obiect_pe_2,f->y-f->inaltime_obiect_pe_2,f->x+f->lungime_obiect_pe_2,f->y+f->inaltime_obiect_pe_2);

                anterior->urm=f->urm;
            }

            if(f!=NULL)
            {
                anterior=f;
                f=f->urm;
            }
        }
        f=prima->urm;
        while(f!=NULL )
        {
            if(f->index>i)
                f->index--;
            if(f->c1>i)
                f->c1--;
            if(f->c2>i)
                f->c2--;
            if(f->c1==i)
                f->c1=0;
            if(f->c2==i)
                f->c2=0;
            f=f->urm;
        }
    }

}

void build(legatura* l,forma* prima,int &nr) //Silviu, Bianca
{
    setbkcolor(COLOR(135,154,103));
    setcolor(BLACK);

    char ty[60]="";
    char t='\n';
    int culoare=RED;
    forma* f=prima;
    delay(200);

    if( l->tip!=2 )
    {
        if(l->tip==3)
        {
            bool eroare=false;
            double val;
            f=prima;
            while(l->index!=f->index)
                f=f->urm;
            desen_forma(f,culoare);
            evaluare_text(l->text,val,eroare);
            setbkcolor(COLOR(135,154,103));
            setcolor(BLACK);

            if(eroare)
            {
                strcpy(ty,"eroare");
                bgiout<<ty;
                outstreamxy(sfarsit_ecran_x+2,nr*16/2);
                //cout<<"eroare";
                return;
            }
            else
            {
                if(val==1)
                    {if(l->urmst!=NULL)
                        build(l->urmst,prima,nr);
                    }
                else
                    {
                    if(l->urmdr!=NULL)
                        build(l->urmdr,prima,nr);
                        }
            }
        }
        if(l->tip==4)
        {
            f=prima;
            while(l->index!=f->index)
                f=f->urm;
            desen_forma(f,culoare);
            setbkcolor(COLOR(135,154,103));
            setcolor(BLACK);

            strcpy(ty,"introdu valoarea lui ");
            ty[21]=l->text[0];

            ty[22]='\0';
            bgiout<<ty;
            outstreamxy(sfarsit_ecran_x+2,nr*16/2);
            nr++;
            bgiout<<t;
            outstreamxy(sfarsit_ecran_x+2,nr*16/2);
            nr++;

            ty[0]='\0';


            int j=0;
            char c='e';
            while(c!=13)
            {
                if (kbhit())
                    c = getch();
                if((c>='0'&&c<='9')||c=='.'||(c=='-' && j==0))
                    {
                        ty[j]=c;
                        j++;
                        ty[j]='\0';
                        bgiout<<ty;
                        outstreamxy(sfarsit_ecran_x+10,nr*16/2);
                        c = 'e';
                    }
            }
            nr++;
            bgiout<<t;
            outstreamxy(sfarsit_ecran_x+2,nr*16/2);
            nr++;

            double introdus;
            introdus=atof(ty);

            if(strchr(ty,'.'))
            {
                for(int u=strlen(ty)-1;u>=0;u--)
                    if(ty[u]=='0')
                        ty[u]='\0';
                    else
                        break;
                if(ty[strlen(ty)-1]=='.')
                    ty[strlen(ty)-1]='\0';
            }
            //cout<<ty;

            for (int i=0; i<26; i++)
            {
                if(var.alph[i]==l->text[0])
                {
                    var.vl[i]=introdus;
                    var.declar[i]=1;

                    setcolor(BLACK);
                    setfillstyle(SOLID_FILL,COLOR(135,154,103));
                    setbkcolor(COLOR(135,154,103));
                    bar(incepe_ecran_x+1,sfarsit_ecran_y+21,sfarsit_ecran_x,ecran_H);

                    for(int y=0; y<26; y++)
                    {
                        if(var.declar[y]==1)
                        {
                            string fghj;
                            fghj=to_String(var.vl[y]);
                            char jk[20];
                            jk[0]=var.alph[y];
                            jk[1]='=';
                            jk[2]='\0';
                            strcpy(jk+2,fghj.c_str());
                            if(strchr(jk,'.'))
                            {
                                for(int u=strlen(jk)-1;u>=0;u--)
                                    if(jk[u]=='0')
                                        jk[u]='\0';
                                    else
                                        break;
                                if(jk[strlen(jk)-1]=='.')
                                    jk[strlen(jk)-1]='\0';
                            }
                            //cout<<jk;
                            setcolor(BLACK);
                            setbkcolor(COLOR(135,154,103));
                            bgiout<<jk;
                            outstreamxy(incepe_ecran_x+2+y%7*((sfarsit_ecran_x-incepe_ecran_x)/7),sfarsit_ecran_y+20+1+(y/7*2+1)*16/2);

                        }
                        else
                        {
                            char jk[20];
                            jk[0]=var.alph[y];
                            jk[1]='=';
                            jk[2]='0';
                            jk[3]='\0';
                            setcolor(BLACK);
                            setbkcolor(COLOR(135,154,103));
                            bgiout<<jk;
                            outstreamxy(incepe_ecran_x+2+y%7*((sfarsit_ecran_x-incepe_ecran_x)/7),sfarsit_ecran_y+20+1+(y/7*2+1)*16/2);
                        }
                    }
                }

            }


            if(l->urm!=NULL)
            {
                build(l->urm,prima,nr);
            }

        }
        if(l->tip==5)
        {
            f=prima;
            while(l->index!=f->index)
                f=f->urm;
            desen_forma(f,culoare);
            setbkcolor(COLOR(135,154,103));
            setcolor(BLACK);

            strcpy(ty,"se afiseaza: ");
            bgiout<<ty;
            outstreamxy(sfarsit_ecran_x+2,nr*16/2);
            nr++;
            bgiout<<t;
            outstreamxy(sfarsit_ecran_x+2,nr*16/2);
            nr++;
            ty[0]='\0';

            for (int i=0; i<26; i++)
            {
                if(var.alph[i]==l->text[0])
                {
                    string fghj;
                    fghj=to_String(var.vl[i]);
                    strcpy(ty,fghj.c_str());
                    if(strchr(ty,'.'))
                    {
                        for(int u=strlen(ty)-1;u>=0;u--)
                            if(ty[u]=='0')
                                ty[u]='\0';
                            else
                                break;
                        if(ty[strlen(ty)-1]=='.')
                            ty[strlen(ty)-1]='\0';
                    }
                    bgiout<<ty;
                    outstreamxy(sfarsit_ecran_x+10,nr*16/2);
                    nr++;
                    bgiout<<t;
                    outstreamxy(sfarsit_ecran_x+2,nr*16/2);
                    nr++;
                    //cout<<var.vl[i];
                    //cout<<endl;
                }
            }
            if(l->urm!=NULL)
            {
                build(l->urm,prima,nr);
            }
        }
        if(l->tip==6)
        {
            f=prima;
            while(l->index!=f->index)
                f=f->urm;
            desen_forma(f,culoare);
            setbkcolor(COLOR(135,154,103));
            setcolor(BLACK);

            bool eroare=false;
            double valoare;
            char text[21];
            strcpy(text,l->text+3);
            evaluare_text(text,valoare,eroare);
            if(eroare)
            {
                strcpy(ty,"eroare");
                bgiout<<ty;
                outstreamxy(sfarsit_ecran_x+2,nr*16/2);
                return;
            }
            else
            {
                strcpy(ty,"valoarea lui ");
                ty[13]=l->text[0];
                ty[14]='\0';
                strcat(ty," este ");
                bgiout<<ty;
                outstreamxy(sfarsit_ecran_x+2,nr*16/2);
                nr++;
                bgiout<<t;
                outstreamxy(sfarsit_ecran_x+2,nr*16/2);
                nr++;

                string fghj;
                fghj=to_String(valoare);
                strcpy(ty,fghj.c_str());
                if(strchr(ty,'.'))
                {
                for(int u=strlen(ty)-1;u>=0;u--)
                    if(ty[u]=='0')
                        ty[u]='\0';
                    else
                        break;
                if(ty[strlen(ty)-1]=='.')
                    ty[strlen(ty)-1]='\0';
                }

                bgiout<<ty;
                outstreamxy(sfarsit_ecran_x+10,nr*16/2);
                nr++;

                bgiout<<t;
                outstreamxy(sfarsit_ecran_x+2,nr*16/2);
                nr++;
                for (int i=0; i<26; i++)
                {
                    if(var.alph[i]==l->text[0])
                    {
                        var.declar[i]=1;
                        var.vl[i]=valoare;
                    }
                }
                for (int i=0; i<26; i++)
                {
                    if(var.alph[i]==l->text[0])
                    {
                        var.vl[i]=valoare;
                        var.declar[i]=1;

                        setcolor(BLACK);
                        setfillstyle(SOLID_FILL,COLOR(135,154,103));
                        setbkcolor(COLOR(135,154,103));
                        bar(incepe_ecran_x+1,sfarsit_ecran_y+21,sfarsit_ecran_x,ecran_H);

                        for(int y=0; y<26; y++)
                        {
                            if(var.declar[y]==1)
                            {
                                string fghj;
                                fghj=to_String(var.vl[y]);
                                char jk[20];
                                jk[0]=var.alph[y];
                                jk[1]='=';
                                jk[2]='\0';
                                strcpy(jk+2,fghj.c_str());
                                if(strchr(jk,'.'))
                                {
                                    for(int u=strlen(jk)-1;u>=0;u--)
                                        if(jk[u]=='0')
                                            jk[u]='\0';
                                        else
                                            break;
                                    if(jk[strlen(jk)-1]=='.')
                                        jk[strlen(jk)-1]='\0';
                                }
                                //cout<<jk;
                                setcolor(BLACK);
                                setbkcolor(COLOR(135,154,103));
                                bgiout<<jk;
                                outstreamxy(incepe_ecran_x+2+y%7*((sfarsit_ecran_x-incepe_ecran_x)/7),sfarsit_ecran_y+20+1+(y/7*2+1)*16/2);

                            }
                            else
                            {
                                char jk[20];
                                jk[0]=var.alph[y];
                                jk[1]='=';
                                jk[2]='0';
                                jk[3]='\0';
                                setcolor(BLACK);
                                setbkcolor(COLOR(135,154,103));
                                bgiout<<jk;
                                outstreamxy(incepe_ecran_x+2+y%7*((sfarsit_ecran_x-incepe_ecran_x)/7),sfarsit_ecran_y+20+1+(y/7*2+1)*16/2);
                            }
                        }
                    }

                }
            }
            if(l->urm!=NULL)
                build(l->urm,prima,nr);
        }
        if(l->tip==7)
        {

            f=prima;
            while(l->index!=f->index)
                f=f->urm;
            desen_forma(f,culoare);
            setbkcolor(COLOR(135,154,103));
            setcolor(BLACK);
            double valoare;
            bool eroare=false;
            char text[21];
            strcpy(text,l->text);
            evaluare_text(text,valoare,eroare);
            if(eroare)
            {
                strcpy(ty,"eroare");
                bgiout<<ty;
                outstreamxy(sfarsit_ecran_x+2,nr*16/2);
                return;
            }
            else
            {
                while(!eroare && valoare==1)
                {
                    if(l->urmst!=NULL)
                        build(l->urmst,prima,nr);
                    evaluare_text(text,valoare,eroare);
                }

                if(!eroare && l->urmdr!=NULL)
                    build(l->urmdr,prima,nr);
                if(eroare)
                {
                    strcpy(ty,"eroare");
                    bgiout<<ty;
                    outstreamxy(sfarsit_ecran_x+2,nr*16/2);
                    return;
                }
            }
        }
    }
    f=prima;
    while(l->index!=f->index)
        f=f->urm;
    desen_forma(f,culoare);

}



int main() //Silviu, Bianca
{

    unsigned int i;
    var.alph[0]='A';
    for (i=1; i<26; i++)
        {var.alph[i]=var.alph[i-1]+1; }
    var.alph[26]='\0';

    initwindow(ecran_W,ecran_H,"interschem");
    setfillstyle(SOLID_FILL,COLOR(135,154,103));
    bar(0,0,ecran_W,ecran_H);


    desen_partea_stanga();
    desen_sus();
    desen_butoane_jos();

    setcolor(COLOR(137,249,163));
    line(incepe_ecran_x,0,incepe_ecran_x,ecran_H);
    line(sfarsit_ecran_x,0,sfarsit_ecran_x,ecran_H);
    line(incepe_ecran_x,incepe_ecran_y,sfarsit_ecran_x,incepe_ecran_y);
    line(incepe_ecran_x,sfarsit_ecran_y,sfarsit_ecran_x,sfarsit_ecran_y);
    line(sfarsit_ecran_x+1,ecran_H-40,ecran_W,ecran_H-40);//pentu coltul din dreapta jos unde se scrie numele fisierului de intrare sau iesire

    char yut[100]="t=0";
    int lungime_pt_afisare_variabile=(sfarsit_ecran_x-incepe_ecran_x)/7;
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<7;j++)
        {

            if(65+i*7+j<=90)
            {
                setcolor(BLACK);
                yut[0]=65+i*7+j;
                bgiout<<yut;
                outstreamxy(incepe_ecran_x+2+j*lungime_pt_afisare_variabile,sfarsit_ecran_y+20+1+(i*2+1)*16/2);
            }
        }
    }

    forma* primul;
    primul=NULL;

    forma* f=new forma;
    f->urm=NULL;
    int apasat=0;

    char ch='~';
    while(ch!=27)
    {

        if(primul==NULL)
        {
            //cout<<"2";
            primul=f;
            adaugare_obiect(primul);

            if(primul->urm==NULL)
                primul=NULL;
        }

        if(primul!=NULL)
        {

            parcurgere(primul->urm,primul->urm);
            legaturi(primul->urm,primul->urm);

            stergere_legaturi(primul->urm,primul->urm);

            //delay(50);
        }


        apasat_buton_sus_jos(apasat);
        if(apasat!=0)
        {
            if(apasat==1)
            {
                Beep(200,20);
                 setfillstyle(SOLID_FILL,COLOR(135,154,103));
                bar(incepe_ecran_x+1,sfarsit_ecran_y+21,sfarsit_ecran_x,ecran_H);

                    bar(sfarsit_ecran_x+1,0,ecran_W,ecran_H-41);
                for (int i=0; i<26; i++)
                {
                    var.vl[i]=0;
                    var.declar[i]=0;
                }
                for(int i=0;i<4;i++)
                {
                    for(int j=0;j<7;j++)
                    {

                        if(65+i*7+j<=90)
                        {
                            yut[0]=65+i*7+j;
                            setbkcolor(COLOR(135,154,103));
                            setcolor(BLACK);
                            bgiout<<yut;
                            outstreamxy(incepe_ecran_x+2+j*lungime_pt_afisare_variabile,sfarsit_ecran_y+20+1+(i*2+1)*16/2);
                        }
                    }
                }

                setcolor(COLOR(48,91,128));
                char ty[50];
                strcpy(ty,"numele fisierului");
                bgiout<<ty;
                setbkcolor(COLOR(135,154,103));
                outstreamxy((sfarsit_ecran_x+ecran_W)/2-textwidth(ty)/2,(ecran_H-40-10)-16/2);

                if(primul!=NULL)
                {
                    salvare_proiect(primul->urm);
                    primul->urm=NULL;
                    primul=NULL;
                }
                else
                    cout<<"nu exista niciun obiect pentru a salva fisierul";
                apasat=0;
                indx=0;

                setcolor(COLOR(135,154,103));
                bgiout<<ty;
                setbkcolor(COLOR(135,154,103));
                outstreamxy((sfarsit_ecran_x+ecran_W)/2-textwidth(ty)/2,(ecran_H-40-10)-16/2);
                exista_start=false;

            }
            if(apasat==2)
            {
                Beep(200,20);
                 setfillstyle(SOLID_FILL,COLOR(135,154,103));
                bar(incepe_ecran_x+1,sfarsit_ecran_y+21,sfarsit_ecran_x,ecran_H);

                    bar(sfarsit_ecran_x+1,0,ecran_W,ecran_H-41);
                for (int i=0; i<26; i++)
                {
                    var.vl[i]=0;
                    var.declar[i]=0;
                }
                for(int i=0;i<4;i++)
                {
                    for(int j=0;j<7;j++)
                    {

                        if(65+i*7+j<=90)
                        {
                            yut[0]=65+i*7+j;
                            setbkcolor(COLOR(135,154,103));
                            setcolor(BLACK);
                            bgiout<<yut;
                            outstreamxy(incepe_ecran_x+2+j*lungime_pt_afisare_variabile,sfarsit_ecran_y+20+1+(i*2+1)*16/2);
                        }
                    }
                }

                exista_start=false;

                setcolor(COLOR(48,91,128));
                char ty[50];
                strcpy(ty,"numele fisierului");
                bgiout<<ty;
                setbkcolor(COLOR(135,154,103));
                outstreamxy((sfarsit_ecran_x+ecran_W)/2-textwidth(ty)/2,(ecran_H-40-10)-16/2);

                primul=f;
                bool ok=true;
                incarcare_proiect(primul,ok);
                if(!ok)
                {
                    primul->urm=NULL;
                    primul=NULL;
                }
                apasat=0;

                setcolor(COLOR(135,154,103));
                bgiout<<ty;
                setbkcolor(COLOR(135,154,103));
                outstreamxy((sfarsit_ecran_x+ecran_W)/2-textwidth(ty)/2,(ecran_H-40-10)-16/2);
                //cout<<indx;
            }
            if(apasat==3)
            {
                setfillstyle(SOLID_FILL,COLOR(135,154,103));
                bar(incepe_ecran_x+1,sfarsit_ecran_y+21,sfarsit_ecran_x,ecran_H);
                for (int i=0; i<26; i++)
                {
                    var.vl[i]=0;
                    var.declar[i]=0;
                }
                for(int i=0;i<4;i++)
                {
                    for(int j=0;j<7;j++)
                    {

                        if(65+i*7+j<=90)
                        {
                            yut[0]=65+i*7+j;
                            setbkcolor(COLOR(135,154,103));
                            setcolor(BLACK);
                            bgiout<<yut;
                            outstreamxy(incepe_ecran_x+2+j*lungime_pt_afisare_variabile,sfarsit_ecran_y+20+1+(i*2+1)*16/2);
                        }
                    }
                }

                int nr_6=1;
                bar(sfarsit_ecran_x+1,0,ecran_W,ecran_H-41);

                int culoare=RED;
                bool ok_3=true;
                Beep(200,20);
                apasat=0;
                if(primul!=NULL)
                {
                det_cod(primul->urm,ok_3);
                if(ok_3)
                {

                    forma* h=primul->urm;
                    while(leg!=NULL && leg->tip!=h->denumire)
                        h=h->urm;
                    desen_forma(h,culoare);
                    build(leg->urm,primul->urm,nr_6);
                }
                delay(1500);
                forma* b=primul->urm;
                while(b!=NULL)
                {
                    //cout<<"2";
                    desen_forma(b,COLOR(135,123,164));
                    b=b->urm;
                }
                //cout<<"1";
                }
                }


            if(apasat==4)
            {
                Beep(200,20);

                if(primul!=NULL)
                {
                    setfillstyle(SOLID_FILL,COLOR(135,154,103));
                    bar(sfarsit_ecran_x+1,0,ecran_W,ecran_H-41);

                    bool ok_3=true;
                    det_cod(primul->urm,ok_3);

                    if(ok_3)
                    {

                    char ty[100];
                    char t='\n';

                    setcolor(BLACK);
                    setbkcolor(COLOR(135,154,103));

                    bgiout<<t;
                    outstreamxy(sfarsit_ecran_x+2,0+1*16/2);


                    strcpy(ty,"#include <iostream>");
                    bgiout<<ty;
                    outstreamxy(sfarsit_ecran_x+2,2*16/2);

                    bgiout<<t;
                    outstreamxy(sfarsit_ecran_x+2,3*16/2);
                    bgiout<<t;
                    outstreamxy(sfarsit_ecran_x+2,4*16/2);

                    strcpy(ty,"using namespace std;");
                    bgiout<<ty;
                    outstreamxy(sfarsit_ecran_x+2,5*16/2);

                    bgiout<<t;
                    outstreamxy(sfarsit_ecran_x+2,6*16/2);


                    bgiout<<t;
                    outstreamxy(sfarsit_ecran_x+2,7*16/2);

                    strcpy(ty,"int main()");
                    bgiout<<ty;
                    outstreamxy(sfarsit_ecran_x+2,8*16/2);

                    bgiout<<t;
                    outstreamxy(sfarsit_ecran_x+2,9*16/2);


                    strcpy(ty,"{");
                    bgiout<<ty;
                    outstreamxy(sfarsit_ecran_x+2,10*16/2);
                    bgiout<<t;
                    outstreamxy(sfarsit_ecran_x+2,11*16/2);

                    unsigned int k=12;

                    for(i=0;i<26;i++)
                        var.declar[i]=0;

                    bool ok_1=false;
                    bool ok_2=false;
                    parcurger_cod(primul->urm);
                    for( i=0;i<26 && ok_2==false;i++)
                    {
                        if(var.declar[i]!=0)
                        {
                            ok_2=true;
                        }
                    }
                    if(ok_2)
                    {
                        strcpy(ty,"   double ");


                        unsigned int i=0;
                        for( i=0;i<26 && ok_1==false;i++)
                        {
                            if(var.declar[i]==1 )
                            {
                                unsigned int j=strlen(ty);
                                ty[j]=var.alph[i];
                                ty[j+1]='\0';
                                ok_1=1;
                            }

                        }

                        for(;i<26;i++)
                        {
                            if(var.declar[i]==1)
                            {

                                strcat(ty,", ");

                                unsigned int j=strlen(ty);
                                ty[j]=var.alph[i];
                                ty[j+1]='\0';

                            }
                        }

                        strcat(ty,";");

                        bgiout<<ty;
                        outstreamxy(sfarsit_ecran_x+2,k*16/2);
                        k++;
                        bgiout<<t;
                        outstreamxy(sfarsit_ecran_x+2,k*16/2);
                        k++;
                    }
                    restul_codului(leg,1,k);

                    strcpy(ty,"}");
                    bgiout<<ty;
                    outstreamxy(sfarsit_ecran_x+2,k*16/2);
                    k++;
                    bgiout<<t;
                    outstreamxy(sfarsit_ecran_x+2,k*16/2);


                }
                else
                {
                    setcolor(BLACK);
                    setbkcolor(COLOR(135,154,103));


                    char ty[10]="incorect";
                    bgiout<<ty;
                    outstreamxy(sfarsit_ecran_x+2,0+1*16/2);

                }

                }
                apasat=0;
            }

            if(apasat==5)
            {
                Beep(200,20);
                apasat=0;
                setfillstyle(SOLID_FILL,COLOR(135,154,103));
                bar(incepe_ecran_x+1,incepe_ecran_y+1,sfarsit_ecran_x,sfarsit_ecran_y);
                if(primul!=NULL)
                {
                    primul->urm=NULL;
                    primul=NULL;
                }
                exista_start=false;
                indx=0;
            }

            if(apasat==6)
            {
                setbkcolor(COLOR(135,154,103));
                            setcolor(BLACK);
                lungime_pt_afisare_variabile=(sfarsit_ecran_x-incepe_ecran_x)/7;
                for(int i=0;i<4;i++)
                {
                    for(int j=0;j<7;j++)
                    {

                        if(65+i*7+j<=90)
                        {
                            yut[0]=65+i*7+j;
                            bgiout<<yut;
                            outstreamxy(incepe_ecran_x+2+j*lungime_pt_afisare_variabile,sfarsit_ecran_y+20+1+(i*2+1)*16/2);
                        }
                    }
                }
                Beep(200,20);
                apasat=0;
                if(primul!=NULL)
                {
                    sterge_forma(primul);
                    indx--;
                }
                if(primul!=NULL&&primul->urm==NULL)
                {

                    primul=NULL;
                }

            }


        }

    if (kbhit())
			ch = getch();
    }

    return 0;
}

