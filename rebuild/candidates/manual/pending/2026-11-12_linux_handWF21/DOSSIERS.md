## 0x004CE1B0

# DOSSIER 0x004CE1B0 CSubtitledMessage::AddMasterShotCameraTagFromText (814 bytes)

## 1. Retail shape (xdis 004ce1b0 0x33e)
- __thiscall, `ret 8` (2 dword args). No frame pointer (`sub esp,0x74; push ebx; push esi`), so /Oy on (no optimize("y",off)).
- NO SEH prologue even though CStringParser + 3 CCharStrings with dtors live in the frame -> TU without EH unwinding (as sibling rows; default lane flags).
- Not a size TU: `mov bl,al; neg bl; sbb bl,bl; inc bl` = `!SkipPastNextItemIfIs(tmp)` stored before the temp dies; normal speed idioms.
- Arg reads: text = [entry+8] (2nd arg, into the parser ctor), time = [entry+4] (1st arg, stored as raw dword into pair.first).
- Exits: fail-1 ("(") -> shared block 0x4CE2FF that INLINES ~CStringParser (list dtor 0x4046B0, ~CCharString x2, ~CBaseClass 0x99A2E0);
  fail-2 (",") 0x4CE2F6 = ~first then that block; fail-3 (",") 0x4CE2ED = ~second, ~first, block. All LATER exits call
  ~CStringParser OUT OF LINE (0x4047A0): ")" fail and lookup-1 fail -> 0x4CE4B0, lookup-2 fail -> 0x4CE4B4 (ecx preloaded),
  lookup-3 fail -> 0x4CE480, success -> 0x4CE46B. So: ONE inlined copy of the dtor, four calls (VC7.1 inline budget). The old
  draft note "inlines at the first three exits" means the three early returns merge into that one inline copy.
- Frame (base = esp after push esi): first str +8, third str +0xC, second str +0x10, literal temp +0x14 (re-used as lookup-3 out),
  lookup-1 out +0x18, lookup-2 out +0x1C, **CCameraTag local at +0x20 (its CameraPosition +0x24 is stored right after lookup 1:
  0x4CE3C2 `mov [esp+28h],ecx`)**, pair<float,CCameraTag> at +0x38..+0x54, CStringParser +0x54..+0x7C (0x28).
  Draft frame 0x5C vs retail 0x74 = exactly 0x18 = sizeof(CCameraTag): retail has a separate CCameraTag local AND the pair.
- Pair built at +0x38: first=time; +4 CameraPositionPerson=0 (eax); +8 CameraPosition=lookup1 (copied from tag+4);
  +0xC CameraFocusPerson=0 (eax); +0x10 CameraFocusPosition=lookup2 (ebx); +0x14 CameraZoomType=0 (ebp, set `xor ebp,ebp` before
  operator!=); +0x18 CameraView = 1 (edi default) or lookup3 when third != "".
- Then `lea edx,pair; push; ecx=this+0x34; call 0x4D18E0` (push_back) and `mov byte [this+0x41],1` (Dirty) -> CVectorMap::Insert.
- Success destroys third, second, first, parser (reverse construction). Returns `mov al,1`/`xor al,al`.

## 2. Identity (name proven by two sources)
- Ego_r.pdb: ghidra_out/egor_pdb_names.tsv 004eb890 `?AddMasterShotCameraTagFromText@CSubtitledMessage@@AAE_NMABVCCharString@@@Z`.
- FableWin.pdb: ghidra_out/fablewin_pdb_names.tsv 01b70630 same mangled name (also _fablewin_api.tsv 017cd990/01b70630).
- Ego_r neighbour order (AddCameraTagFromText 4eb370, AddMasterShot 4eb890, ctors, AddCameraTagFromIndex 4ebc20) matches retail
  0x004CDC40 / 0x004CE1B0 / 0x004CE4E0 (landed AddCameraTagFromIndex). Manifest label is RIGHT.
- Signature: `private: bool __thiscall CSubtitledMessage::AddMasterShotCameraTagFromText(float time, const CCharString& text)`.
  **Old draft had (const CCharString& text, long time) -- WRONG order and type** (retail reads text from arg 2; time is float).
- Sole caller: 0x004CE79A inside CSubtitledMessage::CSubtitledMessage(const NGameText::CTextEntry&) 0x004CE550 (parked, not landed).

## 3. Callees
| addr | landed file | landed name | PDB name / note |
|---|---|---|---|
| 0x0099EBF0 | rebuild/src/compiled/00/99/CCharString_Constructor_0099ebf0.cpp | CCharString(const char*, long=-1) | ??0CCharString@@QAE@PBDJ@Z (fable_string.h:294) |
| 0x0099EAE0 | .../00/99/CCharString_CCharString_0099eae0.cpp | ~CCharString | ok |
| 0x00404720 | .../00/40/CStringParser_CStringParser_00404720.cpp | CStringParser(const CCharString&, cb, const CCharString& filename, long) | ok; called (text, 0, CCharString(""), 0) |
| 0x004046B0 | .../00/40/stdlistCStringParserCommentBoundaries_list_004046b0.cpp | list<CStringParserCommentBoundaries>::~list | inlined-dtor part |
| 0x004047A0 | .../00/40/CStringParser_CStringParser_004047a0.cpp | ~CStringParser (implicit, virtual via CBaseClass; no vptr re-store) | ??1CStringParser@@UAE@XZ |
| 0x0099A2E0 | .../00/99/CBaseClass_CBaseClass_0099a2e0.cpp | ~CBaseClass | ??1CBaseClass@@UAE@XZ -- draft called it CBase::~CBase (WRONG) |
| 0x009B9C60 | .../00/9b/CStringParser_SkipWhitespace_009b9c60.cpp | long SkipWhitespace() | draft had void (PDB `QAEJXZ` returns long) |
| 0x009B9E00 | .../00/9b/CStringParser_SkipPastNextItemIfIs_009b9e00.cpp | bool SkipPastNextItemIfIs(const CCharString&) | ok |
| 0x009BA330 | .../00/9b/CStringParser_ReadAsStringUntilString_009ba330.cpp | CCharString ReadAsStringUntilString(const CCharString&) | ok |
| 0x00A38420 | .../00/a3/CCRCSymbolMap_FindSymbol_00a38420.cpp | CCRCSymbolMap::FindSymbol | `?FindSymbol@CCRCSymbolMap@@QBE_NABVCCharString@@AAJ@Z` = bool FindSymbol(const CCharString&, long&) const. Draft: `CSymbolTable::Lookup(const CCharString&, long*)` WRONG class/name/const/ref |
| global 0x013B8A54 | declared in .../00/4c/CSubtitledMessage_Initialise_004cdb10.cpp | `extern CCRCSymbolMap g_MiscDefTypesSymbolMap;` (descriptive name, PDB has none) | draft `g_CameraSymbolTable` WRONG -- one address one name |
| 0x0099E960 | .../00/99/CCharString_operator_0099e960.cpp | bool CCharString::operator!=(const char*) const | ok (fable_string.h:351, out of line) |
| 0x004D18E0 | .../00/4d/CRetailVectorSPair28_PushBackSPair28004d18e0_004d18e0.cpp | CRetailVector_SPair28::PushBack_SPair28_004d18e0 (made-up) | Ego_r 004f04a0 `?push_back@?$CArray@U?$pair@MVCCameraTag@CSubtitledMessage@@@std@@@@QAEXABU?$pair@MVCCameraTag@CSubtitledMessage@@@std@@@Z` -> NAME_FIXES candidate: CArray<std::pair<float,CSubtitledMessage::CCameraTag> >::push_back |
- Literals: 0x0122D70E "" ; 0x0122E028 "(" ; 0x0122E024 "," ; 0x0122E020 ")" (read from Fable.exe). Each `CCharString(lit)` temp at frame+0x14.

## 4. Members
- this+0x34 = CSubtitledMessage::CameraTags, `CVectorMap<float,CSubtitledMessage::CCameraTag,CKeyPairCompareLess<...> >` (Ego_r struct_layouts:
  +60 with 0x14 Dinkum maps; retail maps are 0x10 so AttitudeTags 0x14, AnimationTags 0x24, CameraTags 0x34 -- confirmed by the landed
  ctor 0x004CDBD0 local CVectorMapTags model). +0x41 = CameraTags.Dirty (CVectorMap.h: Compare +0xC, Dirty +0xD).
- NO shared engine/CSubtitledMessage.h exists (all landed CSubtitledMessage rows model locally). Use engine/CVectorMap.h for the map.
- CVectorMap.h opt-ins: FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE (Insert(const K&, const V&), FableWin.pdb has exactly
  `?Insert@?$CVectorMap@MVCCameraTag@CSubtitledMessage@@...@@QAEXABMABVCCameraTag@CSubtitledMessage@@@Z` @01b72450) and nested
  FABLE_RETAIL_CVECTORMAP_INSERT_VALUE_TYPE (pair built as value_type temporary, not make_pair). push_back must stay out of line:
  engine/CArray.h FABLE_RETAIL_CARRAY_PUSHBACK_NOINLINE / FABLE_RETAIL_CARRAY_PUSHBACK (CArray::push_back(const T&) out of line).
- CCameraTag (0x18, struct_layouts_egor.tsv): +0 EConversationProtagonist CameraPositionPerson, +4 EConversationCameraPosition CameraPosition,
  +8 EConversationProtagonist CameraFocusPerson, +0xC EConversationCameraFocus CameraFocusPosition, +0x10 EConversationCameraZoomType
  CameraZoomType, +0x14 EConversationCameraView CameraView. No ctor in either PDB (POD). Draft's SCameraTag Time/Unused04/CameraA/... names WRONG:
  it flattened pair<float,CCameraTag> into one struct.
- CStringParser: engine/CStringParser.h under FABLE_RETAIL_CSTRINGPARSER (0x28, CBaseClass base, STLport list -> also FABLE_RETAIL_STLP_LIST,
  FABLE_RETAIL_CCHARSTRING). Draft modelled locally: CCharString, CBase, CStringParserCommentList, CStringParser, CSymbolTable, SCameraTag,
  CCameraTags -- CStringParser/CBaseClass/CCharString/CVectorMap all have shared headers.

## 5. Visibility
- ~CStringParser is the implicit virtual dtor: inline-expandable (list dtor + strings + ~CBaseClass), VC7.1 expands it once then calls the
  COMDAT 0x004047A0 at the rest (budget) -- so the dtor must be visible/implicit, NOT declared out of line, NOT forceinline (NOTES: forceinline/plain tried).
- Sibling AddCameraTagFromText 0x004CDC40 (same TU, defined just before; parked hand323) shows the same 1 inline + 4 out-of-line ~CStringParser
  split; its best probe used `#pragma inline_depth(0)` regions ("probe 4cd3 matches first 0x341 bytes").
- Out of line in retail: FindSymbol (other TU), push_back 0x4D18E0 (CArray COMDAT), operator!=(const char*), all CStringParser members.

## 6. Siblings
- rebuild/src/compiled/00/4c/CSubtitledMessage_AddCameraTagFromIndex_004ce4e0.cpp (no flags; local model; calls AddCameraTagFromText(float, const CCharString&)).
- .../00/4c/CSubtitledMessage_CSubtitledMessage_004cdbd0.cpp / _004cdb70.cpp: `#define FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP`, local CVectorMapTags (Compare +0xC, Dirty +0xD).
- .../00/4c/CSubtitledMessage_Initialise_004cdb10.cpp: g_MiscDefTypesSymbolMap; no flags.
- .../00/68/CTCCreatureOpinionOfHero_OnSerialise_00684610.cpp: local `CStringParser parser(x, 0, CCharString(""), 0);` with
  FABLE_RETAIL_CCHARSTRING, FABLE_RETAIL_STLP_LIST, FABLE_RETAIL_CSTRINGPARSER + includes fable_string.h, stlport/fable_containers.h, engine/CStringParser.h.
- Parked hand323 differ for 0x004CDC40 (same group G004cdc40 in pending/2026-11-03_groups/ROWS.tsv): pragma inline_depth(0) at lines 79/128.

## 7. Old draft model errors
1. Parameter order/type: must be (float time, const CCharString& text).
2. Frame 0x5C v 0x74: missing the CCameraTag local -- write `CCameraTag tag;` fill fields (tag.CameraPosition = lookup1 right after lookup 1, etc.) then
   `CameraTags.Insert(time, tag)` (KEYVALUE + VALUE_TYPE pair temp) -> push_back(&pair) then Dirty=1.
3. Lookup out-values: lookup1 at +0x18, lookup2 at +0x1C (separate locals), lookup3 reuses the dead literal slot +0x14; zoom 0 in ebp and view default 1 in edi
   are set after lookup 2 succeeds, before `third != ""`.
4. Wrong names: CSymbolTable::Lookup/g_CameraSymbolTable/CBase/SCameraTag/HasMasterShot -> CCRCSymbolMap::FindSymbol(const&, long&) const,
   g_MiscDefTypesSymbolMap, CBaseClass, CSubtitledMessage::CCameraTag, CVectorMap Dirty.
5. Control flow otherwise right (if(!Skip...) return false chain; ReadAsStringUntilString after SkipWhitespace for 2nd/3rd; first read without SkipWhitespace;
   SkipWhitespace before "(" only).

## 0x006A5610

# DOSSIER 0x006A5610 CThingPhysical::GetFirstCylinderCollisionBetweenPoints (818 B)

## 1. Retail shape (xdis 006a5610 0x342)
- thiscall, `ret 0x14` (5 dword args), returns bool in al (`mov al,1` / `xor al,al`), two epilogues.
- NO frame pointer (`sub esp,0x58 ; push ebx ; push esi ; push edi`), no SEH, no /Oy- pragma needed. Frame = 0x58.
- this in edi; esi = &from (arg2, [esp+0x6c]), ebx = &to (arg3, [esp+0x70]).
- Param slots reused: GetMagnitudeAndNormalise result spilled into the `from` slot [esp+0x6c]; `t` (lineParam out) is the `to`
  slot [esp+0x70]; `radius += cylinderRadius` written back into the radius slot [esp+0x68].
- distSq (float return of 0x00A443A0) stays on the x87 stack across the three radius tests; fail path 0x6a5935 pops it
  (`fstp st0`) then falls into 0x6a5937 (return false).
- Frame slot reuse in retail: [0x28..0x30] = up copy -> later from2D -> later (top-bottom) axis; [0x4c..0x54] = up*MeshHeightOffset
  temp -> later top; [0x1c..0x24] = to2D -> later offset (hit-bottom); [0x10..0x18] = bottom2D (point) -> later hitNormal temp;
  [0x40..0x48] = delta (to-from); [0x44/0x48] also briefly hold up.Y*h, up.Z*h before delta is computed; [0x34..0x3c] bottom;
  [0x5c..0x60] axis*along Y/Z.

## 2. Name / signature (two sources)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv @006845b0, egor_signatures.tsv) AND FableWin.pdb (_fablewin_api.tsv @01f22030,
  naming_stragglers/FableWin.exe.signatures.tsv): `?GetFirstCylinderCollisionBetweenPoints@CThingPhysical@@QBE_NMABVC3DVector@@0AAV2@1@Z`
  = public, const, thiscall: `bool CThingPhysical::GetFirstCylinderCollisionBetweenPoints(float, const C3DVector&, const C3DVector&,
  C3DVector&, C3DVector&) const`. Manifest label is CORRECT. Param names (draft): radius, from, to, hitPosition, hitNormal.
- Callers: xref.py finds only 5 "abs .text" hits (0xba9388 etc.) which are misaligned byte noise (0xba9388 is mid-instruction);
  no direct call/vtable caller found. Not a virtual (no rtti-vtables hit).

