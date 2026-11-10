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
#ifdef FABLE_RETAIL_CTHINGMARKER_CONSTRUCT
    // Retail opt-in (chunk 1021, row 0x0083D460 CThingMarker::Create; needs FABLE_RETAIL_CTHINGGAMEOBJECT_CTOR and
    // FABLE_RETAIL_CTHINGGAMEOBJECT_LEVELSCRIPT): FableWin.pdb ??0CThingMarker@@QAE@XZ (0x01D3FA70, inline in retail: CThingGameObject(11)
    // then the CThingMarker vtable 0x01245524, 0x0083D49F) and Ego_r.pdb ?ConstructFromParams@CThingMarker@@IAE_NJABVC3DVector@@ABVCCharString@@@Z
    // (protected; Ego_r 0x0082E6F0, expanded in retail's Create: CThingGameObject::ConstructFromParams(def, pos, -1, 0, name, 0, 0, true)
    // 0x008388D0, then Health / MaxHealth = 1.0, 0x0083D4C3..0x0083D4DF). Bodies only: no layout change.
    CThingMarker() : CThingGameObject(StatGetType()) { }
protected:
    bool ConstructFromParams(long defIndex, const C3DVector& position, const CCharString& scriptName)
    {
        if (!CThingGameObject::ConstructFromParams(defIndex, position, -1, 0, scriptName, 0, 0, true))
            return false;
        Health = 1.0f;
        MaxHealth = 1.0f;
        return true;
    }
public:
#endif
};
FABLE_STATIC_ASSERT(sizeof(CThingMarker)==0xd0);
#endif
