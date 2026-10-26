// CTCNoise::vfunc_9() -- per-frame update: count the life timer down and kill the parent thing when it reaches zero
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00749860, length 29 bytes.
// Callees: CThing::Kill @ 0x004C9B80 (__fastcall, one argument, pushed as 1).
// PDB CTCNoise: Definition (CTCNoiseDef, 0x34 bytes) at +0x0c, LifeTimer +0x40.
// Shape: the decrement goes through eax and is stored; the zero test re-reads the member at the join.
class CThing { public:
    void Kill(bool removeNow);                      // 0x004C9B80
};

struct CTCBase_00749860 {
    virtual void v00();
    CThing*       ParentThing;                      // +0x04
    unsigned char _pad08[4];
};

class CTCNoise : public CTCBase_00749860 { public:
    unsigned char Definition[0x34];                 // +0x0c CTCNoiseDef
    long          LifeTimer;                        // +0x40
    virtual void vfunc_9();
};

void CTCNoise::vfunc_9()
{
    if (LifeTimer > 0)
        --LifeTimer;
    if (LifeTimer == 0)
        ParentThing->Kill(true);
}