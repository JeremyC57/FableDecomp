// CTCCarriedActionUseRead::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CTCCarriedActionUseRead 0x004DCDA4, then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCTCCarriedActionUseRead@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DCD88, 28 bytes, thiscall, `ret 4`; slot 0 of the CTCCarriedActionUseRead vtable 0x01240834
//   (rtti-vtables.tsv; RTTI .?AVCTCCarriedActionUseRead@@ : CTCCarriedActionUseBase : CTCActionUseBase).
// Names (agentNF20, 2026-11-11; was the free stand-in CTCCarriedActionUseRead_ScalarDeletingDestructor calling
//   CTCCarriedActionUseRead_Destruct): the destructor it calls is Ego_r.pdb / FableWin.pdb ??1CTCCarriedActionUseRead@@UAE@XZ 0x004DCDA4
//   (landed; it destroys the class's own member, then tail-jumps ~CTCActionUseBase 0x004DA967), operator delete 0x00BFE9BC.
// Shape: VC7.1 emits the deleting destructor with the vtable, i.e. in a TU that constructs the class: MakeOne is that construction.
//   Local models: no engine header (CTCCarriedActionUseBase, the class); CTCActionUseBase is the shared header (opt-in
//   FABLE_RETAIL_CTCACTIONUSEBASE_DTOR for its constructor declaration).
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCACTIONUSEBASE_DTOR
#include "engine/CTCActionUseBase.h"

class CTCCarriedActionUseBase : public CTCActionUseBase {                  // RTTI base of CTCCarriedActionUseRead (local model: no engine header)
public:
    CTCCarriedActionUseBase(CThing& thing);
};

class CTCCarriedActionUseRead : public CTCCarriedActionUseBase {
public:
    CTCCarriedActionUseRead(CThing& thing) : CTCCarriedActionUseBase(thing) { }
    virtual ~CTCCarriedActionUseRead();                                  // 0x004DCDA4
    virtual ETCInterfaceType GetInterfaceType() const;                   // CTCBase slot 21 (declared: the vtable is emitted here)
    virtual CCharString GetClassTypeName() const;                        // CTCBase slot 22
};

void* MakeOne(CThing& thing)
{
    return new CTCCarriedActionUseRead(thing);
}
