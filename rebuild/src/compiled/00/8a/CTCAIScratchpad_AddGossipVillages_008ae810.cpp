// CTCAIScratchpad::AddGossipVillages(CCharString name,CCharString value) -- Append a gossip value in its category.
// Leaf: ?AddGossipVillages@CTCAIScratchpad@@SIXVCCharString@@0@Z
// Identity: FableWin static fastcall two-string overload; own static map013bae50 and string-vector overflow00433530.
// Name fix (NF208, 2026-11-20, FP-VECTORINSERT q6332 / q6341): the row-local CCharString / vector / map / CTCAIScratchpad models are
//   replaced by fable_string.h, stlport/fable_containers.h, stlport/fable_map.h and engine/CTCAIScratchpad.h (opt-ins
//   FABLE_RETAIL_CTCAISCRATCHPAD_GOSSIP_MAPS for the static map 0x013BAE50, FABLE_RETAIL_CTCAISCRATCHPAD_ADDGOSSIPVILLAGES for the
//   declaration). push_back's overflow call now binds the STLport protected spelling of 0x00433530
//   (?_M_insert_overflow@?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@IAE...). Retail homes the __false_type tag on
//   the live by-value `value` slot only when the callee's body is visible (declaration-only measured DIFFER 92v115, the tag in its
//   own `push ecx` word): FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE keeps the real stl/_vector.c body visible but out of line, and
//   FABLE_RETAIL_STLP_UNINIT_COPY_FORCEINLINE (the self row 0x00433530's own macro) makes the pick-any copy that body emits the
//   owner's bytes. map::operator[] stays the out-of-line 0x007AEBE0 (fable_map.h declaration).
#include "fable_compiler.h"
#include "rebuild_abi.h"
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_STLP_UNINIT_COPY_FORCEINLINE
#include "stlport/fable_containers.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCAISCRATCHPAD_GOSSIP_MAPS
#define FABLE_RETAIL_CTCAISCRATCHPAD_ADDGOSSIPVILLAGES
#include "engine/ETCInterfaceType.h"
#include "engine/CTCAIScratchpad.h"

void FABLE_FASTCALL CTCAIScratchpad::AddGossipVillages(CCharString name,CCharString value){GossipVillages[name].push_back(value);}