## 3. Callees (draft spelling vs landed/PDB)
| addr | landed row | landed / PDB name | draft spelling |
|---|---|---|---|
| 0x00661F90 | rebuild/src/compiled/00/66/CThingCreatureBase_GetCombatRadius_00661f90.cpp | PDB `?GetCombatRadius@CThingCreatureBase@@QBEMXZ` (const thiscall). Header: CThingCreatureBase.h `FABLE_RETAIL_CTHINGCREATUREBASE_COMBATRADIUS` declares `float GetCombatRadius() const;`. NOTE landed row is a free `__fastcall CThingCreatureBase_GetCombatRadius` hack (NAME_FIXES candidate) | GetCombatRadius OK |
| vslot +0x120 (slot 72) of CTCPhysicsBase vtable 0x0125D2C4 = 0x006B1290 | rebuild/src/compiled/00/6b/CTCPhysicsBase_GetRHSet_006b1290.cpp | `?GetRHSet@CTCPhysicsBase@@UBEABVCRightHandedSet@@XZ` -> `virtual const CRightHandedSet& GetRHSet() const` (CTCPhysicsBase.h line ~128 under FABLE_RETAIL_CTCPHYSICSBASE vtable list) | **WRONG: draft `GetUp()` virtual**. Real: `PhysicsTC->GetRHSet().GetUp()` (CRightHandedSet::GetUp inline, Up at +0, `FABLE_RETAIL_CRIGHTHANDEDSET` in engine/CRightHandedSet.h); the 12-byte up is copied by value (3 int movs to [0x28]) |
| 0x004C73D0 (called twice; 2nd result unused) | rebuild/src/compiled/00/4c/CThing_GetPosition_004c73d0.cpp | `CThing::GetPos() const` (PDB ?GetPos@CThing@@QBEABVC3DVector@@XZ, CThing.h line 411 `const C3DVector& GetPos() const;`) | **WRONG: draft `GetPosition`** (and it is CThing's, not CThingPhysical::PeekPos 0x006A4D00 twin) |
| 0x00A14480 | rebuild/src/compiled/00/a1/C3DVector_GetMagnitudeAndNormalise_00a14480.cpp | `?GetMagnitudeAndNormalise@C3DVector@@QAEMXZ` (non-const, normalises delta in place, returns length); C3DVector.h opt-in `FABLE_RETAIL_C3DVECTOR_GETMAGNITUDEANDNORMALISE` | **WRONG: draft `GetLength() const`** (semantics too: delta IS unit after the call; hit = delta*length*t + from) |
| 0x00A443A0 (__fastcall, ret 8) | rebuild/src/compiled/00/a4/global_GFGetShortestDistance2DSquaredBetweenPointAndLine_00a443a0.cpp | `?GFGetShortestDistance2DSquaredBetweenPointAndLine@@YIMABVC2DVector@@00AAM@Z` = `float __fastcall GF...(const C2DVector& lineStart, const C2DVector& lineEnd, const C2DVector& point, float& lineParam)`; no shared-header declaration exists (grep rebuild/include: none) | **WRONG: draft `GetClosestPointOnLineSegment(..., float* t)`** - must be `float&` |
| 0x00A14440 (x2: axis, hitNormal) | rebuild/src/compiled/00/a1/C3DVector_AccurateNormalise_00a14440.cpp | `?AccurateNormalise@C3DVector@@QAEAAV1@XZ` returns C3DVector& | **WRONG: draft `Normalise()` void** |
Args at the 2D call: ecx=&from2D (from.X,from.Y @0x28), edx=&to2D (to.X,to.Y @0x1c), push &bottom2D (bottom.X,bottom.Y @0x10), push &t.
Globals: 0.0001f @0x0129BA3C, 1.0f @0x0122DED8.

## 4. Members
- this+0x60 CThing::PhysicsTC (CTCPhysicsBase*) - CThing.h line 70; inline `GetPhysicsTC() const` at CThing.h:410 (needs FABLE_RETAIL_CCLASS/DEF_CLASS_BASE/CTHING chain; CThing.h line 6 confirms PhysicsTC+60, PBaseDef+70 in retail).
- this+0x6c CThing::ThingTypeFlags (unsigned long) - test `& 8` (creature bit; CThingCreatureBase::StatGetTypeFlags()==15). Landed CPhysicalPrimitiveSphere_IsPosInside_00814e10 uses `if (flags & 8) radius = ((CThingCreatureBase*)this)->GetCombatRadius();`.
- this+0x70 CThing::PBaseDef (CDefPointer<const CThingBaseDef>) - read as `mov ecx,[edi+70h]` then [ecx+58h]/[edx+5Ch] (no null test): PBaseDef-> or PeekBaseDef() (CThing.h:260, inline `return *PBaseDef`).
- CThingBaseDef +0x58 MeshHeightOffset (float), +0x5C MeshHeight (float) - rebuild/include/engine/CThingBaseDef.h under `FABLE_RETAIL_CTHINGBASEDEF` (retail offsets; Ego_r struct_layouts has them at +0x5c/+0x60, 4 bytes later). Draft names CylinderBottom/CylinderHeight are made up -> use MeshHeightOffset/MeshHeight.
- CTCPhysicsBase +0x34 Radius (float; struct_layouts_egor CTCPhysicsBase 52 Radius) - CTCPhysicsBase.h line 204, inline `GetRadius() const` line 254.
- CRightHandedSet: Up +0, Forward +12 (CRightHandedSet.h; GetUp under FABLE_RETAIL_CRIGHTHANDEDSET).
- Draft models LOCALLY (all have shared headers): C2DVector (engine/C2DVector.h), C3DVector (engine/C3DVector.h), CTCPhysicsBase
  (engine/CTCPhysicsBase.h), CThingBaseDef (engine/CThingBaseDef.h), CThingPhysical (engine/CThingPhysical.h), CThingCreatureBase
  (engine/CThingCreatureBase.h); also missing CRightHandedSet.h.

## 5. Callee visibility
- Out of line in retail (not visible / non-inline): GetCombatRadius, GetPos (16-aligned 0x4C73D0, called twice), GetMagnitudeAndNormalise,
  AccurateNormalise, GF...2DSquared, GetRHSet (virtual).
- Inlined in retail: C3DVector ops (operator*, +, -), C2DVector(x,y) ctor (or C3DVector::ToC2DVector under FABLE_RETAIL_C3DVECTOR_TO2D),
  CRightHandedSet::GetUp, GetPhysicsTC, PBaseDef deref, dot product, fabs (`fabs` intrinsic -> `fabs` insn).
- Same TU neighbours (CThingPhysical rows 0x006A4B70..0x006A5D90; GetFirstCollisionBetweenPoints 0x006A5460 directly precedes) -
  they call nothing that is inlined here.
- C3DVector opt-ins of interest: FABLE_RETAIL_C3DVECTOR (base), _GETMAGNITUDEANDNORMALISE, _TO2D, _SUBTRACT_XY_GROUP / _ADD_XY_GROUP /
  _SCALE_X_PAREN (paren grouping variants of -,+,* that change x87 order). No STLport here.

## 6. Siblings (same TU, rebuild/src/compiled/00/6a/)
- CThingPhysical_GetFirstCollisionBetweenPoints_006a5460.cpp: same signature family; uses `C3DVector delta = to - from; float length =
  delta.GetMagnitudeAndNormalise(); if (!(length > 0.0001f)) return false;` - local struct model, `#include "fable_compiler.h"`,
  `#pragma intrinsic(sqrt)`, `GetPhysicsTC()` inline getter.
- CThingPhysical_GetFirstCollision_006a4fb0.cpp: shared-header template: defines FABLE_RETAIL_CCHARSTRING, _CCLASS, _DEF_CLASS_BASE,
  _CTCBASE, _CTHING, _CTHINGPHYSICAL, _CTCPHYSICSBASE, _C3DVECTOR, _C3DVECTOR_NORMALISE_SQUARE_HELPER, _GFFASTONEOVERSQRT_NEWTON_GROUPED,
  _CTHINGPHYSICAL_FIRSTCOLLISION; includes fable_string.h, engine/C3DVector.h, CThing.h, CThingPhysical.h, CTCPhysicsBase.h.
- CThingPhysical_IsInRange_006a4df0.cpp, CThingPhysical_PeekPos_006a4d00.cpp, CThingPhysical_IsThingAbleToComeBetweenCameraAndObservedThing_006a5ab0.cpp.
- No pragmas (no optimize("y",off)) in these siblings.
- Need a new opt-in for this function's declaration in CThingPhysical.h (none exists: grep GetFirstCylinder = 0 hits) - for a row,
  declare via a new opt-in or the author's coordinator note.

## 7. Old draft model errors vs disassembly
- Callee spellings wrong (GetUp/GetPosition/GetLength/Normalise/GetClosestPointOnLineSegment with float*), see table 3.
- `up` is GetRHSet().GetUp() copied (C3DVector up = ...) - draft gets it right as a copy but via a fake virtual.
- `delta.GetLength()` const vs real non-const GetMagnitudeAndNormalise: draft semantics wrong though bytes identical.
- Draft's flow/compare order matches retail: 2D test `!(dy*dy+dx*dx > 0.0001f)` (dx=from.X-to.X, dy=from.Y-to.Y; retail computes x then y,
  squares y first: `fld st0 ; fmul st1` on dy then dx), three tests on t*length/(t-1)*length/distSq, hitPos, offset, axis normalise,
  fabs dot (Z,Y,X order), hitNormal = offset - axis*along built in a temp then copied to hitNormal, AccurateNormalise(hitNormal),
  a second GetPos() with discarded result, then hit.Z < top.Z and hit.Z > bottom.Z.
- Residual 849v818 is frame layout (0x78 vs 0x58): retail shares slots between up / from2D / axis and between upScaled / top, and
  between to2D / offset and bottom2D / hitNormal temp. Suggests: up and the 2D points as temporaries with limited lifetime
  (e.g. `C3DVector up = ...` in a block that ends after top; 2D points as unnamed `C2DVector(from.X, from.Y)` / ToC2DVector()
  args), `top` assigned into the slot of `up*MeshHeightOffset` (e.g. bottom = up*off; bottom += GetPos()? no - retail stores up*off at
  0x4c then bottom at 0x34 = temp + pos), hitNormal computed into a temp (`hitNormal = offset - axis*along;` already gives copy).

## 0x007D71E0

# DOSSIER 0x007D71E0 CTCTavernGameCardPairs::ShuffleCards (818 bytes)

## 1. Retail shape (xdis 007d71e0 0x342)
- thiscall (ecx -> ebx), plain `ret`, `sub esp,2Ch` + push ebx/ebp/esi/edi. NO ebp frame (ebp = &Pack, `lea ebp,[ebx+134h]`), NO SEH frame
  (CDefPointer dtor without EH state) -- same as the base override 0x008F10B0 ("no SEH frame. Flags: none").
- Speed-optimised (loop heads aligned: `lea ebx,[ebx]` 0x7D726A, `lea esp,[esp]` 0x7D734C / 0x7D7429). No optimize("s").
- Frame (F = esp+0x10 after pushes): def CDefPointer F+4 (zeroed `mov [esp+18h],edi` before push, 0x7D71F0); F+8 = loop-1 byte offset i*4;
  F+0xC = loop-1 counter i / loop-2 byte offset j*4 / loop-3 strength-reduced `53-k` counter (starts 0x35, fild); F+0x10 = card temp
  (address taken: push_back/insert value) and later `first` of the inline shuffle; F+0x14..0x1C = pos (C3DVector); F+0x20 = GetPackPosition
  return temp; the __true_type tag byte at F+3 (`lea ecx,[esp+1Bh]` after 2 pushes, 0x7D7308).
- Flow: GetCardDef(&def) [call 8F0B20]; order = GetCardOrder() [vtbl +0xE4 = slot 57]; NumCards(+0x2FC) = def->CardOrders[order].Offset.size()
  (0x48 records `lea eax,[eax+eax*8]`*8, Offset vector at +0x3C, C2DVector `sar 3`); std::random_shuffle(Pack.begin(), Pack.end()) [call 7D7860
  fastcall ecx/edx];
  Loop1 `for (i=0; i<NumCards/2; )`: it = begin advanced by i with `if (i>0) it += i` (test/jle/add, 0x7D7256); search [begin,it) for a card with
  same Value(+8) as *it (*it and its Value hoisted into edx/esi before the scan); found -> card=*it (frame F+0x10), inline erase(it)
  (`next!=finish` -> memmove), inline push_back(card) (placement-new null test, overflow -> _M_insert_overflow 7D5D60 with (finish, card, tag, 1, true));
  continue WITHOUT ++i (jmp 0x7D7292 re-tests); not found -> ++i (and byte offset +=4).
  Loop2 `for (j=NumCards/2; j<NumCards; ++j)`: it = begin (+j*4 if >0); value = begin[j - NumCards/2]->Value; scan from it: if (*it)->Value==value
  skip; else advance p until match (do-while `mov edx,[eax+4]; add eax,4; cmp [edx+8],ecx; jne`); if p!=it: card=*p -> F+0x10, inline erase(p)
  (`add dword [ebp+4],-4`), insert(it, card) [call 7D78A0, ret 8].
  Inline random_shuffle(begin, begin + NumCards (if NumCards>0)) -- SAME algorithm as the out-of-line 7D7860, here expanded (rand 0xBFEB16, idiv).
  Loop3 `for (unsigned k=0; k < def->CardPack.size() (/0x44 via imul 78787879h; sar 5); ++k)` def re-read from frame every pass (address escaped
  through the out-of-line GetCardDef): pos(0,0,(53-k)*CardSeparation(+5C)+TableSeparation(+60)); pos += GetPackPosition() [call 8F0B80, hidden ret];
  card = Pack[k]; HideMesh(card->Thing) [5E9B10 fastcall]; PositionMesh(card->Thing, GamePositionPos(+108), GamePositionRHSet(+F0), pos, .5f, .5f)
  [5EABC0 fastcall]; card->Move(pos, true) [8F0660]; card->FinalRotation(+10)=0.5f; card->FaceDown(+25)=true.
  Def release at end WITH null test: `test ecx,ecx; je; mov esi,[ecx+4]; lea eax,[ecx+4]; dec esi; mov edx,esi; test edx,edx; mov [eax],esi; jne; call [vtbl+4]`
  = the FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF shape (CDefPointer.h:110, count read through a reference: `mov ebx,[eax+4]; add eax,4; dec ebx; mov [eax],ebx`).

## 2. Identity (name proven, 3 sources)
- Ego_r.pdb (egor_pdb_names.tsv / egor_signatures.tsv): `?ShuffleCards@CTCTavernGameCardPairs@@UAEXXZ` @ ego 0x007D4630, between NewRound 0x7D42E0
  and the ctor 0x7D4A50 -- same order as retail (NewRound 0x7D70E0, ShuffleCards 0x7D71E0, ctor 0x7D7520).
- Vtable: rtti-vtables.tsv CTCTavernGameCardPairs 0x0126F6FC slot 60 = 007D71E0 (xref: abs .rdata 0x0126F7EC = 0x126F6FC+0xF0); base
  CTCTavernGameCardBase::ShuffleCards 0x008F10B0 is slot 60 of 0x01288294 (landed row header says Pairs overrides it with 0x007D71E0).
- naming_stragglers/Fable.exe.signatures.tsv: 007d71e0 CTCTavernGameCardPairs::ShuffleCards 818. Manifest label is CORRECT.
- Signature: `virtual void ShuffleCards();` (public virtual, UAEXXZ), no params, void. Only caller: the vtable (no direct calls).

## 3. Callees (landed file / landed name / PDB name) -- draft spelling problems flagged
- 0x008F0B20 rebuild/src/compiled/00/8f/CTCTavernGameCardBase_GetCardDef_008f0b20.cpp; PDB ?GetCardDef@CTCTavernGameCardBase@@QAEXPAV?$CDefPointer@$$CBVCTavernGameCardBaseDef@@@@@Z
  (takes CDefPointer<const CTavernGameCardBaseDef>*). Draft: local `void GetCardDef(CDefPointer*)` on a local struct -- use header.
- vtbl+0xE4 slot 57: PDB/header `virtual long GetCardOrder();` (card_sorting_types.h:65; landed Pairs override 0x007D5F90). DRAFT WRONG: `GetLevel`.
- 0x007D7860 rebuild/src/compiled/00/7d/std_randomshuffle_007d7860.cpp: std::random_shuffle<CCard**> __fastcall (/Gr row). Draft WRONG: global `random_shuffle`.
  Header opt-in: FABLE_RETAIL_STLP_RANDOM_SHUFFLE_2ARG (fable_containers.h:1195, declaration only, __fastcall).
- 0x00BFEAE6 global_memmove_00bfeae6.cpp (memmove, from inline erase / __copy_trivial).
- 0x007D5D60 stdvector_Minsertoverflow_007d5d60.cpp: vector<CCard*>::_M_insert_overflow(CCard**, CCard* const&, const __true_type&, size_type, bool) ret 14h.
  Draft: `const bool& trivial` param -- WRONG type (const __true_type&); header vector declares it protected (fable_containers.h:588).
- 0x007D78A0 STLvectorCCard_insert_007d78a0.cpp: vector<CCard*>::insert(iterator, const CCard*&) ret 8 (only caller is this row). Header opt-in
  FABLE_RETAIL_STLP_VECTOR_INSERT_OUTOFLINE (fable_containers.h:507, declaration only) gives the call.
- 0x00BFEB16 global_rand_00bfeb16.cpp (rand, inline shuffle).
- 0x008F0B80 CTCTavernGameCardBase_GetPackPosition_008f0b80.cpp; PDB ?GetPackPosition@CTCTavernGameCardBase@@QAE?AVC3DVector@@XZ.
- 0x005E9B10 CTCTavernGame_HideMesh_005e9b10.cpp; PDB ?HideMesh@CTCTavernGame@@SIXPAVCThing@@@Z (static __fastcall, CThing*). Draft: free
  `HideMesh(CThingObject*)` -- WRONG scope; use FABLE_RETAIL_CTCTAVERNGAME_HIDEMESH (CTCTavernGame.h:107).
- 0x005EABC0 (not landed); PDB ?PositionMesh@CTCTavernGame@@SIXPAVCThing@@ABVC3DVector@@ABVCRightHandedSet@@1MM@Z. Draft: free function -- use
  FABLE_RETAIL_CTCTAVERNGAME_POSITIONMESH (CTCTavernGame.h:101).
- 0x008F0660 CCard_Move_008f0660.cpp; PDB ?Move@CCard@@QAEXABVC3DVector@@_N@Z (header card_sorting_types.h:11).
- def vtbl+4 (slot 1, Delete) via CDefPointer release.

## 4. Members (retail offsets; headers)
- this (CTCTavernGameCardBase, engine/card_sorting_types.h, sizeof 0x318 retail): GamePositionRHSet +0xF0 (CRightHandedSet only with
  FABLE_RETAIL_CTCTAVERNGAMECARDBASE_RHSET, else opaque char[24]), GamePositionPos +0x108 (C3DVector), Pack +0x134 (std::vector<CCard*>),
  NumCards +0x2FC (long, protected). No shared header for CTCTavernGameCardPairs: every landed Pairs row declares
  `class CTCTavernGameCardPairs : public CTCTavernGameCardBase { public: virtual void ShuffleCards(); };` locally (e.g. 0x007D6260).
- CTavernGameCardBaseDef (engine/CTavernGameCardBaseDef.h, needs FABLE_RETAIL_CTAVERNGAMECARDBASEDEF): CardSeparation +0x5C float,
  TableSeparation +0x60 float, CardOrders +0x6C vector<CCardPositionsDef> -- OPAQUE `unsigned char[0xc]`, NO typed opt-in exists (landed NewRound
  0x7D70E0 / CardSorting NewRound 0x7D5A00 / Blackjack GetPosition 0x7DA760 all model it locally) -> author must either add an opt-in
  (e.g. FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_CARDORDERS, std::vector<CCardPositionsDef>, include engine/CCardPositionsDef.h) or reach it via a cast;
  CardPack +0x78 std::vector<CCardDef> with FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_CARDPACK (include engine/CCardDef.h first; sizeof CCardDef 0x44).
  Refcount at def+4 (CDefPointeeBase IVRefCount), Delete slot 1.
- CCardPositionsDef (engine/CCardPositionsDef.h): Offset +0x3C std::vector<C2DVector>; record size 0x48.
- CCard (card_sorting_types.h): Thing +0x00 (CThingObject*), Value +0x08 (long), FinalRotation +0x10 (float), FaceDown +0x25 (bool).
- Draft's LOCAL models that have shared headers / real names: C3DVector (engine/C3DVector.h), CRightHandedSet (engine/CRightHandedSet.h), CCard
  (Mesh->Thing, Scale->FinalRotation), CTavernGameCardBaseDef (PackCardSpacing->CardSeparation, PackBaseHeight->TableSeparation,
  Levels->CardOrders, PackStart/PackFinish->CardPack), CCardPairLevel->CCardPositionsDef (CardsStart/Finish->Offset), CDefPointer
  (engine/CDefPointer.h), CVector->std::vector (stlport/fable_containers.h), CTCTavernGameCardPairs (57 dummy virtuals; use base header).

## 5. Callee-body visibility
- Same TU (CardPairs file, COMDATs at 0x7D7860 / 0x7D78A0 / 0x7D5D60 inside the CardPairs address block): std::random_shuffle body VISIBLE
  (2nd call is expanded inline, 1st is a call to 7D7860 -- open question: VC7.1 inline budget vs. a different spelling for one site);
  vector insert body (called out of line); _M_insert_overflow (out of line); erase and push_back expanded inline (default header bodies).
- NOT visible (other TU, CardBase file 0x8F....): GetCardDef 0x8F0B20, GetPackPosition 0x8F0B80, CCard::Move 0x8F0660 -- all called. Contrast:
  the base ShuffleCards 0x8F10B0 has them in its own TU and defines them there; DO NOT copy that.
- CTCTavernGame::HideMesh/PositionMesh: declaration-only opt-ins.
- STLport: FABLE_RETAIL_STLP_RANDOM_SHUFFLE_2ARG is declaration-only -> it would make BOTH sites calls; for the inline one the author needs the
  body (e.g. local inline std::random_shuffle body as in the landed 0x7D7860 row, or a separate non-opt-in path).

