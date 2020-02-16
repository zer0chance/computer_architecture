#include "myTerm.h"


int main(void) 
{
    mt_clrscr();
    
    mt_gotoXY(20, 20);

    int rows, cols;
    if (!mt_getscreensize(&rows, &cols))
        printf("\n Rows: %d\n Cols: %d\n", rows, cols);
    else
        printf("\n Can`t get screen size.\n");
    
    if (mt_setfgcolor(FG_GREEN)) printf("\n Can`t set this fg color.\n");
    if (mt_setbgcolor(BG_BLUE)) printf("\n Can`t set this bg color.\n");
    printf("\n\nTEST TEXT\n\n");    

    return 0;
}





