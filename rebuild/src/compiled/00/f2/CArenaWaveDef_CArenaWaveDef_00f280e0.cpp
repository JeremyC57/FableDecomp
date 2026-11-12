// CArenaWaveDef::CArenaWaveDef() -- default constructor: CDefClassBase(), this class's vtable 0x012FB2F0, then the empty STLport
// vector Creatures (+0x2C..+0x34); NumWaveCreatures (+0x28) and ShortWave (+0x38) are left uninitialised
// Leaf: ??0CArenaWaveDef@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F280E0, length 29 bytes.
// Identity: stores vtable 0x012FB2F0, the vtable of the landed CArenaWaveDef copy constructor 0x00F25C60 (slot 0 the landed
//   ??_GCArenaWaveDef 0x00F25CB0, slots 18 / 19 / 20 the landed CArenaWaveDef::Transfer 0x00F261F0 / Copy 0x00F25F50 /
//   GetSizeofClass 0x00F25CA0); reloc_bindings.tsv already binds ??0CArenaWaveDef@@QAE@XZ to 0x00F280E0 (caller 0x00F280A0);
//   Ego_r.pdb names the class (record sizeof 0x40 with the Dinkumware vector; retail 0x3C, new engine/CArenaWaveDef.h).
// Callee: CDefClassBase::CDefClassBase() 0x0042D9E0 (FableWin.pdb ??0CDefClassBase@@AAE@XZ): CSubDefClassBase's constructor is
//   expanded around it, its vtable store dead (opt-ins FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL / _CSUBDEFCLASSBASE_CTOR_INLINE).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL
#define FABLE_RETAIL_CSUBDEFCLASSBASE_CTOR_INLINE
#define FABLE_RETAIL_CARENAWAVEDEF_IMPLICIT_DTOR
#define FABLE_RETAIL_CARENACREATUREDEF_IMPLICIT_DTOR
#include "engine/CArenaWaveDef.h"

CArenaWaveDef::CArenaWaveDef()
{
}
