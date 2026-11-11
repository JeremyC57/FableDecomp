// CHeroSoulsWaveDef::`scalar deleting destructor'(unsigned int flags) -- the implicit destructor (Creatures, then the inline base destructors), then free this when bit 0 of flags is set
// Leaf: ??_GCHeroSoulsWaveDef@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F26F40, 46 bytes, thiscall, `ret 4`, returns this.
// Identity: slot 0 of vtable 0x012FB498, the vtable CHeroSoulsWaveDef::CHeroSoulsWaveDef 0x00F28BA0 stores; the member at +0x2C
//   is destroyed by 0x00F26F70, which steps 0x34-byte elements (sizeof CHeroSoulsCreatureDef, Ego_r Creatures +0x2C);
//   Ego_r.pdb ??1CHeroSoulsWaveDef@@UAE@XZ / FableWin.pdb ??_GCHeroSoulsWaveDef@@UAEPAXI@Z name it.
// Callees: 0x00F26F70 (~vector<CHeroSoulsCreatureDef>), the CDefPointeeBase vptr 0x01230BA0, CResource::~CResource 0x009FC550,
//   operator delete 0x00BFE9BC.
// Shape (family K050): the compiler-generated destructor stores no vptr of its own (header opt-in
//   FABLE_RETAIL_CHEROSOULSWAVEDEF_IMPLICIT_DTOR); the inline ctor below is only this TU's emitter of the vtable and so of ??_G.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CHEROSOULSWAVEDEF_IMPLICIT_DTOR
#define FABLE_RETAIL_CHEROSOULSWAVEDEF_DEF_VIRTUALS
#include "engine/CHeroSoulsWaveDef.h"
inline CHeroSoulsWaveDef::CHeroSoulsWaveDef() { }
CHeroSoulsWaveDef* NewHeroSoulsWaveDef() { return new CHeroSoulsWaveDef; }
