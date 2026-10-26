#pragma optimize("s",on)
class CPersistContext { public:                       // typed transfer helpers: each starts `push 0x122d70e ; call 0x404500` (CheckCRC)
    template <class T> void Transfer(T& value);                  // 0x00431102 Transfer(long&)
    void Transfer_00452e01(int& value);             // 0x00452E01 per-type transfer instantiation, element type not attributed
};

class CVillagerInteractionsDef { public:
    char pad[0x3c];
    int fieldA;   // +0x3c
    int fieldB;   // +0x40
    int fieldC;   // +0x44
};

void __fastcall CVillagerInteractionsDef_Transfer(CVillagerInteractionsDef* def, void* /*edx*/, CPersistContext* ctx)
{
    ctx->Transfer(*(long*)&(def->fieldA));
    ctx->Transfer_00452e01(def->fieldB);
    ctx->Transfer(*(long*)&(def->fieldC));
}