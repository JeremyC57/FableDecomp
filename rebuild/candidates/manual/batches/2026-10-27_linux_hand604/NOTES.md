# Hand chunk 604 (CombatDialogueManager .. global): 4 of 10 byte-exact

Ten parked rows from the parked-row retry sweep. 4 are now RELOCATION_MATCH with behaviour tests that pass under g++ and VC7.1.
6 stay parked, each with a better or equally good best file in differ/. The vland dry run against
$S/batch/chunk604_oracle.tsv passed all 4 rows (RELOCATION_MATCH PASS). Its WARNs are listed under Header debt and Label corrections.

## What matched (4)
- **libvorbis (global) 0x00C9BC10 drft_forward**, `@drft_forward@8`, /Gr. The catalog called it DiscardAsyncData. It is libvorbis
  1.0.1 smallft.c `drft_forward`. Neighbours: 0x00C9BC30 drft_backward, 0x00C9BC50 drft_init, 0x00C9BC90 drft_clear. Single
  caller: 0x00C9F0FE. Shape lesson: the parked file had a stand-in drftf1. VC7.1 gave it retail's private register convention
  (c in edi, ifac in ecx) only once the whole real chain was in the TU: drfti1..drftf1, with dradf2/dradf4/dradfg as same-TU
  `static __declspec(noinline) __stdcall` bodies (upstream text, `#pragma intrinsic(cos,sin)`).
- **CPersistContext 0x009BB0D0 CleanOutLine(long,long)**, protected. It blanks [start,end) of the string being cleaned and keeps
  the newlines. Two shape lessons:
  - `PeekData()` must be the preset-plus-if form (`text = ""; if (PStringData) text = PStringData->PData;`); the ternary form fails.
  - The loop sits in a non-static `inline` helper that takes the text pointer as a parameter. That flips the SIB base/index order.
  - The helper's name `BlankLineRange` is descriptive (no symbol is emitted). A `static inline` helper emitted its own symbol.
- **CLoadingMemoryFailureHandler 0x009D5A30 UnregisterBank(CBankFileAsync*)**. The catalog said CBankFile::PopLoadErrorQueue.
  Under the handler's critical section it finds the bank in its CArray and erases it. Shape lesson: the earlier "tag slot gap"
  park was a modelling gap. It matched with:
  - the PDB `CArray<T>::Find` wrapper over `std::find`;
  - the STLport `__find` random-access body as a same-TU noinline `__fastcall` template COMDAT (the symbol matches the landed one);
  - a `random_access_iterator_tag` with a user ctor;
  - the real STLport `erase` (`if (pos+1 != end()) __copy_trivial(pos+1, _M_finish, pos); --_M_finish;`).
- **CAIStateGroup_HandyMan 0x0091B190 Update()**. While in work hours and the work spot is alive, it walks to the spot. On arrival
  it performs CCreatureAction_UseObject. It always terminates. Shape lessons:
  - The real class chain, read from the RTTI CHD (vtable-4 -> COL -> CHD), fixes the register chains: HandyMan :
    CAIStateGroup_WorkBase<CAIStateGroupBase> : CAIStateGroupBase : CBaseClass, with PBrain at +4.
  - IsInWorkHours is an inline in WorkBase over `CTCVillage::IsWorkTime` (0x00658B20, not IsCookTime).
  - The parked `if (!a || b)` early return was split into two ifs.
  - Every callee is the Ego_r.pdb spelling. CBaseClass comes from engine/CBaseClass.h.

