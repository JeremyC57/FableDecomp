#ifndef FABLE_ENGINE_CTHINGHOLYSITE_H
#define FABLE_ENGINE_CTHINGHOLYSITE_H
#include "engine/CThingGameObject.h"
// Retail opt-in (Codex pass 12): original PDB sole base and full native allocation.
// CDefPointer<const CThingHolySiteDef> Def +d0, PlayerStart +d4; native allocation52ac90.
class CThingHolySite:public CThingGameObject {
private:
    unsigned char Def[4]; // +d0 CDefPointer<const CThingHolySiteDef>, ctor52accd
    unsigned char PlayerStart; // +d4, original sole trailing member; native totald8
    unsigned char _pad_0xd5[3];
public: static CClass* __fastcall Alloc();
};
FABLE_STATIC_ASSERT(sizeof(CThingHolySite)==0xd8);
#endif
