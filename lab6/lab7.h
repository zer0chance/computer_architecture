#include <stdio.h>

#define BITS_IN_GIGABYTE 8589934592

#define CHS_SECTOR_SIZE     20
#define LARGE_SECTOR_SIZE   24
#define IDECHS_SECTOR_SIZE  28
#define LBA_SECTOR_SIZE     32


typedef struct CHS
{
    __uint16_t C;
    __uint16_t H;
    __uint16_t S; // 20 bit

    double capacity;
} tCHS;


typedef struct LARGE
{
    __uint16_t C;
    __uint16_t H;
    __uint16_t S; // 24 bit
    
    double capacity;
} tLARGE;


typedef struct IDECHS
{
    __uint16_t C;
    __uint16_t H;
    __uint16_t S;  // 28 bit

    double capacity;
} tIDECHS;


typedef struct LBA
{
    __uint16_t C;
    __uint16_t H;
    __uint16_t S; // 32 bit
    
    double capacity;
} tLBA;


void set_chs_capacity(tCHS* d)
{
    d->capacity = d->C * d->H * d->S * CHS_SECTOR_SIZE / BITS_IN_GIGABYTE; 
}


void set_large_capacity(tLARGE* d)
{
    d->capacity = d->C * d->H * d->S * LARGE_SECTOR_SIZE / BITS_IN_GIGABYTE; 
}


void set_idechs_capacity(tIDECHS* d)
{
    d->capacity = (double)(d->C * d->H * d->S * IDECHS_SECTOR_SIZE) / BITS_IN_GIGABYTE; 
}

q
void set_lba_capacity(tLBA* d)
{
    d->capacity = d->C * d->H * d->S * LBA_SECTOR_SIZE / BITS_IN_GIGABYTE; 
}


int g_lba2chs (tLBA old, tCHS* new)
{

}

int g_lba2large (tLBA old, tLARGE* new)
{

} 

int g_lba2idechs (tLBA old, tIDECHS *new)
{

}

int g_chs2large (tCHS old,  tLARGE* new)
{

}

int g_chs2lba(tCHS old, tLBA* new)
{

}

int g_chs2idechs (tIDECHS old, tLBA* new)
{

}

int g_large2chs (tLARGE old, tCHS* new)
{

}

int g_large2idechs (tLARGE old, tIDECHS* new)
{

}

int g_large2lba (tLARGE old, tLBA* new)
{

}

int g_idechs2chs (tIDECHS old, tCHS* new)
{

}

int g_idechs2lagre (tIDECHS old,  tLARGE* new)
{

}

int g_idechs2lba (tIDECHS old, tLBA* new)
{

}
