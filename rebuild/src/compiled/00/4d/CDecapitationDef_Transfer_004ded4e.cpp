#pragma optimize("s",on)
#include "engine/CDecapitationDef.h"

struct CMember { int x; };
struct CPersistContext { void Persist_00431102(CMember* m); };

void __fastcall CDecapitationDef_Transfer(CDecapitationDef* def, void* /*edx*/, CPersistContext* ctx)
{
    ctx->Persist_00431102((CMember*)&def->BodyParticleEffect);
    ctx->Persist_00431102((CMember*)&def->Head);
}
