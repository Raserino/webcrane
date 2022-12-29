#include <stdio.h>
#include <math.h>
#include <string.h>

#ifndef WEB_CRANE_H_INCLUDED
#define WEB_CRANE_H_INCLUDED

//#include<Arduino.h>

#include <math.h>
#include <string.h>

#define maxNumberPulleys 5

const float r_pulley = 50;
const float L_pulley = 150;

typedef struct
{
    float length;
    int end_index;
    float pulleys[maxNumberPulleys][4];
}CABLE;

float dotprodukt(float vector1[3], float vector2[3])
{
    float dotproduct = 0;
    for (int i = 0; i<= 2; i++)
    {
        dotproduct += vector1[i] * vector2[i];
    }
    return dotproduct;
}

float norm(float vector[3])
{
    return sqrt(dotproduct(vector, vector));
}

void copyarray(float *vector1, float *vector2)
{
    for (int i = 0; i<=2; i++)
    {
        *(vector1 + i) = *(vector2 + i);
    }
}

float getLength(CABLE cable)
{
    float angles[maxNumberPulleys - 2];
    float intersections[maxNumberPulleysn][3];
    //The first intersection equals the first pulley
    for (int j = 0; j<=2 ; j++)
    {
        Schnittpunkte[0][j] = cable.pulleys[0][j];
    }

    // step by step filling up of intersections array
    for (int i = 1; i <= cable.end_index - 1; i++)
    {
        //Generating two vectors connecting three positions
        float vector1[3] = {0};
        float vector2[3] = {0};
        for (int j = 0; j<=2; j++)
        {
            vector1[j] =  cable.pulleys[i - 1][j] - cable.pulleys[i][j];
            vector2[j] =  cable.pulleys[i + 1][j] - cable.pulleys[i][j];
        }

        //Calculating angle between the vectors
        float angle = acos( skprodukt(vector1, vector2) / (norm(vector1)*norm(vector2)) );
        angles[i - 1] = angle; //Zwischenspeichern des Winkels für spätere Berechnung

        //Calculating intersection
        float mvector[3] = {0};
        for (int j = 0; j<=2; j++)
        {
            mvector[j] = vector1[j] + vector2[j];
        } //Calculating mean vector of vector1 and vector2
        float mvectorLength = norm(mvector);
        for (int j = 0; j<=2; j++)
        {
            mvector[j] /= mvectorLength;
        } //Normalizing mean vector

        //Calculating distance between intersection and mounting position of pulley
        float differenzlaenge = radius / sin(angle / 2) - LaengeAufhaengung;
        for (int j = 0; j<=2; j++)
        {
            Schnittpunkte[i][j] = cable.pulleys[i][j] - mvector[j] * differenzlaenge;
        } // final calculating of intersection by shifting the mounting position along the mean vector
        
    }
    //Setting the latest intersection in the position of the last pulley
    for (int j = 0; j<=2 ; j++)
    {
        Schnittpunkte[cable.end_index][j] = cable.pulleys[cable.end_index][j];
    }

    //Calculating length of the cable
    float laenge = 0;
    for (int i = 0; i<= cable.end_index - 1; i++)
    {
        float vector[3] = {0};
        for (int j = 0; j<=2; j++)
        {
            vector[j] = Schnittpunkte[i][j] - Schnittpunkte[i + 1][j];
        } //Calculating connecting vectors between intersections

        laenge += norm(vector);

        if (i != cable.end_index - 1)
        {
            laenge += radius * (3.14159 - angles[i] - 2 / tan( angles[i]/2 ));
        } //Correction of the rounding of the pulley wheels with a correction term
    }

    return laenge;
}



void moveHaken(CABLE *cable, float x, float y, float z)
{
    //iterating through the pulleys array
    for (int j = 0; j <= cable->end_index; j++)
    {
    //if the array has a 1 in the forth array, the pulley belongs to the hook and gets moved
        if (  cable->pulleys[j][3] )
        {
            cable->pulleys[j][0] += x;
            cable->pulleys[j][1] += y;
            cable->pulleys[j][2] += z;
        }
    }
}

void Simulink_moveHaken()
{

}

void setPulley(CABLE *cable, float x, float y, float z, int Haken)
{
    cable->end_index++;
    cable->pulleys[cable->end_index][0] = x;
    cable->pulleys[cable->end_index][1] = y;
    cable->pulleys[cable->end_index][2] = z;
    cable->pulleys[cable->end_index][3] = Haken;
}

#endif // WEB_CRANE_H_INCLUDED