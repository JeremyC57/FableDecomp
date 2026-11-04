# Chunk 771 notes

## Results

| Address | Row | Grade | Attempts | Notes |
|---|---|---|---|---|
| 0x00AC44D0 | CVertexStreamConverter::ConvertFrom | RELOCATION_MATCH | 1 | jump table checked against retail (0x58..0x354) |
| 0x00AC4910 | CVertexStreamConverter::ConvertTo | RELOCATION_MATCH (retail_asm) | 1 | engine/GFFloatToLongNear.h; `// Retail-asm:` cites 0x00AC4999/0x00AC499D |
| 0x009E8C10 | CPersistTraits<CRGBColour>::TransferIn | RELOCATION_MATCH | 14 | TU-local CBasicString bodies, early-return "." test |
| 0x006C9490 | CPersistTraits<COpinionDeed*>::TransferIn | RELOCATION_MATCH | 15 | TU-local bool reader body; ExtraData through a bound reference |
| 0x006159A0 | CPersistTraits<CTCInventoryMap::CUIMiniMapMarker>::TransferIn | RELOCATION_MATCH | 12 | noinline Compare body; SLONG reader for GraphicIndex |
| 0x005C19A3 | CPersistTraits<NInventory::CItem>::TransferBinaryIn | RELOCATION_MATCH | 5 | size TU; pData declared first; augmentation index via a named local |
| 0x006A5D90 | CThingPhysical::ConstructFromLevelScript | DIFFER(1268v1268), 2 lines | 26 | parked; see below |
| 0x00645853 | NPlayerGui::CStatBar::DrawStatBox | not attempted | 0 | out of budget |
| 0x00762620 | NShape::CShape::StatOnSerialise | not attempted | 0 | out of budget |
| 0x00763060 | Polygon_GeneratePath_WithCirculation (catalog) | not attempted | 0 | out of budget |
| 0x00774D20 | CTCShop_SellHighlightedItem (catalog) | not attempted | 0 | out of budget |
| 0x008EEC70 | Mass::Collide | not attempted | 0 | out of budget |
| 0x009BCA20 | DisplayCenteredText | not attempted | 0 | out of budget |
| 0x00E10E60 | NScript::CQ_WaspBossScript::CWaspChaseWoman::Main | not attempted | 0 | out of budget |

Read-only vland (`vland_result.txt`): 6/6 matched rows RELOCATION_MATCH with behaviour PASS.

The seven rows marked "not attempted" got no grades. The brief asks for 10+ grades per row before parking, and that rule is not met
for them; they are left for a later chunk, not parked with a residue.

## Header overlay (`hdr/engine/`, only the parked 0x006A5D90 uses it)

hdrcheck2 run on `hdr/`: `default=same` for every file, and `same` for every existing FABLE_RETAIL_* macro. The only `DIFF` lines
are under the new sub-macros.

| Header | Sub-macro | What it adds |
|---|---|---|
| CThing.h | FABLE_RETAIL_CTHING_LEVELSCRIPT | public `virtual bool ConstructFromLevelScript(const char*)` (0x004CAA00) and protected `CWorldMap& GetWorldMap()` (0x004C79D0); also `class CWorldMap;` |
| CThingPhysical.h | FABLE_RETAIL_CTHINGPHYSICAL_LEVELSCRIPT | the virtual override declaration (0x006A5D90) |
| CThingBaseDef.h | FABLE_RETAIL_CTHINGBASEDEF | the Ego_r members at retail offsets (PDB - 4): bitfield bytes +0x41..+0x43 (Damageable = bit 6 of +0x42), Health +0x50, etc. |
| CRightHandedSet.h | FABLE_RETAIL_CRIGHTHANDEDSET_ANGLES | `CRightHandedSet(const C3DAngles&)` (0x00A229E0) |
| C3DAngles.h | FABLE_RETAIL_C3DANGLES_CTOR | inline `C3DAngles(float, float, float)` (Ego_r/FableWin ??0C3DAngles@@QAE@MMM@Z) |

