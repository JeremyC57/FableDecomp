# DOSSIER 0x0046CE2D -- CPlayerGuiDef::operator=(const CPlayerGuiDef&)  (7312 bytes)

Prior work: none. $W/prior/ is empty; /opt/fable_lane/w22/agent2098 (the earlier chunk) has empty src/ probe/ test/ cand/
dirs (only CHUNK_BRIEF.md listing the row). This is a fresh scout.

Helper files written here: $W/events.txt (every retail block: copy runs / calls with member names), $W/members.tsv
(the full retail member list +0x3c..+0xab0 with retail types, generated from the CPlayerGuiDef.h default path and
checked against every retail offset), $W/ev.py / $W/mem.py (the generators).

## 1. Retail shape (xdis 0x0046CE2D, $W/dis_all.txt)
- `__thiscall`, `ret 4`, returns `this` in eax (`mov eax, ebx` at 0x0046EAB7). Extent 0x0046CE2D..0x0046EABC = 7312 bytes, exact
  (next byte 0x0046EABD is vector<C2DVector>::operator=, a landed row). One straight-line block: NO branches, NO SEH, NO /GS,
  NO frame pointer (ebp is the `other` register, not a frame).
- Prologue: `push ebx ; push ebp ; mov ebp,[esp+0Ch] ; push esi ; push edi ; push ebp ; mov ebx,ecx ; call 0x00431ED0`.
  this = ebx, other = ebp. Epilogue interleaves the last store: `mov eax,[ebp+0AB0h] ; pop edi ; pop esi ; mov [ebx+0AB0h],eax ;
  pop ebp ; mov eax,ebx ; pop ebx ; ret 4`.
- Instruction census: 546 x (`mov eax,[ebp+N]` / `mov [ebx+N],eax`) dword copies, ONE `lea esi,[ebp+54h]; lea edi,[ebx+54h];
  movsd x3` (the 12-byte C3DVector DiffuseVector), 45 x (`lea eax,[ebp+N]; push eax; lea ecx,[ebx+N]; call member::operator=`),
  plus the base call. No byte or word moves (CPlayerGuiDef has no bool/char members; all CRGBColour members copy as one dword).
- Flags: size-optimised def TU, `#pragma optimize("s", on)` (the sibling def operator=s in this neighbourhood all use it:
  0x0046F43E CAbilityButtonGuiDef::operator=, 0x00465EB3 CGlobalDef, 0x0045BE2B CEnvironmentDef). The vector operator=
  callees in this neighbourhood (0x0046EABD, 0x00454886) are `optimize("y",off)`+`("s",on)` -- that is THEIR state; this row has
  no `push ebp; mov ebp,esp`, so do NOT add `optimize("y",off)` (it would force a frame).

## 2. Identity (manifest label is WRONG)
- Manifest says `CParentDefClassBase::operator=` -- propagated from the base call. Real: **CPlayerGuiDef::operator=**,
  leaf **`??4CPlayerGuiDef@@QAEAAV0@ABV0@@Z`** (compiler-generated implicit copy assignment).
- Evidence 1: rebuild/manifest/rtti-vtables.tsv: CPlayerGuiDef vtable 0x012352DC slot 19 (cell 0x01235328) = 0x0046CE28, the 5-byte
  `e9 00000000` (jmp to the next byte = 0x0046CE2D). Slot 19 is `?Copy@CPlayerGuiDef@@UAEXPBVCDefClassBase@@@Z` in
  FableWin.exe.signatures.tsv (0x0046CE28 row) and _fablewin_api.tsv ("definition" 0x019203C7). Copy = `*this = *static_cast<const
  CPlayerGuiDef*>(def);` tail-jumps into this body (same pattern as landed 0x00462FAC/0x00462FB1 CUILocaleGraphicsDef and
  0x0046F49F/0x0046F43E CAbilityButtonGuiDef).
- Evidence 2: ghidra_out/egor_pdb_names.tsv: Ego_r.pdb `??4CPlayerGuiDef@@QAEAAV0@ABV0@@Z` @ 0x00480662, length 7318 (egor_signatures
  row "0 7318 1291") vs retail 7312 -- the same function (Ego is Dinkumware, slightly different sizes). Ego_r ?Copy@CPlayerGuiDef 0x00482910.
