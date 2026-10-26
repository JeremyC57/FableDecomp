#pragma optimize("s",on)
struct A { int x; };
struct B { int x; };
struct C { int x; };

class CPersistContext { public:
    void PersistA(A* p);
    void PersistB(B* p);
    void PersistC(C* p);
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
    ctx->PersistA(&def->a);
    ctx->PersistB(&def->b);
    ctx->PersistC(&def->c);
    ctx->PersistA(&def->a2);
}