#include <stdio.h>
#include <math.h>
//#include <string.h>
#include "webcrane.h"

const float r_Suspension = 0.050;
const float L_Suspension = 0.200;

CABLE cables[4];

float dotproduct(float *vector1, float *vector2)
{
    float ret_dotproduct = 0;
    for (int i = 0; i<3; i++)
    {
        ret_dotproduct += vector1[i] * vector2[i];
    }
    return ret_dotproduct;
}

float norm(float *vector)
{
    return sqrt(dotproduct(vector, vector));
}

void copyarray(float *vector1, float *vector2)
{
    for (int i = 0; i<3; i++)
    {
        *(vector1 + i) = *(vector2 + i);
    }
}

float getLength(CABLE* cable)
{
    float angles[maxNumberSuspensions - 2];
    float intersections[maxNumberSuspensions][3];
    //The first intersection equals the first Suspension
    for (int j = 0; j<3 ; j++)
    {
        intersections[0][j] = cable->Suspensions[0][j];
    }

    //Generating three auxiliary frames
    float pos1[3];
    float pos2[3];
    float pos3[3];

    // step by step filling up of intersections array
    for (int i = 1; i < cable->end_index; i++)
    {
        //Definieren der drei Positionen
        copyarray(pos1, &cable->Suspensions[i-1][0]);
        copyarray(pos2, &cable->Suspensions[i][0]);
        copyarray(pos3, &cable->Suspensions[i+1][0]);

        //Generating two vectors connecting three positions
        float vector1[3] = {0};
        float vector2[3] = {0};
        for (int j = 0; j<3; j++)
        {
            vector1[j] = pos1[j] - pos2[j];
            vector2[j] = pos3[j] - pos2[j];
        }

        //Calculating angle between the vectors
        float angle = acos( dotproduct(vector1, vector2) / (norm(vector1)*norm(vector2)) );
        angles[i - 1] = angle; //Zwischenspeichern des Winkels für spätere Berechnung

        //Calculating intersection
        float mvector[3] = {0};
        for (int j = 0; j<3; j++)
        {
            mvector[j] = vector1[j] + vector2[j];
        } //Calculating mean vector of vector1 and vector2
        float mvectorLength = norm(mvector);
        for (int j = 0; j<3; j++)
        {
            mvector[j] /= mvectorLength;
        } //Normalizing mean vector

        //Calculating distance between intersection and mounting position of Suspension
        float differencelength = L_Suspension - r_Suspension / sin(angle / 2);
        for (int j = 0; j<3; j++)
        {
            intersections[i][j] = pos2[j] + mvector[j] * differencelength;
        } // final calculating of intersection by shifting the mounting position along the mean vector
        
    }
    //Setting the latest intersection to the position of the last Suspension
    for (int j = 0; j<3 ; j++)
    {
        intersections[cable->end_index][j] = cable->Suspensions[cable->end_index][j];
    }

    //Calculating length of the cable
    float length = 0;
    for (int i = 0; i<cable->end_index; i++)
    {
        float vector[3] = {0};
        for (int j = 0; j<3; j++)
        {
            vector[j] = intersections[i][j] - intersections[i+1][j];
        } //Calculating connecting vectors between intersections

        length += norm(vector);

        if (i != cable->end_index - 1)
        {
            length += r_Suspension * (3.14159 - angles[i] - 2 / tan( angles[i]/2 ));
        } //Correction of the curve of the suspension wheels with a correction term
    }

    return length;
}

float Simulink_getLength(int cable_index)
{
    return getLength(cables+cable_index-1);
}

void moveHook(CABLE *cable, float x, float y, float z)
{
    //iterating through the Suspensions array
    for (int j = 0; j <= cable->end_index; j++)
    {
    //if the array has a 1 in the third column (index 3), the Suspension belongs to the hook and gets moved
        if (  cable->Suspensions[j][3] )
        {
            cable->Suspensions[j][0] += x;
            cable->Suspensions[j][1] += y;
            cable->Suspensions[j][2] += z;
        }
    }
}

void Simulink_moveHook(float x, float y, float z)
{
    for (int i = 0; i<4; i++)
    {
        moveHook(cables+i,x,y,z);
    }
    return;
}

void setSuspension(CABLE *cable, float x, float y, float z, int hook)
{
    cable->end_index++;
    cable->Suspensions[cable->end_index][0] = x;
    cable->Suspensions[cable->end_index][1] = y;
    cable->Suspensions[cable->end_index][2] = z;
    cable->Suspensions[cable->end_index][3] = hook;
    return;
}

void init()
{   
    //  ^y
    //  |
    // z.---> x

    // cable 1 (left in sim file)
    cables[0].end_index = -1;
    setSuspension(cables+0,-5  ,4    ,4,0);
    setSuspension(cables+0,-0.1,0.05 ,3,1);
    setSuspension(cables+0,-5  ,0    ,4,0);
    setSuspension(cables+0,-0.1,-0.05,3,1);
    setSuspension(cables+0,-5  ,-4   ,4,0);

    // cable 2 (top in sim file)
    cables[1].end_index = -1;
    setSuspension(cables+1,-4   ,5  ,4,0);
    setSuspension(cables+1,-0.05,0.1,3,1);
    setSuspension(cables+1,0    ,5  ,4,0);
    setSuspension(cables+1,0.05 ,0.1,3,1);
    setSuspension(cables+1,4    ,5  ,4,0);

    // cable 3 (right in sim file)
    cables[2].end_index = -1;
    setSuspension(cables+2,5  ,4    ,4,0);
    setSuspension(cables+2,0.1,0.05 ,3,1);
    setSuspension(cables+2,5  ,0    ,4,0);
    setSuspension(cables+2,0.1,-0.05,3,1);
    setSuspension(cables+2,5  ,-4   ,4,0);

    // cable 4 (down in sim file)
    cables[3].end_index = -1;
    setSuspension(cables+3,-4   ,-5  ,4,0);
    setSuspension(cables+3,-0.05,-0.1,3,1);
    setSuspension(cables+3,0    ,-5  ,4,0);
    setSuspension(cables+3,0.05 ,-0.1,3,1);
    setSuspension(cables+3,4    ,-5  ,4,0);

    return;
}