#include <cstdio>
#include <cstring>

struct CAudioContext {
    void*         __vftable;
    void*         Module;                    // +0x04
    unsigned char _pad08[0x24];
    void*         pfnEntry_2c;               // +0x2c
    void*         pfnEntry_30;               // +0x30
    void*         pfnEntry_34;               // +0x34
    void*         pfnEntry_38;               // +0x38
    void*         pfnEntry_3c;               // +0x3c
    void*         pfnEntry_40;               // +0x40
    void*         pfnEntry_44;               // +0x44
    void*         pfnEntry_48;               // +0x48
    void*         pfnEntry_4c;               // +0x4c
    void*         pfnEntry_50;               // +0x50
    void*         pfnEntry_54;               // +0x54
    void*         pfnEntry_58;               // +0x58
    void*         pfnEntry_5c;               // +0x5c
    void*         pfnEntry_60;               // +0x60
    void*         pfnEntry_64;               // +0x64
    void*         pfnEntry_68;               // +0x68
    void*         pfnEntry_6c;               // +0x6c
    void*         pfnEntry_70;               // +0x70
    void*         pfnEntry_74;               // +0x74
    void*         pfnEntry_78;               // +0x78
    void*         pfnEntry_7c;               // +0x7c
    void*         pfnEntry_80;               // +0x80
    void*         pfnEntry_84;               // +0x84
    void*         pfnEntry_88;               // +0x88
    void*         pfnEntry_8c;               // +0x8c
    void*         pfnSourceUnqueueBuffers;   // +0x90
    void*         pfnEntry_94;               // +0x94
    void*         pfnListener3f;             // +0x98
    void*         pfnEntry_9c;               // +0x9c
    void*         pfnEntry_a0;               // +0xa0
    void*         pfnEntry_a4;               // +0xa4
    void*         pfnEntry_a8;               // +0xa8
    void*         pfnEntry_ac;               // +0xac
    void*         pfnEntry_b0;               // +0xb0
    void*         pfnEntry_b4;               // +0xb4
    void*         pfnEntry_b8;               // +0xb8
    void*         pfnEntry_bc;               // +0xbc
    void*         pfnEntry_c0;               // +0xc0
    void*         pfnEntry_c4;               // +0xc4
    void*         pfnEntry_c8;               // +0xc8
    void*         pfnEntry_cc;               // +0xcc
    void*         pfnEntry_d0;               // +0xd0
    void*         pfnEntry_d4;               // +0xd4
    void*         pfnEntry_d8;               // +0xd8
    void*         pfnEntry_dc;               // +0xdc
    void*         pfnEntry_e0;               // +0xe0
    void*         pfnEntry_e4;               // +0xe4
    void*         pfnGetProcAddress;         // +0xe8
    void*         pfnGetEnumValue;           // +0xec
    void*         pfnEAXSet;                 // +0xf0
    void*         pfnEAXGet;                 // +0xf4
    unsigned char _padf8[4];
    void*         Device;                    // +0xfc
    void*         Context;                   // +0x100
    bool          UseALGetProcAddress;       // +0x104
    void Reset(bool force);
};

void CAudioContext::Reset(bool force)
{
    if (force || UseALGetProcAddress) {
        pfnEntry_2c = 0;
        pfnEntry_30 = 0;
        pfnEntry_34 = 0;
        pfnEntry_38 = 0;
        pfnEntry_3c = 0;
        pfnEntry_40 = 0;
        pfnEntry_44 = 0;
        pfnEntry_48 = 0;
        pfnEntry_4c = 0;
        pfnEntry_50 = 0;
        pfnEntry_54 = 0;
        pfnEntry_58 = 0;
        pfnEntry_5c = 0;
        pfnEntry_60 = 0;
        pfnEntry_64 = 0;
        pfnEntry_68 = 0;
        pfnEntry_6c = 0;
        pfnEntry_70 = 0;
        pfnEntry_74 = 0;
        pfnEntry_78 = 0;
        pfnEntry_7c = 0;
        pfnEntry_80 = 0;
        pfnEntry_84 = 0;
        pfnEntry_88 = 0;
        pfnEntry_8c = 0;
        pfnSourceUnqueueBuffers = 0;
        pfnEntry_94 = 0;
        pfnListener3f = 0;
        pfnEntry_9c = 0;
        pfnEntry_a0 = 0;
        pfnEntry_a4 = 0;
        pfnEntry_a8 = 0;
        pfnEntry_ac = 0;
        pfnEntry_b0 = 0;
        pfnEntry_b4 = 0;
        pfnEntry_b8 = 0;
        pfnEntry_bc = 0;
        pfnEntry_c0 = 0;
        pfnEntry_c4 = 0;
        pfnEntry_c8 = 0;
        pfnEntry_cc = 0;
        pfnEntry_d0 = 0;
        pfnEntry_d4 = 0;
        pfnEntry_d8 = 0;
        pfnEntry_dc = 0;
        pfnEntry_e0 = 0;
        pfnEntry_e4 = 0;
        pfnGetProcAddress = 0;
        pfnGetEnumValue = 0;
        pfnEAXSet = 0;
        pfnEAXGet = 0;
    }
}

static int AllZero(CAudioContext& c)
{
    void** p = &c.pfnEntry_2c;
    for (int i = 0; i < 51; ++i) if (p[i] != 0) return 0;
    return 1;
}
static void Fill(CAudioContext& c)
{
    void** p = &c.pfnEntry_2c;
    for (int i = 0; i < 51; ++i) p[i] = (void*)(&c);
    c.Device = (void*)&c; c.Context = (void*)&c;
}
int main()
{
    static CAudioContext c;
    int ok = 1;
    Fill(c); c.UseALGetProcAddress = false; c.Reset(false);
    if (AllZero(c) || c.pfnEAXGet != (void*)&c) ok = 0;            // untouched
    Fill(c); c.UseALGetProcAddress = false; c.Reset(true);
    if (!AllZero(c) || c.Device != (void*)&c || c.Context != (void*)&c) ok = 0;
    Fill(c); c.UseALGetProcAddress = true; c.Reset(false);
    if (!AllZero(c) || !c.UseALGetProcAddress) ok = 0;
    if (!ok) { std::printf("00c55b70_TEST FAIL\n"); return 1; }
    std::printf("00c55b70_TEST PASS\n");
    return 0;
}