None of the six matched rows needs the overlay. SIBLINGS.tsv lists no landed rows to recheck, because no landed row defines the
new macros.

## Header debt
- 0x005C19A3 still has row-local models for four things. The final vland warns SHARED_HEADER_AVAILABLE only for
  CDataInputStream and CCountedPointer. The earlier CDefinitionManager and CItem warnings are gone since the row moved to the
  shared string and definition-manager headers; it also gives RELOC_NAME for 0x005C04BF (see NAME_FIXES).
  - CMemoryDataInputStream / CDataInputStream::ReadString: the retail CDataInputStream branch has no ReadString.
  - CCountedPointer<CPerItemData>: retail calls the dtor COMDAT 0x005C04BF out of line, and the header's body is inline.
    This was not tried.
  - NInventory::CItem / CPerItemData: engine/CItem.h is a global flattened `struct CItem`, so it cannot carry the pointer's
    ctor/dtor.

  The same-TU landed siblings 0x005C2AD8 and 0x00614AF0 use the same local models. The row now uses the shared fable_string.h
  (retail CCharString) and engine/CGameDefinitionManager.h (retail CDefinitionManager branch).
- The landed ConvertStream row uses `struct CVertexStreamConverter`. Ego_r says it is a `class`, and the matched
  0x00AC44D0 / 0x00AC4910 rows mangle it as `V`.
- The landed 0x009E9080 and 0x00403FF0 persist-traits rows use old local models, not engine/CPersistTraits.h.
- EVertexStreamDataType has no PDB enumerator names in hand. The rows use descriptive VSDT_FLOAT1=0 .. VSDT_DEC3N_SCALED=13.
  Replace them if a PDB enum dump turns up.

## C++ attempts before the helper (0x00AC4910 ConvertTo)

Retail site 0x00AC4999 / 0x00AC499D: `fld dword ptr [esp+10h]; fistp dword ptr [esp+0Ch]` (round to nearest under the current
FPU mode, then a bare dword fistp).

| # | Spelling | VC7.1 3077 output |
|---|---|---|
| 1 | `(long)x` | `call __ftol2` |
| 2 | `(int)x` | `call __ftol2` |
| 3 | `(short)x` | `call __ftol2` |
| 4 | `long t = x;` | `call __ftol2` |
| 5 | `(long)(x + 0.5f)` | `call __ftol2` |
| 6 | `floor` with /Oi + `#pragma intrinsic(floor)` | `call _floor` + `__ftol2` |
| 7 | same-TU inline `Near()` helper (plain C++) | `call __ftol2` |
| 8 | `(long)(double)x` | `call __ftol2` |
| 9 | /QIfist | `fistp qword ptr` (64-bit), not the dword form |
| 10 | intrinsic `_mm_cvtss_si32(_mm_set_ss(x))` | `cvtss2si` (SSE), not x87 |
| 11 | macro form | the shared headers have no `GF_FLOAT_TO_LONG_NEAR` macro, and the brief forbids `__asm` in the row source |
| 12 | `GFFloatToLongNear` function form (engine/GFFloatToLongNear.h) | byte match: the value is stored to the parameter slot, then `fld` and the dword `fistp` |

## Callee names used (one address, one name; PDB spellings)
- CPersistTraitsSLONG::TransferIn 0x009E2590 (static fastcall)
- CPersistTraits<unsigned char>::TransferIn 0x009E9080
- CPersistTraits<long>::TransferIn 0x005D8167 (PROPOSAL)
- CPersistTraits<C3DVector>::TransferIn 0x009E84D0
- CPersistTraits<float>::TransferIn 0x0040F7E0
- CPersistTraits<CCharString>::TransferIn 0x0040E510
- CPersistTraits<CIntelligentPointer<CThing> >::TransferIn 0x006C9900
- COpinionDeed ctor 0x006C54C0
- CBasicString<char>::operator==(const char*) 0x004115A0
- CStringParser: PeekNextItemType 0x009BA900, ReadAsSymbol 0x009BA790, ReadAsString 0x009BA6F0, ReadAsFloat 0x009BA650,
  ReadAsInteger 0x009BA540, NextItemExists 0x009BA4F0, SkipPastString 0x009B9BA0, ctor(const char*, ...) 0x00414E30
