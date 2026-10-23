#pragma optimize("s",on)
struct CPersistContext {
    void PersistA_0045228f(int& r);
    void PersistB(int& r);
};

struct CVillagerInteractionsDef {
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