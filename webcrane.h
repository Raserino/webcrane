#ifndef WEB_CRANE_H_INCLUDED
#define WEB_CRANE_H_INCLUDED

#define maxNumberSuspensions 5

typedef struct
{
    int end_index;
    float Suspensions[maxNumberSuspensions][4];
}CABLE;

extern CABLE cables[4];

float dotproduct(float vector1[3], float vector2[3]);

float norm(float vector[3]);

void copyarray(float *vector1, float *vector2);

float getLength(CABLE* cable);

float Simulink_getLength(int cable_index);

void moveHook(CABLE *cable, float x, float y, float z);

void Simulink_moveHook(float x, float y, float z);

void setSuspension(CABLE *cable, float x, float y, float z, int hook);

void init();

#endif // WEB_CRANE_H_INCLUDED