- Evidence 3: the copied layout is exactly CPlayerGuiDef's (sizeof retail 0xab4 = engine/CPlayerGuiDef.h FABLE_STATIC_ASSERT; last
  store +0xAB0 MouseOverlayGraphic) and the container offsets match its members (section 4).
- Signature: `CPlayerGuiDef& CPlayerGuiDef::operator=(const CPlayerGuiDef&)` -- public, non-virtual, returns non-const ref.
  Write it IMPLICIT (do not declare it); emit it by defining `void CPlayerGuiDef::Copy(const CDefClassBase* def) { *this =
  *static_cast<const CPlayerGuiDef*>(def); }` in the row (as 0x0046F43E does). Callers: only 0x0046CE28 (xref.py).
- NAME_FIXES candidate: landed rebuild/src/compiled/00/46/CPlayerGuiDef_Copy_0046ce28.cpp models Copy as a free
  `__fastcall CPlayerGuiDef_Copy(CPlayerGuiDef*, void*)` tail-calling `CPlayerGuiDef_Copy_impl` -- both PDB-wrong spellings;
  real ones `?Copy@CPlayerGuiDef@@UAEXPBVCDefClassBase@@@Z` and `??4CPlayerGuiDef@@QAEAAV0@ABV0@@Z` (this row).

## 3. Callees (retail order; all are `ecx = &this->M, push &other.M`)
| addr | landed row | landed name / leaf | PDB / correct name | flag |
|---|---|---|---|---|
| 0x00431ED0 | 00/43/CParentDefClassBase_operator_00431ed0.cpp | CParentDefClassBase::operator= `??4CParentDefClassBase@@QAEAAV0@ABV0@@Z` | same (both PDBs) | ok |
| 0x00454886 (x16) | 00/45/stdvector_operator_00454886.cpp | `??4?$vector@JV?$allocator@J@std@@@std@@QAEAAV01@ABV01@@Z` | std::vector<long>::operator= | ok |
| 0x0046EABD (x3) | 00/46/stdvector_operator_0046eabd.cpp | `??4?$vector@VC2DVector@@...` | std::vector<C2DVector>::operator= | ok |
| 0x0045EF2D (x5) | 00/45/stdvector_operator_0045ef2d.cpp | `??4?$vector@MV?$allocator@M@std@@@std@@QAEAAV01@ABV01@@Z` | std::vector<float>::operator= | ok |
| 0x0099EFB0 (x7) | 00/99/CCharString_operator_0099efb0.cpp | CCharString::operator= | `??4CCharString@@QAEABV0@ABV0@@Z` (const CCharString& return; fable_string.h:436) | ok |
| 0x0045BAAA (x5) | 00/45/stdvectorCRGBColour_operator_0045baaa.cpp | `??4?$vector` (truncated leaf) | Ego_r `??4?$vector@VCRGBColour@@V?$allocator@VCRGBColour@@@std@@@std@@QAEAAV01@ABV01@@Z` | leaf incomplete |
| 0x0046ED15 (x2) | 00/46/VectorCQuickAccessItemButtonGuiDef_Assign_0046ed15.cpp | `Vector<CQuickAccessItemButtonGuiDef>::Assign`, Leaf: Assign | Ego_r+FableWin `??4?$vector@VCQuickAccessItemButtonGuiDef@@V?$allocator@VCQuickAccessItemButtonGuiDef@@@std@@@std@@QAEAAV01@ABV01@@Z` | **WRONG name** |
| 0x0046F06A (x2) | 00/46/STLvectorCAbilityButtonGuiDef_operator_0046f06a.cpp | leaf `??4?$vector@VCAbilityButtonGuiDef@@@std@@IAEAAU01@ABU01@@Z` | Ego_r+FableWin `...QAEAAV01@ABV01@@Z` (public, class) | **WRONG leaf** (I/U) |
| 0x00432EE9 (x2) | 00/43/CCharStringVector_operator_00432ee9.cpp | `??4CCharStringVector@` | Ego_r 0x0043D936 / FableWin `??4?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@QAEAAV01@ABV01@@Z` | **WRONG name** |
| 0x00432B38 (x2: ScriptSprites, MiniMapGraphics) | 00/43/CRetailTree00432bab_operator_00432b38.cpp | `CRetailTree_00432bab::operator=` (made-up) | STLport `_Rb_tree<CCharString, pair<const CCharString,long>, _Select1st<..>, less<CCharString>, allocator<..>>::operator=` (map::operator= is inline `_M_t = x._M_t`; Ego/FableWin only have Dinkumware `??4?$_Tree@V?$_Tmap_traits@VCCharString@@J...`) | **made-up name** |
| 0x0046EBE4 (x1: GameActionValues) | 00/46/CRetailTree0046ec57_operator_0046ebe4.cpp | `CRetailTree_0046ec57::operator=` (made-up) | STLport `_Rb_tree<CCharString, pair<const CCharString,EGameAction>, ...>::operator=` (FableWin `??4?$map@VCCharString@@W4EGameAction@@...` / `??4?$_Tree@...EGameAction...`) | **made-up name** |
No vtable-slot calls. No globals.

