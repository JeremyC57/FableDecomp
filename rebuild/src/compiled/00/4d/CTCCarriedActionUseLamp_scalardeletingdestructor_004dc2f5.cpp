// CTCCarriedActionUseLamp::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CTCCarriedActionUseLamp (it only runs ~CTCActionUseBase 0x004DA967), then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCTCCarriedActionUseLamp@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DC2F5, 28 bytes, thiscall, `ret 4`; slot 0 of the CTCCarriedActionUseLamp vtable 0x0123F91C (rtti-vtables.tsv).
// Names (agentNF20, 2026-11-11; was a free __fastcall stand-in calling a *_Destruct / C3DAnimationBlendState stand-in): the destructor it
//   calls is ??1CTCActionUseBase@@UAE@XZ 0x004DA967 (FableWin.pdb; shared engine/CTCActionUseBase.h, opt-in FABLE_RETAIL_CTCACTIONUSEBASE_DTOR),
//   operator delete 0x00BFE9BC. Hierarchy (ghidra_out/rtti_bases.tsv): CTCCarriedActionUseLamp : CTCCarriedActionUseBase : CTCActionUseBase.
// Shape: VC7.1 emits the deleting destructor with the vtable, i.e. in a TU that constructs the class: MakeOne is that construction.
//   No class between it and CTCActionUseBase adds a member with a destructor; local models: no engine header.
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCACTIONUSEBASE_DTOR
#include "engine/CTCActionUseBase.h"

class CTCCarriedActionUseBase : public CTCActionUseBase {                  // RTTI base of CTCCarriedActionUseLamp (local model: no engine header)
public:
    CTCCarriedActionUseBase(CThing& thing);
};

class CTCCarriedActionUseLamp : public CTCCarriedActionUseBase {
public:
    CTCCarriedActionUseLamp(CThing& thing) : CTCCarriedActionUseBase(thing) { }
    virtual ETCInterfaceType GetInterfaceType() const;                   // CTCBase slot 21 (declared: the vtable is emitted here)
    virtual CCharString GetClassTypeName() const;                        // CTCBase slot 22
};

void* MakeOne(CThing& thing)
{
    return new CTCCarriedActionUseLamp(thing);
}
