#pragma optimize("s",on)
#include "engine/CSmashableDef.h"  // retyped onto the PDB layout; byte parity re-verified

class CPersistContext { public:
    template <class T> void Transfer(T& value);
    void PersistB(unsigned char* p);
};

void __fastcall CSmashableDef_Transfer(CSmashableDef* def, int edx, CPersistContext* ctx)
{
    ctx->Transfer(*(bool*)(reinterpret_cast<unsigned char*>(&def->Smashable)));
    ctx->PersistB(reinterpret_cast<unsigned char*>(&def->ReplacementObject_Val));
    ctx->Transfer(*(long*)(reinterpret_cast<int*>(&def->SmashParticleEmitter)));
}
