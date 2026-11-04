# Chunk 773 notes

## Results

| address | name | grade | grades | where |
|---|---|---|---|---|
| 00F38A10 | CSurfaceCompositor::SetBlendMode | RELOCATION_MATCH | 5 | src/ + test/ |
| 00EAF1E0 | NScript::CV_MurderTwistFinalScript::PlayBeingRumbledCutscene | RELOCATION_MATCH | 3 | src/ + test/ |
| 00671600 | CThingCreatureDef::Transfer | DIFFER(1449v1450) dl=4 | 10 | differ/ |
| 00EBE620 | NScript::CV_OracleScript::CVO_Woman::Main | DIFFER(1500v1482) dl=4 | 10 | differ/ |
| 005C2118 | CPersistTraits<NInventory::CItem>::TransferIn | DIFFER(1571v1569) dl=32 | 6 (+~25 cc-only probes) | differ/ |
| 00524290, 0069A2A0, 006B97E0, 008185B0, 00A58130, 00A8F1B0, 00F3C990 | (see STATUS.tsv) | NOT_ATTEMPTED | 0 | surveyed only |

vland (read-only, `$D/vland_result.txt`): both matched rows are RELOCATION_MATCH + behaviour PASS. There are no warnings, because the
CRenderStateCaptureBlock local was replaced by `engine/CRenderStateCaptureBlock.h`. Every test, including the differ/ tests,
passes under g++ and compiles under VC7.1 /Od.

Helpers: none. No `__asm`, `naked`, `_emit` or `volatile`. None of the rows I attempted has a rounding or asm site.

## Header overlay (`$D/hdr/`)

- `engine/CRenderStateManager.h` gets the new `FABLE_RETAIL_CRENDERSTATEMANAGER_COLOURWRITE4`. It makes
  `SetColourWriteEnable(bool,bool,bool,bool)` a `__forceinline` body building the mask
  `(r?1:0)|(g?2:0)|(b?4:0)|(a?8:0)` into `SetRenderState(mask, DeviceState.ColourWriteEnable)`. Without the macro, the
  default declaration is unchanged.
  - Evidence: `?SetColourWriteEnable@CRenderStateManager@@QAEX_N000@Z` exists only in FableWin.pdb, and retail 0x00F38F71
    stores the folded mask 7 in place.
  - Used by the matched 0x00F38A10.
  - hdrcheck2: default=same, all macros same.
- `stlport/fable_containers.h` gets the new `FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE`. It turns
  `vector::erase(first,last)` into a declaration only, because retail calls the COMDAT.
  - Used only by the parked 0x00671600, so the coordinator can drop it if that row stays parked.
  - hdrcheck2: default=same, pre-existing macros same, the new macro DIFF (expected).
- SIBLINGS.tsv lists the 11 + 55 direct landed includers. No landed source defines either new macro, so their preprocessed
  text is unchanged.
- Header debt:
  - 0x00671600 and 0x005C2118 use row-local models: CPersistContext / CThingCreatureDef, and the CCharString / CStringParser
    / CItem / CPerItemData stand-ins in the style of the landed 0x00580194 / 0x005C19A3.
  - 0x00EBE620 nests CVO_Woman in a local CV_OracleScript, because there is no shared header for it.

## Callees bound (PDB spellings)

- 0x00F38A10: GFGetSystemManager, CSystemManager::GetDisplayManager, CDisplayManager::GetRenderManager, and the
  CRenderStateManager inline setters (SetAlphaBlendEnable / SetSourceAlphaBlendMode / SetDestAlphaBlendMode /
  SetTextureOperation / SetTextureAlphaOperation / SetColourWriteEnable). These go through the retail CRenderStateManager
  header.
- 0x005C2118:
  - CStringParser: ReadAsSymbol 0x009BA790, PeekNextItemType 0x009BA900, ReadAsInteger 0x009BA540.
  - CBasicString<char>::Compare 0x00411570 (fastcall).
  - CCharString: () 0x0099E4B0, ~ 0x0099EAE0, operator!=(const char*) 0x0099E960.
  - CPersistTraits<bool>::TransferIn 0x00403FF0 and CPersistTraits<CCharString>::TransferIn 0x0040E510.
  - CPersistTraitsSLONG::TransferIn 0x009E2590 (static __fastcall, FableWin.pdb).
  - vector<long>::resize 0x004428F0.
  - CGameDefinitionManager::Get 0x0044C6B0 and CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410.
  - NInventory::CItem::CItem(long,long,const CPerItemData*,long) 0x005BEC40, CItem::operator= 0x005C0C24,
    ~CCountedPointer<CPerItemData> 0x005C04BF, free 0x00BFEA14.
  - Literals: "(" 0x0122E028, "," 0x0122E024, ")" 0x0122E020, "" 0x0122D70E, "NULL" 0x012393E4.

## Parked rows: residue and next idea

### 0x00671600 CThingCreatureDef::Transfer: DIFFER(1449v1450), 10 grades

Residue: the IsIndestructible flag merge, bit 7 of +0x148.
- Retail: `mov al,[flag]; mov cl,[old]; mov dl,[+0x149]; and cl,7f; shl al,7; or al,cl`.
- Ours: the old byte is loaded first, giving `and al,7f` (1 byte shorter).
- The other seven flags of the byte use the xor-merge form and match.

Tried, all with no effect:
- `!= 0`, `?:`, casts, `& 1`, `!!`, `| false`.
- Direct-init, a named default, a function-scope flag, a reference read.
- An unsigned char bitfield (one field and all fields).
- /G5 /G6 /G7.

