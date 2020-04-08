#include <stdio_ext.h>
#include <signal.h>
#include <sys/time.h>

#define TWO_POW_FIFTEEN 16384

int input_x = 27;
int input_y = 3;

int mpos_x = 5;
int mpos_y = 3;

int selected_pos = 0;

enum
{
    FG_BLACK   = 30,
    FG_RED     = 31, 
    FG_GREEN   = 32,
    FG_YELLOW  = 33, 
    FG_BLUE    = 34, 
    FG_MAGENTA = 35, 
    FG_CYAN    = 36, 
    FG_WHITE   = 37
};


enum
{
    BG_BLACK   = 40,
    BG_RED     = 41, 
    BG_GREEN   = 42,
    BG_YELLOW  = 43, 
    BG_BLUE    = 44, 
    BG_MAGENTA = 45, 
    BG_CYAN    = 46, 
    BG_WHITE   = 47
};


enum
{
    A     = 10,
    B     = 11,
    C     = 12,
    D     = 13,
    E     = 14,
    F     = 15,
    PLUS  = 16,
    MINUS = 17
};


int bc_chars[18][2] =
{
    {4286677377, 2172748287},  // 0
    {253704039, 117901063},    // 1
    {4278387459, 4290822399},  // 2
    {4278387711, 50529279},    // 3
    {3284386755, 4278387459},  // 4
    {4290822399, 50529279},    // 5
    {4290822399, 3284386815},  // 6
    {4278387462, 202116108},   // 7
    {4286677503, 2172748287},  // 8
    {4291019775, 50727960},    // 9
    {2126758911, 3284386755},  // A
    {4274242558, 4274242558},  // B
    {4290822336, 3233857791},  // C
    {4240622017, 3250701052},  // D
    {4290822399, 3233857791},  // E
    {4290822399, 3233857728},  // F
    {404232447, 4279769112},   // +
    {255, 4278190080}          // -
};


int left_shift(int n, unsigned int k) 
{
     unsigned int bit;
     for (int i = 0; i < k; i++) 
     {
        bit = n & 0x8000 ? 1 : 0;
        n <<= 1;
        n |= bit;            
    }

    return n;      
}


int right_shift (int n, unsigned int k) 
{
     unsigned int bit;
     for (int i = 0; i < k; i++) 
     {
        bit = n & 0x0001;
        n >>= 1;
        n |= bit << 15;
    }

    return n;      
}
