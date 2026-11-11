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
#ifdef FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
    // Original debug02511d30 uses nonconst template source and Get/SetPointer.
    template<class U> CIntelligentPointer(CIntelligentPointer<U>& other) { SetPointer(other.Get()); }
#endif
#ifdef FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE_OUTOFLINE
    // Retail opt-in (chunk 2026, row 0x0094E7B0 CAIStateGroup_EarthTroll::Update): the FableWin.pdb converting constructor from a non-const
    // source, ??$?0VCThingCreatureBase@@@?$CIntelligentPointer@$$CBVCThing@@@@QAE@AAV?$CIntelligentPointer@VCThingCreatureBase@@@@@Z, called out of
    // line at 0x0091F990 (base ctor 0x00A01B10, vptr 0x01237F1C, GetPItem on the source, SetPItem; `ret 4`) to build the by-value
    // CIntelligentPointer<const CThing> argument of AIAction_LookAtThing in its stack slot (0x0094EC84 `sub esp,8 ; mov ecx,esp ; push ebx`).
    // Declaration only (the CONVERT_MUTABLE opt-in above is the same member with its inline body).
    template<class U> CIntelligentPointer(CIntelligentPointer<U>& other);
#endif
#ifdef FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
    // Retail opt-in (chunk 867, row 0x0064BFBA NPlayerGui::CDrawHighlightingTargetingCursor::Draw): the (T*) constructor called out of
    // line (0x004A96F0 for CThing, FableWin ??0?$CIntelligentPointer@VCThing@@@@QAE@PAVCThing@@@Z) for the temporary of
    // `PTargetedThing = targeting->GetPTargetedThing();`. Declaration only.
    CIntelligentPointer(T* pointer);
#else
    CIntelligentPointer(T* pointer) { SetPointer(pointer); }
#endif
#ifdef FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_OUTOFLINE
    // Same row: ??4?$CIntelligentPointer@VCThing@@@@QAEAAV0@ABV0@@Z called out of line (0x004A9760, the ASSIGN branch's body as a COMDAT).
    CIntelligentPointer& operator=(const CIntelligentPointer& other);
#endif
    void Set(T* pointer) { SetPointer(pointer); }
#ifdef FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
    T& operator*()const{return *GetPointer();}
#endif
#ifdef FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
    // Retail opt-in (Codex pass18): original018f05d0/018f3510/01ca3ac0.
    T* Get() const { return GetPointer(); }
    T* operator->()const { return GetPointer(); }
#else
    T* Get() const { return static_cast<T*>(GetPItem()); }             // FableWin ?Get@?$CIntelligentPointer@..@@QBEPAV..@XZ
    T* operator->() const { return Get(); }                             // FableWin ??C?$CIntelligentPointer@..@@QBEPAV..@XZ
#endif
#ifdef FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
    // Chunk 754 (row 0x00945BD0 `PCookingPot = pot;`): FableWin.pdb ??4?$CIntelligentPointer@VCThing@@@@QAEAAV0@ABV0@@Z, inline in
    // retail: the item read into a local, then SetPItem (0x00A01B50 then 0x00A01B90; the destination is formed after the read,
    // rows 0x00943C20 / 0x00945BD0), not the implicit member-wise copy of PData.
    #ifdef FABLE_RETAIL_CTCTARGETINGAI_ATTACK
    CIntelligentPointer& operator=(const CIntelligentPointer& other){SetPointer(other.GetPointer());return *this;}
#else
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { CBaseObject* item = other.GetPItem(); SetPItem(item); return *this; }
#endif
    // Chunk 755 row 0x00729F60 (OnInitialActivate, 0x0072A061..0x0072A06E) also uses this form; some TUs keep an out-of-line
    // copy (0x004A9760).
#endif
#ifdef FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
    // Chunk 754 (row 0x0082A890 CMiniMapDisplay::AddMarker): FableWin.pdb ??0?$CIntelligentPointer@$$CBVCThing@@@@QAE@ABV0@@Z, inline in
    // retail: the base ctor 0x00A01B10, the vptr, then GetPItem on the source 0x00A01B50 and SetPItem 0x00A01B90 (the list node's copy
    // in the inline push_back), not the implicit member-wise copy of PData.
#ifdef FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
    CIntelligentPointer(const CIntelligentPointer& item) {SetPointer(item.GetPointer());}
#else
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
#endif
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
#if defined(FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN) || defined(FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN) || defined(FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE)
    T* GetPointer()const{return static_cast<T*>(GetPItem());}
#endif
    #ifdef FABLE_RETAIL_CTCTARGETINGAI_ATTACK
    void SetPointer(T* pointer){SetPItem(pointer);}
#else
    void SetPointer(const T* pointer) { SetPItem(const_cast<CBaseObject*>(static_cast<const CBaseObject*>(pointer))); }
#endif
};
#pragma pack(pop)
FABLE_STATIC_ASSERT(sizeof(CBaseIntelligentPointer)==8);
// Retail opt-in (Codex pass 17): FableWin 01CD9DD0 original pointer/int
// comparison used by IS_THING_ALIVE; the debug-only nonzero-int assertion is absent in retail.
#ifdef FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
template<class T> inline bool FABLE_FASTCALL operator==(const CIntelligentPointer<T>& pointer,int) { return pointer.Get()==0; }
template<class T> inline bool FABLE_FASTCALL operator!=(const CIntelligentPointer<T>& pointer,int) {
    return pointer.Get()!=0;
}
#endif
#ifdef FABLE_RETAIL_CTCTARGETINGAI_ATTACK
// Original FableWin025370d0 calls GetPItem then compares the raw object.
template<class T> inline bool FABLE_FASTCALL operator!=(const CIntelligentPointer<T>& pointer,const CBaseObject* item){return pointer.GetPItem()!=item;}
#endif
#ifdef FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE
// Retail opt-in (chunk 867, row 0x0064BFBA): the free comparison of a raw item with an intelligent pointer, FableWin.pdb
// ??$?9VCThing@@@@YI_NPBVCBaseObject@@ABV?$CIntelligentPointer@VCThing@@@@@Z, out of line at 0x005CBDD2 (`mov ecx,<thing> ;
// lea edx,<pointer> ; call`). Declaration only.
template<class T> bool FABLE_FASTCALL operator!=(const CBaseObject* item, const CIntelligentPointer<T>& pointer);
#endif
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
#ifdef FABLE_RETAIL_CINTELLIGENTPOINTER_EQUAL_RAW
// FableWin01cfe990, exact free comparison: pointer first, raw base item second.
template<class T> inline bool FABLE_FASTCALL operator==(const CIntelligentPointer<T>& pointer,const CBaseObject* item) { return pointer.GetPItem()==item; }
#endif
#endif
