// CThingNoiseDef::~CThingNoiseDef() -- compiler-generated destructor: the Noise member (CTCNoiseDef's implicit def chain, down to
// ~CDefPointeeBase's vtable reset and ~CResource), then a tail jump to the compiler-generated ~CThingBaseDef
// Leaf: ??0CThingNoiseDef@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045C5F6, 25 bytes, thiscall, tail `jmp`.
// Identity (two sources): Ego_r.pdb and FableWin.pdb ??1CThingNoiseDef@@UAE@XZ; its only caller is the scalar deleting destructor
//   0x0045C5DA, slot 0 of the CThingNoiseDef vftable 0x0123388C (RTTI .?AVCThingNoiseDef@@ : CThingBaseDef). It stores no
//   CThingNoiseDef vtable, so it is the compiler-generated destructor (GOTCHAS agentFAM10). Callees: ??_7CDefPointeeBase@@6B@
//   0x01230BA0 stored at Noise (+0x70), ??1CResource@@UAE@XZ 0x009FC550, ??1CThingBaseDef@@UAE@XZ 0x00454B4C (tail; FableWin.pdb names
//   it, and FableWin's ~CThingNoiseDef calls it; Ego_r.pdb inlines it; retail's body is ~CThingComponentSet 0x00450BA0 on +0x39 then
//   `jmp` ~CParentDefClassBase 0x00430300, no vtable store: the compiler-generated one too).
// Emission (the TU retail used): Ego_r.pdb puts Alloc, the constructor, the scalar deleting destructor and this destructor of
//   CThingNoiseDef in one object, definition_manager.obj. In retail they sit together: Alloc 0x0045C5C1, ??_G 0x0045C5DA, this
//   0x0045C5F6. Alloc's `new CThingNoiseDef` compiles the inline constructor (its out-of-line COMDAT is 0x00455EF0; Alloc tail-jumps to
//   it), whose vftable store emits the vftable, hence ??_G and this out-of-line ??1. So this row is Alloc as retail wrote it, graded on
//   the ??1 COMDAT it emits (GOTCHAS agentFAM13). The TU is size-optimised (odd addresses, as in every def TU row): optimize("s");
//   at /O2 the destructor is expanded into ??_G and no out-of-line ??1 is emitted (NF355 measurements a1 / a3 / p1).
// Pick-any: engine/CDefPointeeBase.h is included ABOVE the size pragma (scaffolding, not retail). Its inline ~CDefPointeeBase gives
//   this TU a ??_7CDefPointeeBase / ??_GCDefPointeeBase COMDAT; compiled under the pragma that copy is the /Os form (`pop ecx`), while
//   the copy retail kept, 0x0042D9A0, is the /O2 form (`add esp,4`). Included first, it takes the command-line /O2 and equals
//   0x0042D9A0; no other COMDAT or the leaf changes (NF355 probe p3). Fragile to the include order of the shared def headers.
// Model: the shared engine/CThingNoiseDef.h (FABLE_RETAIL_CTHINGNOISEDEF_VIRTUAL_MODEL) on engine/CThingBaseDef.h (retail branch,
//   Components typed so ~CThingBaseDef is the compiler-generated one; ctor declared) and engine/CTCNoiseDef.h (implicit destructor,
//   inline constructor). Was a local CTCKraken stand-in under ?EndAttackBeam@CTCKraken@@QAEXXZ, which is 0x007FC800's name.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGBASEDEF_COMPONENTS
#define FABLE_RETAIL_CTHINGBASEDEF_CTOR_DECL
#define FABLE_RETAIL_CTCNOISEDEF_IMPLICIT_DTOR
#define FABLE_RETAIL_CTCNOISEDEF_CTOR_INLINE
#define FABLE_RETAIL_CTHINGNOISEDEF_VIRTUAL_MODEL
#include "engine/CDefPointeeBase.h"                        // above the pragma: see Pick-any
#pragma optimize("s", on)
#include "engine/CThingNoiseDef.h"

CDefClassBase* CThingNoiseDef::Alloc()
{
    return new CThingNoiseDef;
}