## Parked for Fable 5.1 (6)
- 0x008AF6E0 CombatDialogueManager::CanSaySpeech DIFFER(150v150): retail keeps the shared fail block (`pop esi ; xor al,al ; pop ebp ; ret 8`) inline after the first test and the GetInterface miss jumps back to it; VC7.1 sinks it to the end | tried: result variable, positive nesting, goto label inside the first if, `return hasTalk`, do/while(0), positive third test, `*ppOut = 0` miss store, plain-inline GetInterface (out of line, 124), same-TU IsSpeechAllowed body, flag/pragma sweep | next: lane gap: block placement (0x0085FB30 family); first respell the invented CSpeechGroupInfoMap::find as the landed 0x008AEF20 `_Rb_tree<...>::find` const symbol.
- 0x00815420 CTCHitLocations::GetBoundingSphere DIFFER(498v498): pure callee-saved swap. Retail has this=edi (pushed first) and it=ebx; ours has this=ebx and it=edi. All 70 lines are register names | tried: it at function top, `*it++` reference, it++ step, this-> spelling, STLport vector for HitLocations, if/else instead of the cached early return; front()-based (keeps this=edi, reloads begin, 21 lines) | next: replace the invented `v00..v10` CPhysicalPrimitiveBase virtuals with the real ones and try a named `const C3DVector&` for the sphere centre.
- 0x00627400 CTCQuickAccessMenu::ConstructExpressionCategoryList DIFFER(742v742): one scheduling pair. Retail emits `lea edx,[esp+2Ch] ; mov ecx,esp` (the CDeletion copy-ctor setup) right after the argument-slot push; ours emits it after `mov edi,[esi]` | tried: STLport _List_base/_STLP_alloc_proxy list ctor (750), Method set in the ctor (750), named target pointer, single-pointer list ctor (746) | next: the real NUISystem::CComponent virtual table in place of the v000 stand-in.
- 0x0065CB6B CTCVillage::ReportCrime DIFFER(747v747): murder-arm GetTC key constant 4. Ours keeps it in ebx for the key store and `cmp [eax],ebx` and pushes an immediate for bitset::test. Retail keeps it in esi for the test push and the key store and compares against an immediate | tried: shared outer stats variable (NDIFF 43), `bool hasMurderStats = GetTC`, block scope, if-wrapped GetTC, scoreboard declared first | next: build the arm on engine/CTCVillage.h plus the real CThing::GetTC template (the landed out-parameter shape) instead of the local forceinline copy.
- 0x00584B86 CTCInventory::AssignThingPC DIFFER(873v854): the !isAbility arm's `new CActionParamLong(thingID)` temporary goes to ebp-1Ch; retail puts it in the dead thingID slot [ebp+8] like the other arm. Different slots stop the two arm tails cross-jumping (+19 bytes) and shift the upper frame by 4 | tried: named value plus one push_back (865), conditional new in one push_back (853, a register select), arms reversed, block copy of thingID; the 854-byte sec1b form (named param local) has NDIFF 76 v 55 here | next: find a source form where thingID's last read is outside the temporary's full-expression (retail still reads [ebp+8] after operator new).
- 0x00919AE0 CAIStateGroup_BuyFromSeller::Update DIFFER(2220v2220): the participant loop loads `*it` straight into ebx; retail loads ecx and does `mov ebx,ecx` after the compare | tried: temporary IP, reference participant, reversed compare, static_cast, continue, non-const iterator; IP ctor as plain inline, base-init, CBaseObject local, `T* const&` | next: the CIntelligentPointer temp-ctor shape from a landed row that loads through ecx.

## Skipped
None.

## Label corrections
- 0x00C9BC10 DiscardAsyncData -> libvorbis `drft_forward` (`@drft_forward@8`).
- 0x00C98E80 FFT_2D_DispatchByRadix -> libvorbis smallft.c static `drftf1` (vland RELOC_NAME WARN).
- 0x009D5A30 CBankFile::PopLoadErrorQueue -> `CLoadingMemoryFailureHandler::UnregisterBank`.
- 0x00658B20 CTCVillage::IsCookTime -> `CTCVillage::IsWorkTime` (vland RELOC_NAME WARN; 0x00658B6B is IsCookTime).
- 0x009BB0D0: landed callers bind `?CleanText@CPersistContext@@QAEXJJ@Z`; the retail name is protected `CleanOutLine` (NAME_FIXES.tsv).

## Callee names
All matched rows bind Ego_r.pdb spellings. The vland reloc list was checked against retail:
- 0x0091B190: GetPos, GetPVillage, IsWorkTime, AIAction_MoveToPos, CCreatureAction_UseObject(CThingCreatureBase&, CThing&, bool),
  AIAction_PerformActionAndWait, ~CBaseIntelligentPointer, ~CCreatureActionBase, Terminate.
- 0x009D5A30: CCriticalSectionLock ctor `??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z` (matches ego_r.exe.signatures)
  and its dtor; the `__find` COMDAT; `_memmove`.
- 0x009BB0D0: the "" literal at 0x0129AAF4.
- 0x00C9BC10: `_drftf1@20` (same TU, static).

Descriptive (not proven) names: the inline helper `BlankLineRange` (0x009BB0D0, emits no symbol) and the test-only names in test/.
Parameter names come from the code, not the PDB.

## Header debt
- **engine/CAIStateGroup_HandyMan.h and engine/CAIStateGroupBase.h** are flat (vptr as a data member, no base chain), so
  0x0091B190 models the RTTI chain locally.
- **engine/CCreatureActionBase.h** is flat (vptr data member, no virtual dtor). Local polymorphic model; `// Layout:` comment in the source.
- **engine/CCriticalSectionLock.h** has the right data but no ctor/dtor declarations. Local model with the same members;
  `// Layout:` comment in the source.
- **Missing headers**: CPersistContext, CLoadingMemoryFailureHandler, CTCHitLocations, NUISystem::CComponent, NUISystem::CDeletion.
- **Parked rows next to existing headers**: CTCVillage.h (0x0065CB6B), CTCInventory.h (0x00584B86) and
  CAIStateGroup_BuyFromSeller.h (0x00919AE0) are not used. The parked files still carry local models and v000-style
  virtual stand-ins (00584b86, 00627400, 00815420), plus invented struct keys and a map stand-in in 008af6e0.
  `EDeletionMethod` enumerator names were not recovered, so the parked files use `(EDeletionMethod)1`; chunk 604 removed
  the invented DELETION_METHOD_* names.