## 4. Members (all offsets; full list $W/members.tsv; per-block map $W/events.txt)
- Shared header: rebuild/include/engine/CPlayerGuiDef.h (generated; Ego_r names; retail STLport layout, sizeof 0xab4 asserted).
  **Problem**: its default path is FLAT -- `void* __vftable; _pad_0x04..0x3b` instead of a CParentDefClassBase base, containers as
  `unsigned char X[0xc]`, CRGBColour as `unsigned char[4]`, C2DVector/C3DVector split into _X/_Y(/_Z) floats, CCharString as
  `CCharStringData* X_PStringData`, and `#pragma pack(push,1)`. An implicit operator= of that class copies the pads and never calls
  anything -> unusable as-is. Existing opt-ins only retype single members: FABLE_RETAIL_CPLAYERGUIDEF_CRIME (CrimeBarIconOffset /
  CrimeBarBorderOffset as C2DVector), _MINIGAME_OFFSET, _FLOURISH, _QUICKACCESSEFFECTOFFSET, _PROGRESS, _QUESTINFO, _TARGETING,
  _SCRIPTSPRITES (ScriptSprites map<CCharString,long> +0x934), _ABILITYBUTTONS (AbilityButtons vector<CAbilityButtonGuiDef> +0x7FC,
  but it DEFINES a local opaque `class CAbilityButtonGuiDef` that collides with the real shared engine/CAbilityButtonGuiDef.h).
  None gives the base class or the other 43 containers.
- Recommendation: add ONE new whole-class opt-in in the $W/hdr overlay (e.g. `FABLE_RETAIL_CPLAYERGUIDEF_CLASS`; full copy of the
  current header + an `#ifdef` branch): `class CPlayerGuiDef : public CParentDefClassBase` (natural packing for the derived part --
  base is 0x39 packed, first member lands at +0x3c by alignment, exactly as the 0x0046F43E sibling model), virtual Copy declared, and
  the members of $W/members.tsv in offset order with real types: long / float / unsigned long / CRGBColour (engine/CRGBColour.h) /
  C2DVector (engine/C2DVector.h, 129 members) / C3DVector DiffuseVector (engine/C3DVector.h) / CCharString (fable_string.h) /
  std::vector<long|float|C2DVector|CRGBColour|CCharString|CQuickAccessItemButtonGuiDef|CAbilityButtonGuiDef> /
  std::map<CCharString,long> (ScriptSprites +0x934, MiniMapGraphics +0x94C) / std::map<CCharString,EGameAction> (GameActionValues
  +0x940; engine/EGameAction.h). Element types: engine/CQuickAccessItemButtonGuiDef.h (0x50) and engine/CAbilityButtonGuiDef.h
  (0x5C) -- both need FABLE_RETAIL_DEF_CLASS_BASE; do NOT also define FABLE_RETAIL_CPLAYERGUIDEF_ABILITYBUTTONS (duplicate class).
- Retail-verified member offsets (each call's ecx == the header's offset; each copy run lies exactly between them):
  vectors/strings/maps at +0x150 +0x18C +0x198 +0x1A4 +0x35C +0x384 +0x394 +0x3A0 +0x444 +0x450 +0x45C +0x4E0(str) +0x540 +0x54C
  +0x558 +0x5D4(str) +0x5E8(str) +0x5EC(str) +0x644(str) +0x708 +0x728 +0x734 +0x7FC +0x808 +0x82C +0x86C +0x878 +0x884 +0x890
  +0x89C +0x8A8 +0x8B4 +0x8F8(str) +0x910 +0x91C +0x928 +0x934(map) +0x940(map) +0x94C(map) +0x958 +0x9A0 +0x9AC +0xA24 +0xA40
  +0xA94(str). Ego_r names in $W/events.txt. ScriptSprites +0x934 also independently confirmed by chunk 855 (header comment).
