// CThingNoiseDef::`scalar deleting destructor'(unsigned int) -- the implicit ~CThingNoiseDef (0x0045C5F6), then operator delete when bit 0
// of the flags is set; returns this
// Leaf: ??_GCThingNoiseDef@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045C5DA, 28 bytes, thiscall, `ret 4`.
// Identity (two sources): Ego_r.pdb and FableWin.pdb ??_GCThingNoiseDef@@UAEPAXI@Z; slot 0 of the CThingNoiseDef vftable 0x0123388C
//   (ghidra_out/labels_rtti_port_dtors.tsv); a scalar body (no array loop), so ??_G (deleting-destructor ruling). VC7.1 emits it only from
//   the TU that emits the vftable, the constructor's TU: C++ cannot name ??_G, so no in-place respell of the old free stand-in exists.
// Emission (the TU retail used): Ego_r.pdb puts Alloc, the constructor, the scalar deleting destructor and the destructor of
//   CThingNoiseDef in one object, definition_manager.obj; in retail they sit together (Alloc 0x0045C5C1, ??_G 0x0045C5DA, ??1 0x0045C5F6).
//   Alloc's `new CThingNoiseDef` compiles the inline constructor (out-of-line COMDAT 0x00455EF0, Alloc's tail `jmp`) with CTCNoiseDef's
//   inline constructor expanded for Noise (its own out-of-line COMDAT is 0x0044C1BB); the vftable stores emit both vftables, hence ??_G
//   and the implicit ??1. So this row is Alloc as retail wrote it, graded on the COMDAT named by the Leaf line (GOTCHAS agentFAM13). The
//   same text is landed as 0x0045C5F6 (Leaf ??1); each object is section-for-section identical to that one. The TU is size-optimised
//   (odd addresses, alignment-1 COMDATs, as in every def TU row): optimize("s").
// Strong Alloc (NF363, option A; advisor ruling, owner delegated, 2026-11-24): like 0x0045C5F6 this object also defines
//   ?Alloc@CThingNoiseDef@@SIPAVCDefClassBase@@XZ strongly, byte- and relocation-identical (one LNK2005 per extra object with identical
//   bodies, the nf184 pattern of the landed 0x00432C40 / 0x004317D0 / 0x004304E0 Alloc rows). One retail TU, several catalog addresses:
//   the one-source-several-addresses route (NF359 option S) would remove the duplicates.
// Pick-any: engine/CDefPointeeBase.h is included ABOVE the size pragma (scaffolding, not retail). Its inline ~CDefPointeeBase gives
//   this TU a ??_7CDefPointeeBase / ??_GCDefPointeeBase COMDAT; compiled under the pragma that copy is the /Os form (`pop ecx`), while
//   the copy retail kept, 0x0042D9A0, is the /O2 form (`add esp,4`). Included first, it takes the command-line /O2 and equals
//   0x0042D9A0 (NF355 probe p3). Fragile to the include order of the shared def headers.
// Model: the shared engine/CThingNoiseDef.h (FABLE_RETAIL_CTHINGNOISEDEF_VIRTUAL_MODEL) on engine/CThingBaseDef.h (retail branch,
//   Components typed so ~CThingBaseDef is the compiler-generated one; ctor declared) and engine/CTCNoiseDef.h (implicit destructor,
//   inline constructor).
// Was (NF363): the free stand-in ?CThingNoiseDef_ScalarDeletingDestructor@@YIPAXPAXHI@Z (NF355 bound its ~CThingNoiseDef call through the
//   FABLE_RETAIL_CTHINGNOISEDEF_DTOR_DECL opt-in, which this row no longer defines).
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
