// CHeroSoulsRoundDef::`scalar deleting destructor'(unsigned int flags) -- the implicit destructor (Waves, then the inline base destructors), then free this when bit 0 of flags is set
// Leaf: ??_GCHeroSoulsRoundDef@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F26DF0, 46 bytes, thiscall, `ret 4`, returns this.
// Identity: slot 0 of vtable 0x012FB430, the vtable CHeroSoulsRoundDef::CHeroSoulsRoundDef 0x00F35780 stores; the member at +0x2C
//   is destroyed by 0x00F26E20, which steps 0x38-byte elements (sizeof CHeroSoulsWaveDef, Ego_r Waves +0x2C);
//   Ego_r.pdb ??_ECHeroSoulsRoundDef@@UAEPAXI@Z / FableWin.pdb ??_GCHeroSoulsRoundDef@@UAEPAXI@Z name it.
// Callees: 0x00F26E20 (~vector<CHeroSoulsWaveDef>), the CDefPointeeBase vptr 0x01230BA0, CResource::~CResource 0x009FC550,
//   operator delete 0x00BFE9BC.
// Shape (family K050): the compiler-generated destructor stores no vptr of its own (header opt-in
//   FABLE_RETAIL_CHEROSOULSROUNDDEF_IMPLICIT_DTOR); the inline ctor below is only this TU's emitter of the vtable and so of ??_G.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CHEROSOULSROUNDDEF_IMPLICIT_DTOR
#define FABLE_RETAIL_CHEROSOULSWAVEDEF_DEF_VIRTUALS
#include "engine/CHeroSoulsWaveDef.h"
#include "engine/CHeroSoulsRoundDef.h"
inline CHeroSoulsRoundDef::CHeroSoulsRoundDef() { }
CHeroSoulsRoundDef* NewHeroSoulsRoundDef() { return new CHeroSoulsRoundDef; }
