#include "lab7.h"

struct partition_table 
{
    tPartition table[4];
    struct partition_table* next;
} *first, *temp, *last; 

tPartition tm;
tIDECHS geomet;
tLARGE geom,adr;
tLBA disk, adres = 1;

int main()
{

    int t;
    int k = 0;
    int b = 1;
    int free_size; 
    int type_oc; 
    int bool_oc;
    char c;

    geom.C = 1023; 
    geom.H = 255;
    geom.S = 63;

    first = malloc(sizeof(struct partition_table)); 

    last = first;

    printf("Input IDECHS geometry\n");

    while(1) 
    {
        printf("C:");
        scanf("%d", &t);

        if(t > geom.C)  
            printf("Error\n");
        else break;    
    }

    geomet.C=t;

    while(1) 
    {
        printf("H:");
        scanf("%d", &t);

        if(t > geom.C)  
            printf("Error\n");
        else break;  
    }

    geomet.H=t;

    while(1) 
    {
        printf("S:");
        scanf("%d",&t); 

        if(t>geom.S)
            printf("Error\n");
        else break; 
    }

    geomet.S=t;

    g_idechs2lba(geomet, &disk); 

    printf("Disk sise: %.2f Gb\n", (float) disk / (1024 * 2048)); 

    free_size = disk / 2;

    do {

        do {
            printf("Free space: %d Кб\n",free_size); // выводим оставшееся место

            printf("Section size (Кб): ");

            scanf("%d", &t); 
        } while (t > free_size); 

        if(t == 0) break;

        free_size -= t; 

        tm.startLBA = adres; 
        tm.size = t; 
        adres += tm.size; 

        if(a_lba2large(geom,tm.startLBA, &adr)) 
            tm.startCHS = adr;
        else
            tm.startCHS=geom;

        if (a_lba2large(geom,adres, &adr)) 
            tm.endCHS=adr; 
        else
            bool_oc=1;

        while (bool_oc)
        {
            bool_oc=0;

            printf("Тип ОС:\n"); 
            printf("1 - FAT16\n");
            printf("2 - FAT32\n");
            printf("3 - Linux swap\n");
            printf("4 - Linux\n");
            printf("5 - HPFS/NTFS\n");

            scanf("%d",&type_oc);

            switch (type_oc)
            { 
            case 1:
                tm.type = 0x04;
                break;

            case 2:
                tm.type = 0x0c;
                break;

            case 3:
                tm.type = 0x82;
                break;

            case 4:
                tm.type = 0x83;
                break;

            case 5:
                tm.type = 0x07;
                break;

            default:
                bool_oc = 1;
            }
        }

        tm.active = 0;

        if(b) {

            printf("Set section as active (y/n): ");

            do {
                scanf("%c", &c);
            } while((c != 'n') && (c != 'y'));

            if ((c == 'y')) 
            {
                tm.active = 0x80;
                b = 0;
            }

        }

        if (((tm.startCHS.C==geom.C)&&(tm.startCHS.H==geom.H)&&(tm.startCHS.S==geom.S))||(k==3)) 
        {
            last->table[k]=tm;
            last->table[k].type=0x05;

            if (k!=3)
                last->table[k+1].startLBA=0;
            
            temp = malloc(sizeof(struct partition_table));

            last->next=temp;
            last=temp;
            last->table[0]=tm;
            last->table[1].startLBA=0;

            k=1;
        } 
        else
        {
            last->table[k]=tm;
            last->table[++k].startLBA=0;
        }

    } while (free_size != 0);

    printf("Active |   CHS start   |   ОС   |  CHS finish   |  LBA start  | Size\n");
    
    temp = first;

    while (temp != NULL) 
    { 
        int i;

        for (i=0;i<4;i++){

        if (temp->table[i].startLBA==0) break;

        printf("  %2x       %2d %2d %2d       %2x       %2d %2d %2d         %4ld         %d \n",   \
                temp->table[i].active,                  \
                temp->table[i].startCHS.C,              \
                temp->table[i].startCHS.H,              \
                temp->table[i].startCHS.S,              \
                temp->table[i].type,                    \
                temp->table[i].endCHS.C,                \
                temp->table[i].endCHS.H,                \
                temp->table[i].endCHS.S,                \
                temp->table[i].startLBA,                \
                temp->table[i].size); 
        }

    temp=temp->next; 
    }

    return 0;
}