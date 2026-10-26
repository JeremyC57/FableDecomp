// CTCNymph::GetRandomFlightHeight() const -- a random height between the def's MinHeight and MaxHeight (1000 steps)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080F130, length 93 bytes.
// Callees: CTCBase::GetWorld() @ 0x00686A70, CWorld::GetPRandomSeed() @ 0x0049D810 (label "CVertexBufferWin32::DoSizeof"
// is propagated), GFROR13 @ 0x00497890 (__fastcall). Constants: 0.001f @ 0x01266300, 2^32 @ 0x0122DCB4.
// CNymphDef: MaxHeight +0x2c, MinHeight +0x30.
// Shape: one inlined world-random step (`seed = seed * 0x24A1 + 0x24DF ; seed = GFROR13(seed)`) then `% 1000` (unsigned
// `div`), converted as an unsigned long (`fild ; jge ; fadd 2^32`). The step `(max - min) * 0.001f` must be a NAMED float
// local: written inline (literal or extern constant, either operand order) VC7.1 multiplies by r first and by the
// constant last.
extern unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range == 0)
        return 0;
    return seed % range;
}

class CWorld { public:
    unsigned long* GetPRandomSeed();                // 0x0049D810
};

class CNymphDef { public:
    unsigned char _pad00[0x2c];
    float         MaxHeight;                        // +0x2c
    float         MinHeight;                        // +0x30
};

struct CTCBase_0080f130 {
    virtual void v00();
    unsigned char _pad04[8];
    CWorld* GetWorld() const;                       // 0x00686A70
};

class CTCNymph : public CTCBase_0080f130 { public:
    const CNymphDef* PDef;                          // +0x0c
    float GetRandomFlightHeight() const;
};

float CTCNymph::GetRandomFlightHeight() const
{
    unsigned long* seed = GetWorld()->GetPRandomSeed();
    unsigned long r = WorldRandomRange(*seed, 1000);
    float step = (PDef->MaxHeight - PDef->MinHeight) * 0.001f;
    return step * (float)r + PDef->MinHeight;
}