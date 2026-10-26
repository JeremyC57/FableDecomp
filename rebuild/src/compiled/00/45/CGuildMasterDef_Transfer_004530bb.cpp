#pragma optimize("s",on)
struct CMember { int x; };

class CPersistContext { public:
    void Persist(CMember* m);  // __fastcall: ecx=this, stack=m
};

class CGuildMasterDef { public:
    unsigned char pad0[0x28];
    CMember m28;
    CMember m2c;
};

void __fastcall CGuildMasterDef_Transfer(CGuildMasterDef* def, void* edx, CPersistContext* ctx)
{
    ctx->Persist(&def->m28);
    ctx->Persist(&def->m2c);
}