#pragma optimize("s",on)
#include "engine/CDecapitationDef.h"

struct CMember { int x; };
struct CPersistContext { void Persist(CMember* m); };

void __fastcall CDecapitationDef_Transfer(CDecapitationDef* def, void* /*edx*/, CPersistContext* ctx)
{
    ctx->Persist((CMember*)&def->BodyParticleEffect);
    ctx->Persist((CMember*)&def->Head);
}
