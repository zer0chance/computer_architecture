#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct
{
    uint64_t C : 10; 
    uint64_t H : 4;
    uint64_t S : 6;
} tCHS;

typedef struct
{
    uint64_t C : 10; 
    uint64_t H : 8;
    uint64_t S : 6;
} tLARGE;

typedef struct
{
    uint64_t C : 16; 
    uint64_t H : 4;
    uint64_t S : 8;
} tIDECHS;

typedef uint64_t tLBA;

typedef struct
{
    tLARGE startCHS;
    tLARGE endCHS;
    tLBA startLBA;

    uint32_t size;
    uint8_t  active;
    uint8_t  type;
} tPartition;


int g_lba2chs (tLBA, tCHS *);
int g_lba2large (tLBA, tLARGE *);
int g_lba2idechs (tLBA, tIDECHS *);
int g_chs2large (tCHS, tLARGE *);
int g_chs2idechs (tCHS, tIDECHS *);
int g_chs2lba (tCHS, tLBA *);
int g_large2chs (tLARGE, tCHS *);
int g_large2idechs (tLARGE, tIDECHS *);
int g_large2lba (tLARGE, tLBA *);
int g_idechs2chs (tIDECHS, tCHS *);
int g_idechs2lagre (tIDECHS, tLARGE *);
int g_idechs2lba (tIDECHS, tLBA *);

int a_lba2chs (tCHS, tLBA, tCHS *);
int a_lba2large (tLARGE, tLBA, tLARGE *);
int a_lba2idechs (tIDECHS, tLBA, tIDECHS *);
int a_chs2lba (tCHS, tCHS, tLBA *);
int a_large2lba (tLARGE, tLARGE, tLBA *);
int a_idechs2lba (tIDECHS, tIDECHS, tLBA *);
int a_large2chs (tLARGE, tCHS, tLARGE, tCHS *);
int a_large2idechs (tLARGE, tIDECHS, tLARGE, tIDECHS *);
int a_chs2large (tCHS, tLARGE, tCHS, tLARGE *);
int a_idechs2large (tIDECHS, tLARGE, tIDECHS, tLARGE *);
int a_chs2idechs (tCHS, tIDECHS, tCHS, tIDECHS *);
int a_idechs2chs (tIDECHS, tCHS, tIDECHS, tCHS *);


int g_lba2chs (tLBA g1, tCHS *g2)
{
    int L1;
    int nlog;
    int k = 2;

    L1 = g1 / 63;

    nlog = L1 / 1023;

    while (nlog > k)
    {
        if (k == 16) break;
        k *= 2;
    }

    if (k == 16)
        k = 15;

    if (nlog / k < 1024) 
    {
        g2->C = L1 / k;
        g2->H = k;
        g2->S = 63;
    } 
    else
        return 0;

    return 1;
}

int g_lba2large (tLBA g1, tLARGE *g2)
{
    int L1;
    int nlog;
    int k = 2;

    L1 = g1 / 63;

    nlog = L1 / 1024;

    while (nlog / k > 0)
    {
        if (k == 256) break;
        k *= 2;
    }

    if (k==256)
        k=255;

    if (nlog / k < 1024)
    {
        g2->C = L1 / k;
        g2->H = k;
        g2->S = 63;
    } else 
        return 0;

    return 1;
}

int g_lba2idechs (tLBA g1, tIDECHS *g2)
{
    int L1;
    int nlog;
    int k = 2;

    L1 = g1 / 255;

    nlog = L1 / 65535;

    while (nlog / k > 0)
    {
        if (k == 16) break;
        k *= 2;
    }

    if(k == 16) k = 15;

    if (nlog / k < 65536)
    {
        g2->C=L1/k;
        g2->H=k;
        g2->S=25;
    } else 
        return 0;

    return 1;
}

int g_chs2large (tCHS g1, tLARGE *g2)
{
    int c,h;
    c=g1.C;
    h=g1.H;

    do {
        c /= 2;
        h *= 2;
    } while ((c / 2 > 0) && (h * 2 < 256));

    g2->C=c;
    g2->H=h;
    g2->S=g1.S;

    return 1;
}

int g_chs2idechs (tCHS g1, tIDECHS *g2) 
{
    int c,h;

    c = g1.C;
    h = g1.H;

    do {
        c *= 2;
        h /= 2;
    } while ((c * 2 < 65536) && (h / 2 > 0));

    g2->C = c;
    g2->H = h;
    g2->S = g1.S;

    return 1;
}

int g_chs2lba (tCHS g1, tLBA *g2)
{
    *g2 = g1.C * g1.H * g1.S;

    return 1;
}

int g_large2chs (tLARGE g1, tCHS *g2)
{
    int c,h;
    c = g1.C;
    h = g1.H;

    do {
        c *= 2;
        h /= 2;
    } while ((c*2<1024) && (h / 2 > 0));

    if (c > 1023 || h > 15) return 0;

    g2->C = c;
    g2->H = h;
    g2->S = g1.S;

    return 1;
}

int g_large2idechs (tLARGE g1, tIDECHS *g2)
{
    int c,h;

    c=g1.C;
    h=g1.H;

    do {
        c*=2;
        h/=2;
    } while ((c * 2 < 1024) && (h / 2 > 0));

    if (c > 65536 || h > 15) return 0;

    g2->C = c;
    g2->H = h;
    g2->S = g1.S;

    return 1;
}

