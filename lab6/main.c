#include "lab7.h"


int main()  
{
    tIDECHS d;

    printf("\n Heads: ");
    scanf("%hd", &d.H);
    printf("\n Cylinders: ");
    scanf("%hd", &d.C);
    printf("\n Sectors: ");
    scanf("%hd", &d.S);

    set_idechs_capacity(&d); 

    printf("\n C: %hd H: %hd S: %hd\n Capacity: %.3lf\n\n", d.C, d.H, d.S, d.capacity );

    return 0;
}