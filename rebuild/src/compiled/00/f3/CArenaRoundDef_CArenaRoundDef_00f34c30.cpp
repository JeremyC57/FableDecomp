// CArenaRoundDef::CArenaRoundDef() -- default constructor: CDefClassBase(), this class's vtable 0x012FB288, then the empty STLport
// vector Waves (+0x2C..+0x34); NumWaves (+0x28) is left uninitialised
// Leaf: ??0CArenaRoundDef@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F34C30, length 29 bytes.
// Identity: stores vtable 0x012FB288, the vtable of the landed CArenaRoundDef copy constructor 0x00F34F80 (slot 0 the landed
//   ??_GCArenaRoundDef 0x00F25B40, slots 18 / 19 the landed CArenaRoundDef::Transfer 0x00F27E60 / Copy 0x00F268F0);
//   rebuild/oracles/reloc_bindings.tsv already binds ??0CArenaRoundDef@@QAE@XZ to 0x00F34C30 (caller 0x00F34BF0); Ego_r.pdb /
//   FableWin.pdb ??0CArenaRoundDef@@QAE@XZ. The catalog's "CVtable_012fb288" was a vtable placeholder.
// Callee: CDefClassBase::CDefClassBase() 0x0042D9E0 (FableWin.pdb ??0CDefClassBase@@AAE@XZ): CSubDefClassBase's constructor is
//   expanded around it, its vtable store dead (opt-ins FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL / _CSUBDEFCLASSBASE_CTOR_INLINE).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL
#define FABLE_RETAIL_CSUBDEFCLASSBASE_CTOR_INLINE
#define FABLE_RETAIL_CARENAROUNDDEF_IMPLICIT_DTOR
#include "engine/CArenaRoundDef.h"
#include "engine/CArenaWaveDef.h"            // the element type of Waves (complete for the vector members)

CArenaRoundDef::CArenaRoundDef()
{
}
