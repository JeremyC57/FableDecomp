#ifndef FABLE_ENGINE_CTHINGOBJECT_H
#define FABLE_ENGINE_CTHINGOBJECT_H
#include "engine/CThingGameObject.h"
// Retail opt-in (Codex pass 12): original PDB sole base and full native allocation.
// CDefPointer<const CThingObjectDef> PDef +d0, DieOffTimer +d4, DiesOff +d8; native allocation527320.
#ifdef FABLE_RETAIL_CTHINGOBJECT_CREATE
class CRightHandedSet;
#endif
#ifdef FABLE_RETAIL_CTHINGOBJECT_DEFINITION
// Retail opt-in (agentNF9, rows 0x00703050 / 0x007030C0 / 0x00703110 / 0x00703150): PDef typed as the Ego_r.pdb member
// CDefPointer<const CThingObjectDef> (same 4 bytes at +0xD0), the inline PeekDefinition and the virtual overrides those rows define.
// Needs FABLE_RETAIL_CTHINGOBJECTDEF_VALIDATE (the def's members), FABLE_RETAIL_CTHING_MOREACCESS and the CGameDefinitionManager chain.
#include "engine/CDefPointer.h"
#include "engine/CThingObjectDef.h"
#include "engine/CGameDefinitionManager.h"
class CMaterialDef; class CPersistContext;
#endif
class CThingObject:public CThingGameObject {
protected:
#ifdef FABLE_RETAIL_CTHINGOBJECT_DEFINITION
    CDefPointer<const CThingObjectDef> PDef; // +d0 (Ego_r.pdb member type), ctor7031ec
#else
    unsigned char PDef[4]; // +d0 CDefPointer<const CThingObjectDef>, ctor7031ec
#endif
    long DieOffTimer; // +d4, ctor7031f2
    bool DiesOff; // +d8, ctor7031f8
    unsigned char _pad_0xd9[7];
public: static CClass* __fastcall Alloc();
#ifdef FABLE_RETAIL_CTHINGOBJECT_DEFINITION
    // Retail opt-in (agentNF9). Spellings: Ego_r.pdb / FableWin.pdb `@CThingObject@@`. CThingObject vtable 0x01264DCC: slot 3
    // SerialiseGameState 0x007030C0, slot 4 ConstructFromLevelScript 0x00703110, slot 58 PeekPMaterial 0x00703150 (the catalog's
    // vfunc_3 / vfunc_4 / vfunc_58); ConstructFromParams 0x00703050 is FABLE_RETAIL_CTHINGOBJECT_CONSTRUCTFROMPARAMS. PeekDefinition
    // (?PeekDefinition@CThingObject@@IBEABVCThingObjectDef@@XZ) has no out-of-line copy in Fable.exe: retail reads PDef in place
    // (0x00703150 `mov eax,[esi+0D0h] ; mov ecx,[eax+84h]`). Declarations only otherwise.
protected:
    const CThingObjectDef& PeekDefinition() const { return *PDef; }
    virtual bool SerialiseGameState(CPersistContext& context);           // 0x007030C0 ?SerialiseGameState@CThingObject@@MAE_NAAVCPersistContext@@@Z
    virtual bool ConstructFromLevelScript(const char* script);           // 0x00703110 ?ConstructFromLevelScript@CThingObject@@MAE_NPBD@Z
    virtual CDefPointer<const CMaterialDef> PeekPMaterial() const;       // 0x00703150 ?PeekPMaterial@CThingObject@@MBE?AV?$CDefPointer@$$CBVCMaterialDef@@@@XZ
public:
#endif
#ifdef FABLE_RETAIL_CTHINGOBJECT_ONHIT
    // Retail opt-in (chunk 794, row 0x007039A0): Ego_r.pdb / FableWin.pdb ?OnHit@CThingObject@@MAEXAAVCHitParameters@@@Z (protected
    // virtual override of CThingGameObject::OnHit 0x00839940, which it calls by name). Declaration only: no layout change.
protected:
    virtual void OnHit(CHitParameters& params);                        // 0x007039A0
public:
#endif
#ifdef FABLE_RETAIL_CTHINGOBJECT_CREATE
    // Retail opt-in (chunk 1003, row 0x005B3AEF CTCInventoryClothing::OnInitialActivate): Ego_r.pdb
    // ?Create@CThingObject@@SIPAV1@JABVC3DVector@@JPBVCRightHandedSet@@KABVCCharString@@@Z (static __fastcall, out of line at 0x00703210:
    // the caller passes the def index in ecx, the position in edx and pushes the name, flags, set and player). Declaration only.
    static CThingObject* __fastcall Create(long defIndex, const C3DVector& position, long playerNumber, const CRightHandedSet* rhSet,
                                           unsigned long flags, const CCharString& scriptName);   // 0x00703210
#endif
#ifdef FABLE_RETAIL_CTHINGOBJECT_CONSTRUCTFROMPARAMS
    // Retail opt-in (chunk 1014, row 0x00703210 CThingObject::Create): Ego_r.pdb
    // ?ConstructFromParams@CThingObject@@IAE_NJABVC3DVector@@JKABVCCharString@@@Z (protected, out of line at 0x00703050; Create
    // pushes the name, flags, player, position and def index). Declaration only: no layout change.
protected:
    bool ConstructFromParams(long defIndex, const C3DVector& position, long playerNumber, unsigned long flags,
                             const CCharString& scriptName);                                     // 0x00703050
public:
#endif
#ifdef FABLE_RETAIL_CTHINGOBJECT_STATGETTYPE
    // Chunk 756 (row 0x009104D0): Ego_r.pdb / FableWin.pdb ?StatGetType@CThingObject@@SIJXZ; the thing-type list 5 that
    // GetAllLeafThings<CThingObject> 0x00910A90 scans (`push 5 ; call PeekTypeList`).
    static long __fastcall StatGetType() { return 5; }
#endif
#ifdef FABLE_RETAIL_CTHINGOBJECT_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool CanBlockUpperLevelNavigatingCreature() const;  // slot 56, 0x00702CD0
public:
#endif
#ifdef FABLE_RETAIL_CTHINGOBJECT_VSLOTS2
// agentFAM04 (2026-11-11): retail CThingObject overrides (TLC vtable slots aligned with the Xbox 360 Test PDB order;
// Ego_r.pdb / FableWin.pdb spellings, all public virtual). Declaration only (no layout change).
public:
    virtual bool IsEntitySerialisable() const;                           // slot 22, 0x00703470
    virtual bool IsFragile() const;                                      // slot 54, 0x00703000
    virtual bool CanBlockNavigatingCreature() const;                     // slot 55, 0x00702CC0
    virtual bool IsCover() const;                                        // slot 64, 0x00703780
    virtual long GetMaterialOnHitByMelee() const;                        // slot 70, 0x00703040
public:
#endif // FABLE_RETAIL_CTHINGOBJECT_VSLOTS2
};
FABLE_STATIC_ASSERT(sizeof(CThingObject)==0xe0);
#endif
