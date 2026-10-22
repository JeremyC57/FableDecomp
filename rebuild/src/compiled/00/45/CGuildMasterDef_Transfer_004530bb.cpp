#pragma optimize("s",on)
struct CMember { int x; };

struct CPersistContext {
    void Persist(CMember* m);  // __fastcall: ecx=this, stack=m
};

struct CGuildMasterDef {
    unsigned char pad0[0x28];
    CMember m28;
    CMember m2c;
};

void __fastcall CGuildMasterDef_Transfer(CGuildMasterDef* def, void* edx, CPersistContext* ctx)
{
    ctx->Persist(&def->m28);
    ctx->Persist(&def->m2c);
}