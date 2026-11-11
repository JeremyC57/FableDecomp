#ifndef FABLE_ENGINE_CTHINGMARKER_H
#define FABLE_ENGINE_CTHINGMARKER_H
#include "engine/CThingGameObject.h"
#ifdef FABLE_RETAIL_CTHINGMARKER_DEFINITION
// Retail opt-in (agentNF9): see the class body. Needs FABLE_RETAIL_CTHING_MOREACCESS and the CGameDefinitionManager chain.
#include "engine/CDefPointer.h"
#include "engine/CEngineGraphic.h"
#include "engine/CThingMarkerDef.h"
#include "engine/CGameDefinitionManager.h"
#endif
// Both original PDBs: CThingGameObject base, no added data; retail RTTI agrees.
class CThingMarker : public CThingGameObject { public:static long __fastcall StatGetType(){return 11;}
#ifdef FABLE_RETAIL_CTHINGMARKER
    // Header wave 2026-11-03 (row 0x00522A20): Ego_r.pdb ?Alloc@CThingMarker@@SIPAVCClass@@XZ.
    static CClass* __fastcall Alloc();
#endif
#ifdef FABLE_RETAIL_CTHINGMARKER_DEFINITION
    // Retail opt-in (agentNF9, rows 0x0083D190 / 0x0083D280). Ego_r.pdb ?PeekDefinition@CThingMarker@@IBE?AV?$CDefPointer@$$CBVCThingMarkerDef@@@@XZ
    // (protected const, the def by value; no out-of-line copy in Fable.exe: GetProperties expands it, the copy's add-ref and the local's
    // release sharing one null test, 0x0083D2A6), ?GetCurrentGraphic@CThingMarker@@IBE?AVCEngineGraphic@@XZ (protected const, 0x0083D190;
    // its only caller is the CThingMarker virtual 0x0083D2E0) and ?GetProperties@CThingMarker@@UBEKXZ (public const virtual, vtable
    // 0x01245524 slot 48 = 0x0083D280).
protected:
    CDefPointer<const CThingMarkerDef> PeekDefinition() const
    {
        CDefPointer<const CThingMarkerDef> def;
        PeekDefinitionManager().GetDef(DefGlobalIndex, &def);
        return def;
    }
    CEngineGraphic GetCurrentGraphic() const;                            // 0x0083D190
public:
    virtual unsigned long GetProperties() const;                         // 0x0083D280
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
#ifdef FABLE_RETAIL_CTHINGMARKER_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool IsEntitySerialisable() const;  // slot 22, 0x00529C10
public:
#endif
};
FABLE_STATIC_ASSERT(sizeof(CThingMarker)==0xd0);
#endif
