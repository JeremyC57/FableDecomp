#ifndef FABLE_ENGINE_CTHINGOBJECT_H
#define FABLE_ENGINE_CTHINGOBJECT_H
#include "engine/CThingGameObject.h"
// Retail opt-in (Codex pass 12): original PDB sole base and full native allocation.
// CDefPointer<const CThingObjectDef> PDef +d0, DieOffTimer +d4, DiesOff +d8; native allocation527320.
class CThingObject:public CThingGameObject {
protected:
    unsigned char PDef[4]; // +d0 CDefPointer<const CThingObjectDef>, ctor7031ec
    long DieOffTimer; // +d4, ctor7031f2
    bool DiesOff; // +d8, ctor7031f8
    unsigned char _pad_0xd9[7];
public: static CClass* __fastcall Alloc();
#ifdef FABLE_RETAIL_CTHINGOBJECT_ONHIT
    // Retail opt-in (chunk 794, row 0x007039A0): Ego_r.pdb / FableWin.pdb ?OnHit@CThingObject@@MAEXAAVCHitParameters@@@Z (protected
    // virtual override of CThingGameObject::OnHit 0x00839940, which it calls by name). Declaration only: no layout change.
protected:
    virtual void OnHit(CHitParameters& params);                        // 0x007039A0
public:
#endif
#ifdef FABLE_RETAIL_CTHINGOBJECT_STATGETTYPE
    // Chunk 756 (row 0x009104D0): Ego_r.pdb / FableWin.pdb ?StatGetType@CThingObject@@SIJXZ; the thing-type list 5 that
    // GetAllLeafThings<CThingObject> 0x00910A90 scans (`push 5 ; call PeekTypeList`).
    static long __fastcall StatGetType() { return 5; }
#endif
#ifdef FABLE_RETAIL_CTHINGOBJECT_CREATE
    // Nested opt-in (chunk 850, row 0x00772BB0 CTCShop::AddSaleItem): Ego_r.pdb
    // ?Create@CThingObject@@SIPAV1@JABVC3DVector@@JPBVCRightHandedSet@@KABVCCharString@@@Z, out of line in retail at 0x00703210
    // (reloc_bindings; __fastcall: def index in ecx, position in edx, the other four pushed). Declaration only.
    static CThingObject* __fastcall Create(long def_index, const C3DVector& pos, long player, const CRightHandedSet* rhset,
                                           unsigned long flags, const CCharString& script_name);   // 0x00703210
#endif
};
FABLE_STATIC_ASSERT(sizeof(CThingObject)==0xe0);
#endif
