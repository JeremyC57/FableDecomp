// CTCActionUseTorch::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CTCActionUseTorch (it only runs ~CTCActionUseBase 0x004DA967), then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCTCActionUseTorch@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DBFAF, 28 bytes, thiscall, `ret 4`; slot 0 of the CTCActionUseTorch vtable 0x0123F124
//   (rtti-vtables.tsv; FableWin.pdb ??_GCTCActionUseTorch@@UAEPAXI@Z).
// Names (agentNF20, 2026-11-11; was the free stand-in CTCActionUseTorch_ScalarDeletingDestructor calling CTCActionUseTorch_Destruct):
//   the destructor it calls is ??1CTCActionUseBase@@UAE@XZ 0x004DA967 (FableWin.pdb; shared engine/CTCActionUseBase.h, opt-in
//   FABLE_RETAIL_CTCACTIONUSEBASE_DTOR), operator delete 0x00BFE9BC.
// Shape: VC7.1 emits the deleting destructor with the vtable, i.e. in a TU that constructs the class: MakeOne is that construction.
//   The class adds no member with a destructor (the call goes straight to ~CTCActionUseBase); local model: no engine header.
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCACTIONUSEBASE_DTOR
#include "engine/CTCActionUseBase.h"

class CTCActionUseTorch : public CTCActionUseBase {
public:
    CTCActionUseTorch(CThing& thing) : CTCActionUseBase(thing) { }
    virtual ETCInterfaceType GetInterfaceType() const;                   // CTCBase slot 21 (declared: the vtable is emitted here)
    virtual CCharString GetClassTypeName() const;                        // CTCBase slot 22
};

void* MakeOne(CThing& thing)
{
    return new CTCActionUseTorch(thing);
}