- CPersistContext::PersistError 0x009BAA90
- CDataInputStream: ReadWithSrcChunkOverflow 0x00993CA0, ReadString(unsigned long) 0x00996BE0
- vector<long>::resize(n, const long&) 0x004428F0
- CCharString::operator!=(const char*) 0x0099E960
- CGameDefinitionManager::Get 0x0044C6B0
- CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410
- NInventory::CItem: ctor 0x005BEC40, operator= 0x005C0C24
- ~CCountedPointer<CPerItemData> 0x005C04BF (see NAME_FIXES)
- CThing::ConstructFromLevelScript 0x004CAA00
- CThing::GetWorldMap 0x004C79D0
- CRightHandedSet(const C3DAngles&) 0x00A229E0
- CTCParamsPhysicsBase(const C3DVector&, const CRightHandedSet&) 0x006A06E0
- ~CBaseClassNonCopyable 0x0099A300
- CBaseIntelligentPointer: ctor 0x00A01B10, GetPItem 0x00A01B50, SetPItem 0x00A01B90, dtor 0x00A01C10
- ~list<CStringParserCommentBoundaries> 0x004046B0
- ~CBaseClass 0x0099A2E0

## Parked: 0x006A5D90 CThingPhysical::ConstructFromLevelScript (DIFFER 1268v1268, 2 lines, 26 grades)

**Residue.** At 0x006A5FD4 / 0x006A5FD8, retail stores the C3DAngles temporary's AngleYZ (`mov [esp+34h],eax`) before
`fstp dword ptr [esp+20h]` (position.Y). Ours emits them the other way round. Everything else matches, including the frame, the
register choice (PhysicsTC in esi, vtable in edi) and the bitfield merge.

**Levers found.**
- A shared `return false` tail.
- `Health = MaxHealth = def.Health` (MaxHealth is stored first).
- TU-local CBasicString Compare / operator== bodies, so VC threads the null test once over the keyword chain.
- `Drawable = true` written before the hit flags.
- The C3DAngles temporary built before PhysicsTC is read. A row-local inline helper `ConstructPhysics(PhysicsTC, position,
  C3DAngles(...))` does this; with a plain local `physics` pointer the diff is 6 lines.

**Tried without effect.**
- The `physics` local in several positions, as a reference, as `const`, as `CTCBase*`.
- `position.Y = a + b` forms and a `(float)` cast.
- A named angles local or rh set (worse).
- Swapping the helper's argument order.
- A C3DAngles ctor body in XY/XZ/YZ store order (worse).

**Next ideas.**
- The retail helper may be a CThingPhysical member (an inline `SetPosAndRHSet`-like method). Look in Ego_r/FableWin for an
  inline CThingPhysical method that takes (C3DVector, C3DAngles).
- The position update may be one C3DVector `+=` of an offset vector whose Z is never written.
- Try QFE 4035.

## Not attempted (identity and analysis for the next chunk)

- **0x00762620 NShape::CShape::StatOnSerialise** (static fastcall; CTCShapeManager&, CPersistContext&, long, const C2DCoordI&,
  C2DBoxF&; `ret 0Ch`).
  - Builds section names with CCharString operator+ (0x0099F570, 0x0099F600) and CCharString from-long (0x0099F830).
  - Persist calls 0x004109A0, 0x00410BE0 and 0x00410620.
  - Then a polygon walk: 0x0066E370, 0x006F0810, 0x007642C0, 0x00760BC0, 0x00761F40, 0x00762580.
