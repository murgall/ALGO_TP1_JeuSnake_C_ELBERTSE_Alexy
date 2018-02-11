#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>


#define ORIGIN_GRILLE 10
#define LONGUEUR_GRILLE 70
#define LARGEUR_GRILLE 30
#define TAILLE_SNAKE_MAX 35


typedef enum
{
    FAUX,VRAI,
} Booleen;

typedef struct
{
    int x;
    int y;
    char direction;
} xy;

typedef struct
{
    xy place;
    Booleen istete;
    Booleen istail;
    Booleen isbody;

} snake;

typedef struct
{
    xy place;
    Booleen ispomme;
}food;

void ecran_accueil();
void gotoXY(int a, int b);
void affich_info(int vie,int tailleSnake);
void Fail(snake serpent[TAILLE_SNAKE_MAX],int vie,xy coord_head,int tailleSnake,food pomme);
void Crea_grille();
food Food(food pomme);
void affich_Food(food pomme);
void move(snake serpent[TAILLE_SNAKE_MAX],xy coord_head,int tailleSnake,int vie,food pomme);
void Deplacement(xy coord_head,snake serpent[TAILLE_SNAKE_MAX],int tailleSnake);
void Clear_grille();
void hidecursor();



int main()
{
    hidecursor();
    //initialisation des variables
    srand(time(NULL));      // initialisation du random
    xy head;     //déclare le type objet de la tête  du serpent.
    food pomme;
    int Longueur_snake = 5;
    int i=0;
    int vie = 0;
    snake serpent[TAILLE_SNAKE_MAX];
    ecran_accueil();
    system("cls");
    Crea_grille();      //creation de la grille
    head.x = 25;
    head.y = 20;
    head.direction = 'D';
    for (i=1;i<Longueur_snake-1;i++)
    {
        serpent[i].place.x=head.x-i;
        serpent[i].place.y=head.y;
    }
    pomme= Food(pomme);
    move(serpent,head,Longueur_snake,vie,pomme);
    system("pause");
    return 0;
}

void gotoXY(int nb1, int nb2)
//BUT: Permet de pointer un coordonné précis dans la console
//ENTREE : un nombre nb1 servant de coordonné en x et un nombre nb2 servant de coordonnée en y
//SORTIE : La position pointé par les coordonnées nb1 et nb2
{
    HANDLE a;   //permet de référencer une structure
    COORD b;
    fflush(stdout);
    b.X = nb1;
    b.Y = nb2;
    a = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(a,b);
}


void ecran_accueil()
//BUT : Ecran d'introduction du jeu
{
    gotoXY(32,15);
    printf("Bienvenue sur Snake! Veuillez appuyer sur n'importe quelle touche pour continuer.\n");
    gotoXY(20,19);
    printf("L'objectif est d'avoir le plus grand score possible, sans que la tete du serpent\n");
    gotoXY(20,20);
    printf(" ne touche le bord du terrain ou son coprs(queue et corps).");
    getch();
}

void affich_info(int vie,int tailleSnake)
//BUT : Aide le joueur en lui montrant les input possible dans le jeu ainsi que le nombre de vie qu'il dispose
//ENTREE : le nombre de vie
//SORTIE : Information input + vie ectuelle
{
    int score = -5 +tailleSnake;
    gotoXY(60,5);
    printf("Utiliser les fleches directionnelles pour vous deplacer.");
    gotoXY(55,6);
    printf("Vous pouvez appuyer sur echap pour sortir du jeu a tout moment..");
    gotoXY(90,15);
    printf("LIFE : %d",vie);
    gotoXY(90,19);
    printf ("Score : %d", score);
}

