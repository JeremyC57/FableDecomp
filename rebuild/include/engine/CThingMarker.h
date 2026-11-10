#ifndef FABLE_ENGINE_CTHINGMARKER_H
#define FABLE_ENGINE_CTHINGMARKER_H
#include "engine/CThingGameObject.h"
// Both original PDBs: CThingGameObject base, no added data; retail RTTI agrees.
class CThingMarker : public CThingGameObject { public:static long __fastcall StatGetType(){return 11;}
#ifdef FABLE_RETAIL_CTHINGMARKER
    // Header wave 2026-11-03 (row 0x00522A20): Ego_r.pdb ?Alloc@CThingMarker@@SIPAVCClass@@XZ.
    static CClass* __fastcall Alloc();
#endif
#ifdef FABLE_RETAIL_CTHINGMARKER_CREATE
    // Opt-in (chunk 1012, row 0x006B2260 CAtmosProcess::Update): Ego_r.pdb / FableWin.pdb
    // ?Create@CThingMarker@@SIPAV1@JABVC3DVector@@ABVCCharString@@@Z, out of line in retail at 0x0083D460 (it constructs a
    // CThingPhysical of type 11 = StatGetType; NAME_FIXES_QUEUE c524). Declaration only.
    static CThingMarker* __fastcall Create(long defIndex, const C3DVector& position, const CCharString& scriptName);   // 0x0083D460
#endif
};
FABLE_STATIC_ASSERT(sizeof(CThingMarker)==0xd0);
#endif