- Local models elsewhere that have a shared header: the older def operator= rows (CGlobalDef 0x00465EB3, CEnvironmentDef 0x0045BE2B)
  model CCharString / vectors locally -- do not copy that; the strict verifier rejects local `this` models when a header exists.

## 5. Callee-body visibility
- vector operator= COMDATs for C2DVector (0x0046EABD), QuickAccessItemButtonGuiDef (0x0046ED15), AbilityButtonGuiDef (0x0046F06A) and
  the EGameAction tree op= (0x0046EBE4) sit right after this row (0x0046EABD..0x0046F0xx) -> instantiated from THIS TU (bodies
  visible), but VC7.1 still calls vector::operator= out of line. Header opt-ins: `FABLE_RETAIL_STLP_VECTOR_ASSIGN` (declaration only,
  fable_containers.h:424) or `FABLE_RETAIL_STLP_VECTOR_C` (body visible, still called; :429/:577). Without one of them STLport
  vector has NO declared operator= in the default path -> the implicit one would copy 3 pointers inline: WRONG. Start with
  VECTOR_ASSIGN (simplest); either must produce `call`.
- map: fable_map.h `class map` declares no operator=, its only member `_M_t` is `_Rb_tree`, which declares `_Rb_tree&
  operator=(const _Rb_tree&);` out of line (fable_map.h:112) -> implicit map op= collapses to `call _Rb_tree::operator=` with ecx =
  &member (map's _M_t at +0) -- exactly retail. Needs FABLE_RETAIL_STLP_MAP? Not for operator=; include "stlport/fable_map.h".
- CCharString::operator= declared out of line in fable_string.h:436 (body landed 0x0099EFB0) -- call. Good.
- CParentDefClassBase::operator=: use FABLE_RETAIL_DEF_CLASS_BASE (+ FABLE_PDCB_TYPED_MEMBERS if you define CDefString/CVectorMap
  first, as 0x0046F43E does). Do NOT define FABLE_RETAIL_PDCB_IMPLICIT_ASSIGN / _FORCEINLINE_ASSIGN: retail CALLS 0x00431ED0.
- C2DVector / C3DVector / CRGBColour must have NO user-declared operator= active (C2DVector.h FABLE_RETAIL_C2DVECTOR_ASSIGN3D only
  adds operator=(const C3DVector&), which also suppresses nothing for the copy op= -- avoid it anyway). Trivial copy gives the
  2-mov (C2DVector), 1-mov (CRGBColour) and movsd x3 (C3DVector, /Os 12-byte block copy) shapes seen.

## 6. Sibling landed rows to copy idioms from
- rebuild/src/compiled/00/46/CAbilityButtonGuiDef_operator_0046f43e.cpp -- THE model: implicit op= emitted via a Copy definition,
  `#pragma optimize("s", on)`, pack(1) only around `#define FABLE_RETAIL_DEF_CLASS_BASE` + `#define FABLE_PDCB_TYPED_MEMBERS` +
  `#include "engine/CParentDefClassBase.h"`, derived class natural-packed, C2DVector copied by 2 movs. (It uses a local STL
  model; you should use the stlport headers instead.)
- rebuild/src/compiled/00/46/NUISystemCUILocaleGraphicsDef_Copy_00462fac.cpp -- `optimize("s",on)`, FABLE_RETAIL_DEF_CLASS_BASE,
  FABLE_RETAIL_STLP_VECTOR, FABLE_RETAIL_STLP_MAP + shared header engine/NUISystem_CUILocaleGraphicsDef.h; Copy = `*this = *static_cast<..>(def)`.
- rebuild/src/compiled/00/46/CGlobalDef_operator_00465eb3.cpp, 00/45/CEnvironmentDef_operator_0045be2b.cpp -- same family,
  `optimize("s",on)`, explicit memberwise text (older style, local models).
- Neighbouring callees: 00/46/stdvector_operator_0046eabd.cpp (`optimize("y",off)`+`("s",on)`), 00/45/stdvector_operator_00454886.cpp.
