#include "webcrane.h"
#include <stdio.h>

int main()
{
    init();
    for (int i = 0; i<4; i++)
    {
        float L = getLength(cables+i);
        printf("Length cable: %.2f\n",L);
    }
    
    return 0;
}