- **0x00E10E60 CWaspChaseWoman::Main.** Strings "WaspChaser" 0x012E01EC, "ChasedWomanNav" 0x012E0464 (+ index, mod-6 ring of
  nav nodes), 0x012E0438 and 0x012E0424.
  - CGameScriptInterface slots used: +0x1C, +0x20, +0x118, +0x120, +0x1B0, +0x270, +0x5B0, +0x5B4, +0x5B8, +0x7C0.
  - Other calls: global 0x0143E8F8 slots +0x160 / +0x164; scripted-thing handle 0x007E7300 / 0x007E7310 / 0x007E7360 /
    0x007E7450; CScriptThing helpers 0x00CBE2FF / 0x00CBE3EA / 0x00CBE716; 0x00CD4450 / 0x00CD4470 (a scoped object).
  - Master data at +0x14 (bytes +0x4B and +0x4F, counter +0x50).
  - The landed sibling CWaspChaser::Main 0x00E10BF0 is the template to follow.
- **0x009BCA20 DisplayCenteredText.**
  - Two CWStringParser locals built inline: CBaseClass ctor, vptr 0x01231718, then Set 0x00A0C8E0.
  - First it counts the '\n' lines with ReadAsStringUntilCharacter 0x00A0CCD0 / SkipWhitespace 0x00A0C910 and
    CWideString::operator!= 0x0099B150 against L"" 0x0122D70C.
  - Then for each line: the font's vtable +4 extent and +8 height, GFAXToVX / GFAYToVY, GFRoundV?ToNearestPixel. Three black
    shadow draws and one white Draw2DText 0x009DD8F0 (the CRenderManager2D from `[0x009A4EC0()+0x60]`).
  - The landed NInventory::CTCInventoryBase::Draw2DText 0x005BCAFE is the closest model.
- **0x008EEC70 Mass::Collide** (static fastcall, `ret 28h`): x87 only. Calls Mass helpers 0x008EEB40 (C2DVector result) and
  0x008EEA80.
- **0x00774D20 CTCShop sell.**
  - CTCShop list/map at +0x34: 0x00777120, 0x00778280.
  - Item def lookup 0x00772250 (7-dword copy).
  - std::bitset<274> test with the `_Xran` throw path (0x00BFEAF2 / 0x00BFEAEC).
  - Map finds 0x0040F020 / 0x004365B0, then 0x005BEAD5.
- **0x00645853 DrawStatBox:** `lea ebp,[esp-58h]` /O1 frame, 10 arguments.
- **0x00763060:** polygon path generation with six 0x006F0810 / 0x0066E370 pairs and six frees.

## GOTCHAS one-liners
- `CBasicString<T>::Compare` / `operator==(const T*)` bodies in the TU let VC7.1 hoist a CCharString's PStringData null test out
  of a keyword if/else chain (one `test esi,esi`, then an inline `repe cmpsb` chain and an out-of-line chain). Without them every
  compare re-tests the pointer.
- When retail keeps a member pointer in esi and its vtable in edi across argument construction (`mov esi,[ebp+60h]` once), read
  the member into a local before the call; `member->Virtual(args)` reloads `this` after the argument calls. When retail reads the
  pointer only after the argument temporaries, pass it to an inline helper as an argument (arguments evaluate right to left).
- VC7.1 shrink-wraps callee-saved pushes: a register used only inside a loop is pushed at the loop head. When retail pushes it at
  entry, look for a use outside the loop (here edi held the vtable of the post-loop virtual call).
- Ego_r CThingBaseDef sits 4 bytes lower in retail (CParentDefClassBase is 0x39, not 0x3D): Health is at +0x50 and Damageable at
  +0x42 bit 6.
- A `jmp <other function>` body at a function start (0x00838930) is the tail call of a forwarding override, not an import thunk.
  Name it after the class whose overrides call it.
- CRightHandedSet has no single-C3DVector constructor in Ego_r. The (float, float, float)-built argument is C3DAngles
  (AngleXY, AngleYZ, AngleXZ).
- A size-TU binary reader (0x005C19A3) places a value it reads before a call into a named local, so the call result is computed
  before the element address (`long index = ...; v[i] = index;`). Declaring the null pointer local before the struct local puts
  its zero store first in the frame init.
