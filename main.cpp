#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <time.h>
#include <math.h>

typedef struct structOfSphere{  //Δομή για τις σφαίρες
    int PositionX;              //Θέση στον x άξονα(Κέντρο σφαίρας)
    int PositionY;              //Θέση στον y άξονα(Κέντρο σφαίρας)
    int Color;                  //Χρώμα της σφαίρας
    int Mass;                   //Μάζα της σφαίρας
    int Radius;                 //Ακτίνα της σφαίρας
    int SpeedX;                 //Ταχύτητα της σφαίρας σε pixels/sec
    int SpeedY;                 //Ταχύτητα της σφαίρας σε pixels/sec
}Sphere;

double twoPointDistance(double pointAX,double pointAY,double pointBX,double pointBY){ //Επιστρέφει την απόσταση 2 σημείων
    return sqrt(pow(pointBX-pointAX,2)+pow(pointBY-pointAY,2));
}

char spheresCollide(double distanceOfCenter,double radiusA,double radiusB){ //Ελέγχει αν δύο σφαίρες συγκρούονται
    if (distanceOfCenter<=(radiusA+radiusB)){
        return 1;
    }
    return 0;
}

char readFile(FILE *file,Sphere **totalSpheres,int *sizeOfTotal){ // Συνάρτηση που διαβάζει το αρχείο
    int positionX,positionY,color,mass,radius,speedX,speedY,amountOfSpheres,i;
    fscanf(file,"%d",&amountOfSpheres);
    if (amountOfSpheres<=0){ //Έλεγχος τιμής για τον αριθμό σφαιρών
        return 0;
    }
    *totalSpheres=(Sphere*)malloc(amountOfSpheres*sizeof(Sphere));
    *sizeOfTotal=amountOfSpheres;
    for (i=0;i<amountOfSpheres;i++){
        fscanf(file,"%d%d%d%d%d%d%d",&positionX,&positionY,&color,&mass,&radius,&speedX,&speedY);
        if ((positionX<0 || positionX>600) || (positionY<0 || positionY>400)){ //Έλεγχος τιμής για αρχική θέση x και y
            return 0;
        }
        if ((color<0 || color>15) || (mass<0 || mass>10000) || (radius<1 || radius>100)){ //Έλεγχος τιμής χρώματος,μάζας και ακτίνας
            return 0;
        }
        if ((speedX<-100 || speedX>100) || (speedY<-100 || speedY>100)){ //Έλεγχος τιμής για αρχική ταχύτητα
            return 0;
        }
        (*totalSpheres)[i].PositionX=positionX;
        (*totalSpheres)[i].PositionY=positionY;
        (*totalSpheres)[i].Color=color;
        (*totalSpheres)[i].Mass=mass;
        (*totalSpheres)[i].Radius=radius;
        (*totalSpheres)[i].SpeedX=speedX;
        (*totalSpheres)[i].SpeedY=speedY;
    }
    return 1;
}

