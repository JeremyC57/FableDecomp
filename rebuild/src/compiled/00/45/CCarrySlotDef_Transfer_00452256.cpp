#pragma optimize("s",on)
struct A { int x; };
struct B { int x; };
struct C { int x; };

class CDefString;
class CPersistContext { public:
    template <class T> void Transfer(T& value);
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
    ctx->Transfer(*(CDefString*)(&def->a));
    ctx->Transfer(*(long*)(&def->b));
    ctx->Transfer(*(bool*)(&def->c));
    ctx->Transfer(*(CDefString*)(&def->a2));
}