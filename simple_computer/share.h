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


// 11111111
// 11000011
// 11000011
// 11111111
// 00000011
// 00000110
// 00001100
// 00011000


int bc_chars[12][2] =
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
    {404232447, 4279769112},   // +
    {255, 4278190080}          // -
};