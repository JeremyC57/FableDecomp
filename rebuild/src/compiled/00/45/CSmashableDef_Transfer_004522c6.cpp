#pragma optimize("s",on)
#include "engine/CSmashableDef.h"  // retyped onto the PDB layout; byte parity re-verified

struct CPersistContext {
    void PersistA(unsigned char* p);
    void PersistB(unsigned char* p);
    void PersistC(int* p);
};

void __fastcall CSmashableDef_Transfer(CSmashableDef* def, int edx, CPersistContext* ctx)
{
    ctx->PersistA(reinterpret_cast<unsigned char*>(&def->Smashable));
    ctx->PersistB(reinterpret_cast<unsigned char*>(&def->ReplacementObject_Val));
    ctx->PersistC(reinterpret_cast<int*>(&def->SmashParticleEmitter));
}
