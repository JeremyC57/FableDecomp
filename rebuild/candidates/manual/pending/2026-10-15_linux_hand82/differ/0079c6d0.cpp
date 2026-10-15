// CTCBulletTime::GetBulletTimeElapsedTicks() const -- ticks of bullet time used so far: ceil(ticks-per-second * m_fDuration) - BulletTimeRemainingTicks
// Flags: /QIfist
// Grade: DIFFER(64v71) -- lane gap: inline __asm; the retail body is a frame-pointer function whose rounding
//        is an inline-asm block on memory locals (`mov [ebp-0Ch],0.5f ; mov [ebp-8],1.0f ; fld x ;
//        fadd [ebp-0Ch] ; fistp dword [ebp-4] ; fild [ebp-4] ; fsub [ebp-8] ; fstp [ebp-14h]`, then an
//        integer compare of the float bits and `dec dword ptr [ebp-4]`); compiler-emitted C++ cannot
//        produce a bare `fistp dword` into a named local (/QIfist gives `fistp qword` + copies).
//
// Retail 0x0079C6D0, 71 bytes. Globals: 0x01375550 = the game's ticks-per-second long (`fild`).
// m_fDuration is +0x2c (Ego_r.pdb CTCBulletTime). The ceiling is the classic FPU trick: round
// (x + 0.5) to nearest with the default rounding mode, then step back one when (result - 1) equals x
// exactly (x was already integral and the half rounded up). The same inline-asm helper is inlined
// into 0x0079C680 (remaining percentage) and 0x0079C6D0 (elapsed ticks); 0x004FA9A0 is a standalone
// copy taking the float argument. No byte-pure reconstruction is possible: kept as the C model.

extern long g_TicksPerSecond;                       // 0x01375550

inline long CeilToLong(float value)                 // inline __asm FPU helper in the original
{
    long result = (long)(value + 0.5f);             // fistp: round to nearest
    float back = (float)result - 1.0f;
    if (*(long*)&value == *(long*)&back)
        --result;
    return result;
}

#pragma pack(push, 1)
struct CTCBulletTime
{
    void*         __vftable;                        // +0x00 (CTCBase)
    void*         ParentThing;                      // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x4];                   // +0x08
    long          BulletTimeRemainingTicks;         // +0x0c
    long          BulletTimeAbilityLevel;           // +0x10
    unsigned char _pad_0x14[0x18];                  // +0x14
    float         m_fDuration;                      // +0x2c

    long GetBulletTimeElapsedTicks() const;
};
#pragma pack(pop)

long CTCBulletTime::GetBulletTimeElapsedTicks() const
{
    return CeilToLong(g_TicksPerSecond * m_fDuration) - BulletTimeRemainingTicks;
}
