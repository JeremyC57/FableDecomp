#pragma optimize("s",on)
#include "engine/CDecapitationDef.h"

struct CMember { int x; };
class CPersistContext { public: template <class T> void Transfer(T& value); };

void __fastcall CDecapitationDef_Transfer(CDecapitationDef* def, void* /*edx*/, CPersistContext* ctx)
{
    ctx->Transfer(*(long*)((CMember*)&def->BodyParticleEffect));
    ctx->Transfer(*(long*)((CMember*)&def->Head));
}
