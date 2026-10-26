#pragma optimize("s",on)
// Byte-exact reconstruction of CEngineLocalDetailLayerDef::Transfer @ 0x00477cb1

struct SubA { int a; };
struct SubB { int b; };

class CPersistContext { public:
    void PersistA(SubA* obj);   // __fastcall: ecx=this(persist), obj on stack
    void PersistB(SubB* obj);
};

class CEngineLocalDetailLayerDef { public:
    char pad[0x3c];
    SubA m3c;   // at +0x3c
    char pad2[0x48 - 0x3c - (int)sizeof(SubA)];
    SubB m48;   // at +0x48
    void Transfer(CPersistContext* ctx);
};

void CEngineLocalDetailLayerDef::Transfer(CPersistContext* ctx)
{
    ctx->PersistA(&this->m3c);
    ctx->PersistB(&this->m48);
}