void start(Sphere *totalSpheres,int sizeOfTotal){ //Συνάρτηση η οποία δημιουργεί τα γραφικά
    clock_t start,finish, previous;
    double step;
    double *PositionX,*PositionY,*SpeedX,*SpeedY,*Radius,*Mass,tempSpeedA,tempSpeedB;
    int i,j;
    int gdriver = DETECT, gmode, errorcode;
    initgraph(&gdriver, &gmode, "C:\\TC\\BGI");
    errorcode = graphresult();
    if (errorcode != grOk) {
      printf("Graphics error: %s\n", grapherrormsg(errorcode));
      system ("pause");
      exit(1);
    }
    PositionX=(double*)malloc(sizeOfTotal*sizeof(double)); //Δημιουργία αντίστοιχων δεικτών σε double για λόγους ακρίβειας
    PositionY=(double*)malloc(sizeOfTotal*sizeof(double));
    SpeedX=(double*)malloc(sizeOfTotal*sizeof(double));
    SpeedY=(double*)malloc(sizeOfTotal*sizeof(double));
    Radius=(double*)malloc(sizeOfTotal*sizeof(double));
    Mass=(double*)malloc(sizeOfTotal*sizeof(double));
    for (i=0;i<sizeOfTotal;i++){
        PositionX[i]=(double)totalSpheres[i].PositionX;
        PositionY[i]=(double)totalSpheres[i].PositionY;
        SpeedX[i]=(double)totalSpheres[i].SpeedX;
        SpeedY[i]=(double)totalSpheres[i].SpeedY;
        Radius[i]=(double)totalSpheres[i].Radius;
        Mass[i]=(double)totalSpheres[i].Mass;
    }
    start=clock();
    previous=start;
    do{
        finish = clock();
        step = (finish-previous)*1.0/CLOCKS_PER_SEC;
        if (step >= 0.03){
            previous = finish;
            for (i=0;i<sizeOfTotal;i++){
                setfillstyle(SOLID_FILL, BLACK);
                setcolor(BLACK);
                fillellipse(PositionX[i],PositionY[i],Radius[i],Radius[i]);
                PositionX[i]+= SpeedX[i]*step;
                PositionY[i]+= SpeedY[i]*step;
                if (PositionX[i]+Radius[i]>=(getmaxx()-2) || PositionX[i]-Radius[i]<=2)
                    SpeedX[i] *= -1;
                if (PositionY[i]+Radius[i]>=(getmaxy()-2) || PositionY[i]-Radius[i]<=2)
                    SpeedY[i] *= -1;
                switch (totalSpheres[i].Color){ //Ανάλογα με το χρώμα δημιουργούμε την ανάλογη σφαίρα
                    case 0: setfillstyle(SOLID_FILL,BLACK);
                            setcolor(BLACK);
                            break;
                    case 1: setfillstyle(SOLID_FILL,RED);
                            setcolor(RED);
                            break;
                    case 2: setfillstyle(SOLID_FILL,DARKGRAY);
                            setcolor(DARKGRAY);
                            break;
                    case 3: setfillstyle(SOLID_FILL,LIGHTRED);
                            setcolor(LIGHTRED);
                            break;
                    case 4: setfillstyle(SOLID_FILL,BLUE);
                            setcolor(BLUE);
                            break;
                    case 5: setfillstyle(SOLID_FILL,MAGENTA);
                            setcolor(MAGENTA);
                            break;
                    case 6: setfillstyle(SOLID_FILL,LIGHTBLUE);
                            setcolor(LIGHTBLUE);
                            break;
                    case 7: setfillstyle(SOLID_FILL,LIGHTMAGENTA);
                            setcolor(LIGHTMAGENTA);
                            break;
                    case 8: setfillstyle(SOLID_FILL,GREEN);
                            setcolor(GREEN);
                            break;
                    case 9: setfillstyle(SOLID_FILL,BROWN);
                            setcolor(BROWN);
                            break;
                    case 10: setfillstyle(SOLID_FILL,LIGHTGREEN);
                             setcolor(LIGHTGREEN);
                             break;
                    case 11: setfillstyle(SOLID_FILL,YELLOW);
                             setcolor(YELLOW);
                             break;
                    case 12: setfillstyle(SOLID_FILL,CYAN);
                             setcolor(CYAN);
                             break;
                    case 13: setfillstyle(SOLID_FILL,LIGHTGRAY);
                             setcolor(LIGHTGRAY);
                             break;
                    case 14: setfillstyle(SOLID_FILL,LIGHTCYAN);
                             setcolor(LIGHTCYAN);
                             break;
                    case 15: setfillstyle(SOLID_FILL,WHITE);
                             setcolor(WHITE);
                             break;
                }
                fillellipse(PositionX[i],PositionY[i],Radius[i],Radius[i]);
            }
            for (i=0;i<sizeOfTotal;i++){ //Έλεγχος αν συγκρούονται οι σφαίρες
                for (j=i;j<sizeOfTotal;j++){
                    if (j!=i){
                        if (spheresCollide(twoPointDistance(PositionX[i],PositionY[i],PositionX[j],PositionY[j]),Radius[i],Radius[j])){
                            tempSpeedA=SpeedX[i];
                            tempSpeedB=SpeedX[j];
                            SpeedX[i]=(2*(Mass[j]/(Mass[i]+Mass[j]))*tempSpeedB)+(((Mass[i]-Mass[j])/(Mass[i]+Mass[j]))*tempSpeedA);
                            SpeedX[j]=(2*(Mass[i]/(Mass[i]+Mass[j]))*tempSpeedA)+(((Mass[j]-Mass[i])/(Mass[i]+Mass[j]))*tempSpeedB);
                            tempSpeedA=SpeedY[i];
                            tempSpeedB=SpeedY[j];
                            SpeedY[i]=(2*(Mass[j]/(Mass[i]+Mass[j]))*tempSpeedB)+(((Mass[i]-Mass[j])/(Mass[i]+Mass[j]))*tempSpeedA);
                            SpeedY[j]=(2*(Mass[i]/(Mass[i]+Mass[j]))*tempSpeedA)+(((Mass[j]-Mass[i])/(Mass[i]+Mass[j]))*tempSpeedB);
                        }
                    }
                }
            }
        }
    }
    while (!kbhit()); //Αυτή η επανάληψη θα γίνεται μέχρι ο χρήστης να πατήσει ένα οποιοδήποτε πλήκτρο
    closegraph();
    free(PositionX);
    free(PositionY);
    free(SpeedX);
    free(SpeedY);
    free(Radius);
    free(Mass);
}

int main()
{
    int sizeOfTotal=0;
    FILE *file;
    Sphere *totalSpheres;
    totalSpheres=NULL;
    if ((file=fopen("input.txt","r"))!=NULL){ //¶νοιγμα του αρχείου
        if (readFile(file,&totalSpheres,&sizeOfTotal)){ //Διάβασμα του αρχείου
            start(totalSpheres,sizeOfTotal); //Εκκίνηση (δημιουργία γραφικών κτλπ)
            fclose(file);
            free(totalSpheres);
        }
        else{
            printf("Error while reading file\n");
            fclose(file);
            system("PAUSE");
        }
    }
    else{
        printf("Error while opening file\n");
        system("PAUSE");
    }
    return 0;
}