An unsigned char flag local gives retail's register order but adds a `setne`. So does `!!flag && true`.

Next idea: give the source flag a non-trivial value tree that folds away after VC7.1's operand ordering. Two candidates:
- a `__forceinline` bool accessor returning a member of a local struct;
- the CPersistContext::Transfer<bool> body visible in the TU, as a same-TU noinline stand-in like 0x00580194, so the flag
  local's home changes.

### 0x00EBE620 CVO_Woman::Main: DIFFER(1500v1482), 10 grades

Residue: there are two movie handle sites, talk then hit. Retail expands the
CScriptGameResourceObjectMovieBase ctor at the talk site and calls the COMDAT 0x006E7B60 at the hit site. VC7.1 here expands
both.

Tried, all with no effect:
- the forceinline pause-ctor macro;
- TRetVoid / `_Fail_Val` / PauseMyEntitiesPlease / speech_movie spellings (the EB0830 style);
- a bool local for the hit condition, and the `?:`, `!= false`, de-Morgan and `|` forms;
- while / for / goto / do loops.

Diagnostic results:
- The budget theory is ruled out: 16 extra inline calls ahead of the loop do not flip the site.
- `/d2inlT` moves both sites together.
- Three extra nested ifs around the hit block would flip it, so retail's hit site is about 8x colder.
- `FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR` calls both sites, so it is no good here.

Next idea: per-site, the hit block has to sit lower in the frequency tree than the talk block. Candidates:
- the generated-script `if (x) {} else if (y)` shape of 0x00EE1E40 CTH_Maze, as a source the code still matches;
- or a per-site `CScriptGameResourceObjectMovieBase::InlineCopy`-style tag ctor in the shared header.

### 0x005C2118 CPersistTraits<NInventory::CItem>::TransferIn: DIFFER(1571v1569), 6 grades

Parked under budget. The control flow, calls and literals match. Two residues remain:
- Frame-slot order (24 lines). Retail packs defName -0xC, name -0x10, symbol/type -0x14, count/number -0x18,
  pData/i -0x1C. VC7.1 here gives name -0xC, symbol/type -0x10, pData/i -0x14, count/number -0x18, defName -0x1C.
- The loop index is not cached in esi. Retail does `lea esi,[eax+esi*4]` and `mov esi,[i]; inc esi; ... mov [i],esi`.

Fixed along the way:
- Shared-header-style `operator==(const char*)` with `return true / return false` arms. This threads the strcmp arm
  straight into the branch, as retail does.
- Loop arm order `!= PARSED_ITEM_INTEGER` first.
- The tail written as nested ifs. This gives `number` the `count` slot and fixed the frame size, 0x60 → 0x5C.
- boolTraits declared before stringTraits.

Probes that changed nothing (cc only):
- declaration order of pData / data / hasData / count / i / name;
- loop forms (do / continue);
- `i++`, `int` index, the operator[] spelling, a reference temp;
- `/O1`, `/Ob1`, `/Ox`;
- optimize "t" vs "s";
- by-value `TransferIn()` overloads (these add a copy ctor);
- a forceinline helper for the has-data block (worse).

Next idea: the slot order looks like the reverse of our weight order. Two things to try:
- a by-reference `CPerItemData` traits TransferIn (PDB `CPersistTraits<CPerItemData>`, if one exists) holding the has-data
  block as a real callee inlined from the TU;
- `#pragma optimize("g", off)` variants.

## Not attempted (surveyed only)

The session ran out of budget for seven rows, each 1.3–1.6 KB with unproven identities. The survey notes are in STATUS.tsv.
- 0x00A8F1B0 is a VC7.1 unroll-by-4 x87 loop (no SIMD), so it is not hand asm.
- 0x008185B0 is a fastcall angle sweep (fsin/fcos), not a polygon clipper. The catalog name looks wrong.

## GOTCHAS one-liners

- CSurfaceCompositor::SetBlendMode 0x00F38A10: VC7.1 lays switch case blocks out in source order. Write the cases in
  retail's block order (2,1,3,4) or an unchanged-source arm reloads its dest value. The 4-bool SetColourWriteEnable
  must be inline (FABLE_RETAIL_CRENDERSTATEMANAGER_COLOURWRITE4) to fold to `mov ...,7`.
- `std::equal_to<T>()` passed as a predicate temporary needs a user ctor, or VC7.1 zero-stores its byte. This is the same
  rule as `__false_type` and the iterator tags.
- `vector::erase(first,last)` is a called COMDAT in the CThingCreatureDef TU, so opt in to
  FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE there.
- NScript cutscenes (0x00EAF1E0): `inline CScriptThing::~CScriptThing() {}` in the TU reproduces retail's
  expanded-then-called dtor split. A per-guard named handle reference reproduces the Release tail.
- Script text parsers (0x005C2118): write CCharString::operator==(const char*) with `if (!PStringData) { if (strcmp(text,"")==0)
  return true; return false; }`. The one-expression `return strcmp(...)==0` form adds `sete al; test al,al` where retail
  branches on the `repe cmpsb` flags.
- Text TransferIn tails: nested `if (ReadAsSymbol()==",") { long number; ... }` lets VC7.1 share the address-taken `number`
  slot with the has-data `count`. Early returns at function scope cost a 4-byte slot.
- 0x00EBE620: the MovieBase ctor's per-site inline decision follows site frequency, not a per-function budget (16 extra
  inline calls change nothing). OUTOFLINE_CTOR is all-or-nothing per TU.