## 6. Sibling landed rows
- rebuild/src/compiled/00/8f/CTCTavernGameCardBase_ShuffleCards_008f10b0.cpp (base override, Flags none): FABLE_RETAIL_CCHARSTRING, CWIDESTRING,
  CDEFSTRINGTABLE, DEF_CLASS_BASE, CDEFPOINTER_RELEASE_POINTEE, CTCTAVERNGAME, CTCTAVERNGAME_GETDEF_OUTOFLINE, CTCTAVERNGAME_POSITIONMESH,
  CTCTAVERNGAME_HIDEMESH, CTAVERNGAMECARDBASEDEF(_CARDPACK,_TYPENAME), CRESOURCELIST, CDEFINITIONMANAGER, CDEFCLASSBASE_GETSUBDEF_NOINLINE,
  CGAMECOMPONENT, CCLASSFACTORY, CTHINGPHYSICAL, CTHINGGAMEOBJECT, CMAINGAMECOMPONENT, SCOPED_PTR, C3DVECTOR_PACKED, CCARD_POSITIONS,
  CTCTAVERNGAMECARDBASE_RHSET, STLP_RANDOM_SHUFFLE_2ARG; includes stlport/fable_containers.h first, fable_game.h, CThingObject, CRightHandedSet,
  CCardDef, CTavernGameDef, CTavernGameCardBaseDef, card_sorting_types.h. Loop3 source is near-identical (uses LocalPos stores instead of Move).
- rebuild/src/compiled/00/7d/CTCTavernGameCardPairs_CardsFinishedMoving_007d6260.cpp: header-based Pairs model (local class on CTCTavernGameCardBase).
- rebuild/src/compiled/00/7d/CTCTavernGameCardPairs_NewRound_007d70e0.cpp (neighbour, same TU): local models, `NumCards = def->CardOrders[order].Offset.size()`
  with order fetched into a local first; inline push_back with null test.
- rebuild/src/compiled/00/7d/CTCTavernGameCardSorting_NewRound_007d5a00.cpp (calls ShuffleCards; CardOrders local model).
- rebuild/src/compiled/00/7d/CTCTavernGameCardPairs_GetCardOrder_007d5f90.cpp / OnSerialise_007d6830.cpp: FABLE_RETAIL_CTCTAVERNGAME + card_sorting_types.h.

## 7. Old draft model errors (residue DIFFER 705v818)
- Advance(p,n) `if(n>0) p+=n` as a template: retail strength-reduces i*4 / j*4 into frame words (F+8, F+0xC) -> the original is likely a counting
  loop `CCard** it = Pack.begin(); for (long n = 0; n < i; ++n) ++it;` (VC7.1 folds to `if (i>0) it += i` with an induction byte offset), or
  `begin() + i` behind an iterator helper; try the counting loop first.
- Def release null test elided: draft's CDefPointer modelled locally; retail keeps `test ecx,ecx` and the COUNTREF count-through-reference shape
  -> use engine/CDefPointer.h + FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF (base row used _POINTEE because its def was register-held).
- Loop1 in retail hoists `*it` (edx) and `(*it)->Value` (esi) before the scan and reuses edx as the pushed card (`mov esi,edx` 0x7D72CC):
  spell `CCard* card = *it;` BEFORE the scan loop, compare `(*p)->Value == card->Value`.
- Loop2: first test is on *it itself then do-while from it+1 (draft `while ((*p)->Value != value) ++p;` is right form; keep).
- Wrong names: GetLevel, random_shuffle global, Mesh/Scale/PackCardSpacing/PackBaseHeight/Levels/CardsStart/PackStart, free HideMesh/PositionMesh,
  `const bool&` overflow tag.
- Draft's push_back passes an uninitialised `bool trivial`: retail passes the address of a __true_type tag byte (F+3); use header _IsPODType().

## 0x00E83230

# DOSSIER 0x00E83230 (820 bytes, 0xE83230..0xE83563 + int3 pad)

## 1. Retail shape (xdis 00e83230 0x344)
- __thiscall, no stack args, plain `ret` (two epilogues: 0xE83340 and 0xE83563). NOT `ret 4`.
- No frame pointer (`sub esp,0x14 ; push ebx,ebp,esi,edi`), ebp = this. No SEH prologue -> no /EHsc (CCharString temps
  are destroyed without EH states). Speed-optimised (no size idioms).
- Frame: 0x14 local bytes: [esp+0x14] (post-push) = shared CCharString slot (GetDataString return temp AND the
  "VILLAGE_*" name temp), [esp+0x18] = CScriptThing return temp of GetNearestWithDefName (dtor 0x4AA840 at the common
  tail 0xE83327), [esp+0x13] byte = compare result for compares 1..3; compare 4 keeps the result in bl.
- ebx = &Me (this+8) held across compares 1..3 (`lea ebx,[ebp+8]`); vtable re-read each time (`mov eax,[ebx]`).
- Each `Me.GetDataString() == "X"`: null PStringData -> inline strcmp(text,"") as `repe cmpsb` len strlen("X")+1
  against "" @0x0122D70E; else `Compare(PData,text)` 0x411570 + `neg/sbb/inc` -> exactly the
  FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ + FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE bodies. Temp dtor right after.
- Branch order / values: "BowerstoneSlums"(0x12CB55C)->1 + "VILLAGE_BOWERSTONE_SLUMS"(0x12C5778);
  "KnotholeGlade"(0x12C513C)->4 + 0x12C50AC; "Oakvale"(0x12EB0B8)->2 + 0x12C544C; "BanditCamp"(0x12EB0AC)->3 + 0x12C53BC;
  else ->4. Each arm: `if (IsActiveThreadTerminating()) return;` then store region (+0x48) BEFORE CCharString ctor call.
- Last compare: IsActiveThreadTerminating is called ONCE before `test bl` (0xE834EE) -> both the BanditCamp arm and
  the final else begin with the same check and VC hoisted it; final else writes 4 via `mov ecx,[ebp+0x14]`.
- ARM 1 IS DIFFERENT: the CScriptThing assignment to Village is INLINED (CCountedPointer ShareData body, reads
  Info [eax+8]->esi then Data [eax+4]->edi, target ebp=parent+0xA0, release via [info+4](info+8) + operator delete
  0xBFE9BC, store Data then Info, inc RefCount). Arms 2-4 CALL operator= 0x008AB980 (ecx=parent+0x9C).
  -> classic VC7.1 inline-budget pattern: the implicit (compiler-generated) CScriptThing::operator= is inlined at the
  first site and the COMDAT is called at later ones. Arm 1 also caches Game in esi (`mov esi,ecx`, no reload),
  arms 2-4 reload `mov ecx,[ebp+4]` after GetHero. Arm 1 loads GetHero's vtable from ecx into eax and GetNearest's
  into edi; arms 2-4 load GetNearest's vtable into esi first (`mov edx,ecx; mov esi,[edx]; mov edx,[ecx]`).
- All four assign arms jump to the common tail 0xE83327 (~CScriptThing temp, ~CCharString name, epilogue).

## 2. Identity -- manifest label `_global::HandleVillageSelection` is WRONG (forge autoname, propagated)
- Real: `NScript::CV_FisticuffsClubScript::CFisticuffsClubRing::SetRelevantRegion()`, private, non-virtual, void,
  non-const. Mangled leaf: `?SetRelevantRegion@CFisticuffsClubRing@CV_FisticuffsClubScript@NScript@@AAEXXZ`.
- Evidence 1: only caller = 0xE8299C inside landed `CFisticuffsClubRing::Main` 0x00E82950 (xref.py), called with
  ecx=this right after the IsActiveThreadTerminating check and before ParentClass->InitialiseVariables 0xE82650.
- Evidence 2: Ego_r.pdb (ghidra_out/egor_pdb_names.tsv) CFisticuffsClubRing has exactly Alloc, SetRelevantRegion,
  OnPredicateFail, Main (+ctor/dtor/GetParentScript in FableWin); FableWin.exe.signatures.tsv orders them
  Alloc 039f46b0, ctor, GetParentScript, OnPredicateFail, Main 039f4800, SetRelevantRegion 039f5e80 (1357 bytes debug)
  -- identical to retail order (Alloc E82870, GetParentScript E82910, sdd E82920, OnPredicateFail E82940, Main E82950,
  this E83230). The only unaccounted PDB member is SetRelevantRegion.
- Evidence 3 (semantics): writes CV_FisticuffsClubScript::RelevantRegion (enum REGION_BOWERSTONE=1, OAKVALE=2,
  BANDITCAMP=3, KNOTHOLE=4; rebuild/include/engine/NScript_CV_FisticuffsClubScript.h) and Village.
- NAME_FIXES: landed Main 0x00E82950 declares this as `void SetUpRing(); // 0x00E83230` (made-up) -> respell
  SetRelevantRegion. ghidra_out/labels_chunk283_corrections_2026-10-22.tsv / labels_manifest_sync label is wrong too.
- Landed sibling filenames spell the class `CVFisticuffsClubScript` (filename mangling only); PDB is CV_FisticuffsClubScript.

## 3. Callees
| target | landed row | landed name | PDB name | draft spelling |
|---|---|---|---|---|
| 0x00F35B30 | 00/f3/CEntityScriptBase_IsActiveThreadTerminating_00f35b30.cpp | CEntityScriptBase::IsActiveThreadTerminating() const | ?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ | `IsInterrupted()` non-const -- WRONG |
| CScriptThing vslot 3 (+0xC) | header engine/CScriptThing.h (0x004AA900) | GetDataString() const -> CCharString | same | ok |
| 0x00411570 | 00/41/CBasicStringchar_Compare_00411570.cpp | CBasicString<char>::Compare | ?Compare@?$CBasicString@D@@KIJPBD0@Z (protected static fastcall) | free `Compare` -- wrong scope |
| 0x0099EAE0 | 00/99/CCharString_CCharString_0099eae0.cpp | ~CCharString | ??1CCharString | ok |
| 0x0099EBF0 | 00/99/CCharString_Constructor_0099ebf0.cpp | CCharString(const char*, long=-1) | ok | ok |
| Game vslot 70 (+0x118) | engine/CGameScriptInterfaceBase.h:93 | CScriptThing& GetHero() (0x00891CA0) | ok | ok |
| Game vslot 81 (+0x144) | engine/CGameScriptInterfaceBase.h:104 | CScriptThing GetNearestWithDefName(const CScriptThing&, const CCharString&) const (0x0089A880) | ok | ok |
| 0x008AB980 | 00/8a/CScriptThing_operator_008ab980.cpp | CScriptThing::operator= | ??4CScriptThing@@ | ok, but it is the IMPLICIT op= COMDAT |
| 0x004AA840 | 00/4a/CScriptThing_CScriptThing_004aa840.cpp | ~CScriptThing | ??1CScriptThing@@UAE@XZ | ok |
| 0x00BFE9BC | 00/bf/global_operatordelete_00bfe9bc.cpp | operator delete (MSVCR71) | - | ok (inside inlined op=) |

## 4. Members
- this (CFisticuffsClubRing : CEntityScriptBase, Ego_r 28 bytes): Game +4 (engine/CEntityScriptBase.h; pointer, or a
  protected reference with FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS), Me +8 (CScriptThing), ParentClass +0x14
  (CV_FisticuffsClubScript*), PMasterData +0x18. NO shared header defines CFisticuffsClubRing (only forward-declared in
  engine/NScript_CV_FisticuffsClubScript.h); landed Main defines it locally -> define it as a nested class the same
  way engine/CV_FisticuffsClubScript.h defines CFisticuffsMember.
- Parent (engine/NScript_CV_FisticuffsClubScript.h, retail = PDB-0x14; Ego_r 220 = retail 0xC8): RelevantRegion
  (anonymous enum, PDB +92) = retail +0x48; Village (CScriptThing, PDB +176) = retail +0x9C, its PImp Data +0xA0 /
  Info +0xA4. Header needs FABLE_RETAIL_NSCRIPT (+ FABLE_RETAIL_CLEVELSCRIPTBASE as same-TU siblings use).
- CScriptThing (engine/CScriptThing.h, FABLE_RETAIL_NSCRIPT): FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT leaves op=
  implicit (TU expands/calls COMDAT 0x8AB980). CCharString (fable_string.h): FABLE_RETAIL_CCHARSTRING,
  _FORCEINLINE_TEXT_EQ (line 419: null -> strcmp(text,"")), FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE (line 86).
- Locally modelled in the old draft though shared headers exist: CCharString/CCharStringData, CCPPointerInfo /
  CCountedPointerWords (-> engine/CCountedPointer.h), CScriptThing, CGameScriptInterface_Calls
  (-> engine/CGameScriptInterfaceBase.h), CVillageQuest (-> NScript_CV_FisticuffsClubScript.h),
  CVillageSelectionScript (-> CEntityScriptBase.h + nested CFisticuffsClubRing).

## 5. Visibility
- Same TU = CV_FisticuffsClubScript source (retail 0xE80xxx..0xE8Dxxx). SetRelevantRegion is emitted AFTER Main and
  Main calls it out of line -> defined after Main in source; it inlines nothing of its own class.
- Inline in retail: CCharString::operator==(const char*) and CBasicString text ==; implicit CScriptThing::operator=
  (first site inlined, later sites call COMDAT 0x8AB980). Out of line: CCharString ctor/dtor, ~CScriptThing,
  IsActiveThreadTerminating, Compare. No STLport.
- Opt-ins: FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT, FABLE_RETAIL_CCHARSTRING(_FORCEINLINE_TEXT_EQ),
  FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE, FABLE_RETAIL_NSCRIPT, FABLE_RETAIL_CLEVELSCRIPTBASE.

## 6. Siblings
- 00/e8/NScriptCVFisticuffsClubScriptCFisticuffsMember_Main_00e84b80.cpp and ..CTyler_Main_00e87030.cpp (SAME TU):
  #define block lines 11-24: FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT, _CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR,
  _NSCRIPT, _CLEVELSCRIPTBASE, _BASE_OBJECT, _CCHARSTRING, _CCHARSTRING_CONCAT_STRING, _CCHARSTRING_FORCEINLINE_EQ,
  _CCHARSTRING_FORCEINLINE_TEXT_EQ, _CBASICSTRING_TEXT_EQ_INLINE, _DEF_CLASS_BASE, _CDEFSTRINGTABLE,
  _SUSPENDABLE_PROCESS, _STLP_MAP; includes engine/CV_FisticuffsClubScript.h. No // Flags, no optimize pragmas.
- 00/e8/NScriptCVFisticuffsClubScriptCFisticuffsClubRing_Main_00e82950.cpp: the caller; local CFisticuffsClubRing
  model, `#pragma inline_depth(0)` around `return` after IsActiveThreadTerminating (inline-depth revisit flag).
- 00/e0/CTraderScript_HandleTrader_00e0a510.cpp (matched chunk 283, same shape: GetDataString()=="X" chain with
  IsActiveThreadTerminating per arm; local CCharString FORCEINLINE operator== + shared CScriptThing.h).
- 00/e8/NScriptCVFisticuffsClubScriptCFisticuffsClubRing_GetParentScript_00e82910.cpp (shared-header idiom).

## 7. Old draft model errors
- Wrong name/class (HandleVillageSelection on a local class); IsInterrupted instead of const IsActiveThreadTerminating.
- Arm 1 hand-written `Village.PImp = (...).PImp` with local counted-pointer struct; retail is the same
  `ParentClass->Village = Game->GetNearestWithDefName(Game->GetHero(), "VILLAGE_BOWERSTONE_SLUMS");` as the other arms,
  with the inline-vs-call split coming from the compiler (implicit op= via IMPLICIT_ASSIGNMENT). Draft's named
  `CCharString name` + `game` local in arm 1 are probably invented to force the register shape (818v820 remained).
- Draft header says 832 bytes; real 820. Draft uses `ParentClass->Selection` -> real `RelevantRegion` enum
  (REGION_BOWERSTONE/REGION_KNOTHOLE/REGION_OAKVALE/REGION_BANDITCAMP, else REGION_KNOTHOLE).
- Draft else-arm and BanditCamp arm both keep their own IsActiveThreadTerminating check -- correct (VC hoists it).

## 0x00B4AC10

# DOSSIER 0x00B4AC10 -- CEnginePrimitiveRenderer2D::CEnginePrimitiveRenderer2D()  (826 bytes, body 0x33a)

## 1. Retail shape (xdis 00b4ac10 0x34a)
- __thiscall, `this` -> esi, plain `ret` (0x00B4AF49), returns this in eax. Frame `sub esp,8` (one 8-byte CCountedPointer temp slot at [esp+0x10]),
  pushes ebx/ebp/esi/edi. NO `push ebp; mov ebp,esp` (frame pointer omitted -> no optimize("y",off)). NO SEH prologue even though it
  has new-expressions and members with dtors -> TU built without EH (/EHsc absent), like siblings (`// Flags: none`).
- edi = 0 hoisted (xor edi,edi) and used for all zero stores / null compares; ebx = 1.0f (0x3f800000) later.
- Order: CEngineComponent::CEngineComponent() 0xB59710; vptr = 0x012A2308; zero +0x0c..+0x44 (PrimitiveManagers 3 dwords, five
  CCountedPointers Data/Info, First2DPrim, First2DDisplacementPrim) -- i.e. member default ctors, interleaved with the static guard test.
- Function-local static guard: bit 1 of 0x014377FC; static object 0x014377F0..F8 (12 bytes = CArray<CEngineScene*>) zeroed, atexit(0x0122AD60).
- Five `new` + ctor(&empty_scene) + Reset, in this ORDER (member offsets): +0x18 (new 0x318, ctor 0xBA1AC0, Reset 0xB4B360),
  +0x20 (new 0x690, 0xBD1F30, Reset 0xB4B410), +0x28 (new 0x38, 0xBAD040, Reset 0xB4B4C0), +0x38 (new 0x28, base ctor 0xBBBF40 then
  INLINE vptr store 0x012A22D0, Reset 0xB4B620), +0x30 (new 0x38, 0xBE1E20, Reset 0xB4B570). Null new -> `xor eax,eax` (standard new-expr).
- Five AddPrimitiveManager(0xB4ABB0) calls, same order: +0x18, +0x20, +0x28, +0x38, +0x30. Each: Data/Info loaded, stored to temp
  [esp+0x10]/[esp+0x14], `if (Info) ++Info->RefCount` (inc [ebp]), call; after the call retail RE-USES the Info register (ebp/ebx), no reload,
  release `dec [ebp]; jne` (flags from dec, no reload/cmp), `mov ecx,[ebp+8]; call [ebp+4]; push ebp; call delete 0xBFE9BC`. No Data=0/Info=0
  dead stores on the temp.
- FOV: `FOV2D = engineDef->FOV_2D(+0x5c) * 0.0027777778f(0x1238E00 = 1/360) * 6.2831855f(0x128F608)`; `fst [esi+0x48]; fcomp 0.0f(0x12A22AC);
  test ah,0x41; jp` -> `if (FOV2D <= 0.0f) FOV2D = 0.78539819f (0x3f490fdb, pi/4)`.
