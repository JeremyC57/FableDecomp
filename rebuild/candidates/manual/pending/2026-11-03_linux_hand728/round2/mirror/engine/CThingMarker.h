#ifndef FABLE_ENGINE_CTHINGMARKER_H
#define FABLE_ENGINE_CTHINGMARKER_H
#include "engine/CThingGameObject.h"
// Both original PDBs: CThingGameObject base, no added data; retail RTTI agrees.
class CThingMarker : public CThingGameObject { public:static long __fastcall StatGetType(){return 11;}
#ifdef FABLE_RETAIL_CTHINGMARKER
    // Header wave 2026-11-03 (row 0x00522A20): Ego_r.pdb ?Alloc@CThingMarker@@SIPAVCClass@@XZ.
    static CClass* __fastcall Alloc();
#endif
};
FABLE_STATIC_ASSERT(sizeof(CThingMarker)==0xd0);
#endif
