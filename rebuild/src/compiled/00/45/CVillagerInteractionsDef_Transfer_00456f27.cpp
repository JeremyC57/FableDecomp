#pragma optimize("s",on)
class CDefString;
class CPersistContext { public:
    template <class T> void Transfer(T& value);
};

class CVillagerInteractionsDef { public:
    char pad[0x3c];
    int fieldA;   // +0x3c
    int fieldB;   // +0x40
    int fieldC;   // +0x44
};

void __fastcall CVillagerInteractionsDef_Transfer(CVillagerInteractionsDef* def, void* /*edx*/, CPersistContext* ctx)
{
    ctx->Transfer(*(CDefString*)&(def->fieldA));
    ctx->Transfer(*(CDefString*)&(def->fieldB));
    ctx->Transfer(*(float*)&(def->fieldC));
}