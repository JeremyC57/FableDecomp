#pragma optimize("s",on)
struct CMember { int x; };

class CPersistContext { public:
    template <class T> void Transfer(T& value);  // __fastcall: ecx=this, stack=m
};

class CGuildMasterDef { public:
    unsigned char pad0[0x28];
    CMember m28;
    CMember m2c;
};

void __fastcall CGuildMasterDef_Transfer(CGuildMasterDef* def, void* edx, CPersistContext* ctx)
{
    ctx->Transfer(*(float*)(&def->m28));
    ctx->Transfer(*(float*)(&def->m2c));
}