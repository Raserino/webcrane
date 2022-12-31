#include "webcrane.h"
#include <stdio.h>

int main()
{
    init();
    for (int i = 0; i<4; i++)
    {
        float L = Simulink_getLength(i+1);
        printf("Length cable: %.2f\n",L);
    }
    
    return 0;
}
