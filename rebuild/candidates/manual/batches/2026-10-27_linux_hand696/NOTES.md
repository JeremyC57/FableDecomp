# Hand chunk 696 (CScriptThing .. CTCInventoryAbilities): 10 of 10 byte-exact

All ten NA039 rows were identified from their own vtable / caller / PDB evidence first; nine of ten catalog labels were wrong
(only CTCInventoryAbilities::GetCategory was right). 1 MATCH + 9 RELOCATION_MATCH, every test PASS under g++ and VC7.1,
read-only vland: WINS 10 (no RELOC_IDENTITY, the switch row's jump table passes RELOC_JUMPTABLE).

## What matched (10)
- **CScriptThing::~CScriptThing 0x004AA840** (??1CScriptThing@@UAE@XZ, 4,187 callers) -- an empty user dtor over the retail
  engine/CScriptThing.h + engine/CCountedPointer.h: the inline CCountedPointer release gives the `dec [eax]`, Info reload,
  DeleteFunc, `delete Info`, Data/Info zero stores. The masked byte twin 0x00AC3B80 stores the C3DClothPrimitive vtable
  0x0129DF74 and tail-jumps to ~CBaseClassNonCopyable 0x0099A300: it is ??1C3DClothPrimitive (Ego_r/FableWin), a different
  symbol -- the catalog's "~C3DClothPrimitive" label on 0x004AA840 was propagated from it.
- **NUISystem::CClickable::Update 0x0055ACB0** -- vtable 0x0124BD2C (RTTI COL .?AVCClickable@NUISystem@@) slot 1; two
  `if (!WasHovered && WasX) WasX = false;` statements then `CHoverable::Update(deltaTime)` (tail jmp 0x0055B890). Retail offsets
  = Ego_r less 0x30 (WasHovered +0x160, WasLeftClicked +0x170, WasRightClicked +0x184), as engine/CHoverable.h has them.
- **CTCInventoryAbilities::GetCategory 0x005EDE37** -- /Os packed TU; a switch with three case groups (1 / explicit 0 / 2) and
  a default 0. The explicit-zero group keeps its own index-table entry (1) whose code VC merges with the default (entry 3):
  without that group the index table would differ. Jump and index tables compared byte for byte against the object.
- **CTCAnimationComplex::~CTCAnimationComplex 0x0070B5B0** -- `if (PAnimation) delete PAnimation;` then ~CTCBase; 0 callers
  because the slot-0 ??_G 0x0070BB80 inlines it.
- **Implicit destructors (no vtable store), emitted with a one-line `p->T::~T()` helper (chunk 656/657):**
  NCreatureMode::CModeJackOfBladesScream 0x00740540 and NCreatureMode::CModeBriarRoseTeleportInvulnerable 0x008C36D0 (masked
  twins; the inline `virtual ~CAnimFilterBase() {}` + implicit ~CAnimFilterOfGroup expand in the out-of-line ??1),
  CSpawnedFuncBase 0x00CDD4C0 (base from engine/CSuspendableProcessMicrothread.h; PPredicate's ~CCountedPointer stays the
  out-of-line 0x00CDD4F0 call), NScript::CV_TalentlessBardScript::CTB_Bard 0x00ED7DD0 (base from engine/CEntityScriptBase.h).
- **CMemoryDataOutputStream::PeekData 0x00995E70** (MATCH) -- CArray<char>::GetAsCArray() inlined: `size() > 0 ? &front() :
  GetZeroSizeArray()` (FableWin ?GetAsCArray@?$CArray@D@@QBEPBDXZ / ?GetZeroSizeArray@?$CArray@D@@ABEPBDXZ); `empty()` gave
  `cmp` where retail subtracts.
- **CManagedTexture::~CManagedTexture 0x00A69E20** -- `if (Data.GetRefCount() == 2) Release();` with the PDB
  CIVCountedPointer<CManagedTextureData> accessors (GetRefCount `Object ? count : 0`, Release copying Object to a local first --
  reading the member twice reloads it after the `dec`, 64v61).

## Names and evidence (all proven, two sources each; none descriptive)
| row | symbol | evidence |
|---|---|---|
| 0x004AA840 | ??1CScriptThing@@UAE@XZ | vtable 0x01238C8C RTTI + ??_G 0x004AB170; Ego_r + FableWin name |
| 0x0055ACB0 | ?Update@CClickable@NUISystem@@UAEXM@Z | RTTI COL of vtable 0x0124BD2C slot 1; Ego_r name; donor order vs CHoverable::Update |
| 0x005EDE37 | ?GetCategory@CTCInventoryAbilities@@QAE?AW4EUISpellCategory@1@W4EHeroAbility@@@Z | Ego_r donor order + sizes (307 / 72-73 bytes) between GetAbilityInventoryThingName and GetAbilityGraphicIndex; callers in ConstructPCMagicList |
| 0x0070B5B0 | ??1CTCAnimationComplex@@UAE@XZ | vtable 0x012650A4 RTTI; Ego_r dtor right after OnCreate (0xE0) = retail slot-4 OnCreate + 0xE0 |
| 0x00740540 | ??1CModeJackOfBladesScream@NCreatureMode@@UAE@XZ | ??_G 0x00740520 slot 0 of vtable 0x01267D5C (RTTI); FableWin name; Ego_r members |
| 0x008C36D0 | ??1CModeBriarRoseTeleportInvulnerable@NCreatureMode@@UAE@XZ | ??_G 0x008C36B0 slot 0 of vtable 0x01280228 (RTTI); FableWin name; Ego_r members |
| 0x00995E70 | ?PeekData@CMemoryDataOutputStream@@QBEPBXXZ | Ego_r donor order + gaps (four neighbours); landed callers |
| 0x00A69E20 | ??1CManagedTexture@@UAE@XZ | vtable 0x0129C880 RTTI + ??_G 0x00A00620; Ego_r + FableWin name |
| 0x00CDD4C0 | ??1CSpawnedFuncBase@@UAE@XZ | ctor 0x00CDD450 vtable 0x012C2F9C, ??_G 0x00CDD4A0; Ego_r + FableWin name; FableWin donor order to the CCountedPointer dtor |
| 0x00ED7DD0 | ??1CTB_Bard@CV_TalentlessBardScript@NScript@@UAE@XZ | ??_G 0x00ED7DB0 of vtable 0x012F3494 stored by the landed CTB_Bard ctor 0x00ED7D50; Ego_r + FableWin name; Ego_r members |

Callee names: every callee uses its reloc_bindings spelling (checked by address with calls.py): ~CAnimateThing 0x0070BA90,
~CTCBase 0x00686830, ~CBaseIntelligentPointer 0x00A01C10, ~CCharString 0x0099EAE0, ~CModeStandBase 0x0073D860,
~CEntityScriptBase 0x00F35B40, CManagedTexture::Release 0x00A69AD0, ~CSuspendableProcessMicrothread 0x00A44620 (MAE),
~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC, CAnimFilterBase vtable 0x0122FFCC. New binding (no prior spelling):
0x00CDD4F0 ??1?$CCountedPointer@$$CBVCPredicateBase@NScript@@@@QAE@XZ (vland RELOC_NAME warning: catalog label
`_global::CCountedPointer_Release_00cdd4f0`). No new name collides with a landed row's spelling.
Enumerators of EHeroAbility / EUISpellCategory: no ordinal source (the PDB name sets carry no enum values), cases are numeric.

## Parked for Fable 5.1 (0)

## Skipped
None. Extents: all ten oracle extents are right (0x00995E70 ends at its `ret` with one int3 before 0x00995E80, the unlisted
CopyDataToString; 0x005EDE37's 72 bytes include its jump table and index table and end where GetAbilityGraphicIndex starts).

## Label corrections
- 0x004AA840 C3DClothPrimitive::~C3DClothPrimitive -> CScriptThing::~CScriptThing (twin 0x00AC3B80 is the real ~C3DClothPrimitive).
- 0x0070B5B0 CAInputProcess::~CAInputProcess -> CTCAnimationComplex::~CTCAnimationComplex.
- 0x00CDD4C0 / 0x00740540 / 0x008C36D0 / 0x00ED7DD0 "_Dest_val<std::allocator<CMacro>,CMacro>" -> ~CSpawnedFuncBase,
  ~CModeJackOfBladesScream, ~CModeBriarRoseTeleportInvulnerable, ~CTB_Bard (four classes' implicit destructors).
- 0x0055ACB0 CClickable::Update -> NUISystem::CClickable::Update (namespace).
- 0x00A69E20 CEnginePrimitiveHandle::Clear -> CManagedTexture::~CManagedTexture.
- 0x00CDD4F0 `_global::CCountedPointer_Release_00cdd4f0` -> CCountedPointer<const NScript::CPredicateBase>::~CCountedPointer.
- 0x00ED7DB0 (landed as CLandscapeBackgroundPatch::`vector_deleting_destructor') -> ??_GCTB_Bard@CV_TalentlessBardScript@NScript@@UAEPAXI@Z.
- 0x00F35B40 catalog "~CFrontEndScreen" -> ~CEntityScriptBase; 0x00686830 catalog "~CChunkedFileChunk" -> ~CTCBase (bindings
  carry twelve invented spellings; NAME_FIXES.tsv).

## Header debt
- engine/CMemoryDataOutputStream.h types Data as opaque `unsigned char[0xc]` (it is CArray<char>) and engine/CDataOutputStream.h
  flattens the vptr: 0x00995E70 models both locally.
- engine/CTCBase.h, engine/CModeStandBase.h (also `struct`), engine/CHoverable.h (also `struct`, global namespace; retail
  NUISystem::CHoverable is `V`) flatten the vptr into a data member: 0x0070B5B0, 0x00740540, 0x008C36D0, 0x0055ACB0 use local
  polymorphic models with `// Layout:` lines.
- engine/CSpawnedFuncBase.h (FABLE_RETAIL_SUSPENDABLE_PROCESS) declares a user `virtual ~CSpawnedFuncBase();` -- retail's is
  implicit (no vtable store) -- and flattens Name / AssociatedRegionName / PPredicate into raw pointers; it should carry
  `CCharString Name, AssociatedRegionName; CCountedPointer<const NScript::CPredicateBase> PPredicate;` and no dtor declaration.
- engine/CCountedPointer.h's inline ~CCountedPointer expands in ~CSpawnedFuncBase where retail calls the out-of-line
  0x00CDD4F0 (76v35, probe 3): 0x00CDD4C0 keeps a local declaration-only CCountedPointer.
