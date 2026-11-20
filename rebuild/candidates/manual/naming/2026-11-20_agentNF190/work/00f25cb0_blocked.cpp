// CArenaWaveDef::`scalar deleting destructor'(unsigned int flags) -- the implicit destructor (Creatures, then the inline base destructors), then free this when bit 0 of flags is set
// Leaf: ??_GCArenaWaveDef@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F25CB0, 46 bytes, thiscall, `ret 4`, returns this.
// Identity: slot 0 of vtable 0x012FB2F0 (the only reference), the vtable the landed CArenaWaveDef ctors 0x00F280E0 / 0x00F25C60
//   store; slots 19 / 20 are the landed CArenaWaveDef::Copy 0x00F25F50 / GetSizeofClass 0x00F25CA0; Ego_r.pdb
//   ??_GCArenaWaveDef@@UAEPAXI@Z / ??1CArenaWaveDef@@UAE@XZ (FableWin.pdb ??1CArenaWaveDef@@UAE@XZ) name it. The member at +0x2C
//   is destroyed by 0x00F25CE0, (vector<CArenaCreatureDef> Creatures; its landed row 0x00F25CE0 is the CRetailVector_poly0x38 stand-in, 0x38-byte elements).
//   Was: ??_GCUnprovenDef_00f25cb0@@UAEPAXI@Z (descriptive stand-in classes CUnprovenDef_00f25cb0 / CUnprovenMember_00f25cb0).
// Callees: 0x00F25CE0 (~vector<CArenaCreatureDef>), the CDefPointeeBase vptr 0x01230BA0, CResource::~CResource 0x009FC550,
//   operator delete 0x00BFE9BC.
// Shape (family K050, as the landed sibling CArenaRoundDef 0x00F25B40): the compiler-generated destructor stores no vptr of its
//   own (header opt-in FABLE_RETAIL_CARENAWAVEDEF_IMPLICIT_DTOR); the inline ctor and `new` below are only this TU's emitter of the vtable and so of ??_G.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL
#define FABLE_RETAIL_CSUBDEFCLASSBASE_CTOR_INLINE
#define FABLE_RETAIL_CARENAWAVEDEF_IMPLICIT_DTOR
#define FABLE_RETAIL_CARENACREATUREDEF_IMPLICIT_DTOR
#include "engine/CArenaWaveDef.h"
inline CArenaWaveDef::CArenaWaveDef() { }
CArenaWaveDef* NewArenaWaveDef() { return new CArenaWaveDef; }
