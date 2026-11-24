// CThingNoiseDef::~CThingNoiseDef() -- compiler-generated destructor: the Noise member (CTCNoiseDef's implicit def chain, down to
// ~CDefPointeeBase's vtable reset and ~CResource), then a tail jump to the compiler-generated ~CThingBaseDef
// Leaf: ??1CThingNoiseDef@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045C5F6, 25 bytes, thiscall, tail `jmp`.
// Identity (two sources): Ego_r.pdb and FableWin.pdb ??1CThingNoiseDef@@UAE@XZ; its only caller is the scalar deleting destructor
//   0x0045C5DA, slot 0 of the CThingNoiseDef vftable 0x0123388C (RTTI .?AVCThingNoiseDef@@ : CThingBaseDef). It stores no
//   CThingNoiseDef vtable, so it is the compiler-generated destructor (GOTCHAS agentFAM10). Callees: ??_7CDefPointeeBase@@6B@
//   0x01230BA0 stored at Noise (+0x70), ??1CResource@@UAE@XZ 0x009FC550, ??1CThingBaseDef@@UAE@XZ 0x00454B4C (tail; FableWin.pdb names
//   it, and FableWin's ~CThingNoiseDef calls it; Ego_r.pdb inlines it; retail's body is ~CThingComponentSet 0x00450BA0 on +0x39 then
//   `jmp` ~CParentDefClassBase 0x00430300, no vtable store: the compiler-generated one too).
// Option S unit (owner ruling, advisor decision 2026-11-24; NF376 pilot; unit NF379): this file is ONE retail translation unit's slice,
//   CThingNoiseDef's def TU in Ego_r.pdb / FableWin.pdb definition_manager.obj, and serves these catalog addresses in retail address
//   order, one body each (the written body is Alloc; the others are what Alloc's `new CThingNoiseDef` makes VC7.1 emit):
//     0x00455EF0 ??0CThingNoiseDef@@QAE@XZ        36 B  (row file includes this TU; inline ctor, out-of-line COMDAT, Alloc's tail `jmp`)
//     0x0045C5C1 ?Alloc@CThingNoiseDef@@SIPAVCDefClassBase@@XZ  (no catalog row; EQ to retail, NF359 retcmp)
//     0x0045C5DA ??_GCThingNoiseDef@@UAEPAXI@Z    28 B  (row file includes this TU)
//     0x0045C5F6 ??1CThingNoiseDef@@UAE@XZ        25 B  (this row)
//   Both PDBs put Alloc, ??0CThingNoiseDef, the scalar deleting destructor and ??1CThingNoiseDef in definition_manager.obj (Ego_r #2046 /
//   #624 / #2047 / #631; FableWin #384 / #385 / #397 / #398). Each address is graded on its own leaf and keeps its own behaviour test.
//   Before the unit, 0x00455EF0 / 0x0044C1BB / 0x0045C5DA carried this same text (NF363 option A), so four objects defined Alloc strongly
//   (three LNK2005, byte-identical); a link takes ONE object of this TU (this row's or one includer's), so Alloc has one definer.
//   The unreferenced out-of-line COMDAT ??0CTCNoiseDef 0x0044C1BB (inline CTCNoiseDef ctor; FableWin.pdb definition_manager.obj #386, no
//   Ego_r.pdb copy) is emitted by this TU too; its row joins the unit by ruling (NF379-Q1).
// Emission: Alloc's `new CThingNoiseDef` compiles the inline constructor with CTCNoiseDef's inline constructor expanded for Noise; the
//   vftable stores emit both vftables, hence ??_G and this out-of-line implicit ??1 (GOTCHAS agentFAM13). The TU is size-optimised (odd
//   addresses, as in every def TU row): optimize("s"); at /O2 the destructor is expanded into ??_G and no out-of-line ??1 is emitted
//   (NF355 measurements a1 / a3 / p1).
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
