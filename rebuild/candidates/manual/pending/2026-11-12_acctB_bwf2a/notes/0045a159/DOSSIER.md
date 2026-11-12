# DOSSIER -- retail 0x0045A159 (6481 bytes) = CInventoryDef::operator=(const CInventoryDef&)

## 1. Retail shape
- __thiscall, `ret 4`, returns *this (eax=ebx). No frame pointer (ebp = `other`), no SEH, no /GS, no locals.
  Straight line, no branches. See OUTLINE.md "Frame facts".
- Size idioms: every scalar through eax only, 12/14-byte structs via `lea esi/lea edi ; movsd.. movsw` => `#pragma optimize("s", on)`
  (same as landed siblings CInventoryCategoryDef::operator= 0x0045BD42 and CEnvironmentDef::operator= 0x0045BE2B, which also have
  no ebp frame). Do NOT add optimize("y", off) (that is the CSpecialAbilitiesMultiArrowDef 0x0045F7C7 TU, which has a frame).

## 2. Identity -- the manifest label `CParentDefClassBase::operator=` is WRONG (propagated)
- Real name: CInventoryDef::operator=, leaf `??4CInventoryDef@@QAEAAV0@ABV0@@Z`
  signature `CInventoryDef& CInventoryDef::operator=(const CInventoryDef& other)` (public, __thiscall).
- Evidence 1: its ONLY caller is 0x0045A154 `jmp 0x0045A159` (xref.py), which is CInventoryDef's Copy vtable slot
  (ghidra_out/rtti_port_compare.tsv: 0x0045A154 = ?Copy@CInventoryDef@@UAEXPBVCDefClassBase@@@Z; landed as
  rebuild/src/compiled/00/45/CInventoryDef_Copy_0045a154.cpp, a stand-in forwarder to `ForwardTarget_0045a159`).
  Same Copy-jmp-operator= pairing as the landed CInventoryCategoryDef 0x0045BD3D/0x0045BD42, CEnvironmentDef 0x0045BE26/0x0045BE2B.
