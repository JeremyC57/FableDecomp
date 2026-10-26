#pragma optimize("s",on)
struct CSubA { char b[4]; };
struct CSubB { char b[4]; };

class CRGBColour;
class CPersistContext { public:                       // typed transfer helpers: each starts `push 0x122d70e ; call 0x404500` (CheckCRC)
    template <class T> void Transfer(T& value);                // 0x00431102 Transfer(long&)
};

class CPlayerDef { public:
    char pad3c[0x3c];
    CSubA m3c;   // sub-object at +0x3c
    CSubB m40;   // sub-object at +0x40
    void Transfer(CPersistContext& ctx);
    void TransferB(CPersistContext& ctx); // base transfer at 0x4595a3, takes &self->m40
};

void CPlayerDef::Transfer(CPersistContext& ctx)
{
    ctx.Transfer(*(long*)(&this->m3c));
    // second call: this=ecx=ctx, pushes &self->m40, target 0x4595a3
    reinterpret_cast<CPersistContext*>(&ctx)->Transfer(*(CRGBColour*)(reinterpret_cast<CSubA*>(&this->m40)));
}