void Fail(snake serpent[TAILLE_SNAKE_MAX],int vie,xy coord_head,int tailleSnake,food pomme)
//BUT : Permet de retirer une vie ou terminer le jeu si le serpent se mord ou va sur les bord de la grille.Si le joueur perd une vie, le serpent est réinitialisé et remis au point de départ
// ENTREE : Les coordonnées du serpent, le nombre de vie avant l'appel de cette procedure,la taille du serpent
// SORTIE : le nouveau nombre de vie ou le game over.
{

       vie--;
       if (vie>-1)      //tant que le joueur possède plusieurs vie, réinitialise le jeu
       {
           system("cls");
           Crea_grille();
           coord_head.x=25;
           coord_head.y=20;
           coord_head.direction='D';
           tailleSnake=5;
           pomme=Food(pomme);
           move(serpent,coord_head,tailleSnake,vie,pomme);

       }
       else
       {
            system("close");        //game over
            exit(0);
       }

}

void Crea_grille()
//BUT : crée la grille du jeu snake
{
    int i;
    for(i=ORIGIN_GRILLE;i<LONGUEUR_GRILLE+1;i++)
   {
       gotoXY(i,ORIGIN_GRILLE);     //crée le bord superieur de la grille
           printf("#");
       gotoXY(i,LARGEUR_GRILLE);    // crée le bord inferieur de la grille
           printf("#");
   }
   for(i=ORIGIN_GRILLE;i<LARGEUR_GRILLE+1;i++)
   {
       gotoXY(ORIGIN_GRILLE,i);     // crée le bord gauche de la grille
           printf("#");
       gotoXY(LONGUEUR_GRILLE,i);   // crée le bord droit de la grille
       printf("#");
   }
}



food Food(food pomme)
//BUT : Permet de generer aléatoirement la position de la pomme sur la grille
//ENTREE : la position actuelle de la pomme (définie ou non)
//SORTIE : La nouvelle position aléatoire de la pomme
{
    do
    {
        pomme.place.x=rand()%60 + 11;       //place aléatoirement la pomme sur la grille de jeu
        pomme.place.y=rand()%20 + 11;
    }while((pomme.place.x == LONGUEUR_GRILLE)||(pomme.place.y == LARGEUR_GRILLE));
    return pomme;
}

void affich_Food(food pomme)
//BUT : Permet d'afficher la position de la pomme sur la grille de jeu
//ENTREE : La position actuelle de la pomme
//SORTIE : l'affichage de la pomme
{
    gotoXY(pomme.place.x,pomme.place.y);        //pointe la position de la pomme
    printf("F");
}

void move(snake serpent[TAILLE_SNAKE_MAX],xy coord_head,int tailleSnake, int vie,food pomme)
//BUT : Gère le contenu de la grille (serpent,pomme) et surtout l'orientation du serpent ainsi que le déplacement lié à l'orientation
//ENTREE : coordonnée de la tête du serpent, la taille actuelle de celui ci, le nombre de vie restante ainsi que de la position de la pomme.
//SORTIE : L'orientation du serpent, la nouvelle position de la tête
{
    int i=0;
    fflush(stdin);
    do{
        for(i=1;i<tailleSnake-1;i++)        //vérifie si la tête du serpent se trouve sur le bord ou dans son corps
        {
            if (((coord_head.x==ORIGIN_GRILLE)||(coord_head.x==LONGUEUR_GRILLE)||(coord_head.y==ORIGIN_GRILLE)||(coord_head.y==LARGEUR_GRILLE))
            ||((serpent[0].place.x==serpent[i].place.x)&&(serpent[0].place.y==serpent[i].place.y)))
            {
                Fail(serpent,vie,coord_head,tailleSnake,pomme);
            }
        }

    Clear_grille();
    if ((coord_head.x==pomme.place.x)&&(coord_head.y==pomme.place.y))   //si le serpent mange la pomme
    {
        tailleSnake++;
        pomme=Food(pomme);
    }
    affich_Food(pomme);
        affich_info(vie,tailleSnake);
        if (coord_head.direction=='D')      //si le serpent se dirige à droite
        {
            Deplacement(coord_head,serpent,tailleSnake);
            Sleep(200);
            coord_head.x++;
        }
        else if (coord_head.direction=='G')      //si le serpent se dirige à gauche
        {
            Deplacement(coord_head,serpent,tailleSnake);
            Sleep(200);
            coord_head.x--;
        }
        else if (coord_head.direction=='H')      //si le serpent se dirige en haut
        {
            Deplacement(coord_head,serpent,tailleSnake);
            Sleep(200);
            coord_head.y--;
        }
        else if (coord_head.direction=='B')      //si le serpent se dirige en bas
        {
            Deplacement(coord_head,serpent,tailleSnake);
            Sleep(200);
            coord_head.y++;
        }
    }while(!kbhit());
    if((GetAsyncKeyState(VK_RIGHT)&&coord_head.direction!='D'&&coord_head.direction!='G')||
       (GetAsyncKeyState(VK_LEFT)&&coord_head.direction!='G'&&coord_head.direction!='D')||
       (GetAsyncKeyState(VK_UP)&&coord_head.direction!='B'&&coord_head.direction!='H')||
       (GetAsyncKeyState(VK_DOWN)&&coord_head.direction!='H'&&coord_head.direction!='B'))
    {
        if (GetAsyncKeyState(VK_UP))
            coord_head.direction = 'H';     //set la direction en haut
        if (GetAsyncKeyState(VK_DOWN))
            coord_head.direction = 'B';     //set la direction en bas
        if (GetAsyncKeyState(VK_LEFT))
            coord_head.direction = 'G';     //set la direction à gauche
        if (GetAsyncKeyState(VK_RIGHT))
            coord_head.direction = 'D';     //set la direction à droite


    }
    else if (GetAsyncKeyState(VK_ESCAPE))
        {
            system("close");
            exit(0);
        }
        move(serpent,coord_head,tailleSnake,vie,pomme);

}