- Evidence 2: Ego_r.pdb names `??4CInventoryDef@@QAEAAV0@ABV0@@Z` (ghidra_out/egor_pdb_names.tsv @ Ego_r 0x0047965F).
- Evidence 3: the copied offsets/widths are exactly Ego_r.pdb CInventoryDef's 391 members (struct_layouts_egor.tsv, sizeof 2656)
  in declaration order: I generated the expected copy stream from the PDB layout (retail off = PDB off - 4 - 4*(#vectors before),
  C2DVector/CEngineGraphic byte-packed) and it equals the retail stream (523 events) except one scheduling swap
  (script output: $W/member_map.tsv, $W/layout_computed.tsv). The base call 0x00431ED0 is CParentDefClassBase::operator=.
- NAME_FIXES candidate: Copy row 0x0045A154 forwards to `ForwardTarget_0045a159`; once this row lands the target is
  `??4CInventoryDef@@QAEAAV0@ABV0@@Z` (CInventoryDef::operator=). manifest functions.tsv module/name for 0x0045A159 should
  become CInventoryDef / operator=.

## 3. Callees
| retail | landed row | landed name / leaf | PDB name | note |
|---|---|---|---|---|
| 0x00431ED0 | rebuild/src/compiled/00/43/CParentDefClassBase_operator_00431ed0.cpp | ??4CParentDefClassBase@@QAEAAV0@ABV0@@Z | same | out of line; shared header declares it by default (CParentDefClassBase.h, do NOT define FABLE_RETAIL_PDCB_IMPLICIT_ASSIGN / _FORCEINLINE_ASSIGN) |
| 0x00454886 (x8) | 00/45/stdvector_operator_00454886.cpp | ??4?$vector@JV?$allocator@J@std@@@std@@QAEAAV01@ABV01@@Z | same | manifest module std::vector<long>; agrees |
| 0x0045BC09 (x1) | 00/45/stdvector_operator_0045bc09.cpp | ??4?$vector@KV?$allocator@K@std@@@std@@QAEAAV01@ABV01@@Z | same | vector<unsigned long>; StatLevelsTexts |
| 0x0045BAAA (x1) | 00/45/stdvectorCRGBColour_operator_0045baaa.cpp | leaf truncated `??4?$vector` | ??4?$vector@VCRGBColour@@V?$allocator@VCRGBColour@@@std@@@std@@QAEAAV01@ABV01@@Z | catalog label "CActiveFile_AssignFrom" is wrong (behavioural); StatLevelsColours |
No vtable calls, no globals.

## 4. Members (all 391 -- $W/members_decl.txt has type/name/offset in order)
- Shared header rebuild/include/engine/CInventoryDef.h exists but is PARTIAL: members only to MeshAreaBRPos (+0xAC), no
  operator=, includes engine/CParentDefClassBase.h, engine/C2DVector.h, stlport/fable_containers.h; no FABLE_RETAIL_* opt-ins
  of its own. The author must extend it via the header overlay ($W/hdr/: full copy of the current header + an `#ifdef`
  block, e.g. a new FABLE_RETAIL_CINVENTORYDEF_FULL adding the remaining members from +0xB4 and the operator= declaration).
  (An older 2656-byte PDB-offset model sits in rebuild/include/engine/_quarantine/CInventoryDef.h -- PDB offsets, do not use.)
- Base: engine/CParentDefClassBase.h with `#define FABLE_RETAIL_DEF_CLASS_BASE` (0x39 bytes, pack 1; polymorphic; first
  member float at +0x3C by natural alignment). CInventoryDef itself is NOT packed (bool +0x58 then vector +0x5C; ulong
  TradeTextOwned realigned to +0x6C4 after 2 bytes padding; float +0xA30 after a pad of 2).
- C2DVector MUST be the packed retail one: `#define FABLE_RETAIL_C2DVECTOR` (engine/C2DVector.h: pack(1), user copy ctor, no
  user operator= -> trivial 2-dword copy). Without it the C2DVectors after the 14-byte CEngineGraphic runs (e.g.
  MainDescriptionBoxTLPos at +0x9FE, MainAnimBoxExtents +0xA26) land on 4-byte boundaries and every offset after 0x92C breaks.
- CEngineGraphic: engine/CEngineGraphic.h (pack 1, 14 bytes; don't define FABLE_RETAIL_CENGINEGRAPHIC_CTOR -- not needed,
  copy is trivial either way). 36 members.
- C3DVector: engine/C3DVector.h (12 bytes, no user operator=): DollCentreOffset +0xD8, StatsPersonalityDollCentreOffset +0x2E4.
- CRGBColour: engine/CRGBColour.h (union B,G,R,A / IntValue; no user operator= -> one dword): BackGroundColour +0xD0,
  Strength/Skill/WillLevelColour +0x108/+0x114/+0x120, QuestCardFeat/Core/ExclusiveColour +0x794/+0x798/+0x79C.
- CDefString GenericFont +0xCC: engine/CDefString.h (needs FABLE_RETAIL_CDEFSTRINGTABLE + FABLE_RETAIL_CCHARSTRING, as
  CBrainDef_CBrainDef_00459abe.cpp does); no user operator= -> one dword.
- EInventoryIdentifier InventoryIdentifier +0x1F0: no shared enum -- declare `enum EInventoryIdentifier { ... }` locally
  (4-byte enum).
- std::vector<long|unsigned long|CRGBColour>: stlport/fable_containers.h; define FABLE_RETAIL_STLP_VECTOR_ASSIGN (declares
  vector::operator= out of line, chunk 735) -- retail calls all three. FABLE_RETAIL_STLP_VECTOR_C (body visible) is the
  alternative; for a body with no live values across calls either should give the same bytes. Retail emitted the
  vector<CRGBColour>::operator= COMDAT right after this function (0x0045BAAA, then 0x0045BC09), i.e. this TU instantiated
  them, but still calls them.
- Ego_r vs retail: retail offset = Ego_r offset - 4 (base 0x39 vs 0x3D) - 4 per preceding vector (STLport 12 vs PDB 16).
  The earlier scout file $W/layout_retail.tsv is WRONG past the first CEngineGraphic run (it ignored packing); use
  $W/member_map.tsv / layout_computed.tsv.
- Locally-modelled classes to avoid: none required -- every member type has a shared header except the enum.

## 5. Callee visibility
- All callees are out-of-line calls in retail; nothing inline to expand. 0x00431ED0 is retail's out-of-line COMDAT of the
  implicit CParentDefClassBase::operator= (header default = declared, correct here; the /Os callers "reproduce" per header note).
- STLport vector op= instances: declaration via FABLE_RETAIL_STLP_VECTOR_ASSIGN.
- CEngineGraphic/C3DVector/C2DVector/CRGBColour/CDefString assignments are trivial (implicit), emitted inline as moves.

## 6. Siblings (same TU family: def Copy/operator= at 0x0045Bxxx, size TU)
- 00/45/CInventoryCategoryDef_operator_0045bd42.cpp: `#pragma optimize("s", on)`, `#define FABLE_RETAIL_DEF_CLASS_BASE`,
  class declared with `virtual void Copy(const CDefClassBase*)` defined as `*this = *static_cast<const X*>(other);` and
  NO operator= statement (implicit COMDAT emitted). Leaf ??4CInventoryCategoryDef@@QAEAAV0@ABV0@@Z.
- 00/45/CEnvironmentDef_operator_0045be2b.cpp: optimize("s", on), base include wrapped in pack(push,1)/pop, explicit
  memberwise operator= written out in declaration order (CCharString op= out of line).
- 00/45/CSpecialAbilitiesMultiArrowDef_operator_0045f7c7.cpp: explicit memberwise with vector<long> op= 0x00454886 (but that
  TU is y-off; use its statement style only).
- 00/62/CConfigOptionsDefaultsDef_operator_00625350.cpp: explicit operator= with base op= + member op= 0x0045BC09.
- 00/45/CBrainDef_CBrainDef_00459abe.cpp: CDefString-include defines (FABLE_RETAIL_CDEFSTRINGTABLE, FABLE_RETAIL_CCHARSTRING).
None of them carry a `// Flags:` line (default /O2 + pragmas).

## Approach (recommendation)
Either (A) explicit `CInventoryDef& operator=(const CInventoryDef&)` declared in the class and defined in the row:
base op= call, then 391 `M = other.M;` in PDB order ($W/members_body.txt is ready to paste), `return *this;` -- the style of
0x0045BE2B; or (B) implicit operator= emitted from a `Copy` body, as 0x0045BD42 (lesson f4005). With 391 members, (A)
is the safer first try; the only scheduling residue risk is the ClothingDoll/DollCentreOffset swap at 0x0045A2A7, which the
compiler should produce itself. Grade the operator= leaf ??4CInventoryDef@@QAEAAV0@ABV0@@Z.
