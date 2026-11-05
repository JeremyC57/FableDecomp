// CCardDef::~CCardDef() -- implicit definition destructor: free the SubDefInfoMap vector storage (+0x29), then the inline CDefClassBase / CDefPointeeBase levels (CDefPointeeBase vtable) and ~CResource (tail jump)
// Leaf: ??1CCardDef@@UAE@XZ
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D7950, 33 bytes: push esi ; mov esi,ecx ; mov eax,[esi+29h] ; test eax,eax ; je ;
//   push eax ; call free ; add esp,4 ; mov dword ptr [esi],0x01230BA0 ; mov ecx,esi ; pop esi ; jmp 0x009FC550. No caller
//   (unreferenced COMDAT: every use in its TU is inlined; the linker keeps it).
// Identity (proven): it sits in the CTCTavernGameCardPairs TU (NewRound 0x007D70E0, ShuffleCards 0x007D71E0, the ctor
//   0x007D7520, GetRandomCard 0x007D75A0), whose GetRandomCard builds a local CCardDef (inline default ctor, `mov [esp+14h],
//   0x01243A44` = CCardDef's vtable, RTTI .?AVCCardDef@@) and destroys it by name; right after it come the class's other
//   unreferenced COMDATs: the implicit operator= 0x007D79C0 (CDefClassBase::operator= 0x00431F10, InstantiationName,
//   SubDefInfoMap, CardName +0x3c, CardVal +0x40) and the default ctor 0x007D7A00 (stores vtable 0x01243A44). FableWin.pdb
//   names exactly that trio ??1CCardDef@@UAE@XZ, ??4CCardDef@@QAEAAV0@ABV0@@Z, ??0CCardDef@@QAE@XZ; Ego_r.pdb has no ??1
//   (compiler-generated: no vtable store of its own). RTTI CCardDef : CParentDefClassBase : CDefClassBase : CDefPointeeBase :
//   CResource (rtti_bases.tsv); CCardDef's own scalar deleting dtor 0x004EC0D4 calls ~CParentDefClassBase 0x00430300 (the
//   landed twin, whose body this inlines). The manifest label "CTCInventoryItem::OnDie" is propagated.
// Callees: free 0x00BFEA14 (STLport malloc allocator: ~_Vector_base of SubDefInfoMap), CResource::~CResource 0x009FC550.
// Layout: CDefClassBase from engine/CDefClassBase.h (retail model, packed, 0x25); CParentDefClassBase (Ego_r.pdb
//   InstantiationName CDefString +0x25, SubDefInfoMap CVectorMap +0x29, retail 0x39) is modelled locally: engine/
//   CParentDefClassBase.h declares ~CParentDefClassBase out of line (0x00430300), and this body inlines it (its derived
//   implicit destructors expand it here; the derived deleting dtors call it). CVectorMap 0x10 (vector 0xC, Compare +0xC,
//   Dirty +0xD) as in the landed CFrontEndDef ctor 0x00431DF0. CCardDef: CardName +0x3c, CardVal +0x40 (Ego_r 64/68 - 4).
// Shape: CParentDefClassBase and CCardDef declare no destructor (VC7.1 drops their dead vtable stores); the harness below
//   destroys a local CCardDef by name, as GetRandomCard does, which emits the implicit ??1. It is not a retail function.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

class CDefString { public:
    long TablePos;                                         // +0x00
};

#include "engine/CVectorMap.h"


#pragma pack(push, 1)                                      // retail: the def chain is packed (CDefString at +0x25)
class CParentDefClassBase : public CDefClassBase {         // vtable 0x01230CBC
public:
    CDefString InstantiationName;                          // +0x25
    CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >
        SubDefInfoMap;                                     // +0x29
};
#pragma pack(pop)

class CCardDef : public CParentDefClassBase {              // vtable 0x01243A44
public:
    CCardDef();                                            // FableWin ??0CCardDef@@QAE@XZ (retail COMDAT 0x007D7A00)
    virtual void Transfer(CPersistContext& context);       // slot 18 0x004EC0B3 (Ego_r ?Transfer@CCardDef@@UAEXAAVCPersistContext@@@Z)
    virtual void Copy(const CDefClassBase* other);         // slot 19 0x004EC0F0 (Ego_r ?Copy@CCardDef@@UAEXPBVCDefClassBase@@@Z)
    virtual unsigned long GetSizeofClass();                // slot 20 0x004EC0AF (Ego_r ?GetSizeofClass@CCardDef@@UAEKXZ)
    long CardName;                                         // +0x3c
    long CardVal;                                          // +0x40
};

// TU harness (not a retail function): a local card definition destroyed by name, as GetRandomCard 0x007D75A0 does.
void TUHarness_CCardDef()
{
    CCardDef card;
}