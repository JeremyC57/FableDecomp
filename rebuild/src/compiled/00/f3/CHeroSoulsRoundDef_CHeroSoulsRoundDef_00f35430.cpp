// CHeroSoulsRoundDef::CHeroSoulsRoundDef() -- default constructor: CDefClassBase(), this class's vtable 0x012FB430, then the empty
// STLport vector Waves (+0x2C..+0x34); NumWaves (+0x28) is left uninitialised
// Leaf: ??0CHeroSoulsRoundDef@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F35430, length 29 bytes.
// Identity: stores vtable 0x012FB430, the vtable of the landed CHeroSoulsRoundDef copy constructor 0x00F35780 (slot 0 the landed
//   ??_GCHeroSoulsRoundDef 0x00F26DF0, slots 18 / 19 the landed Transfer 0x00F285D0 / Copy 0x00F27B90); reloc_bindings.tsv already
//   binds ??0CHeroSoulsRoundDef@@QAE@XZ to 0x00F35430 (caller 0x00F353F0); Ego_r.pdb / FableWin.pdb ??0CHeroSoulsRoundDef@@QAE@XZ.
// Callee: CDefClassBase::CDefClassBase() 0x0042D9E0 (FableWin.pdb ??0CDefClassBase@@AAE@XZ): CSubDefClassBase's constructor is
//   expanded around it, its vtable store dead (opt-ins FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL / _CSUBDEFCLASSBASE_CTOR_INLINE).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL
#define FABLE_RETAIL_CSUBDEFCLASSBASE_CTOR_INLINE
#define FABLE_RETAIL_CHEROSOULSROUNDDEF_IMPLICIT_DTOR
#define FABLE_RETAIL_CHEROSOULSWAVEDEF_IMPLICIT_DTOR
#define FABLE_RETAIL_CHEROSOULSWAVEDEF_DEF_VIRTUALS
#include "engine/CHeroSoulsRoundDef.h"
#include "engine/CHeroSoulsWaveDef.h"        // the element type of Waves (complete for the vector members)

CHeroSoulsRoundDef::CHeroSoulsRoundDef()
{
}