void Deplacement(xy coord_head,snake serpent[TAILLE_SNAKE_MAX],int tailleSnake)
//BUT : Permet de faire déplacer le corps du serpent ainsi que d'afficher distinctement la tête, le corps et la queue du serpent
//ENTREE : La taille du serpent, les coordonnées de sa tête actuelle ainsi que de son corps
//SORTIE : Les nouvelles coordonnées du serpent ainsi que de l'affichage du serpent
{

    int i = tailleSnake;
    int tete = 0;
    for(i=0;i<tailleSnake-1;i++)    //réinitialise les conditions booleen du serpent
    {
        serpent[i].istete=FAUX;
        serpent[i].istail=FAUX;
        serpent[i].isbody=FAUX;
    }
    for(i=tailleSnake-1;i>0;i--)
    {
        serpent[i].place.x=serpent[i-1].place.x;    //place la partie précédente du corps à la position suivante(exemple : le corps serpent[1].place.x devient serpent[0].place.x)
        serpent[i].place.y=serpent[i-1].place.y;
        if (i==tailleSnake-1)
        {
             serpent[i].istail=VRAI;        //set la queue du serpent

        }
        else
        {
            serpent[i].isbody=VRAI;         //set le corps du serpent

        }
    }
    serpent[tete].place.x = coord_head.x;   //set la tête du serpent
    serpent[tete].place.y = coord_head.y;
    serpent[tete].istete = VRAI;
    for(i=0;i<tailleSnake;i++)
    {
        gotoXY(serpent[i].place.x,serpent[i].place.y);
        if (serpent[i].istail==VRAI)
        {
            printf("T");                //affiche la queue du serpent
        }
        else if (serpent[i].istete==VRAI)
        {
            printf("S");                //affiche la tête du serpent
        }
        else if (serpent[i].isbody==VRAI)
        {
        printf("0");                    //affiche le corps du serpent
        }

    }
}


void hidecursor()
//BUT : Permet de masquer le curseur dans la console pour le confort visuelle
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}


void Clear_grille()
//BUT : permet de nettoyer l'interieur dela grille de jeu
{
    int i;
    int j;
    for(i=ORIGIN_GRILLE+1;i<LONGUEUR_GRILLE;i++)
    {
        for(j=ORIGIN_GRILLE+1;j<LARGEUR_GRILLE;j++)
        {
            gotoXY(i,j);
            printf(" ");
        }
    }
}






