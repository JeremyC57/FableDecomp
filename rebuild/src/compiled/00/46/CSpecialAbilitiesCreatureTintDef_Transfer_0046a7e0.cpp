// Leaf: ?CSpecialAbilitiesCreatureTintDef_Transfer@@YIXPAVCVillagerInteractionsDef@@PAXPAVCPersistContext@@@Z
#pragma optimize("s",on)
class CPersistContext { public:
    template <class T> void Transfer(T& value);
    void PersistB_00466a47(int& r);
};

class CVillagerInteractionsDef { public:
    char pad[0x3c];
    int fieldA;   // +0x3c
    int fieldB;   // +0x40
    int fieldC;   // +0x44
};

void __fastcall CSpecialAbilitiesCreatureTintDef_Transfer(CVillagerInteractionsDef* def, void* /*edx*/, CPersistContext* ctx)
{
    ctx->Transfer(*(float*)&(def->fieldA));
    ctx->Transfer(*(float*)&(def->fieldB));
    ctx->PersistB_00466a47(def->fieldC);
}
// Stand-in (owner rule 2026-11-05): CSpecialAbilitiesCreatureTintDef_Transfer for CSpecialAbilitiesCreatureTintDef::Transfer; RTTI vtable slot ?Transfer@CSpecialAbilitiesCreatureTintDef@@UAEXAAVCPersistContext@@@Z + PDB name set (egor/fablewin).
