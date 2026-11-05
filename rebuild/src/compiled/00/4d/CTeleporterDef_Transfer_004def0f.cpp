// Leaf: ?CTeleporterDef_Transfer@@YIXPAVCTCNoiseDef@@PAXPAVCPersistContext@@@Z
#pragma optimize("s",on)
class CPersistContext { public:                       // typed transfer helpers: each starts `push 0x122d70e ; call 0x404500` (CheckCRC)
    template <class T> void Transfer(T& value);                 // 0x00431061 Transfer(float&)
};

class CTCNoiseDef { public:
    char pad[0x28];
    int a;
    int b;
    int c;
};

void __fastcall CTeleporterDef_Transfer(CTCNoiseDef* def, void* edx, CPersistContext* ctx)
{
    ctx->Transfer(*(float*)(&def->a));
    ctx->Transfer(*(long*)(&def->b));
    ctx->Transfer(*(bool*)(&def->c));
}
// Stand-in (owner rule 2026-11-05): CTeleporterDef_Transfer for CTeleporterDef::Transfer; RTTI vtable slot ?Transfer@CTeleporterDef@@UAEXAAVCPersistContext@@@Z + PDB name set (egor/fablewin).
