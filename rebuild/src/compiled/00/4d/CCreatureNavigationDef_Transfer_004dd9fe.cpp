#pragma optimize("s",on)
struct A { int x; };
struct B { int x; };
struct C { int x; };

class CPersistContext { public:                       // typed transfer helpers: each starts `push 0x122d70e ; call 0x404500` (CheckCRC)
    template <class T> void Transfer(T& value);                // 0x00431061 Transfer(float&)
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
    ctx->Transfer(*(float*)(&def->a));
    ctx->Transfer(*(float*)(&def->b));
    ctx->Transfer(*(float*)(&def->c));
    ctx->Transfer(*(bool*)(&def->a2));
}