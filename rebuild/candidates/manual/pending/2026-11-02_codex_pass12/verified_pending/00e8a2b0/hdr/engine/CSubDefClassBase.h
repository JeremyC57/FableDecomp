#ifndef FABLE_ENGINE_CSUBDEFCLASSBASE_H
#define FABLE_ENGINE_CSUBDEFCLASSBASE_H
// Retail model (hand-written 2026-10-29, wave 1 class-model migration). RTTI .?AVCSubDefClassBase@@ : CDefClassBase
// (ghidra_out/rtti_bases.tsv); vtable 0x01231D54 = CDefClassBase's 25 slots with only slot 0 replaced (scalar deleting dtor
// 0x0044F620; slots 1..24 identical to 0x01230BCC, 18..20 _purecall). No members of its own: 0x25 bytes like its packed base
// (Ego_r.pdb 0x25; see engine/CDefClassBase.h for the pack(1) evidence). Ctor 0x0044C0C0
// (??0CSubDefClassBase@@QAE@XZ, bound in rebuild/oracles/reloc_bindings.tsv). FableWin.pdb ??1CSubDefClassBase@@UAE@XZ is a
// user-declared virtual dtor; the landed derived scalar-deleting rows expand it inline, so it is defined inline here.
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#error engine/CSubDefClassBase.h is the retail def model: define FABLE_RETAIL_DEF_CLASS_BASE before including it
#else
#include "rebuild_abi.h"
#include "engine/CDefClassBase.h"

class CSubDefClassBase : public CDefClassBase {
public:
    CSubDefClassBase();                                                  // 0x0044C0C0
    virtual ~CSubDefClassBase() { }                                      // slot 0  0x0044F620 (scalar deleting; inline dtor)
};
FABLE_STATIC_ASSERT(sizeof(CSubDefClassBase) == 0x25);

#endif // FABLE_RETAIL_DEF_CLASS_BASE
#endif // FABLE_ENGINE_CSUBDEFCLASSBASE_H
