// Leaf: ?CSimBuildingDef_Transfer@@YIXPAVCVillagerInteractionsDef@@PAXPAVCPersistContext@@@Z
#pragma optimize("s",on)
class CPersistContext { public:                       // typed transfer helpers: each starts `push 0x122d70e ; call 0x404500` (CheckCRC)
    template <class T> void Transfer(T& value);                  // 0x00431102 Transfer(long&)
};

class CVillagerInteractionsDef { public:
    char pad[0x3c];
    int fieldA;   // +0x3c
    int fieldB;   // +0x40
    int fieldC;   // +0x44
};

void __fastcall CSimBuildingDef_Transfer(CVillagerInteractionsDef* def, void* /*edx*/, CPersistContext* ctx)
{
    ctx->Transfer(*(long*)&(def->fieldA));
    ctx->Transfer(*(long*)&(def->fieldB));
    ctx->Transfer(*(long*)&(def->fieldC));
}
// Stand-in (owner rule 2026-11-05): CSimBuildingDef_Transfer for CSimBuildingDef::Transfer; RTTI vtable slot ?Transfer@CSimBuildingDef@@UAEXAAVCPersistContext@@@Z + PDB name set (egor/fablewin).
