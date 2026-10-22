#pragma optimize("s",on)
struct A { int x; };
struct B { int x; };
struct C { int x; };

struct CPersistContext {                       // typed transfer helpers: each starts `push 0x122d70e ; call 0x404500` (CheckCRC)
    void TransferLong(void* value);                 // 0x00431102 Transfer(long&)
    void Transfer_00466a47(A* value);               // 0x00466A47 per-type transfer instantiation, element type not attributed
};

struct CCarrySlotDef {
    unsigned char _pad[0x28];
    A a;   // +0x28
    B b;   // +0x2c
    C c;   // +0x30
    A a2;  // +0x34
};

void __fastcall CCarrySlotDef_Transfer(CCarrySlotDef* def, void* edx, CPersistContext* ctx)
{
    ctx->TransferLong(&def->a);
    ctx->TransferLong(&def->b);
    ctx->TransferLong(&def->c);
    ctx->Transfer_00466a47(&def->a2);
}