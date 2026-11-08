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
    // Header wave 2026-11-03 (Codex pass 12 row 0x008EDFF0 constructs PCreature(&creature) inline, SetPItem 0x00A01B90 call):
    // FableWin ??0?$CIntelligentPointer@VCThing@@@@QAE@XZ / QAE@PAVCThing@@@Z, ?Set@..QAEXPAVCThing@@@Z and the private
    // ?SetPointer@?$CIntelligentPointer@$$CBVCThing@@@@AAEXPBVCThing@@@Z.
    CIntelligentPointer() { }
#ifdef FABLE_RETAIL_CINTELLIGENTPOINTER_OUTOFLINE
    // Retail opt-in (chunk 858, row 0x00682810 CTCCreatureOpinionOfHero::InformOfDeed): FableWin.pdb ??0?$CIntelligentPointer@VCThing@@@@QAE@PAVCThing@@@Z
    // and ??4?$CIntelligentPointer@VCThing@@@@QAEAAV0@ABV0@@Z called out of line there (0x004A96F0 on the temporary built from
    // CTCDInternalMarker::Create's result, then 0x004A9760 into the member, then ~CBaseIntelligentPointer 0x00A01C10 on the temporary).
    // Declarations only: callers call the COMDATs.
    CIntelligentPointer(T* pointer);                                     // 0x004A96F0 (T = CThing)
    CIntelligentPointer& operator=(const CIntelligentPointer& other);    // 0x004A9760 (T = CThing)
#else
    CIntelligentPointer(T* pointer) { SetPointer(pointer); }
#endif
    void Set(T* pointer) { SetPointer(pointer); }
    T* Get() const { return static_cast<T*>(GetPItem()); }             // FableWin ?Get@?$CIntelligentPointer@..@@QBEPAV..@XZ
    T* operator->() const { return Get(); }                             // FableWin ??C?$CIntelligentPointer@..@@QBEPAV..@XZ
#ifdef FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
    // Chunk 754 (row 0x00945BD0 `PCookingPot = pot;`): FableWin.pdb ??4?$CIntelligentPointer@VCThing@@@@QAEAAV0@ABV0@@Z, inline in
    // retail: the item read into a local, then SetPItem (0x00A01B50 then 0x00A01B90; the destination is formed after the read,
    // rows 0x00943C20 / 0x00945BD0), not the implicit member-wise copy of PData.
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { CBaseObject* item = other.GetPItem(); SetPItem(item); return *this; }
    // Chunk 755 row 0x00729F60 (OnInitialActivate, 0x0072A061..0x0072A06E) also uses this form; some TUs keep an out-of-line
    // copy (0x004A9760).
#endif
#ifdef FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
    // Chunk 754 (row 0x0082A890 CMiniMapDisplay::AddMarker): FableWin.pdb ??0?$CIntelligentPointer@$$CBVCThing@@@@QAE@ABV0@@Z, inline in
    // retail: the base ctor 0x00A01B10, the vptr, then GetPItem on the source 0x00A01B50 and SetPItem 0x00A01B90 (the list node's copy
    // in the inline push_back), not the implicit member-wise copy of PData.
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
#endif
#ifdef FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_OUTOFLINE
    // Retail opt-in (chunk 801, row 0x00960120 CAIStateGroup_DeedReactionRoot::Update): FableWin.pdb
    // ??0?$CIntelligentPointer@$$CBVCThing@@@@QAE@ABV0@@Z called out of line in retail (0x00654D58: the base ctor 0x00A01B10, the vptr
    // 0x01237F1C, GetPItem on the source, SetPItem -- the COPY branch's body), for the by-value argument of
    // AIAction_PerformActionTargetFaceAndWait at 0x00960646. Declaration only: callers call the COMDAT.
    CIntelligentPointer(const CIntelligentPointer& other);
#endif
#ifdef FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT
    // Retail opt-in (chunk 795, row 0x0092FAB0): the converting constructor from another instantiation, out of line in retail
    // (0x00930D20 CIntelligentPointer<const CThing> from CIntelligentPointer<CThingPlayerCreature>: the base ctor 0x00A01B10, the
    // vptr 0x01237F1C, GetPItem on the source, SetPItem; the landed row's body). Declaration only: callers call the COMDAT.
    template<class U> CIntelligentPointer(const CIntelligentPointer<U>& other);
#endif
private:
    void SetPointer(const T* pointer) { SetPItem(const_cast<CBaseObject*>(static_cast<const CBaseObject*>(pointer))); }
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
