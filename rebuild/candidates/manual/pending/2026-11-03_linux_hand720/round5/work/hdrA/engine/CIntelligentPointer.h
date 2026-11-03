#ifndef FABLE_ENGINE_CINTELLIGENTPOINTER_H
#define FABLE_ENGINE_CINTELLIGENTPOINTER_H
#include "rebuild_abi.h"
class CBaseObject;
class CBaseObjectPointer;
#ifdef FABLE_RETAIL_CINTELLIGENTPOINTER
// Retail opt-in (2026-11-03): the intelligent-pointer classes are byte-packed (alignment 1), so a member of these types sits at
// an odd offset inside a naturally aligned owner. Evidence: retail CTCTavernGame::FrameUpdate 0x005EB9E0 reads the interface-0x33
// CTCOwnedEntity::POwner (CIntelligentPointer<CThing>) at +0x0D (`lea edi,[eax+0Dh]` at 0x005EBACD, then `call 0x00A01B50`
// GetPItem), which Ego_r.pdb also puts at +0x0D (struct_layouts_egor.tsv: SwitchableNavigationTCAdded bool +0x0C, POwner +0x0D,
// OwnerUID unsigned __int64 +0x18); OwnerUID at +0x18 (not +0x15) shows the owner itself is not packed. Ego_r.pdb has 60 of its
// 648 CIntelligentPointer<...> members at offsets that are not multiples of 4 (e.g. CAtmosProcess +0x06, CTCInventoryWeapons
// +0x159, CTCVillage +0x149, CEventVillagerInteraction +0x09), so the packing is a property of the pointer classes. Lionhead's
// own header packed them: the owner convention for engine classes is `#pragma pack(push, 1)` / `#pragma pack(pop)` in the
// class's own header (GOTCHAS 2026-11-02 owner; <pshpack1.h> is for SDK structs).
// Members: Ego_r.pdb / FableWin.pdb ??0CBaseIntelligentPointer@@QAE@XZ (0x00A01B10), ??1..UAE@XZ (0x00A01C10),
// ?GetPItem@..QBEPAVCBaseObject@@XZ (0x00A01B50), ?SetPItem@..QAEXPAVCBaseObject@@@Z (0x00A01B90); PData at +4 behind the vptr.
#pragma pack(push, 1)
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                                  // 0x00A01C10
    CBaseObject* GetPItem() const;                                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                                           // +0x04
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
#ifdef FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR
    // Retail opt-in (2026-11-03, chunk 720 row 0x0065B854): the converting constructor from the pointee, out of line in retail
    // (one COMDAT per instantiation: ??0?$CIntelligentPointer@VCThing@@@@QAE@PAVCThing@@@Z 0x004A96F0, <CThingBuilding> 0x0050F380,
    // <CThingAICreature> 0x0050F3F0, <CThingObject> 0x0065E133, <CThingTrackNode> 0x0065E15D, <CThingMarker> 0x0065E21E; every
    // caller pushes the pointer and calls it). Declaration only: no layout change.
    CIntelligentPointer(T* item);
#endif
    T* Get() const { return static_cast<T*>(GetPItem()); }             // FableWin ?Get@?$CIntelligentPointer@..@@QBEPAV..@XZ
    T* operator->() const { return Get(); }                             // FableWin ??C?$CIntelligentPointer@..@@QBEPAV..@XZ
};
#pragma pack(pop)
FABLE_STATIC_ASSERT(sizeof(CBaseIntelligentPointer)==8);
#else
// Ego_r TPI CBaseIntelligentPointer: independent polymorphic class, PData +4;
// retail GetPItem 00a01b50 receives this+0 and reads PData +4.
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;
private:
    CBaseObjectPointer* PData;
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* Get() const { return static_cast<T*>(GetPItem()); }
    T* operator->() const { return Get(); }
};
FABLE_STATIC_ASSERT(sizeof(CBaseIntelligentPointer)==8);
#endif
#endif
