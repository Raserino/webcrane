#include <stdio.h>
#include <math.h>
#include <string.h>

#ifndef WEB_CRANE_H_INCLUDED
#define WEB_CRANE_H_INCLUDED

//#include<Arduino.h>

#include <math.h>
#include <string.h>

#define maxAnzahlAufhaengungen 5

const float radius = 5;
const float LaengeAufhaengung = 20;

typedef struct
{
    float laenge;
    int end_index;
    float aufhaengungen[maxAnzahlAufhaengungen][4];
}SEIL;

float skprodukt(float vector1[3], float vector2[3])
{
    float skalarprodukt = 0;
    for (int i = 0; i<= 2; i++)
    {
        skalarprodukt += vector1[i] * vector2[i];
    }
    return skalarprodukt;
}

float norm(float vector[3])
{
    return sqrt(skprodukt(vector, vector));
}

void copyarray(float *vector1, float *vector2)
{
    for (int i = 0; i<=2; i++)
    {
        *(vector1 + i) = *(vector2 + i);
    }
}


float getLaenge(SEIL seil)
{
    float angles[maxAnzahlAufhaengungen - 2];
    float Schnittpunkte[maxAnzahlAufhaengungen][3];
    //Der erste Schnittpunkt ist gleich der Position der ersten Aufhängung
    for (int j = 0; j<=2 ; j++)
    {
        Schnittpunkte[0][j] = seil.aufhaengungen[0][j];
    }

    //Schrittweise Auffüllen der Schnittpunkte-Matrix
    for (int i = 1; i <= seil.end_index - 1; i++)
    {

        //Erzeugen zweier Vektoren, welche die drei Positionen verbinden
        float vector1[3] = {0};
        float vector2[3] = {0};
        for (int j = 0; j<=2; j++)
        {
            vector1[j] =  seil.aufhaengungen[i - 1][j] - seil.aufhaengungen[i][j];
            vector2[j] =  seil.aufhaengungen[i + 1][j] - seil.aufhaengungen[i][j];
        }

        //Berechnen des Winkels zwischen den beiden Vektoren
        float angle = acos( skprodukt(vector1, vector2) / (norm(vector1)*norm(vector2)) );
        angles[i - 1] = angle; //Zwischenspeichern des Winkels für spätere Berechnung

        //Berechnen des Schnittpunkts
        float mvector[3] = {0};
        for (int j = 0; j<=2; j++)
        {
            mvector[j] = vector1[j] + vector2[j];
        } //Erzeugen des Mittenvektors von vector1 und vector2
        float mvectorLength = norm(mvector);
        for (int j = 0; j<=2; j++)
        {
            mvector[j] /= mvectorLength;
        } //Normieren des Mittenvektors

        //Berechnenen des Abstands des Schnittpunkts und der Aufhängungsposition
        float differenzlaenge = radius / sin(angle / 2) - LaengeAufhaengung;
        for (int j = 0; j<=2; j++)
        {
            Schnittpunkte[i][j] = seil.aufhaengungen[i][j] - mvector[j] * differenzlaenge;
        } //finales Ausrechnen des Schnittpunkts durch Verschieben der Aufhängungsposition entlang des Mittenvektors
    }
    //Setzen des letzten Schnittpunkts in die Position der letzten Aufhängung
    for (int j = 0; j<=2 ; j++)
    {
        Schnittpunkte[seil.end_index][j] = seil.aufhaengungen[seil.end_index][j];
    }

    //Nun wird die Seillänge berechnet
    float laenge = 0;
    for (int i = 0; i<= seil.end_index - 1; i++)
    {
        float vector[3] = {0};
        for (int j = 0; j<=2; j++)
        {
            vector[j] = Schnittpunkte[i][j] - Schnittpunkte[i + 1][j];
        } //Berechnend er Verbindungsvektoren der Sschnittpunkte

        laenge += norm(vector);

        if (i != seil.end_index - 1)
        {
            laenge += radius * (3.14159 - angles[i] - 2 / tan( angles[i]/2 ));
        } //Korrigieren der Rundung der Rolle mit einem Korrekturterm
    }

    return laenge;
}

void moveHaken(SEIL *seil, float x, float y, float z)
{
    //Die Funktion läuft die Aufhängungen-Matrix durch
    for (int j = 0; j <= seil->end_index; j++)
    {
    //Wenn die Matrix in der 4. Spalte eine 1 hat, wird die Aufhängung verschoben
        if (  seil->aufhaengungen[j][3] )
        {
            seil->aufhaengungen[j][0] += x;
            seil->aufhaengungen[j][1] += y;
            seil->aufhaengungen[j][2] += z;
        }
    }
}

void setAufhaengung(SEIL *seil, float x, float y, float z, int Haken)
{
    seil->end_index++;
    seil->aufhaengungen[seil->end_index][0] = x;
    seil->aufhaengungen[seil->end_index][1] = y;
    seil->aufhaengungen[seil->end_index][2] = z;
    seil->aufhaengungen[seil->end_index][3] = Haken;

}

#endif // WEB_CRANE_H_INCLUDED