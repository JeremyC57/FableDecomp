#pragma optimize("s",on)
struct A { int x; };
struct B { int x; };
struct C { int x; };

struct CPersistContext {                       // typed transfer helpers: each starts `push 0x122d70e ; call 0x404500` (CheckCRC)
    void TransferFloat(void* value);                // 0x00431061 Transfer(float&)
    void TransferBool(A* value);                    // 0x0043314A Transfer(bool&)
};

struct CCarrySlotDef {
    unsigned char _pad[0x28];
    A a;   // +0x28
    B b;   // +0x2c
    C c;   // +0x30
    A a2;  // +0x34
};

void __fastcall CCarrySlotDef_Transfer(CCarrySlotDef* self, void* edx, CPersistContext* ctx)
{
    ctx->TransferFloat(&self->a);
    ctx->TransferFloat(&self->b);
    ctx->TransferFloat(&self->c);
    ctx->TransferBool(&self->a2);
}