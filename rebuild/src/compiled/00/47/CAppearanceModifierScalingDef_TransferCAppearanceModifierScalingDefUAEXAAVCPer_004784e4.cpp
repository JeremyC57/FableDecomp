#pragma optimize("s",on)
struct CPersistContext {                       // typed transfer helpers: each starts `push 0x122d70e ; call 0x404500` (CheckCRC)
    void TransferFloat(int& value);                 // 0x00431061 Transfer(float&)
};

struct CVillagerInteractionsDef {
    char pad[0x3c];
    int fieldA;   // +0x3c
    int fieldB;   // +0x40
    int fieldC;   // +0x44
};

void __fastcall CVillagerInteractionsDef_Transfer(CVillagerInteractionsDef* self, void* /*edx*/, CPersistContext* ctx)
{
    ctx->TransferFloat(self->fieldA);
    ctx->TransferFloat(self->fieldB);
    ctx->TransferFloat(self->fieldC);
}