// CHeroSoulsWaveDef::CHeroSoulsWaveDef() -- default constructor: CDefClassBase(), this class's vtable 0x012FB498, then the empty
// STLport vector Creatures (+0x2C..+0x34); NumWaveCreatures (+0x28) is left uninitialised
// Leaf: ??0CHeroSoulsWaveDef@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F28850, length 29 bytes.
// Identity: stores vtable 0x012FB498, the vtable of the landed CHeroSoulsWaveDef copy constructor 0x00F28BA0 (slot 0 the landed
//   ??_GCHeroSoulsWaveDef 0x00F26F40, slots 18 / 19 the landed Transfer 0x00F27490 / Copy 0x00F271D0); reloc_bindings.tsv already
//   binds ??0CHeroSoulsWaveDef@@QAE@XZ to 0x00F28850 (caller 0x00F28810); Ego_r.pdb / FableWin.pdb ??0CHeroSoulsWaveDef@@QAE@XZ.
// Callee: CDefClassBase::CDefClassBase() 0x0042D9E0 (FableWin.pdb ??0CDefClassBase@@AAE@XZ): CSubDefClassBase's constructor is
//   expanded around it, its vtable store dead (opt-ins FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL / _CSUBDEFCLASSBASE_CTOR_INLINE).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL
#define FABLE_RETAIL_CSUBDEFCLASSBASE_CTOR_INLINE
#define FABLE_RETAIL_CHEROSOULSWAVEDEF_IMPLICIT_DTOR
#define FABLE_RETAIL_CHEROSOULSWAVEDEF_DEF_VIRTUALS
#include "engine/CHeroSoulsWaveDef.h"

CHeroSoulsWaveDef::CHeroSoulsWaveDef()
{
}