- LightVector (+0x7c) = (1,1,1) then call 0xA14440 (AccurateNormalise, ecx=&LightVector, result ignored).
- `fld 0.1f (0x12A22C0)` loaded BEFORE the colour stores (depth kept in st0 across them). Colours: +0x4c..0x58 = .5,.5,.5,1; +0x5c..0x68 = .5,.5,.5,1;
  +0x6c..0x74 = 0 (edi), +0x78 = 1.0.
- Loop i=0..0x52 (eax, also spilled to [esp+0x10] for `fild`): if (i==0x48) { Layer2DDistances[0x48](+0x1a8 direct) = d + 0.405f(0x139A3C4);
  d += 0.81f(0x139A3BC) } else { Layer2DDistances[i] = d + 0.00054878066f(0x139A3D0); d += 0.0010975613f(0x139A3C0) };
  Enable2DLayer[i](+0x320) = true; Layer2DSortDistances[i](+0x1d4) = (float)i (fild of i). `cmp eax,0x53; jl`. Final `fstp st(0)`.
  Note: the constants are half-width/width pairs (0.405 = 0.81/2; 0.000548780 = 0.001097561/2) -- probably source `d + w*0.5f` folded, or
  named consts; the duplicated store in each branch suggests `float w = (i==0x48) ? big : small` was NOT used (two separate stores). Draft form is right.

## 2. Name / signature (two sources)
- `??0CEnginePrimitiveRenderer2D@@QAE@XZ`: Ego_r.pdb 0x009EB690 (ghidra_out/egor_pdb_names.tsv) and FableWin.pdb 0x02D9C4D0
  (naming_stragglers/FableWin.exe.signatures.tsv, _fablewin_api.tsv). Public, no args. Manifest label is CORRECT.
- Header comment rebuild/include/engine/CEnginePrimitiveRenderer2D.h retail branch already names it at 0x00B4AC10; vtable 0x012A2308
  (rtti-vtables.tsv: CEnginePrimitiveRenderer2D). Caller: CEngine::Initialise new(0x374) site 0x00B26736 (xref).
- Ego_r ordering: ... ~CEnginePrimitiveRenderer2D 0x9EB4D0, AddPrimitiveManager 0x9EB610, ctor 0x9EB690 -> AddPrimitiveManager is
  DEFINED EARLIER IN THE SAME TU (retail too: 0xB4ABB0 immediately precedes 0xB4AC10).
