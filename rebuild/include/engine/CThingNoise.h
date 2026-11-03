#ifndef FABLE_ENGINE_CTHINGNOISE_H
#define FABLE_ENGINE_CTHINGNOISE_H
#include "engine/CThing.h"
// Retail opt-in (Codex pass 12): original PDB sole base and full native allocation.
// No own members; native allocation527380.
class CThingNoise:public CThing {

public: static CClass* __fastcall Alloc();
};
FABLE_STATIC_ASSERT(sizeof(CThingNoise)==0x98);
#endif
