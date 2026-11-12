# agentFAM06: family `_global::CIVCountedPointer_*` / `UninitCopy_CIVCountedPointer_*` (688 landed rows)

Result: **17 rows renamed** (`rows/`, RENAMES.tsv section 1), final batch `work/final.out`: 7 MATCH + 10 RELOCATION_MATCH, every
leaf confirmed with `objdump -t`; 17/17 VC7.1 behaviour tests PASS with the catalog PassPattern (`work/tests.tsv`); g++: 11 PASS,
6 PREFAIL (the unchanged landed tests fail g++ on their own: `fable_compiler.h` include / `__fastcall` stand-ins). One new
self-contained test (`rows/005f8736_test.cpp`: the old one linked the stand-in symbol). At most 5 grades on a row (`work/gradelog.txt`,
probe and final regrade included). **671 rows left** (LEFT.tsv). No git, nothing written under the repo, BUSY.txt / RESERVED rows
untouched (none of the family is in either), one compile at a time.

## What the family really is (`work/shapes.txt`, `work/xrefs.tsv`)
Grouped by masked bytes: 422 x 27-byte speed-TU release, 147 x 24-byte size-TU release, 48 x 47-byte size-TU `void f(const X&)`
assignment, 36 x 45 / 3 x 32-byte adopting Reset(T*), 8 x 26/29-byte member releases at [this+N], and 24 one-off bodies whose
catalog label was propagated onto real functions (Activate, CatchMessage, ~CTCHeroStats, ...).
- A call / jmp / absolute-reference scan of the whole image (`work/xrefs.py`, every hit re-disassembled from its function start:
  the 11 raw "absolute" hits are all bytes inside other instructions) finds callers for only 27 family addresses, and none of the
  27/24/45/47-byte bodies has one. They are COMDATs the /OPT:NOREF link kept.
- Those bodies are not CIVCountedPointer-specific: CDefPointer<T> (engine/CDefPointer.h, 249 T in FableWin.pdb, Release / ~ /
  Reset / SetDynamicCast) compiles to the same bytes as CIVCountedPointer<T> (30 T). COMDAT selection keeps one body per symbol, so
  the ~570 release bodies are about one per (member, T) across both templates, mostly CDefPointer. With no caller nothing proves
  the template, the member (Release vs ~, identical bytes) or T.
- The 47-byte bodies return nothing (no `mov eax,esi`; the CDefPointer operator= 0x00592844 is 49 bytes and returns this), so they
  are not operator=; no CIVCountedPointer member takes `const X&` and returns void; the shape is CDefPointer<T>::SetDynamicCast<U>.

## Renamed (17), per shape / T
- STLport helpers (3): `std::__uninitialized_copy<CIVCountedPointer<CManagedTextureData>*, ...>` 0x00A6AFD0 and
  `std::__uninitialized_copy<const CDefPointer<const CThingBaseDef>*, CDefPointer<const CThingBaseDef>*>` 0x0071EA10 (both /Gr,
  over fable_containers.h + CIVCountedPointer.h / CDefPointer.h); `std::fill<CLipSyncDataBankEntry*, CLipSyncDataBankEntry>` 0x00A43890
  (/Gr; the landed NGameText::CDataBankTextEntry was wrong: the caller is vector<CLipSyncDataBankEntry>::_M_fill_insert in the
  lip-sync TU).
- Implicit pair destructors (2): `std::pair<long, CDefPointer<const CContextSensitiveItemDef>>::~pair` 0x005930F3,
  `std::pair<long, CDefPointer<const CHeroAbilityDef>>::~pair` 0x005F8736 (shared std::pair + CDefPointer.h, size TU).
- Compiler-generated CTC component destructors (4): ~CTCSoundAtmosphere 0x004DB47D, ~CTCInventoryItem 0x004DC600,
  ~CTCInterestingToVillagers 0x004DC916, ~CTCCarriedActionUseRead 0x004DCDA4 (identity = the RTTI vtable whose slot 0 calls it +
  the Ego_r PDef member released + the base dtor it jumps to).
- C3DAnimationBlendState::SetBaseAnimation(const CIVCountedPointer<C3DAnimationStateBase>&) 0x00A3C2D0 (the one true CIVCountedPointer
  member use: operator= on CSharedData::BaseAnimation, over the new SHAREDDATA opt-in).
