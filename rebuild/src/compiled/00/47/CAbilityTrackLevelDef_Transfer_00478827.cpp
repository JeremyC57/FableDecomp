#pragma optimize("s",on)
struct CPersistContext {                       // typed transfer helpers: each starts `push 0x122d70e ; call 0x404500` (CheckCRC)
    void TransferLong(int& value);                  // 0x00431102 Transfer(long&)
    void Transfer_00452e01(int& value);             // 0x00452E01 per-type transfer instantiation, element type not attributed
};

struct CVillagerInteractionsDef {
    char pad[0x3c];
    int fieldA;   // +0x3c
    int fieldB;   // +0x40
    int fieldC;   // +0x44
};

void __fastcall CVillagerInteractionsDef_Transfer(CVillagerInteractionsDef* self, void* /*edx*/, CPersistContext* ctx)
{
    ctx->TransferLong(self->fieldA);
    ctx->Transfer_00452e01(self->fieldB);
    ctx->TransferLong(self->fieldC);
}