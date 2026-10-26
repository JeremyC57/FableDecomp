#pragma optimize("s",on)
class CPersistContext { public:
    void PersistA_0045228f(int& r);
    void PersistB(int& r);
};

class CVillagerInteractionsDef { public:
    char pad[0x3c];
    int fieldA;   // +0x3c
    int fieldB;   // +0x40
    int fieldC;   // +0x44
};

void __fastcall CVillagerInteractionsDef_Transfer(CVillagerInteractionsDef* def, void* /*edx*/, CPersistContext* ctx)
{
    ctx->PersistA_0045228f(def->fieldA);
    ctx->PersistA_0045228f(def->fieldB);
    ctx->PersistB(def->fieldC);
}