#ifndef FABLE_ENGINE_CTHINGMARKER_H
#define FABLE_ENGINE_CTHINGMARKER_H
#include "engine/CThingGameObject.h"
// Both original PDBs: CThingGameObject base, no added data; retail RTTI agrees.
class CThingMarker : public CThingGameObject { public:
#ifdef FABLE_RETAIL_INIT_THING_TYPES
    static CClass* __fastcall Alloc();
#endif
static long __fastcall StatGetType(){return 11;} };
FABLE_STATIC_ASSERT(sizeof(CThingMarker)==0xd0);
#endif
