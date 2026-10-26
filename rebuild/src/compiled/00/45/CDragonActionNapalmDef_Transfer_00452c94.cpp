#pragma optimize("s",on)
struct Field { unsigned char b[4]; };

class CPersistContext { public:
    template <class T> void Transfer(T& value);
};

class CDragonActionNapalmDef { public:
    unsigned char _pad[0x28];
    Field f0; // +0x28
    Field f1; // +0x2c
    Field f2; // +0x30
    Field f3; // +0x34
};

void __fastcall CDragonActionNapalmDef_Transfer(CDragonActionNapalmDef* def, int edx, CPersistContext* ctx)
{
    ctx->Transfer(*(float*)(&def->f0));
    ctx->Transfer(*(float*)(&def->f1));
    ctx->Transfer(*(float*)(&def->f2));
    ctx->Transfer(*(float*)(&def->f3));
}