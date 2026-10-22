#pragma optimize("s",on)
struct Field { unsigned char b[4]; };

struct CPersistContext {
    void Xfer(Field* p);
};

struct CDragonActionNapalmDef {
    unsigned char _pad[0x28];
    Field f0; // +0x28
    Field f1; // +0x2c
    Field f2; // +0x30
    Field f3; // +0x34
};

void __fastcall CDragonActionNapalmDef_Transfer(CDragonActionNapalmDef* def, int edx, CPersistContext* ctx)
{
    ctx->Xfer(&def->f0);
    ctx->Xfer(&def->f1);
    ctx->Xfer(&def->f2);
    ctx->Xfer(&def->f3);
}