- Function-local static name: `empty_scene` (Ego_r 0x0101DC40 / FableWin 0x04012140 `dynamic atexit destructor for 'empty_scene'`),
  type CArray<CEngineScene*> (the manager ctors take `CArray<CEngineScene*>&`, PDB leaves below). Landed _$E2: rebuild/src/compiled/01/22/global_E2_0122ad60.cpp
  (`static CArray<CEngineScene*> empty_scene;` at top scope, ?1 / `2' scope). Its comment wrongly says 0xBA1AC0 is AnimatedMeshes (it is StaticMeshes).

## 3. Callees (landed row / landed name / PDB name)
| addr | landed file | landed name | PDB | draft spelling |
|---|---|---|---|---|
| 0xB59710 | 00/b5/CEngineComponent_CEngineComponent_00b59710.cpp | CEngineComponent::CEngineComponent() | same | ok |
| 0x4012BC | 00/40/global_atexit_004012bc.cpp | atexit | CRT | implicit (static) |
| 0xBFEA1A / 0xBFE9BC | global_operatornew / operatordelete | operator new / delete | | ok |
| 0xBA1AC0 | 00/ba/CEnginePrimitiveManager2DStaticMeshes_..._00ba1ac0.cpp | CEnginePrimitiveManager2DStaticMeshes(CArray<CEngineScene*>&) | ??0CEnginePrimitiveManager2DStaticMeshes@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z (vtable 0x012A5160) | WRONG: draft "CEnginePrimitiveManager2DAnimatedMeshes" |
| 0xBD1F30 | 00/bd/CEnginePrimitiveManager2DAnimatedMeshes_..._00bd1f30.cpp | CEnginePrimitiveManager2DAnimatedMeshes(CArray<CEngineScene*>&) | ??0CEnginePrimitiveManager2DAnimatedMeshes@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z (vtable 0x012A7964) | WRONG: draft "...2DText" |
| 0xBAD040 | 00/ba/CEnginePrimitiveManager2DSprites_..._00bad040.cpp | CEnginePrimitiveManager2DSprites(CEngineSceneArray*) | ??0CEnginePrimitiveManager2DSprites@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z | name ok, arg type: reference to CArray<CEngineScene*> |
| 0xBBBF40 | 00/bb/CEnginePrimitiveManagerParticleGroup_..._00bbbf40.cpp | CEnginePrimitiveManagerParticleGroup(CEngine*) | ??0CEnginePrimitiveManagerParticleGroup@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z (Ego_r 0xA71660) | WRONG: draft "...2DLinesBase"; landed param type CEngine* is also wrong vs PDB |
| inline | (out-of-line copy 0xB4B690, 25 bytes, manifest-gap: `mov eax,[esp+4]; push esi; mov esi,ecx; call 0xBBBF40; mov [esi],0x012A22D0; ret 4`) | CEnginePrimitiveManager2DParticleGroup ctor (derived of CEnginePrimitiveManagerParticleGroup, vtable 0x012A22D0 per rtti-vtables) | Ego_r has the class (Render2DPrimitive etc.), no ??0 symbol -> inline/implicit ctor | WRONG: draft "...2DLines" |
| 0xBE1E20 | 00/be/CEnginePrimitive2DViewportManager_..._00be1e20.cpp | CEnginePrimitive2DViewportManager(CArray<CEngineScene*>&) | ??0CEnginePrimitive2DViewportManager@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z | WRONG: draft "...2DMeshes" |
| 0xB4B360/0xB4B410/0xB4B4C0/0xB4B620/0xB4B570 | 00/b4/CCountedPointer00b4b0X0_Reset_*.cpp (local stand-ins) | CCountedPointer<T>::Reset(T*) for T = StaticMeshes/AnimatedMeshes/Sprites/2DParticleGroup/2DViewportManager (deleters DeleteData 0xB4B020/30/40/60/50) | Ego_r Reset (out of line) | shared engine/CCountedPointer.h has Reset in-class; retail calls them out of line (COMDATs after the ctor) |
| 0xB4ABB0 | 00/b4/CEnginePrimitiveRenderer2D_AddPrimitiveManager_00b4abb0.cpp | AddPrimitiveManager(const CCountedPointer<CEnginePrimitiveManager>&) private | ?AddPrimitiveManager@CEnginePrimitiveRenderer2D@@AAEXABV?$CCountedPointer@VCEnginePrimitiveManager@@@@@Z | WRONG: draft param CCountedPointer<CEnginePrimitiveManager2DBase>; must be CEnginePrimitiveManager, private (AAE) |
| 0xA14440 | 00/a1/C3DVector_AccurateNormalise_00a14440.cpp | C3DVector& C3DVector::AccurateNormalise() | ?AccurateNormalise@C3DVector@@QAEAAV1@XZ | WRONG: draft "Normalise"; engine/C3DVector.h line ~365/440 declares AccurateNormalise |
| 0x0122AD60 | 01/22/global_E2_0122ad60.cpp | _$E2 (atexit dtor of empty_scene) | | draft "CPrimitiveManagerPool s_pool" -> must be `static CArray<CEngineScene*> empty_scene;` |
Global 0x01436E20: CEngineDef* (landed spellings vary: `g_pEngineDef_01436e20` (descriptive, const CEngineDef*), `g_pRenderSettings_01436e20`); no PDB symbol.
  Draft `g_pEngineOptions` is a third spelling -- use `g_pEngineDef_01436e20`.

## 4. Members (struct_layouts_egor.tsv CEnginePrimitiveRenderer2D, Ego_r sizeof 888; retail 0x374 because CArray is 12 bytes in retail
   (PDB 16) -- generated header already applies the retail offsets)
| off | PDB name | type | draft name |
|---|---|---|---|
| +0x00/+04/+08 | CEngineComponent base (vptr, GlobalPointerAddress, RenderEnabled/KeepAlive) | | local CEngineComponent with wrong member names |
| +0x0c | PrimitiveManagers | CArray<CCountedPointer<CEnginePrimitiveManager> > | Managers |
| +0x18 | StaticMesh2DManager | CCountedPointer<CEnginePrimitiveManager2DStaticMeshes> | AnimatedMeshes (wrong) |
| +0x20 | AnimatedMesh2DManager | CCountedPointer<CEnginePrimitiveManager2DAnimatedMeshes> | Text (wrong) |
| +0x28 | Sprite2DManager | CCountedPointer<CEnginePrimitiveManager2DSprites> | Sprites |
| +0x30 | Viewport2DManager | CCountedPointer<CEnginePrimitive2DViewportManager> | Meshes (wrong) |
| +0x38 | Particle2DManager | CCountedPointer<CEnginePrimitiveManager2DParticleGroup> | Lines (wrong) |
| +0x40 | First2DPrim | CEngineInternalPrimitiveBase* | Primitives |
| +0x44 | First2DDisplacementPrim | CEngineInternalPrimitiveBase* | OverlayPrimitives |
| +0x48 | FOV2D | float | HorizontalFOV |
| +0x4c | AmbientLightColour | CRGBFloatColour | ColourA |
| +0x5c | DiffuseLightColour | CRGBFloatColour | ColourB |
| +0x6c | BackLightColour | CRGBFloatColour | ColourC |
| +0x7c | LightVector | C3DVector | LightDirection |
| +0x88 | Layer2DDistances | float[83] | BandCentre |
| +0x1d4 | Layer2DSortDistances | float[83] | BandIndex |
| +0x320 | Enable2DLayer | bool[83] | BandUsed |
- CEngineDef (retail sizeof 0x80, Ego_r 0x84, -4 shift): PDB +0x60 FOV_2D -> retail +0x5c FOV_2D (float). engine/CEngineDef.h is opaque (`_pad_0x25`).
- Shared headers: engine/CEnginePrimitiveRenderer2D.h -- FABLE_RETAIL_CENGINEPRIMITIVERENDERER2D gives the polymorphic class over
  CEngineComponent but OWN MEMBERS ARE ONE OPAQUE `_pad_0xc[0x368]` (sizeof 0x374); only opt-in FABLE_RETAIL_CENGINEPRIMITIVERENDERER2D_LAYER2DTODISTANCE.
  Default branch = flattened generated layout with real names (but split `_Data/_Info`, `_R/_G..`). Header debt: no named-member retail branch;
  every landed sibling (dtor 0xB4AAA0, AddPrimitiveManager 0xB4ABB0) models locally. engine/CEngineComponent.h (FABLE_RETAIL_CENGINECOMPONENT,
  _PROTECTED), engine/CCountedPointer.h, engine/CArray.h, engine/C3DVector.h, engine/CRGBFloatColour.h,
  engine/CEnginePrimitiveManager2DParticleGroup.h (opt-in FABLE_RETAIL_CENGINEPRIMITIVEMANAGER2DPARTICLEGROUP_VIRTUAL_DTOR, flattened base),
  engine/CEnginePrimitive2DViewportManager.h, engine/CEnginePrimitiveManager2DSprites.h, engine/CEnginePrimitiveManager.h exist.
- Classes the old draft models LOCALLY that have shared headers: CCountedPointer (+SCountBlock = CCPPointerInfo), C3DVector, CRGBFloatColour,
  CEngineComponent, CArray (CManagerList / CPrimitiveManagerPool), CEnginePrimitiveRenderer2D, CEnginePrimitiveManager2DSprites,
  CEnginePrimitive2DViewportManager, CEnginePrimitiveManager2DParticleGroup, CEngineDef (as CEngineOptions).

## 5. Callee-body visibility
- AddPrimitiveManager 0xB4ABB0 = SAME TU, defined immediately before the ctor (retail and Ego_r order). The residue (Info kept in ebp across
  the call, `dec [ebp]; jne`, no reload, dead temp stores dropped) is exactly GOTCHAS.md:292 (wf7) -- "a called-but-visible ... keeps Data/Info
  in a register across later calls; declaration-only reloads". Previous attempts (hand286, parked_retry2_sec6) declared AddPrimitiveManager
  only. NEXT: put AddPrimitiveManager's full body (the landed 0xB4ABB0 source: STLport inline push_back + RegisterPrimitiveManager on
  g_pEnginePrimitiveTypeInfo 0x01436E84) in the TU BEFORE the ctor so c2 sees it does not write the temp. If c2 then inlines it, mark the
  visible body `__declspec(noinline)` in the same way as FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR/DTOR (engine/CCountedPointer.h ~l.73/122).
- The converting temp is the member template `CCountedPointer<U>` copy ctor (engine/CCountedPointer.h l.101) -- expanded inline in retail.
  Its dtor is expanded inline too (header dtor `--Info->RefCount; if (Info->RefCount==0)` folds to dec/jne once Info is a register).
  Spelling: `AddPrimitiveManager(StaticMesh2DManager);` (implicit conversion temp) is the natural source.
- The five Reset COMDATs are called out of line (0xB4B360.. are AFTER the ctor, unaligned 0x...60/10/c0/20/70 -> COMDATs). The header's in-class
  Reset would be inlined; need a declaration-only Reset (there is FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL = decl-only Reset(T*)) --
  or the visible+noinline route if register caching across Reset matters (it does not here: members are re-read via ebp/ebx lea).
- CEnginePrimitiveManager2DParticleGroup ctor: inline in this TU (vptr store 0x012A22D0 after the base ctor), also emitted out of line at
  0xB4B690 -> an in-class (inline) ctor `CEnginePrimitiveManager2DParticleGroup(CArray<CEngineScene*>& s) : CEnginePrimitiveManagerParticleGroup(s) {}`.
  Requires it to be polymorphic (virtual dtor opt-in in its header) so VC emits the vptr store.
- CEngineComponent ctor 0xB59710 called (not inlined; its TU is elsewhere). AccurateNormalise out of line (declared in C3DVector.h).
- STLport: CArray<CEngineScene*> default ctor inline (3 zero stores to 0x014377F0..F8); PrimitiveManagers CArray default ctor inline (zero +0c..+14).

## 6. Siblings
- 00/b4/CEnginePrimitiveRenderer2D_AddPrimitiveManager_00b4abb0.cpp: local STLport vector + `#include "engine/CCountedPointer.h"`, local
  CEngineComponent/CEnginePrimitiveRenderer2D (header debt noted), no flags/pragmas. Copy its whole body for visibility.
- 00/b4/CEnginePrimitiveRenderer2D_CEnginePrimitiveRenderer2D_00b4aaa0.cpp (actually ~CEnginePrimitiveRenderer2D): includes generated headers
  under #define-renamed *_PDB names and a local retail projection with all five CCountedPointers + CCPPointerInfo -- reuse its class model.
- 00/b4/CEnginePrimitiveRenderer2D_Set2DLighting_00b4a5f0.cpp: `#include "engine/C3DVector.h"`, AccurateNormalise call; note its name
  Set2DLighting disagrees with PDB ?SetLightingParameters@CEnginePrimitiveRenderer2D@@QAEXABVC3DVector@@ABVCRGBFloatColour@@11@Z (NAME_FIXES candidate).
- 00/b3/CEnginePrimitiveRenderer_CEnginePrimitiveRenderer_00b33520.cpp (3D renderer dtor, same family) -- shared CCountedPointer.h.
- 01/22/global_E2_0122ad60.cpp: `#include "engine/CArray.h"`, `static CArray<CEngineScene*> empty_scene;`.
- 3D twin ctor 0x00B33B50 (CEnginePrimitiveRenderer::CEnginePrimitiveRenderer, 21 AddPrimitiveManager calls) is NOT landed; it shows the
  identical keep-Info-in-register release (0xB3411E..0xB34158) -- whatever fixes this row fixes it.
- Sibling flags in 00/b4: `// Flags: none` (6) / `/Gr` (4, lighting only); `#pragma optimize("y", off)` appears only in CEngineLightingManager rows.

## 7. Old draft model errors
1. Wrong manager class names for 4 of 5 members (see table) and wrong member names everywhere; wrong static (`CPrimitiveManagerPool s_pool`
   instead of `CArray<CEngineScene*> empty_scene`); manager ctors take `CArray<CEngineScene*>&` not a pool pointer.
2. AddPrimitiveManager param type wrong (CCountedPointer<CEnginePrimitiveManager2DBase>; PDB: const CCountedPointer<CEnginePrimitiveManager>&, private).
3. AddPrimitiveManager only declared -> c2 reloads the temp's Info after the call (the whole 35-byte residue). Make it visible (sec. 5).
4. Local CCountedPointer dtor/Reset instead of engine/CCountedPointer.h; local SCountBlock instead of CCPPointerInfo.
5. `Normalise` -> `AccurateNormalise` (returns C3DVector&). `g_pEngineOptions->FieldOfViewDegrees` -> CEngineDef FOV_2D at retail +0x5c.
6. Draft constants: `* (1.0f/360.0f) * 6.2831855f` matches 0x1238E00/0x128F608 order; `<= 0.0f` test matches; loop/if form matches retail.
   Colour/vector store order in retail: LightVector=(1,1,1) + AccurateNormalise FIRST, then Ambient, Diffuse, Back colours -- draft already does that.

## 0x00973DC0

# DOSSIER 0x00973DC0 CAIStateGroup_JackOfBladesBase::SummonPillars (833 B)

## 1. Retail shape (xdis 00973dc0 0x351)
- __thiscall, no params, plain `ret` (0x974100). NO frame pointer (sub esp,0x150; push ebx/ebp/esi; esi=this), NO SEH/EH frame
  even though a CCharString temp and a CCreatureAction_PlayCombatAnimation temp are destroyed every iteration (same as sibling
  RushToPos 0x9735D0: "no EH frame (Jack TU), /O2"). edi is pushed LATE (0x973e2a, after the empty-vector early return).
- Prologue: def local [esp+0x10]=0, `GetCreature().PBaseDef->GetSubDef<CJackOfBladesBattleDef>(&def)` (0x8C33E0).
  count=(Pillars._M_finish-_M_start)>>3 ; `cmp count,0 ; jne` -> early path: if(def && --def->RefCount==0) def vslot1 ; return.
  (single CDefPointer dtor with null check: the end path has no null test because def->TimeBetweenPillars was already deref'd.)
- interval = def +0x4c (TimeBetweenPillars, retail def is 4 bytes shorter than PDB: header CJackOfBladesBattleDef sizeof 0x80) spilled once.
- Loop (do..while, entry 0x973e30, aligned with `lea ecx,[ecx]` pad): `GetWorld()` (0x8FCFC0, result discarded) then
  static fastcall `CWorld::GetFrame()` (0x49D870) => spelling `GetWorld().GetFrame()`.
  `if ((float)((frame - lastFrame) * CMainGameComponent::ConstantFPS) > interval)` (imul [0x1375550], fild, fcomp, test ah,41h, jne skip)
  then lastFrame=frame; ebp=index*8 (`shl ebp,3` then `add edi,ebp` / `lea ecx,[eax+ebp]` per use; Pillars._M_start reloaded every use).
  - IS_THING_ALIVE(Pillars[index]) : GetPItem nonnull && !(item+0x91 &1) [IsDead]; two GetPItem calls.
  - GetPItem; inline GetTC<CTCJackPillar>: test byte [eax+24h],20h (UsingComponents bit 37), key 0x25 local, LowerBound 0x40F020 on
    Components (+0x44), end via [map+4]; if found -> CTCJackPillar::Enable(true) 0x7317D0 (no null test of the TC).
  - IS_THING_ALIVE(PArenaCentre) (+0x1c, ebx=&PArenaCentre held in ebx).
  - centre = PArenaCentre.GetPItem() (ebx) FIRST, then pillar=Pillars[i].GetPItem(); GetPos(pillar)->edi, GetPos(centre)->eax;
    dir = centre.GetPos() - pillar.GetPos() (X,Y,Z loaded in order); fast Normalise; squared sum emitted as Z*Z then Y*Y, add, X*X, add;
    Z*r is DEAD (fstp st0) => only X,Y used => `dir.GetAngleXY()`; atan2(X,Y)*0.15915494 [0x1237F08], then wrap
    (<0.0f || >=1.0f -> fmod(double,1.0) [0x1237F00] via _CIfmod 0xBFEB28, if <0 +=1.0f) == GET_ROUNDED_ANGLE / C3DVector::GetAngleXY.
  - Pillars[i].GetPItem()->PhysicsTC(+0x60) captured in edi BEFORE the atan (mov edi,[eax+60h] at 0x973fb3) then vcall +0x108
    SetFacingAngleXY(angle).
  - pos = Pillars[i].GetPItem()->GetPos() (copy to [esp+2c..34]); `GetWorld()` vslot +0x34 = slot13 PeekWorldMap() const ->
    CWorldMap::GetHeightZBelowPos(pos) 0x63E480 + const[0x122EE58] stored to pos+4 (the Y slot [esp+0x30]! same as draft).
  - Pillars[i].GetPItem()->PhysicsTC vcall +0x7c SetPosition(pos).  ++index (index lives in a frame slot [esp+1c]).
  - Always: CCharString("SUMMON_LOOP" @0x12981FC, -1) ctor 0x99EBF0 built BEFORE GetCreature is loaded (=> implicit const char*
    conversion argument, see RushToPos shape note), CCreatureAction_PlayCombatAnimation(GetCreature(),str,true) 0x8B7FF0 at [esp+48],
    AIAction_PerformAction(const&) 0x97D8B0 -> edi, ~CCombatActionBase 0x62E900 (inline PlayCombatAnimation dtor), ~CCharString 0x99EAE0.
  - `while (index < count && result != AI_ACTION_TERMINATE_PROCESS(1))` (cmp ebp,ebx jge ; cmp edi,1 jne loop). count reloaded to ebx.
- End: def release (dec [ecx+4]; jne; vslot1 [edx+4]) no null test, pop edi, ret.

## 2. Identity (two sources)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv, egor_signatures.tsv) + FableWin.pdb (_fablewin_api.tsv, naming_stragglers signatures):
  `?SummonPillars@CAIStateGroup_JackOfBladesBase@@IAEXXZ` -> protected, void SummonPillars(), non-const, non-virtual.
  Neighbour SummonPillarsOld (??..IAEXXZ) is retail 0x00973900 (manifest). FableWin sizes: SummonPillars 1265 B vs Old 4143 B -> the
  833 B row is the short one. Manifest label is CORRECT. Mangled leaf: ?SummonPillars@CAIStateGroup_JackOfBladesBase@@IAEXXZ.
- Only xref: call from 0x00975039, inside CAIStateGroup_JackOfBladesStage2::Update 0x00974AA0 (manifest range; not landed).
- Shared header has no SummonPillars declaration: author needs a new nested opt-in in engine/CAIStateGroup_JackOfBladesBase.h
  (like FABLE_RETAIL_JACKOFBLADESBASE_RUSHTOPOS) declaring `void SummonPillars();` under protected.

## 3. Callees (landed file | landed name | PDB name | draft spelling)
- 0x008C33E0 00/8c/CDefClassBase_GetSubDefCJackOfBladesBattleDef_008c33e0.cpp | `bool CDefClassBase::GetSubDef<CJackOfBladesBattleDef>(CDefPointer<const CJackOfBladesBattleDef>*) const` | draft `void GetSubDef(CJackDef**)` WRONG (bool return, CDefPointer, const; class CJackOfBladesBattleDef not "CJackDef").
- 0x008FCFC0 00/8f/CAIStateGroupBase_GetWorld_008fcfc0.cpp | `CWorld& CAIStateGroupBase::GetWorld() const` | PDB ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ | draft `GetPWorld()` WRONG.
- 0x0049D870 00/49/CWorld_GetFrame_0049d870.cpp | `static long __fastcall CWorld::GetFrame()` | PDB ?GetFrame@CWorld@@SIJXZ | draft `GetWorldFrame` WRONG.
- 0x00A01B50 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | `CBaseObject* CBaseIntelligentPointer::GetPItem() const` | PDB same | draft returns CThing* (use CIntelligentPointer<CThing> accessors / IS_THING_ALIVE).
- 0x0040F020 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap::LowerBound (engine/CVectorMap.h) | draft local CInterfaceMap WRONG -> use CThing::GetTC<CTCJackPillar> (FABLE_RETAIL_CTHING_GETTC_EXPANDED).
- 0x007317D0 00/73/CTCJackPillar_Enable_007317d0.cpp | CTCJackPillar::Enable(bool) | PDB ?Enable@CTCJackPillar@@QAEX_N@Z | OK. (No shared CTCJackPillar.h; 0x976E20 row declares a minimal class with StatGetInterfaceType()=TCI_JACK_PILLAR.)
- 0x004C73D0 00/4c/CThing_GetPosition_004c73d0.cpp | `const C3DVector& CThing::GetPos() const` | PDB ?GetPos@CThing@@QBEABVC3DVector@@XZ | draft `GetPosition` WRONG.
- 0x00BFEB28 00/bf/global_CIfmod_00bfeb28.cpp | _CIfmod (intrinsic fmod) | via GFAngles.h `#pragma intrinsic(fmod)`.
- 0x0063E480 00/63/CWorldMap_GetHeightZBelowPos_0063e480.cpp | `float CWorldMap::GetHeightZBelowPos(const C3DVector&) const` | PDB same | OK.
- vslot +0x34 on CWorld = slot 13 `virtual const CWorldMap& PeekWorldMap() const` (engine/CWorld.h l.112, 0x004AE8C0) | draft `GetWorldMap()` (slot 12) WRONG name.
- vslot +0x108 on CTCPhysicsBase = slot 66 SetFacingAngleXY(float) (engine/CTCPhysicsBase.h l.114; landed rows cite "slot 66, +0x108") | draft `SetAngleXY` on CPhysicsBody WRONG.
- vslot +0x7c on CTCPhysicsBase = slot 31 SetPosition(const C3DVector&) (CTCPhysicsBase.h l.42) | draft OK name, wrong class.
- 0x0099EBF0 CCharString ctor / 0x0099EAE0 ~CCharString (fable_string.h) | OK.
- 0x008B7FF0 CCreatureAction_PlayCombatAnimation(CThingCreatureBase*/&, const CCharString&, bool) (engine/CCreatureAction_PlayCombatAnimation.h, RushToPos passes GetCreature() and "RUSH_OUTOF").
- 0x0097D8B0 00/97/CAIStateGroupBase_AIActionPerformAction_0097d8b0.cpp landed as `long AIAction_PerformAction(CCreatureActionBase*)` but
  PDB ?AIAction_PerformAction@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@ABVCCreatureActionBase@@@Z = `EAIActionReturn (const CCreatureActionBase&)`
  -> NAME_FIXES candidate for the landed row; shared header has the right decl under FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTIONONCE (CAIStateGroupBase.h ~l.96-101). Draft pointer form WRONG.
- 0x0062E900 ~CCombatActionBase (00/62/CCombatActionBase_CCombatActionBase_0062e900.cpp) | via FABLE_RETAIL_CCOMBATACTIONBASE.
- Globals: 0x013CE558 = GFastOneOverSqrtTable (C3DVector.h l.30; draft g_InvSqrtMantissaTable WRONG); 0x01375550 =
  CMainGameComponent::ConstantFPS (protected static; accessor static GetConstantFPS(); fable_game.h l.73-76, CMainGameComponent.h l.189;
  draft g_FramesPerSecond WRONG). Constants 3.0f 0x122DED4, 0.5f 0x122F59C, 0.0f 0x122DEDC, 1.0f 0x122DED8.

## 4. Members
- this: CAIStateGroupBase::PBrain +4 -> CAIBrain creature +0x18 (GetCreature(), FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE + FABLE_RETAIL_CAIBRAIN_GETCREATURE).
- CAIStateGroup_JackOfBladesBase.h (FABLE_RETAIL_CAISTATEGROUP_JACKOFBLADESBASE): PArenaCentre +0x1c CIntelligentPointer<CThing>,
  Pillars +0x40 std::vector<CIntelligentPointer<CThing> > (STLport 12-byte vector; PDB says +0x44). Elements 8 bytes.
- CThing (engine/CThing.h, FABLE_RETAIL_CTHING): UsingComponents bitset +0x20 (bit 37 -> +0x24 &0x20), Components CVectorMap +0x44,
  PhysicsTC +0x60 (CTCPhysicsBase*, GetPhysicsTC()), PBaseDef +0x70, Dead bit +0x91&1 (IsDead(); used by IS_THING_ALIVE, ThingAlive.h FABLE_RETAIL_IS_THING_ALIVE).
- CDefPointer<const CJackOfBladesBattleDef> (engine/CDefPointer.h; release opt-ins FABLE_RETAIL_CDEFPOINTER_RELEASE_{SELFDELETING_POINTEE,COUNTREF,POINTEE}; pick the one giving `dec [ecx+4]; jne; call [vt+4]`).
- CJackOfBladesBattleDef::TimeBetweenPillars (engine/CJackOfBladesBattleDef.h; retail +0x4c, PDB +0x50 because retail base is 4 smaller).
- Locally modelled in the draft but with shared headers: C3DVector, CCharString, CJackOfBladesBattleDef(CJackDef), CDefClassBase/CDefPointer,
  CVectorMap(CInterfaceMap), CTCPhysicsBase(CPhysicsBody), CThing, CThingCreatureBase, CWorldMap, CWorld, CBaseIntelligentPointer/CIntelligentPointer,
  CCreatureActionBase, CCombatActionBase, CCreatureAction_PlayCombatAnimation, CAIBrain, CAIStateGroupBase, CAIStateGroup_JackOfBladesBase
  (all must come from rebuild/include/engine/*.h). Only CTCJackPillar has no shared header.

## 5. Callee-body visibility
- GetTC<CTCJackPillar>: retail has its own COMDAT at 0x00976E20 (landed, never called) but every use in this TU is expanded -> FABLE_RETAIL_CTHING_GETTC_EXPANDED (forceinline).
- Fast Normalise / GetAngleXY / GET_ROUNDED_ANGLE inline: C3DVector.h FABLE_RETAIL_C3DVECTOR + FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES (GetAngleXY body),
  GFAngles.h GET_ROUNDED_ANGLE (forceinline only under FABLE_RETAIL_JACK_STAGE1_UPDATE, which also changes GFArcTan/fmod(float) forms - check
  which fmod spelling gives `fld qword [1.0]`: retail uses double fmod -> the NON-Stage1 branch, i.e. do NOT define FABLE_RETAIL_JACK_STAGE1_UPDATE).
- Squared-length order variants: FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_ZYX / _XYZ, NORMALISE_PAREN_SQUARES_ZYX (RushToPos; spills squares - probably NOT here since
  retail has no spill and no magnitude reuse), NORMALISE_LOCALS(_XYZ), NORMALISE_UNPAREN, NORMALISE_SQUARE_HELPER. Retail order here: Z^2, Y^2, add, X^2, add.
- Out-of-line (not visible / called): GetPItem, GetPos, GetWorld, CWorld::GetFrame, LowerBound, Enable, GetHeightZBelowPos, CCharString ctor/dtor,
  PlayCombatAnimation ctor, AIAction_PerformAction, ~CCombatActionBase. PlayCombatAnimation dtor itself is inline (only base dtor called).
- Same TU, earlier, landed: RushToPos 0x9735D0, SetupArena 0x972830, Initialise/OnActivate/Cleanup 0x972040..0x972110.

## 6. Siblings to copy
- 00/97/CAIStateGroupJackOfBladesBase_RushToPos_009735d0.cpp (BEST donor: same TU, same idioms - fast Normalise, GetTC expanded, implicit
  CCharString temp, action temp, GetCreature re-read). Defines: FABLE_RETAIL_CAISTATEGROUPBASE, _MEMBERS, _GETCREATURE, _PERFORMACTION,
  FABLE_RETAIL_CAIBRAIN_GETCREATURE, FABLE_RETAIL_CAISTATEGROUP_JACKOFBLADESBASE, FABLE_RETAIL_JACKOFBLADESBASE_RUSHTOPOS, FABLE_RETAIL_CINTELLIGENTPOINTER,
  _COMPARE_NULL_INT, FABLE_RETAIL_CTHING/_CTHINGPHYSICAL/_CTHINGGAMEOBJECT/_CTHINGCREATUREBASE/_CTHINGAICREATURE, FABLE_RETAIL_IS_THING_ALIVE,
  FABLE_RETAIL_CCHARSTRING, FABLE_RETAIL_DEF_CLASS_BASE, FABLE_RETAIL_CTCBASE, FABLE_RETAIL_CCREATUREACTIONBASE, FABLE_RETAIL_CCOMBATACTIONBASE,
  FABLE_RETAIL_CTHING_GETTC_EXPANDED, FABLE_RETAIL_C3DVECTOR(+_NORMALISE_PAREN_SQUARES_ZYX), FABLE_RETAIL_CCREATUREACTION_PLAYCOMBATANIMATION. No pragmas.
- 00/97/CThing_GetTCCTCJackPillar_00976e20.cpp (CTCJackPillar minimal class + TCI_JACK_PILLAR).
- 00/97/CAIStateGroupJackOfBladesBase_SetupArena_00972830.cpp, _Cleanup_00972110.cpp, Stage2_OnActivate_00972490.cpp (same TU).
- 00/8c/CDefClassBase_GetSubDefCJackOfBladesBattleDef_008c33e0.cpp (FABLE_RETAIL_DEF_CLASS_BASE + CParentDefClassBase.h + CDefPointer.h).
- Need for PhysicsTC vcalls: FABLE_RETAIL_CTCPHYSICSBASE (needs FABLE_RETAIL_CTCBASE); for PeekWorldMap: engine/CWorld.h retail class.

## 7. Old-draft model errors
- Entirely local types (see §4) and ~12 wrong names (§3) - rejected by verifier strict mode regardless of bytes.
- GetSubDef modelled void/raw pointer + hand-written refcount release at both exits; retail = one CDefPointer local whose dtor runs at both exits.
- AIAction_PerformAction by pointer & `&temp` of a temporary; must be const& to a temporary `AIAction_PerformAction(CCreatureAction_PlayCombatAnimation(GetCreature(),"SUMMON_LOOP",true))`.
- Explicit `CCharString anim("SUMMON_LOOP",-1)` named local: retail builds the string before loading the creature, consistent with implicit
  const char* conversion (RushToPos note); a named local would also be fine order-wise but lifetimes differ - try implicit first.
- Hand-written WrapUnit/atan2 + Normalise instead of `dir.Normalise(); ...->SetFacingAngleXY(dir.GetAngleXY())` (dead Z scale comes free from a full Normalise whose Z is unused).
- Remaining residue per STATUS (834v833): Pillars[i] addr add vs lea, sum order, SetPosition vcall register. Retail recomputes `Pillars[index]`
  (reload _M_start + index*8) at every use: write `Pillars[index].GetPItem()` / IS_THING_ALIVE(Pillars[index]) at each site, not a cached reference
  (draft cached `slot`/`target` refs -> `add` form). Retail fetches the PhysicsTC for SetFacingAngleXY BEFORE computing the angle (edi) -> write
  `CTCPhysicsBase* phys = ...->PhysicsTC` ordering or `GetPhysicsTC()->SetFacingAngleXY(dir.GetAngleXY())` (arg evaluated after the object).
- Count compared `!= 0` at entry but loop exit `index < count` signed: keep `long count = Pillars.size()` + explicit early-out or rotated while.

## 0x00875B10

# DOSSIER 0x00875B10 -- NCameraModes::CTools::TestMeshForOccupationAndGetType (838 bytes)

## 1. Retail shape (xdis 00875b10 0x356)
- `sub esp,68h; push ebx/ebp/esi/edi`. No frame pointer, no SEH, no /Oy- . `ret 10h` x2 (two epilogues: return 1 at 0x875E3B, return 0 at 0x875E4A).
- __thiscall, `this` (ecx) never read -> 4 stack args: [esp+7C]=mesh (const CPhysicsMeshObject&, kept in edi, passed as ecx),
  [esp+80]=position (const C3DVector&, ebx), [esp+84]=up/direction (const C3DVector&), [esp+88]=float& height.
- Dead arg slots reused: mesh slot [esp+7C] holds Y-edge.X int copy, then the extent `size`, then the 2nd loop counter j (fild).
  The `up` slot [esp+84] (after up is read) holds the FastInvSqrt estimate bits and the normalise factor, then loop counter i (fild). up.Z is int-copied to [esp+1C].
- Frame: 0x10 temp/bits, 0x14..0x1C = n*size (normalised up), 0x20..0x28 = side*size, 0x2C..0x34 = probe `to`, 0x44 Y-edge temp,
  0x50 X-edge temp (reused as `to` in loop 2), 0x5C..0x77 = CMeshIntersectionData (0x1C bytes; *height = [esp+64] = +8 = IntersectionPosition.Z).
- Alignment pads `lea ebx,[ebx]` (0x875D0A) and `mov edi,edi` (0x875D8E) before the two loop heads (normal /O2 loop alignment).
- Loop 1: esi=i 0..1, `(0.5f - (float)i) + same` (fadd st0,st0 = *2) -> __ftol2 (long dir); to=(pos.X,pos.Y [int copies], (float)dir*size*3.0f + pos.Z);
  GetPointIntersectionFromTo(pos,to,isect) false -> return 0; else *height=isect.IntersectionPosition.Z; ++hits(ebp).
- Loop 2: j 0..3, angle = (float)j*0.25f * (double)2pi (0x0124F2B8, double mul) ; fsin computed first on a dup, then fcos (intrinsics on double);
  to = position + n*size*cos + side*size*sin  (code: cos*N products, add pos, then add sin*S products); hit -> ++hits.
- End: `sub ebp,5; je ret1; dec ebp; jne ret0` = `hits==5 || hits==6` (or switch case 5/6). Hits starts 0 (`xor ebp,ebp`).
- Constants: 0x013CE558 GFastOneOverSqrtTable, 0x0122DED4 3.0f, 0x0122F59C 0.5f, 0x0122DEDC 0.0f, 0x0123078C 0.25f, 0x0124F2B8 2pi double.

## 2. Identity (manifest label is right, but namespace + signature are not as the draft has them)
- Fable.exe.signatures.tsv (naming_stragglers): `00875b10 NCameraModes::CTools::TestMeshForOccupationAndGetType 838`.
- bsim_port_audit_final.tsv: `875b10 ?TestMeshForOccupationAndGetType@CTools@NCameraModes@@QBE?BW4EMeshType@2@ABVCPhysicsMeshObject@@ABVC3DVector@@1AAM@Z` (sim 0.73).
- Ego_r.pdb (egor_pdb_names.tsv 0x0085C530) and FableWin.pdb (_fablewin_api.tsv 0x01853D1F/0x029C2E70) same mangled name.
- Mangled leaf: `?TestMeshForOccupationAndGetType@CTools@NCameraModes@@QBE?BW4EMeshType@2@ABVCPhysicsMeshObject@@ABVC3DVector@@1AAM@Z`
  => `const NCameraModes::CTools::EMeshType NCameraModes::CTools::TestMeshForOccupationAndGetType(const CPhysicsMeshObject&, const C3DVector&, const C3DVector&, float&) const`
  (public, const member, returns `const EMeshType` -- `?B` = const-qualified return; EMeshType is a NESTED enum of NCameraModes::CTools (`W4EMeshType@2@`)).
- No enumerator names found in any repo source (only the type). Values used: 0 and 1 (made-up names: mark descriptive).
- Caller: 0x00876A8B (inside unlanded 0x008764E0, labelled DrawPyramid/RenderParticleEmitter_4particles = unreliable), `mov ecx,ebp` (CTools this),
  mesh = a CPhysicsMeshObject frame local from CTCPhysicsBase::GetPhysicsMeshObject 0x006B0850; result compared `cmp eax,1`.
- Draft errors: global `CTools` (needs namespace NCameraModes), non-const method, pointer params (mesh*, height*) instead of const& / float&, non-const return.

## 3. Callees
| addr | landed file | landed/PDB name | draft spelling |
|---|---|---|---|
| 0x00A11C20 | rebuild/src/compiled/00/a1/CPhysicsMeshObject_GetBoundingBoxXEdge_00a11c20.cpp | `?GetBoundingBoxXEdge@CPhysicsMeshObject@@QBE?AVC3DVector@@XZ` (Ego_r 0x00B7F760) | GetExtentA -- WRONG |
| 0x00A11C90 | .../CPhysicsMeshObject_GetBoundingBoxYEdge_00a11c90.cpp | `?GetBoundingBoxYEdge@CPhysicsMeshObject@@QBE?AVC3DVector@@XZ` (Ego_r 0x00B7F7D0) | GetExtentB -- WRONG |
| 0x00A12450 | not landed | `?GetPointIntersectionFromTo@CPhysicsMeshObject@@QBE_NABVC3DVector@@0AAVCMeshIntersectionData@@@Z` (Ego_r 0x00B800D0) | 3rd arg as C3DVector& -- WRONG, it is CMeshIntersectionData& |
| 0x00BFEA70 | rebuild/src/compiled/00/bf/global_ftol2_00bfea70.cpp | __ftol2 (compiler-emitted for (long)float) | ok |
- Manifest/Fable.exe.signatures label 0xA11C20/0xA11C90 as CEngineInternalPrimitiveParticleLine::GetWorldPosition: propagated, wrong (hand309 NOTES agrees).
- All three CPhysicsMeshObject callees are declared in rebuild/include/engine/CPhysicsMeshObject.h:
  edges under `FABLE_RETAIL_CPHYSICSMESHOBJECT_EDGES`; GetPointIntersectionFromTo(const C3DVector&,const C3DVector&,CMeshIntersectionData&) const is unconditional (line 43).

## 4. Members / types
- No CTools member is touched. Only C3DVector X/Y/Z (+0/+4/+8; engine/C3DVector.h, `FABLE_RETAIL_C3DVECTOR`) and
  CMeshIntersectionData (engine/CMeshIntersectionData.h, size 28 static-asserted: IntersectionPosition +0, Normal +0xC, DistanceSquared +0x18); *height = isect.IntersectionPosition.Z.
- CTools header: rebuild/include/engine/NCameraModes_CTools.h (sizeof 0x20) -- has NO declaration of TestMeshForOccupationAndGetType or EMeshType yet; the row must declare
  it (a local `namespace NCameraModes { class CTools {...}; }` as the other CTools rows do, or a new header opt-in). Existing opt-ins: FABLE_RETAIL_NCAMERAMODES_CTOOLS (GetMeshExtent etc.), _LANDSCAPEFROMTO, _POTENTIALFIELD, _STANDARDTHINGS, _WORLD, _ROLLING_*.
- Draft models LOCALLY classes that have shared headers: C3DVector (engine/C3DVector.h), CPhysicsMeshObject (engine/CPhysicsMeshObject.h), its FastInvSqrt (= GFFastOneOverSqrt in C3DVector.h, table `GFastOneOverSqrtTable` not `g_InvSqrtMantissaTable`), and should use CMeshIntersectionData (engine/CMeshIntersectionData.h).

## 5. Inline helpers visible to retail's compiler (all expanded here)
- C3DVector::GetSquaredMagnitude (extent sum), C3DVector::Normalise() (FableWin ?Normalise@C3DVector@@QAEAAV1@XZ = GFFastOneOverSqrt of the squared length, expanded), C3DVector::CrossWithUp() (Y,-X,0) -- the side vector,
  operator*(float), operator+, operator*=.
- Evidence for Normalise+CrossWithUp: the side-vector normalise multiplies Z by the literal 0.0f (`fmul [0x122DEDC]` at 0x875CBE) = `Z*=factor` on a CrossWithUp temp whose Z is 0.0f; its squared length starts with (-nx)^2 (=Y) then ny^2 (=X): Z,Y,X order with the 0 term gone.
  The up normalise also sums Z*Z, Y*Y, X*X (z loaded via int copy into [esp+1C] = a C3DVector copy `C3DVector n = up;`).
- Opt-ins in engine/C3DVector.h: FABLE_RETAIL_C3DVECTOR_CROSSWITHUP (line 160), Normalise bodies: default `GFFastOneOverSqrt(GetSquaredMagnitude())`,
  FABLE_RETAIL_C3DVECTOR_NORMALISE_PAREN_SQUARES_ZYX, _NORMALISE_LOCALS_XYZ, _NORMALISE_LOCALS, _NORMALISE_SQUARE_HELPER(_GROUPED_SUM), _NORMALISE_UNPAREN;
  GetSquaredMagnitude bodies: FABLE_RETAIL_C3DVECTOR (parenthesised), _SQUAREDMAGNITUDE_XYZ, _SQUAREDMAGNITUDE_ZYX;
  GFFastOneOverSqrt bodies: default, _GFFASTONEOVERSQRT_NEWTON_GROUPED / _NEWTON_SQUARE_PAREN / _NEWTON_STEP_PAREN / _NEWTON_PAREN / _ORIGINAL_UNION
  (these PAREN variants exist precisely to move VC7.1's scheduling-region cut -- the hand309 residue is x87 scheduling).
- Out of line (not inlined): the two edge getters, GetPointIntersectionFromTo, __ftol2. No STL.
- Same-TU neighbour that shows the extent idiom: NCameraModes::CTools::GetMeshExtent 0x00875F00 (landed, RELOCATION grade) =
  `mesh.GetBoundingBoxXEdge().GetSquaredMagnitude() + mesh.GetBoundingBoxYEdge().GetSquaredMagnitude()` with defines
  FABLE_RETAIL_C3DVECTOR + FABLE_RETAIL_CPHYSICSMESHOBJECT_EDGES. Its retail: esi=X result, eax=Y result, int copy of X.x, Y loaded z,y,x first.
  In THIS row: esi=X result (called first), eax=Y result, int copy is Y.x and X is loaded z,y,x first -> the operands are the mirror image:
  try `YEdge.GetSquaredMagnitude() + XEdge.GetSquaredMagnitude()` with X still called first (e.g. bind `const C3DVector& x = mesh.GetBoundingBoxXEdge(); const C3DVector& y = mesh.GetBoundingBoxYEdge();` then `y.Sq + x.Sq` or `x.Sq + y.Sq` -- verify which). Final faddp = sum(int-copied vec) + sum(other).

## 6. Siblings (same TU, NCameraModes::CTools, rebuild/src/compiled/00/87/)
- NCameraModesCTools_GetMeshExtent_00875f00.cpp: `// Flags: none`; defines FABLE_RETAIL_CCHARSTRING, _CCLASS, _DEF_CLASS_BASE, _C3DVECTOR, _CTCBASE, _CTHING, _CTCPHYSICSBASE, _CPHYSICSMESHOBJECT_EDGES, _NCAMERAMODES_CTOOLS; includes engine/CPhysicsMeshObject.h + engine/NCameraModes_CTools.h.
- NCameraModesCTools_Get2DRotationAngle_00875a30.cpp (immediately before this row): `#include <math.h>`, `#pragma intrinsic(sqrt, acos)`, local `namespace NCameraModes { class CTools { public: ... const; }; }`, engine/C3DVector.h; this unused.
- CTools_IsPosInViewingFrustrum_00875120.cpp: `#pragma intrinsic(cos)`. PotentialFieldBufferObjects/Points 0x876CA0/0x876E10: `#pragma intrinsic(sqrt)`, FABLE_RETAIL_NCAMERAMODES_CTOOLS_POTENTIALFIELD.
- CTools_GetCollisionThing_00875e60.cpp: GetPointIntersectionFromTo with CMeshIntersectionData (older local model; prefer the shared headers).
- CRossWithUp + Normalise idiom: rebuild/src/compiled/00/95/CAIStateGroupHoverWander_Update_00959bf0.cpp (FABLE_RETAIL_C3DVECTOR_CROSSWITHUP, `// Flags: none`).
- No sibling uses a pragma optimize; default /O2-style flags.

## 7. Old draft model errors vs disassembly
1. Intersection output typed C3DVector (12 bytes) -> must be CMeshIntersectionData (28 bytes): changes frame layout (retail 0x5C..0x77) and *height = isect.IntersectionPosition.Z.
2. Signature: pointer params / non-const / global CTools / non-const return (see 2). mesh must be `const CPhysicsMeshObject&` (edi from the arg, passed as ecx).
3. Getter names GetExtentA/B -> GetBoundingBoxXEdge/YEdge; extent operand order probably mirrored (see 5).
4. Local FastInvSqrt/Normalise(free fn)/C3DVector: use member `C3DVector n = up; n.Normalise();` and `C3DVector side = n.CrossWithUp(); side.Normalise();` (side Z*=factor on literal 0.0f proves the member Normalise on a Z=0 vector; the draft's `C3DVector(normal.Y,-normal.X,0.0f)` is the same as CrossWithUp but its free-function Normalise multiplies via operator* -- check the zero-Z product survives).
5. Then `n *= size; side *= size;` (retail stores n*size to 0x14..0x1C first, then side*size to 0x20..0x28) -- draft matches.
6. Loop 2 expression: retail evaluates sin before cos (fsin on a dup first); `position + normal*cos + side*sin` evaluated as (cos part + pos) + sin part -- consider writing the sin product first or as separate scaled temps; angle is double (`(float)j*0.25f * 6.283185307179586`).
7. Loop counters live in dead arg slots and are fild'ed: `long` counters fine; hits in ebp.
8. Return: `hits==5 || hits==6` matches (sub 5 / dec). Return type `const EMeshType`.

## 0x00E6B900

# DOSSIER 0x00E6B900 (841 bytes) -- NScript::CV_ChapelOfEvilScript::TeleportAcolytes

## 1. Retail shape (xdis 00e6b900 0x359)
- __thiscall (esi=this), `ret 4` (one arg: const CScriptThing& in ebx = [esp+0x50]). Both exits `add esp,0x3c ; ret 4`.
- No frame pointer (sub esp,0x3c; push ebx/ebp/esi/edi). No SEH prologue (no /EHsc), although it has vector + CScriptThing locals.
  Same as the TU neighbours 0x00E6B400 (sub esp,0x34) and 0x00E6B750 (sub esp,0x14). No `// Flags:` in any landed ChapelOfEvil row -> default flags.
- ebp = 0 for the whole function (zero register: vector init, marker PImp init, `false` arg pushes, final dtor zeroing).
- Flow:
  1. vector<CScriptThing> acolytes (zeroed [esp+0x28..0x30]) FIRST, then CCharString("COE_AcolyteEvil" 0x012E8428) temp,
     count = Game->GetAllThingsWithScriptName(tmp, acolytes) (slot 75, +0x12c) -> edi, spilled to [esp+0x18]; ~CCharString.
  2. CScriptThing marker constructed AFTER the call (CBaseClass() 0x0099A2D0 at [esp+0x1c], vptr 0x01238C8C, PImp zeroed).
  3. `if (acolyte.IsAlive())` = VIRTUAL call slot 75 (+0x12c) directly on the parameter (no null-PImp wrapper visible: dispatch through a reference).
  4. alive arm (fall-through): `if (IsActiveThreadTerminating()) return;` -> THE shared exit block at 0xE6B985:
     ~CScriptThing out of line 0x004AA840(marker), ~vector out of line 0x008AC970, ret 4. ALL other terminate-returns jne back here.
     loop: `for (long i = 0; i < count; ++i)`: signed (jle/jl), i in [esp+0x14], count reloaded from [esp+0x18], edi = i*12 strength-reduced.
       test = `acolytes[i] == acolyte` INLINE: ecx=acolytes[i].PImp.Data, push acolyte.PImp.Data, call [vtbl+0x138] (slot 78 IsEqualTo).
       equal: if(T) return; marker = Game->GetThingWithScriptName("M_COE_AcolyteTeleport1" 0x012E8550); Teleport(acolyte, marker, false)
       else : if(T) return; marker = Game->GetThingWithScriptName("M_COE_AcolyteTeleport2" 0x012E8538); Teleport(acolytes[i], marker, false)
       the two arms tail-merge into one EntityTeleportToThing call site (0xE6BA78, slot 472 +0x760).
  5. not-alive arm (0xE6BAA0): if(T) return;
     `if (acolytes.size() != 0 && acolytes[0].IsAlive())` (size via /12 magic 0x2AAAAAAB, `je`; IsAlive virtual on element) { if(T) return; marker = Get(Teleport1); Teleport(acolytes[0], marker, false); }
     `if (acolytes.size() > 1 && acolytes[1].IsAlive())` (cmp 1 ; jbe -> unsigned) { if(T) return; marker = Get(Teleport2); Teleport(acolytes[1], marker, false); }
  6. Each marker reassignment: CCharString temp, slot 72 (+0x120) returns CScriptThing by value into a temp, out-of-line
     CScriptThing::operator= 0x008AB980, temp destroyed OUT OF LINE 0x004AA840, ~CCharString 0x0099EAE0.
  7. Function end: marker's ~CScriptThing EXPANDED inline (vptr store, CCountedPointer release: dec, DeleteFunc(Data), operator delete 0x00BFE9BC,
     Data=0 then Info=0, call ~CBaseClass 0x0099A2E0); ~vector EXPANDED inline (loop: push 0; call [vtbl+0] = virtual ~CScriptThing via _Destroy; free 0x00BFEA14).

## 2. Name / class / signature (two sources)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv, Ego_r addr 00ec5d70) and FableWin.pdb (ghidra_out/_fablewin_api.tsv 017e39f2):
  `?TeleportAcolytes@CV_ChapelOfEvilScript@NScript@@QAEXABVCScriptThing@@@Z`
  = public non-virtual `void NScript::CV_ChapelOfEvilScript::TeleportAcolytes(const CScriptThing&)`.
- Manifest label CV_ChapelOfEvilScript::TeleportAcolytes is RIGHT but under-qualified (missing NScript namespace).
- Already declared in rebuild/include/engine/CV_ChapelOfEvilScript.h: `void TeleportAcolytes(const CScriptThing&); // 0x00E6B900`.
- Callers (xref.py): 0x00E6B825 in landed WatchForRegionEntry 0x00E6B750 (rebuild/src/compiled/00/e6/NScriptCVChapelOfEvilScript_WatchForRegionEntry_00e6b750.cpp, declares TeleportAcolytes(const CScriptThing&)),
  and 0x00E6BFC7 in landed CCOE_AcolyteEvil::Main 0x00E6BC50 (...CCOEAcolyteEvil_Main_00e6bc50.cpp). No vtable slot (non-virtual).

## 3. Callees (landed name vs PDB; draft spelling)
| addr | what | PDB/header name | draft spelling |
|---|---|---|---|
| 0x0099EBF0 / 0x0099EAE0 | CCharString(const char*, long) / ~CCharString | fable_string.h (FABLE_RETAIL_CCHARSTRING) | local struct -> use header |
| 0x0099A2D0 / 0x0099A2E0 | CBaseClass() / ~CBaseClass | engine/CBaseClass.h | local |
| 0x004AA840 | ~CScriptThing | engine/CScriptThing.h (`virtual ~CScriptThing()`, slot0 0x004AB170) | local `__forceinline` |
| 0x008AB980 | CScriptThing::operator= | CScriptThing.h (declared, out of line; do NOT define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT) | ok |
| inline (OOL copy 0x008AAE20) | CScriptThing::operator== -> slot 78 | `IsEqualTo(const CScriptThing&) const` protected, FableWin.pdb `?IsEqualTo@CScriptThing@@MBE_NABV1@@Z` | WRONG: draft "IsSameAs" (made up) on a local CScriptThingImpl |
| slot 75 +0x12c | CScriptThing::IsAlive (0x004AB130) | CScriptThing.h virtual | ok |
| 0x00CB7940 | CScriptBase::IsActiveThreadTerminating() (non-const) | engine/CScriptBase.h | ok |
| Game slot 72 +0x120 | GetThingWithScriptName(const CCharString&) const (0x008A7D60) | CGameScriptInterfaceBase.h pure virtual | `GameCalls` local |
| Game slot 75 +0x12c | GetAllThingsWithScriptName(const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const -> long | CGameScriptInterfaceBase.h | local vector not in std:: (mangling wrong) |
| Game slot 472 +0x760 | EntityTeleportToThing(const CScriptThing&, const CScriptThing&, bool) const (0x0088E540) | CGameScriptInterface(.Base).h | ok |
| 0x008AC970 | std::vector<CScriptThing>::~vector (OOL COMDAT) | | local `vector` outside std |
| 0x00BFE9BC / 0x00BFEA14 | operator delete / free (MSVCR71) | | ok |

## 4. Members
- this+0x40 Game: CLevelScriptBase<CQ_SunnyvaleMasterData>::Game, `CGameScriptInterfaceBase*` (engine/CLevelScriptBase.h; FABLE_RETAIL_CLEVELSCRIPTBASE gives pointer form; _ORIGINAL_MEMBERS makes it a reference). Only member read.
- CScriptThing +4 PImp.Data / +8 PImp.Info (CCountedPointer<CScriptThing>, engine/CScriptThing.h; FABLE_RETAIL_CSCRIPTTHING_ORIGINAL_ACCESS makes PImp private -- then operator== must stay a member, which it is).
- vector _M_start +0 / _M_finish +4 (12-byte STLport).
- Header opt-ins: FABLE_RETAIL_NSCRIPT (required by CScriptThing.h, CV_ChapelOfEvilScript.h, CScriptBase.h), FABLE_RETAIL_CLEVELSCRIPTBASE, FABLE_RETAIL_CCHARSTRING,
  CScriptThing: _IMPLICIT_COPY / _IMPLICIT_ASSIGNMENT / _ORIGINAL_ACCESS / _NULLCOORD / _NULLSTRING; CCountedPointer: _VISIBLE_CALLED_DTOR etc. (default dtor body = retail Data-first zeroing already).
- Draft models LOCALLY (all have shared headers): CCharString (fable_string.h), CCountedPointer/CCPPointerInfo (engine/CCountedPointer.h), CBaseClass, CScriptThing,
  GameCalls (= CGameScriptInterfaceBase/CGameScriptInterface), CScriptBase, CV_ChapelOfEvilScript (engine/CV_ChapelOfEvilScript.h, base CLevelScriptBase<CQ_SunnyvaleMasterData>, not "Owner" but MasterScriptData +0x44).
  vector: no shared std::vector body is used by sibling NScript rows; they model `namespace std { allocator; vector }` locally (RescuedBandit 0x00D91740, CreateScreamers 0x00D84630, Victim Main 0x00E6B400).

## 5. Callee-body visibility
- ~CScriptThing: expanded at the end, OOL at early exits/temps -> define `inline CScriptThing::~CScriptThing() { }` after the include (as AcolyteEvil Main 0x00E6BC50, WatchForAcolyteHit 0x00E6AF20, WatchForRegionEntry 0x00E6B750 -- all same class/TU) and push the exits to OOL with `#pragma inline_depth(0)` (WatchForAcolyteDeath 0x00E6AD20 idiom).
- CScriptThing::operator==: inline here (OOL copy 0x008AAE20 exists): `inline bool CScriptThing::operator==(const CScriptThing& other) const { return PImp->IsEqualTo(*other.PImp); }` (landed 0x00D706E0 HTGGuard line 49). Retail order: loads this->PImp.Data first, then other's.
- ~vector<CScriptThing>: inline at end, OOL 0x008AC970 at exits -> model as RescuedBandit 0x00D91740: `FABLE_FORCEINLINE ~vector()` with `_Destroy` + `free`, and exits under inline_depth(0).
- operator= 0x008AB980 is called (keep header declaration, no IMPLICIT_ASSIGNMENT).
- IsAlive: virtual dispatch, no inline body needed (an inline null-safe wrapper definition in the row is harmless but not exercised).
- TU neighbours visible earlier in the same TU: CCOE_Victim::Main 0x00E6B400 (uses OOL operator== 0x008AAE20 via inline operator!=), WatchForRegionEntry 0x00E6B750 (calls TeleportAcolytes).

## 6. Sibling landed rows (rebuild/src/compiled/00/e6/)
- NScriptCVChapelOfEvilScript_WatchForAcolyteDeath_00e6ad20.cpp: fable_compiler.h, FABLE_RETAIL_CCHARSTRING, FABLE_RETAIL_NSCRIPT, includes fable_string.h, CCountedPointer.h, CScriptThing.h, CGameScriptInterfaceBase.h, CQ_SunnyvaleMasterData.h, CV_ChapelOfEvilScript.h; scoped `#pragma inline_depth(0)` around terminate returns. Best header template.
- NScriptCVChapelOfEvilScriptCCOEAcolyteEvil_Main_00e6bc50.cpp: NSCRIPT, CLEVELSCRIPTBASE, BASE_OBJECT, CCHARSTRING, CSCRIPTTHING_IMPLICIT_COPY ...; `inline CScriptThing::CScriptThing() { } inline CScriptThing::~CScriptThing() { }`.
- NScriptCVChapelOfEvilScriptCCOEVictim_Main_00e6b400.cpp: local std::vector model (inline dtor, size(), operator[]), CScriptThing.h, CGameScriptInterface.h.
- NScriptCVChapelOfEvilScript_WatchForRegionEntry_00e6b750.cpp: do{if(T)return;}while(0) exits; inline ~CScriptThing; local CQuestScriptBase.
- 00/d9/NScriptCQHobbeCaveScript_RescuedBandit_00d91740.cpp: FABLE_FORCEINLINE ~vector<CScriptThing> + depth-0 return -> OOL 0x008AC970 at the exit.
- None carries a `// Flags:` line.

## 7. Old draft's model errors
- Local classes everywhere (verifier warns / strict rejects); vector outside namespace std -> wrong mangling of slot 75 parameter type.
- `IsSameAs`/CScriptThingImpl is a made-up name: it is the inline CScriptThing::operator== -> protected virtual IsEqualTo (slot 78); write `acolytes[i] == acolyte`.
- CScriptBase/`Owner` model instead of CLevelScriptBase<CQ_SunnyvaleMasterData> (header).
- Residue (STATUS.tsv DIFFER 860v841): the merged OOL-exit block sits at the LAST terminate return in ours, retail keeps it at the FIRST (alive arm, 0xE6B985) and every later exit jumps back; in-loop eax/edx rotation. Draft is 19 bytes too long.
  Its inline_depth(0) regions also wrap the marker reassignment -- retail calls operator= OOL anyway and the temp dtor OOL; try wrapping only the returns, or try one-return/goto structure. NOTES "next": inline_depth(0) only on the alive-arm return, and swap arm order (but note retail fall-through is the ALIVE arm: `test al,al; je not_alive`).
- Loop form is right (signed long i < count from the GetAll return, spilled); size() tests right (`!= 0` then `> 1`, unsigned).

## 0x005EC270

# DOSSIER 0x005EC270 CTCTavernGame::ProcessInput (843 B + 0x43 B jump tables)

## 1. Retail shape (xdis 005ec270 0x35b)
- thiscall, `ret 0xc` (3 args). NO frame pointer (`sub esp,0xc; push ebx/ebp/esi; mov esi,ecx; ...; push edi`) -> plain /O2, no /Oy-, no SEH, not optimize("s").
- Frame 0xc: def (CDefPointer) at [esp+0x10]; C2DVector stick at [esp+0x14] X / [esp+0x18] Y.
- Pinned regs whole body: esi=this, edi=&event ([esp+0x20]), ebp=input ([esp+0x28]), ebx=0 (zero reg: def init, `mov [esi+0x21],bl`, `push ebx` ChangeBet(false), YesState/NoState/AltState=0 stores, `push ebx` Clear(false), `cmp [esi+0x34],ebx`, `cmp eax,ebx`, final `cmp ecx,ebx` null test). processed is re-read from [esp+0x24] each SetHandled site.
- def is NEVER cached in a register: every use reloads [esp+0x10] (all 4 callee-saved regs are taken). Release at exit keeps the null test: `mov ecx,[esp+10]; cmp ecx,ebx; je; dec [ecx+4]; jne; call [vtbl+4]`.
- Prologue: `mov ecx,[esi+4]; mov ecx,[ecx+0x70]` (inlined GetDef: ParentThing->PDef) ... `lea eax,[esp+10]; xor ebx,ebx; push eax; mov [esp+14],ebx; call 0x5ED4A0` -> zero store SUNK below the push (the same-TU-visible GetSubDef effect, see EnterGame 0x5EC0F0 notes).
- switch(TavernGameState) on `eax-5`, `cmp eax,0x2a; ja default`; byte index table 0x5EC590 (43 entries), dword table 0x5EC578 (6 targets). Decoded: state 5 -> 0x5EC2B7, 44 -> 0x5EC4C1, 45 -> 0x5EC39E, 46 -> 0x5EC345, 47 -> 0x5EC518 (SetAsPerformedEvent only), everything else -> 0x5EC521 (post-switch). Code layout order: case5, case46, case45, case44, common AnswerQuestion tail 0x5EC4EF/0x5EC4F0, shared SetAsPerformedEvent 0x5EC518, post-switch 0x5EC521.
- Shared tail at 0x5EC4F0: `push <state>; SetState 0x5E9B20; Yes/No/AltState=0 (+0xa8/+0xac/+0xb0); ecx=[0x13B8790]+0x18; push ebx; call Clear 0x492BAB; SetAsPerformedEvent` -> the draft's __forceinline AnswerQuestion tail-merges correctly (the tail is identical text in all 6 uses).
- Case 5: SetStateAfterYesNo args are evaluated right to left: push TavernGameState (noState, re-read [esi+0xc0]), GetText(def->No +0x104), push 6, GetText(def->Yes +0x100), GetTextWithNarrator(def->Quit +0xfc); GameFinished(+0x28)=1 and GameWon(+0x21)=bl stored AFTER the first two pushes (scheduling); then SetStateAfterYesNo then `jmp 0x5EC518` (shared SetAsPerformedEvent).
- Case 45: `cmp YesState,5; jne` FIRST, then IsEventGameAction(0x72), then event.GetType()==2, then GetControlMovementFromGameActionEvent(0x72, event, stick) (pushes &stick, edi, 0x72). Y<-0.2f -> ChangeBet(false) via `push ebx`; else Y>0.2f -> ChangeBet(true) (`push 1`; both share `mov ecx,esi; call`). X<-0.2f -> BaseBet=def->MinBet; X>0.2f -> BaseBet=LastBet. Clamp: `eax=CurrentMoney; if (BaseBet>eax) BaseBet = eax ? eax : def->MinBet`; `if (BaseBet>def->MaxBet) BaseBet=MaxBet`; SetAsPerformedEvent; falls into the button block. Float compares: `fcomp; test ah,5; jp` = `<`, `test ah,0x41; jne` = `>`; consts -0.2f 0x012560B4, 0.2f 0x012305A0 (draft header; not re-dumped).
- Post-switch: `if (TavernGameState==13 && IsEventButtonPressed && IsEventGameAction(0x16)) { SetState(NextTavernGameState +0xc8); SetAsPerformedEvent; }`.
- Draft's control flow (case bodies, condition order, ternary clamp) matches retail exactly; the residue is register allocation / frame (883 v 843).

## 2. Name / signature (two sources)
- Ego_r.pdb names (egor_pdb_names/egor_signatures, Ego_r addr 0x00649BC0) and FableWin.exe.signatures (0x022D49F0): `?ProcessInput@CTCTavernGame@@UAEXABVCInputEvent@@AAVCProcessedInput@@PAVCInputProcessTavernGame@@@Z`.
- rtti-vtables.tsv: CTCTavernGame vtable 0x012560BC slot 28 = 0x005EC270; engine/CTCTavernGame.h lists `virtual void ProcessInput(const CInputEvent&, CProcessedInput&, CInputProcessTavernGame*); // 28 0x005EC270`.
- Callers (xref): the derived overrides call it by name: CTCTavernGameShoveHaPenny::ProcessInput 0x7CF9A0, CoinGolf 0x7D3820, CardPairs 0x7D6870, SpotTheAddition 0x7D8570, CardBlackjack 0x7DA010 (landed, `jmp`; rebuild/src/compiled/00/7d/CTCTavernGameCardBlackjack_ProcessInput_007da010.cpp, uses FABLE_RETAIL_CTCTAVERNGAME + engine/CTCTavernGame.h), CardBase 0x8F12D0.
- Manifest label is CORRECT. Signature: `void CTCTavernGame::ProcessInput(const CInputEvent& event, CProcessedInput& processed, CInputProcessTavernGame* input)` (public virtual, void).

## 3. Callees (landed file / landed name / PDB name)
| addr | landed row | landed name | PDB / header name | draft spelling |
|---|---|---|---|---|
| 0x005ED4A0 | 00/5e/CDefClassBase_GetSubDefCTavernGameDef_005ed4a0.cpp | GetSubDef<CTavernGameDef> | `CDefClassBase::GetSubDef<CTavernGameDef>(CDefPointer<const CTavernGameDef>*) const` (CDefClassBase_GetSubDef.h, FrameUpdate defines it) | `GetSubDef(CDefPointer*)` non-template, non-const - MISMATCH |
| (0x005EA000 inlined) | 00/5e/CTCTavernGame_GetDef_005ea000.cpp | GetDef | `?GetDef@CTCTavernGame@@QAEXPAV?$CDefPointer@$$CBVCTavernGameDef@@@@@Z` inline in header | ok (inline) |
| 0x00687A90 | 00/68/CInputProcessGameBase_IsEventButtonPressed_00687a90.cpp | IsEventButtonPressed (non-const in landed row) | `?IsEventButtonPressed@CInputProcessGameBase@@IBE_NABVCInputEvent@@@Z` = PROTECTED const | draft non-const on CInputProcessTavernGame - MISMATCH (const) |
| 0x00687CF0 | 00/68/Consvalstdallocator...CEntryconst_00687cf0.cpp | garbage `T::Run` (manifest `Forward_IsEventGameAction_via0x24`) | `?IsEventGameAction@CInputProcessGameBase@@QBE_NW4EGameAction@@ABVCInputEvent@@@Z` (body `mov ecx,[ecx+24h]; jmp 0x445BD0` CGamePlayerInterface::IsEventGameAction) | draft `(long, const CInputEvent&)` non-const - MISMATCH (EGameAction, const); landed name is WRONG -> NAME_FIXES candidate |
| 0x00687D00 | 00/68/CInputProcessGameBase_GetEventGameActionVector_00687d00.cpp | GetEventGameActionVector (made up) | `?GetControlMovementFromGameActionEvent@CInputProcessGameBase@@QBE_NW4EGameAction@@ABVCInputEvent@@AAVC2DVector@@@Z` (CInputProcessGameBase.h says so) | draft GetEventGameActionVector - WRONG NAME |
| 0x00A03B40 | 00/a0/CInputEvent_GetType_00a03b40.cpp | CInputEvent::GetType | `?GetType@CInputEvent@@QBE?AW4EInputEventType@@XZ` returns EInputEventType | draft returns long - minor |
| 0x00A0D390 | 00/a0/CWomanToAttract_Init_00a0d390.cpp (ICF fold: `mov byte [ecx+a8],0; mov [ecx+4],2`) | CWomanToAttract::Init | `CProcessedInput::SetAsPerformedEvent()` (Ego_r, CProcessedInput.h) | draft `SetHandled` - WRONG NAME |
| 0x005E9E70 | 00/5e/CTCTavernGame_GetText_005e9e70.cpp | GetText | `?GetText@CTCTavernGame@@QAE?AVCWideString@@K@Z` | ok |
| 0x005EB3A0 | 00/5e/CTCTavernGame_GetTextWithNarrator_005eb3a0.cpp | GetTextWithNarrator | same | ok |
| 0x005EAF30 | 00/5e/CTCTavernGame_SetStateAfterYesNo_005eaf30.cpp (Flags: /GS) | SetStateAfterYesNo | protected `(CWideString, CWideString, ETavernGameStates, CWideString, ETavernGameStates)` | ok |
| 0x005EB810 | 00/5e/CTCTavernGame_ChangeBet_005eb810.cpp | ChangeBet(bool) | same | ok |
| 0x005E9B20 | 00/5e/CTCTavernGame_SetState_005e9b20.cpp | SetState | `?SetState@CTCTavernGame@@QAEXW4ETavernGameStates@1@@Z` | ok |
| 0x00492BAB | 00/49/CSubtitleRenderer_Clear_00492bab.cpp | CSubtitleRenderer::Clear | `?Clear@CSubtitleRenderer@@QAEX_N@Z` | ok |
| def slot 1 (`call [edx+4]`) | - | CDefPointer release | via engine/CDefPointer.h | draft local |
- Global 0x013B8790 = `CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass`, read via inline `CPlayerGui::Get()` (EnterGame naming wave 18; FrameUpdate uses `CPlayerGui::Get().GetSubtitleRenderer()`); the member is `CSubtitleRenderer SubtitleRenderer` +0x18 (CPlayerGui.h line 82). Draft's `g_pPlayerGui->Subtitles` is a WRONG spelling.

## 4. Members
CTCTavernGame (engine/CTCTavernGame.h under FABLE_RETAIL_CTCTAVERNGAME: retail offsets, sizeof 0xF0, static asserts): ParentThing +0x04 (CTCBase), GameWon bool +0x21, GameFinished bool +0x28, BaseBet long +0x34, LastBet long +0x38, CurrentMoney long +0x40, YesState/NoState/AltState ETavernGameStates +0xa8/+0xac/+0xb0, TavernGameState +0xc0, NextTavernGameState +0xc8. Enum `CTCTavernGame::ETavernGameStates` (nested; header gives ETGS_* names but says enumerator names unproven -> keep numbers as landed rows do).
CThing PDef +0x70: via CTCBase/CThing retail headers (GetDef inline = `ParentThing->PeekBaseDef().GetSubDef(def)`).
CTavernGameDef (engine/CTavernGameDef.h, PDB offsets hold): Quit +0xfc, Yes +0x100, No +0x104 (draft's StartQuestionText/StartYesText/StartNoText are WRONG), MinBet +0x110, MaxBet +0x114.
CProcessedInput.h: SetAsPerformedEvent under FABLE_RETAIL_CPROCESSEDINPUT_PERFORMED (decl only).
CInputProcessGameBase.h: IsEventGameAction + IsEventButtonPressed under FABLE_RETAIL_INPUTPROCESS_ZTARGET (IsEventButtonPressed is `protected` there!), GetControlMovementFromGameActionEvent under FABLE_RETAIL_INPUTPROCESS_MOVEMENT (needs C2DVector, engine/C2DVector.h FABLE_RETAIL_C2DVECTOR); both inside FABLE_RETAIL_INPUTPROCESS. CInputProcessTavernGame.h (FABLE_RETAIL_INPUTPROCESS) adds nothing - no friend for CTCTavernGame, so calling the protected IsEventButtonPressed needs a header friend (`friend class CTCTavernGame;` in CInputProcessTavernGame is the C++-legal route; bytes unaffected).
CInputEvent.h: GetType declared (line 32, EInputEventType).
CSubtitleRenderer.h: Clear(bool) under FABLE_RETAIL_CSUBTITLERENDERER_CLEAR. CPlayerGui.h: FABLE_RETAIL_CPLAYERGUI (SubtitleRenderer +0x18).
Locally modelled in the draft but with shared headers: CTCTavernGame, CTavernGameDef, CDefPointer, CDefClassBase, CThing, CInputEvent, CProcessedInput, CInputProcessTavernGame, CSubtitleRenderer, CPlayerGui, C2DVector, CWideString.

## 5. Visibility
- ProcessInput 0x5EC270 sits between EnterGame 0x5EC0F0 and ClearBox 0x5EC5C0 in the CTCTavernGame.cpp block (0x5E9A70..0x5EDA50) = same TU as FrameUpdate 0x5EB9E0, which DEFINES GetSubDef<CTavernGameDef> (template specialization body) and SetState 0x5E9B20 in-TU (FrameUpdate.cpp lines 51-63, 81). In this TU SetState is visible: FrameUpdate/EnterGame inline it at constant-state sites, but call 0x5E9B20 at variable sites (SetState(NextTavernGameState)) - here every SetState arg is a variable, retail calls out of line. Copy FrameUpdate's idiom: define GetSubDef<CTavernGameDef> + SetState body in the TU above ProcessInput.
- GetDef inline (header default; do NOT define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE).
- IsEventGameAction / GetControlMovementFromGameActionEvent are 2-instruction forwarders, called out of line (CInputProcessGameBase TU at 0x687A90.., not visible) - declarations only.
- Hand306 LESSONS: the auto_inline(off) GetSubDef stand-in makes the def stay in a register with the null test folded - that is NOT retail here (retail keeps null test and re-reads [esp+10]); FrameUpdate's real-body form is the safer model. Expect ebx=zero to need def NOT to take a register.
- No STLport helpers called.

## 6. Siblings (copy idioms)
- 00/5e/CTCTavernGame_FrameUpdate_005eb9e0.cpp: macros CCHARSTRING, CCLASS, DEF_CLASS_BASE, CDEFSTRINGTABLE, CRESOURCELIST, CDEFINITIONMANAGER, CTCBASE, CTHING, CINTELLIGENTPOINTER, CTHINGPHYSICAL, CTHINGGAMEOBJECT, CTHINGCREATUREBASE, CTHINGAICREATURE, CTCTAVERNGAME, CPLAYERGUI, CSUBTITLERENDERER, CPLAYERMANAGER, PLAYER, CGAMECOMPONENT, CMAINGAMECOMPONENT, SCOPED_PTR; includes CTavernGameDef.h, CDefinitionManager.h, CTCTavernGame.h, CPlayerGui.h; defines GetSubDef<CTavernGameDef> + SetState in-TU; no pragmas.
- 00/5e/CTCTavernGame_SetState_005e9b20.cpp: CCHARSTRING, CCLASS, DEF_CLASS_BASE, CDEFSTRINGTABLE, CRESOURCELIST, CDEFINITIONMANAGER, CTCBASE, CTHING, CINTELLIGENTPOINTER, CTCTAVERNGAME; `extern bool g_bTavernGameQuitRequested; // 0x013BAE1C`.
- 00/5e/CTCTavernGame_EnterGame_005ec0f0.cpp (neighbour, local models + auto_inline(off) GetSubDef stand-in; def deref'd unconditionally there so no null test - differs from here).
- 00/5e/CTCTavernGame_ChangeBet_005eb810.cpp / GetTotalWinnings_005ea310.cpp: auto_inline(off) GetSubDef stand-ins (def in register idiom).
- 00/7d/CTCTavernGameCardBlackjack_ProcessInput_007da010.cpp: `#define FABLE_RETAIL_CTCTAVERNGAME` + engine/CTCTavernGame.h with the ProcessInput signature.
- SetStateAfterYesNo/YesNoAlt rows carry `// Flags: /GS`; most CTCTavernGame rows `Flags: none` / default.

## 7. Draft model errors
1. Local models everywhere (CTCTavernGame, CTavernGameDef, CDefPointer etc.) - should be shared headers with the macros above.
2. Wrong names: SetHandled -> SetAsPerformedEvent; GetEventGameActionVector -> GetControlMovementFromGameActionEvent; def->StartQuestionText/StartYesText/StartNoText -> Quit/Yes/No; g_pPlayerGui->Subtitles -> CPlayerGui::Get().SubtitleRenderer (or GetSubtitleRenderer()); GetSubDef non-template.
3. Missing const on IsEventButtonPressed/IsEventGameAction/GetControlMovementFromGameActionEvent (QBE/IBE), EGameAction param type, EInputEventType return.
4. GetSubDef not visible (declaration only) and no SetState body in TU; residue "def cached in regs, ebx not zero reg" -> try FrameUpdate's in-TU real GetSubDef + SetState; keep ebx free as zero register (all 0/false stores and `push 0` use ebx).
5. Control flow, case order, condition order, clamp ternary, AnswerQuestion tail: all match retail - keep.

## 0x004093E0 (not landed)

# DOSSIER 0x004093E0 CUserProfileManager::SetDefaultValuesVideo (841 bytes)

## 1. Retail shape (xdis 004093e0 0x359)
- __thiscall, plain `ret` (no args), no frame pointer (`sub esp,10h; push esi; push edi`), no SEH/EH frame even
  though a CDefPointer local is released at the end -> TU built without /EHsc (same as sibling GetMax*Detail 0x004084B0).
- Frame 0x10: frame+0 = colour-depth local (stored [esp+10h] after push ebx/ebp), frame+4 = the CDefPointer result slot
  (GetConfigOptionsDefaultsDef hidden ret; reloaded at the end for the release `dec [ecx+4]; jne; call [eax+4]`),
  frame+8..0x10 = ONE C2DExtentsI temp shared by both GetRenderTargetDimensions calls.
- esi=this, edi=raw def pointer (loaded once at 0x4093FE, kept all function), ebx=width, ebp=height (`or ebx,-1; or ebp,-1`
  AFTER the ClampAntialiasing call, before GFGetSystemManager), ebx/ebp pushed only at 0x4094B3 (shrink-wrapped).
- Order: Gamma=1.0f stored BEFORE the GetConfigOptionsDefaultsDef call. `fild [0137507C]` (first call's limit arg) is
  hoisted above `test edi,edi`. Null-def arm is fall-through (0x40940A); def arm is OUT OF LINE at 0x4095D4 and jumps back
  to the shared tail 0x4094AD (both arms end `lea eax,[esi+38h]; mov [eax],X` -- null arm stores edi(=0)).
- ClampAntialiasing called with `&Antialiasing` computed as `lea eax,[esi+38h]` reused from the store.
- Display-mode search 0x409543: `Resolution=0` (xor edx), guard `begin==end -> skip`, rotated loop, `inc dword [esi+48h]`
  in memory, end reloaded from [esi+84h] each iteration; compares X, Y, BitsPerPixel(depth local in ecx).
- 0x40957E..0x4095D2 + 0x409677 = INLINED ClampResolution(Resolution, g_MaxResolutionWidth) (size via sar 4, unsigned jb;
  unrotated step-down loop `cmp [eax],edi; jle; dec ecx; sub eax,10h; jmp`; store back only when moved).
- 0x40967A..0x4096C8 refresh pick: RefreshRate=0 in memory, i in ecx 1..3 (`jb` unsigned), reads [esi+4Ch] each pass,
  abs via cdq/xor/sub (intrinsic abs), `if (abs(r-t[i]) < abs(r-t[RefreshRate]) && t[i] <= r) RefreshRate=i`.
  NOT peeled (ours peeled it).
- 0x4096CA..0x4096FC = INLINED ClampRefreshRate(RefreshRate, g_MaxRefreshRate) (identical bytes to the unreferenced
  out-of-line copy 0x004069A0, 52 bytes, manifest-gaps tail of row 0x004068D0).
- Tail: ScreenAspectRatio(+74h)=0, VerticalSyncEnabled(+50h)=1, ApplyValues(), then CDefPointer release.

## 2. Identity (label is RIGHT)
- Ego_r.pdb (egor_pdb_names.tsv / egor_signatures.tsv 0x004073D0) and FableWin.pdb (_fablewin_api.tsv 0x01881E20):
  `?SetDefaultValuesVideo@CUserProfileManager@@QAEXXZ` = public: void __thiscall CUserProfileManager::SetDefaultValuesVideo(void).
- Caller: 0x004211D7 (inside 0x0041E6D3 NUISystem::CManager::Action per functions.tsv).
- Same TU (Ego_r address order, all before 0x004073D0): ApplyValues 0x405900 (IAE protected), ClampVideoOption 0x405520,
  ClampRefreshRate 0x4055E0, ClampResolution 0x405DA0, GetConfigOptionsDefaultsDef 0x406C90, ClampAntialiasing 0x407340,
  GetDisplayModes 0x4054D0.

## 3. Callees
| retail | landed row | PDB mangled | draft spelling issue |
|---|---|---|---|
| 0x004083E0 | 00/40/CUserProfileManager_GetConfigOptionsDefaultsDef_004083e0.cpp | ?GetConfigOptionsDefaultsDef@CUserProfileManager@@QBE?AV?$CDefPointer@$$CBVCConfigOptionsDefaultsDef@@@@XZ | draft returns a local `CDefPointer` struct (non-template) -> wrong; use engine/CDefPointer.h `CDefPointer<const CConfigOptionsDefaultsDef>` |
| 0x004068D0 x4 (x8 sites) | 00/40/CUserProfileManager_ClampVideoOption_004068d0.cpp | ?ClampVideoOption@CUserProfileManager@@AAEXAAMMMMM@Z = private void(float&,float,float,float,float) | draft (and landed row) use `float*` -> PDB says `float&`, private |
| 0x00409140 | 00/40/CUserProfileManager_ClampAntialiasing_00409140.cpp | ?ClampAntialiasing@CUserProfileManager@@AAEXAAKJ@Z = private void(unsigned long&, long) | draft/landed use `unsigned long*` -> `unsigned long&` |
| 0x00407370 | 00/40/CUserProfileManager_ApplyValues_00407370.cpp | ?ApplyValues@CUserProfileManager@@IAEXXZ (protected) | ok |
| 0x009A4EC0 x3 | 00/9a/global_GFGetSystemManager_009a4ec0.cpp | ?GFGetSystemManager@@YIPAVCSystemManager@@XZ (__fastcall) | ok |
| 0x009E3820 | 00/9e/CPixelFormat_GetColourDepth_009e3820.cpp | ?GetColourDepth@CPixelFormat@@QBEKXZ -> unsigned long, const | draft returns `long` -> `unsigned long` |
| 0x009BEDC0 x2 | 00/9b/CDisplayManager_GetRenderTargetDimensions_009bedc0.cpp | ?GetRenderTargetDimensions@CDisplayManager@@QAE?BVC2DExtentsI@@XZ -> `const C2DExtentsI` BY VALUE (hidden ret) | draft spells it as `const C2DExtentsI& (C2DExtentsI& out)` -> wrong; engine/CDisplayManager.h line 63 declares the right one |
| inline 0x004069A0 body | NOT landed (unreferenced COMDAT-like copy, manifest-gaps) | ?ClampRefreshRate@CUserProfileManager@@AAEXAAKJ@Z | draft hand-wrote it |
| inline 0x00406D90 body | 00/40/CUserProfileManager_ClampResolution_00406d90.cpp | ?ClampResolution@CUserProfileManager@@AAEXAAKJ@Z | draft hand-wrote it; landed row uses `unsigned long*` |
| def vtable slot 1 | CDefPointeeBase::Delete (engine/CDefPointer.h Release) | | draft local CDefBase model |

## 4. Members
CUserProfileManager (struct_layouts_egor.tsv; retail = PDB before +0x54, PDB-8 after the two input vectors):
Gamma float +0x1C, Antialiasing ulong +0x38, TextureDetail float +0x3C, ShadowDetail +0x40, MeshDetail +0x44,
Resolution ulong +0x48, RefreshRate ulong +0x4C, VerticalSyncEnabled bool +0x50, EffectsDetail float +0x70 (PDB 0x78),
ScreenAspectRatio ulong +0x74 (PDB 0x7C), DisplayModes vector<CDisplayModeDesc> +0x80/+0x84 (PDB 0x88).
- rebuild/include/fable_user_profile.h with FABLE_RETAIL_CUSERPROFILEMANAGER (used by GetMax*Detail rows) names
  Gamma..StereoReversed (+0x1C..+0x51) but +0x52..+0xCB is OPAQUE (`_pad_0x52`) -> EffectsDetail, ScreenAspectRatio,
  DisplayModes are not named anywhere shared; author must add an opt-in (or the engine/CUserProfileManager.h blocks).
- engine/CUserProfileManager.h has FABLE_RETAIL_CUSERPROFILEMANAGER_DISPLAYMODES (std::vector<CDisplayModeDesc>
  + GetDisplayModes 0x00406880) but its other layout blocks each carry own pads ("not combinable"); no video block.
- CConfigOptionsDefaultsDef (retail = PDB-4): Antialiasing +0x3C, ResolutionWidth +0x40, ResolutionHeight +0x44,
  BitDepth +0x48, TextureDetail +0x54, MaxTextureDetail +0x58, ShadowDetail +0x5C, MaxShadowDetail +0x60, MeshDetail +0x64,
  MaxMeshDetail +0x68, EffectsDetail +0x6C, MaxEffectsDetail +0x70. Only header: engine/_quarantine/CConfigOptionsDefaultsDef.h;
  GetMaxEffectsDetail_004084b0.cpp restates it as `: public CParentDefClassBase` -- copy that.
- CSystemManager.h (FABLE_RETAIL_CSYSTEMMANAGER): PDisplayManager CCountedPointer +0x60, inline GetDisplayManager().
- CDisplayManager.h (FABLE_RETAIL_CDISPLAYMANAGER): +0x1A8 = BackBufferFormat (CPixelFormat) -- draft calls it "PixelFormat";
  +0x1C8 = RefreshRate (long). FableWin GetRefreshRate is the D3D query (0x009BE340), NOT this member read.
- CDisplayModeDesc.h: Dimensions(C2DExtentsI X,Y) +0, BitsPerPixel +8, RefreshRate +0xC.
Locally modelled in draft though shared headers exist: CUserProfileManager, CConfigOptionsDefaultsDef (quarantine),
CDefPointer, CDisplayManager, CPixelFormat, CSystemManager, C2DExtentsI, CDisplayModeDesc.
Globals (established spellings): 0x0137507C g_MaxDetailA (effects cap), 0x01375074 g_MaxDetailD (texture cap),
0x01375078 g_MaxDetailB (shadow cap), 0x01375070 g_MaxDetailC (mesh cap) [5 rows each]; 0x01375098 g_DetailA, 0x01375090
g_DetailD, 0x01375094 g_DetailB, 0x0137508C g_DetailC, 0x0137509C g_MemoryA, 0x013750A0 g_MemoryB (GFConfigDetection
0x004017B0 only); 0x01375080 g_MaxAntialiasing / g_ConfigLevel, 0x01375084 g_MaxResolutionWidth / g_FableConfigWidth,
0x01375088 g_MaxRefreshRate / g_maxDisplayRefreshRate (conflicting spellings -- pick GFConfigDetection's); 0x0122DE30
g_refreshRateOptions[4] (GetMaxRefreshRate 0x00406890). Draft names g_CmdLine* are made up and inconsistent; draft also
SWAPS texture/mesh caps in names (0x01375070 is the MESH cap, 0x01375074 the TEXTURE cap).
Arg mapping (verified in disasm): Effects(+70): value g_DetailA, limit g_MaxDetailA, def EffectsDetail/MaxEffectsDetail;
Texture(+3C): g_DetailD / g_MaxDetailD / def +54/+58; Mesh(+44): g_DetailC / g_MaxDetailC / +64/+68;
Shadow(+40): g_DetailB / g_MaxDetailB / +5C/+60. width=g_MemoryA, height=g_MemoryB (if >0).

## 5. Callee-body visibility (GOTCHAS Q22 / eb / wf11)
- /O2 => /Ob2: ClampResolution (84 B, 1 out-of-line caller 0x40C3B5) and ClampRefreshRate (52 B, zero callers) are
  AUTO-INLINED here -> define their real bodies (non-inline, landed ClampResolution recipe: `*(begin()+n)` and the
  __forceinline early-return IsModeWiderThan helper giving the unrotated loop) in the TU BEFORE SetDefaultValuesVideo.
- ClampVideoOption (202 B) and ClampAntialiasing (126 B) stay calls; putting their real bodies in the TU under
  `#pragma auto_inline(off)` (or __declspec(noinline)) may tell c2 they don't capture `&member` (Q32 lesson).
- GetConfigOptionsDefaultsDef: GetMaxEffectsDetail_004084b0.cpp's documented lever -- define its real body (and the
  GetDef<T> template) in the TU; with only a declaration VC gave the def an extra frame word (84v67).
- CDefPointer members are __forceinline (engine/CDefPointer.h; default Release gives `dec [ecx+4]; jne`, matches).
- No STLport helper calls; vector reads are inline (begin/end/size).

## 6. Siblings
- 00/40/CUserProfileManager_GetMaxEffectsDetail_004084b0.cpp (4 rows in one file): defines FABLE_RETAIL_CCHARSTRING,
  DEF_CLASS_BASE, CGAMECOMPONENT, CMAINGAMECOMPONENT, SCOPED_PTR, CRESOURCELIST, CDEFINITIONMANAGER, CUSERPROFILEMANAGER,
  CSTARTUPDEFINITIONMANAGER; includes fable_string.h, fable_game.h, fable_user_profile.h, engine/CParentDefClassBase.h,
  engine/CDefPointer.h, CGameDefinitionManager.h, CStartupDefinitionManager.h; GetConfigOptionsDefaultsDef body in TU.
- 00/40/CUserProfileManager_ClampResolution_00406d90.cpp: fable_compiler.h, local CRetailVector, FABLE_FORCEINLINE IsModeWiderThan.
- 00/40/CUserProfileManager_ClampAntialiasing_00409140.cpp: same unrotated-loop trick (IsAbove), engine/CCountedPointer.h.
- 00/40/CUserProfileManager_ClampVideoOption_004068d0.cpp: FABLE_FORCEINLINE ClampToLimit.
- 00/40/CUserProfileManager_SetDefaultValuesAudio_004083b0.cpp (engine/CUserProfileManager.h AUDIODEFAULTS block).
- Flags in this class: mostly none; /GS only on string rows (LoadFileList etc.). This row: no /GS cookie, no EH.

## 7. Old draft model errors
1. Hand-written resolution/refresh clamp loops instead of the same-TU ClampResolution/ClampRefreshRate bodies (retail
   inlines them, incl. the unrotated loop shape) -> "clamp loops keep index in register" residue.
2. Refresh selection loop peeled in ours; retail keeps one rotated loop with i=1..3, RefreshRate in memory.
3. GetRenderTargetDimensions modeled as ref-out; it returns `const C2DExtentsI` by value (one shared temp slot).
4. Reference params spelled as pointers (ClampVideoOption/ClampAntialiasing/ClampResolution/ClampRefreshRate are `&`).
5. Local non-template CDefPointer/CDefBase models; GetConfigOptionsDefaultsDef only declared (frame-slot lever lost).
6. CDisplayManager member misnamed (PixelFormat -> BackBufferFormat); GetColourDepth return should be unsigned long;
   depth local likely `unsigned long`/long - retail keeps it in a stack slot (frame+0).
7. Block placement (def arm out of line, `fild` of the first limit hoisted) still unexplained; width/height should be
   ebx/ebp initialised -1 just before the GetColourDepth call (draft order already matches; residue likely from 1-5).

