#include <math.h>
#include <string.h>
#include <web_crane.h>

const float radius = 1;
const float LaengeAufhaengung = 1;

typedef struct aufhaengung AUFHAENGUNG;

struct aufhaengung
{
    float position[3];
    int index;
    AUFHAENGUNG *naechsteAufhaengung;
};

typedef struct
{
    float laenge;
    int end_index;
    AUFHAENGUNG *motor;
}SEIL;

#define AnzahlHakenAufhaengungen 10
AUFHAENGUNG *Haken_Aufhaengungen[AnzahlHakenAufhaengungen];

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

float getLaenge(SEIL seil)
{
    AUFHAENGUNG aufhaengung_akt = *seil.motor;
    float angles[seil.end_index - 1];
    float Schnittpunkte[3][seil.end_index + 1];
    float pos1[3],pos2[3],pos3[3];


    for (int i = 0; i<=2 ; i++)
    {
        Schnittpunkte[i][0] = aufhaengung_akt.position[i];
    }

    for (int j = 1; j <= seil.end_index; j++)
    {
        memcpy(pos1, aufhaengung_akt.position, sizeof(aufhaengung_akt.position));
        aufhaengung_akt = *aufhaengung_akt.naechsteAufhaengung;
        memcpy(pos2, aufhaengung_akt.position, sizeof(aufhaengung_akt.position));
        memcpy(pos3, (*aufhaengung_akt.naechsteAufhaengung).position, sizeof((*aufhaengung_akt.naechsteAufhaengung).position));

        float vector1[3] = {0};
        float vector2[3] = {0};
        for (int i = 0; i<=2; i++)
        {
            vector1[i] = pos2[i] - pos1[i];
            vector2[i] = pos2[i] - pos3[i];
        }
        float angle = acos( skprodukt(vector1, vector2) / (norm(vector1)*norm(vector2)) );

        float mvector[3] = {0};

        for (int i = 0; i<=2; i++)
        {
            mvector[i] = vector1[i] + vector2[i];
        }
        float mvectorLength = norm(mvector);
        for (int i = 0; i<=2; i++)
        {
            mvector[i] /= mvectorLength;
        }

        float differenzlaenge = radius / sin(0.5 * angle) - LaengeAufhaengung;

        for (int i = 0; i<=2; i++)
        {
            Schnittpunkte[i][j] = pos2[i] - mvector[i] * differenzlaenge;
        }
    }

    for (int i = 0; i<=2 ; i++)
    {
        Schnittpunkte[i][seil.end_index] = (*aufhaengung_akt.naechsteAufhaengung).position[i];
    }

    float laenge = 0;

    for (int j = 0; j<= seil.end_index; j++)
    {
        float vector[3] = {0};
        for (int i = 0; i<=2; i++)
        {
            vector[i] = Schnittpunkte[i][j] - Schnittpunkte[i][j+1];
        }
        float korrektur = 0;
        if (j < seil.end_index)
        {
            korrektur = radius * (3.14159 - angles[j] - 2 / tan( angles[j]/2 ));
        }
        laenge += norm(vector) + korrektur;
    }

    return laenge;
}

void setAufhaengung(AUFHAENGUNG aufhaengung, AUFHAENGUNG naechsteAufhaengung, float position[3], int index)
{
    for (int i = 0; i<= 2; i++)
    {
        aufhaengung.position[i] = position[i];
    }
    aufhaengung.index = index;
    aufhaengung.naechsteAufhaengung = &naechsteAufhaengung;
}

void moveHaken(float coordinates[3])
{
    for (int i = 0; i <= AnzahlHakenAufhaengungen - 1; i++)
    {
        for (int j = 0; j<= 2; j++)
        {
            (*Haken_Aufhaengungen[i]).position[j] += coordinates[j];
        }
    }
}
