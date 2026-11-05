// Leaf: ?CSummonableCreatureDef_Transfer@@YIXPAVCGuildMasterDef@@PAXPAVCPersistContext@@@Z
#pragma optimize("s",on)
struct CMember { int x; };

class CDefString;
class CPersistContext { public:                       // typed transfer helpers: each starts `push 0x122d70e ; call 0x404500` (CheckCRC)
    template <class T> void Transfer(T& value);             // 0x00431061 Transfer(float&)
};

class CGuildMasterDef { public:
    unsigned char pad0[0x28];
    CMember m28;
    CMember m2c;
};

void __fastcall CSummonableCreatureDef_Transfer(CGuildMasterDef* def, void* edx, CPersistContext* ctx)
{
    ctx->Transfer(*(float*)(&def->m28));
    ctx->Transfer(*(CDefString*)(&def->m2c));
}
// Stand-in (owner rule 2026-11-05): CSummonableCreatureDef_Transfer for CSummonableCreatureDef::Transfer; RTTI vtable slot ?Transfer@CSummonableCreatureDef@@UAEXAAVCPersistContext@@@Z + PDB name set (egor/fablewin).