int g_large2lba (tLARGE g1, tLBA *g2)
{
    *g2 = g1.C * g1.H * g1.S;

    return 1;
}

int g_idechs2chs (tIDECHS g1, tCHS *g2)
{
    tLBA temp;

    g_idechs2lba(g1,&temp);

    if (!g_lba2chs(temp,g2)) return 0;

    return 1;
}

int g_idechs2lagre (tIDECHS g1, tLARGE *g2)
{
    tLBA temp;

    g_idechs2lba(g1, &temp);

    if (g_lba2large(temp,g2) == 0) return 0;

    return 1;
}

int g_idechs2lba (tIDECHS g1, tLBA *g2)
{
    *g2 = g1.C * g1.H * g1.S;

    return 1;
}

int a_lba2chs (tCHS g1, tLBA a1, tCHS *a2)
{
    int a;

    a2->C = a1 / (g1.H * g1.S);
    a = a1 % (g1.H * g1.S);
    a2->H = a / g1.S;
    a2->S = a % g1.S;

    return 1;
}

int a_lba2large (tLARGE g1, tLBA a1, tLARGE *a2)
{
    int a;

    if ((g1.C*g1.H*g1.S)<a1) return 0;

    a2->C = a1 / (g1.H * g1.S);

    a = a1 % (g1.H * g1.S);

    a2->H = a / g1.S;

    a2->S = a % g1.S;

    return 1;
}

int a_lba2idechs (tIDECHS g1, tLBA a1, tIDECHS *a2)
{
    int a;

    a2->C = a1 / (g1.H * g1.S);

    a = a1 % (g1.H * g1.S);

    a2->H = a / g1.S;

    a2->S = a % g1.S;

    return 1;
}

int a_chs2lba (tCHS g1, tCHS a1, tLBA *a2)
{
    if ((g1.C < a1.C) || (g1.H < a1.H) || (g1.S < a1.S)) return 0;

    *a2 = a1.C * (g1.H * g1.S) + a1.H * g1.S + a1.S;

    return 1;
}

int a_large2lba (tLARGE g1, tLARGE a1, tLBA *a2)
{
    if ((g1.C < a1.C) || (g1.H < a1.H) || (g1.S < a1.S)) return 0;

    *a2 = a1.C * (g1.H * g1.S) + a1.H * g1.S + a1.S;

    return 1;
}

int a_idechs2lba (tIDECHS g1, tIDECHS a1, tLBA *a2)
{
    if ((g1.C < a1.C) || (g1.H < a1.H) || (g1.S < a1.S)) return 0;

    *a2 = a1.C * (g1.H * g1.S) + a1.H * g1.S + a1.S;

    return 1;
}

int a_large2chs (tLARGE g1, tCHS g2, tLARGE a1, tCHS *a2)
{
    int c, h, k = 1;

    if (g1.C >= g2.C || g1.H <= g2.H) return 0;

    c = g1.C;
    h = g1.H;

    do {
        c *= 2;
        h /= 2;
        k *= 2;
    } while ((c < g2.C) && (h > g2.H));

    a2->C = g1.C * (a1.H % k) + a1.C;
    a2->H = a1.H % k;
    a2->S = a1.S;

    return 1;
}

int a_large2idechs (tLARGE g1, tIDECHS g2, tLARGE a1, tIDECHS *a2)
{
    int c, h, k = 1;

    if (g1.C >= g2.C || g1.H <= g2.H) return 0;

    c = g1.C;
    h = g1.H;

    do {
        c *= 2;
        h /= 2;
        k *= 2;
    } while ((c < g2.C) && (h > g2.H));

    a2->C = g1.C * (a1.H % k) + a1.C;
    a2->H = a1.H % k;
    a2->S = a1.S;

    return 1;
}

int a_chs2large (tCHS g1, tLARGE g2, tCHS a1, tLARGE *a2)
{
    int c, h, k=1;

    if (g1.C <= g2.C || g1.H >= g2.H) return 0;

    c = g1.C;
    h = g1.H;

    do {
        c /= 2;
        h *= 2;
        k *= 2;
    } while ((c > g2.C) && (h < g2.H));

    a2->C = a1.C % g2.C;
    a2->H = (g2.H / k) * (a1.C % k) + a1.H;
    a2->S = a1.S;

    return 1;
}

int a_idechs2large (tIDECHS g1, tLARGE g2, tIDECHS a1, tLARGE *a2)
{
    tLBA temp;

    a_idechs2lba(g1, a1, &temp);

    if (!a_lba2large(g2, temp, a2)) return 0;

    return 1;
}

int a_chs2idechs (tCHS g1, tIDECHS g2, tCHS a1, tIDECHS *a2)
{
    int c,h,k=1;

    if (g1.C >= g2.C || g1.H <= g2.H) return 0;

    c = g1.C;
    h = g1.H;

    do {
    c *= 2;
    h /= 2;
    k *= 2;
    } while ((c < g2.C) && (h > g2.H));

    a2->C = g1.C * (a1.H % k) + a1.C;
    a2->H = a1.H % k;
    a2->S = a1.S;

    return 1;
}

int a_idechs2chs (tIDECHS g1, tCHS g2, tIDECHS a1, tCHS *a2)
{
    tLBA temp;

    a_idechs2lba(g1, a1, &temp);

    if (!a_lba2chs(g2, temp, a2)) return 0;

    return 1;
}