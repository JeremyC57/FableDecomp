#pragma optimize("s",on)
struct A { int x; };
struct B { int x; };
struct C { int x; };

class CPersistContext { public:                       // typed transfer helpers: each starts `push 0x122d70e ; call 0x404500` (CheckCRC)
    template <class T> void Transfer(T& value);                 // 0x00431102 Transfer(long&)
    void Transfer_00466a47(A* value);               // 0x00466A47 per-type transfer instantiation, element type not attributed
};

class CCarrySlotDef { public:
    unsigned char _pad[0x28];
    A a;   // +0x28
    B b;   // +0x2c
    C c;   // +0x30
    A a2;  // +0x34
};

void __fastcall CCarrySlotDef_Transfer(CCarrySlotDef* def, void* edx, CPersistContext* ctx)
{
    ctx->Transfer(*(long*)(&def->a));
    ctx->Transfer(*(long*)(&def->b));
    ctx->Transfer(*(long*)(&def->c));
    ctx->Transfer_00466a47(&def->a2);
}