- Catalog-only (landed source already emits the PDB symbol; only the `// Leaf:` and an identity line changed) (4): CatchMessage
  0x004BFD10, ~CTCHeroStats 0x005814E5, CAnimationEntry(const CAnimationEntry&, long) 0x005DEC70, std::fill<CParticleEmitterInfo*>
  0x00AF1C70.
- Respelled landed sources (bytes unchanged) (3): CCategory::GetCategoryIdentifier 0x005BD1B3 (enum into NInventory),
  CTCQuestCompletionUI::Activate 0x005E4430 (nested CompletionType), CTCInventoryBase::GetPrerequisitesString 0x005BE3B1 (protected).
  These three and the four catalog-only rows keep their landed local models (see "Header debt"); only the spelling was fixed.

## Left (671), and why (LEFT.tsv has one line per row)
- 422 + 147 unreferenced release COMDATs, 48 unreferenced SetDynamicCast-shaped bodies, 36 + 3 unreferenced adopting Reset bodies,
  8 unreferenced member releases at [this+4] / +0x18 / +0x2C: no caller, template / member / T unprovable (above). Next idea, as
  FAM01: donor TU COMDAT sets (FableWin TU order vs retail neighbours) can bound T sets per TU, never settle Release vs ~.
- Referenced but unproven: 0x00450BBA (owner of a member at +4 whose +0x0C is the pointer; CDefinitionManager TU), 0x005BD4CE (not
  CTCWeapon::IsAnalogueLoading: CTCWeapon's WeaponDef is at +0x0C, the body reads +0x68), 0x0069A8B0 (a CGameCameraManager member
  whose PDB name is not proven; AddMeleeFlourishAbility / AddMeleeCameraMode are in neither PDB).
- 0x00A42980: proven `?BeginLoading@CLipSyncDataBankEntry@@AAEXKAAVCLipSyncDataBank@@@Z` but not respelled (its landed source is
  stand-in classes that collide with engine/CLipSyncDataBank.h; a rewrite).
- 0x00A00D90 (landed CGraphicInfoFrame::InitialiseFrame contradicts the PDB signature), 0x009BDE90 / 0x009BDFA0 (twins: both cannot
  be ??1CSubTitle).

## NAME_FIXES (section 2 not applied)
NAME_FIXES.tsv lists the wrong landed binder spellings of the renamed addresses (the CTC `_Destruct` / `AutoTinyScalarDestructor`
stand-ins, `?GetCategoryIdentifier@CCategory@@QAEJXZ`, the non-nested Activate, the pair helpers), plus three found on the way:
0x004DA967 = ??1CTCActionUseBase@@UAE@XZ (34 stand-in spellings), 0x00686830 = ??1CTCBase@@UAE@XZ (8 stand-in spellings),
0x00A3C1A0 `?Foo@C3DAnimationBlendState@@QAEXXZ` -> MakeOwnCopyOfData, and the proven BeginLoading spelling.

## Descriptive names
None. The six `Emit...Destructor` helpers in 0x005930F3 / 0x005F8736 / 0x004DB47D / 0x004DC600 / 0x004DC916 / 0x004DCDA4 only make
VC7.1 emit the implicit ??1 COMDAT (the landed precedent 0x00695F40); they are marked "not a retail function".

## Header debt
- No shared header for CTCSoundAtmosphere, CTCCarriedActionUseBase / CTCCarriedActionUseRead or CLipSyncDataBankEntry: 0x004DB47D,
  0x004DCDA4 and 0x00A43890 declare them with Ego_r.pdb's members (no padding stand-ins). Candidates for new generated headers.
- engine/CTCInterestingToVillagers.h (and probably every generated CTC header) declares a user destructor; retail's are
  compiler-generated wherever the dtor stores no vtable (opt-in IMPLICIT_DTOR added for this one class).
- engine/CTCInventoryItem.h's retail branch keeps PDef as a raw `CDefPointeeBase*` (opt-in PDEF types it).
- engine/C3DAnimationBlendState.h had no CSharedData body (opt-in SHAREDDATA).
- Rows renamed but not rewritten keep local models: 0x004BFD10, 0x005814E5, 0x005DEC70, 0x00AF1C70, 0x005BD1B3 (local CDefPointer,
  CCategory at the retail +0x14), 0x005E4430 (local CTCQuestCompletionUI), 0x005BE3B1 (local CTCInventoryBase etc.).

## Rule check
No `__asm` / `naked` / `_emit` / `volatile` / `register` / bare `__forceinline` added; no CUnproven* or invented types; no git; no
writes under the repo. The landed 005BD4CE source's `_ReadWriteBarrier` stand-in was not touched (row left).
