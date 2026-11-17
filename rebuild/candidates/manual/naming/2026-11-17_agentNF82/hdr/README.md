# NF82 header overlay (round 7, land1117fu: q10989..q11008)

Every file is a copy of the current repo header with `#ifdef` opt-ins added; the base copies are in `../orig/` (same relative
paths) and were cmp-identical to `rebuild/include/` when the overlay was made and at hand-back. All macro names are new and
member-specific (none reuses an existing macro); no existing line was changed or removed, except that one `#ifdef A` in CVectorMap.h became `#if defined(A) ||
defined(B)` (same expansion without B). With none of the new macros defined each header is token-identical to the repo text:
`../tokid.log` preprocesses landed includers with and without the overlay (`cpp -P -I-`, so each quoted include resolves as after the
merge): 13 sampled includers (up to 4 per header) plus all 55 landed rows that define a parent macro my additions sit inside or test
(FABLE_RETAIL_STLP_HEAP / _SORT_INTERNALS / _POP_HEAP_AUX_PASS22 / _LIST_SIZE_DISTANCE, FABLE_RETAIL_CSYMBOLMAP,
FABLE_RETAIL_CQUESTMANAGER_SAVEGAMESTATE / _ACTIVATEMULTIPLEQUESTS, FABLE_RETAIL_CVECTORMAP_INLINE_CONST_LOWERBOUND,
FABLE_RETAIL_CTHINGDEADCREATURE_CREATE); every pair is IDENTICAL. `../tokid.sh` reruns the sampled part. The landed users of the
older NF68 macro FABLE_RETAIL_STLP_COPY_BACKWARD_NOINLINE (0x00AD7120, 0x00AD7B50, 0x00AD7B90, 0x00AD85D0) are also IDENTICAL; my
sort-TU copy_backward uses its own name (FABLE_RETAIL_STLP_SORT_COPY_BACKWARD_NOINLINE).

The root `fable_string.h` was NOT overlaid: the repo copy already carries NF80's three opt-ins and nothing more was needed (the
q11008 port 0x006C37D0 uses plain FABLE_RETAIL_CWIDESTRING `ToCharString`; FABLE_RETAIL_CWIDESTRING_TOCHARSTRING_BODY is only for the
self row 0x0099B2C0).

| Overlay file | Opt-in(s) | Used by |
|---|---|---|
| engine/CSymbolMap.h | `FABLE_RETAIL_CSYMBOLMAP_NAMESUSED` (with FABLE_RETAIL_CSYMBOLMAP): includes stlport/fable_map.h, types `NamesUsed` as the Ego_r.pdb `std::set<CCharString>` (+0x14, 12 bytes; sizeof stays 0x24) and declares Ego_r.pdb `?RemoveSymbolsOfGroupType@CSymbolMap@@QAEXABVCCharString@@@Z` (public). The `#else` arm is the repo's `unsigned char NamesUsed[12]`. | 0x009E5360 |
| stlport/fable_containers.h | `FABLE_RETAIL_STLP_DISTANCE_INPUT_NOINLINE` (not with `_LIST_SIZE_DISTANCE`): the STLport 4.6 input-iterator `__distance` body as a noinline `__cdecl` COMDAT plus `distance()`. | 0x004BB270 |
| stlport/fable_containers.h | `FABLE_RETAIL_STLP_MAKE_HEAP_BODY` (inside the FABLE_RETAIL_STLP_HEAP scope): STLport 4.6 _heap.c `__make_heap(first, last, comp, _Tp*, _Distance*)`, FABLE_STLP_FASTCALL. | 0x004E1D68 |
| stlport/fable_containers.h | `FABLE_RETAIL_STLP_UNGUARDED_LINEAR_INSERT_BODY` (inside FABLE_RETAIL_STLP_SORT_INTERNALS): the `__unguarded_linear_insert` body in place of the declaration (`#else` = the repo declaration). | 0x004DD80B |
| stlport/fable_containers.h | `FABLE_RETAIL_STLP_INSERTION_SORT_BODY`: STLport 4.6 _algo.c `__insertion_sort(first, last, comp)` (`__fastcall`), with `__linear_insert` declared unless SORT_INTERNALS carries it. | 0x004E3686 |
| stlport/fable_containers.h | `FABLE_RETAIL_STLP_UNGUARDED_INSERTION_SORT_AUX_BODY`: `__unguarded_insertion_sort_aux(first, last, _Tp*, comp)` (`__fastcall`), `__unguarded_linear_insert` declared unless SORT_INTERNALS declares it. | 0x004DFD60 |
| stlport/fable_containers.h | `FABLE_RETAIL_STLP_PARTIAL_SORT_BODY`: `__partial_sort(first, middle, last, _Tp*, comp)` (`__fastcall`) with `make_heap` / `sort_heap` declared as out-of-line `__fastcall` front ends and `__pop_heap` declared unless HEAP / POP_HEAP_AUX_PASS22 carry it. | 0x004E6E9E |
| stlport/fable_containers.h | `FABLE_RETAIL_STLP_SORT_COPY_BACKWARD_NOINLINE` (inside FABLE_RETAIL_STLP_SORT_INTERNALS): `copy_backward(first, last, result)` as a noinline `__fastcall` COMDAT (first / last in ecx / edx, result pushed), the call retail's /O2 CCharString-sort TU makes from `__linear_insert` (0x005E0FE0); the `#else` arm is the repo's inline `copy_backward`. | 0x005E1A40 |
| stlport/fable_containers.h | `FABLE_RETAIL_STLP_PARTIAL_SORT_INLINE_MAKE_HEAP` (nested in FABLE_RETAIL_STLP_PARTIAL_SORT_BODY): `make_heap` as the STLport inline front end over an out-of-line `__make_heap` declaration (unless MAKE_HEAP_BODY gives the body), in place of the out-of-line `__fastcall make_heap` declaration. With FABLE_RETAIL_STLP_HEAP the `__pop_heap` body is visible too and expands (0x005E2EB0); without it `__pop_heap` stays out of line (0x0077F2C0). | 0x005E2EB0, 0x0077F2C0 |
| engine/CVectorMap.h | `FABLE_RETAIL_CVECTORMAP_CONST_LOWERBOUND_COUNT_FIRST`: the out-of-line (noinline) const `LowerBound` with `long count=this->size();` taken before `first` (the existing FABLE_RETAIL_CVECTORMAP_INLINE_CONST_LOWERBOUND text, without making it inline). Note for the coordinator: a probe of the landed 0x004365B0 (CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const) with this opt-in defined also MATCHes, so count-first may be the one original text for every instance. | 0x004B97D0 |
| engine/CQuestManager.h | `FABLE_RETAIL_CQUESTMANAGER_PERSISTINFO_SETTERS` (nested in FABLE_RETAIL_CQUESTMANAGER_ACTIVATEMULTIPLEQUESTS, inside NQuestManager::CRuntimeQuestPersistence): the FableWin.pdb `CPersistInfo` constructors `??0CPersistInfo@CRuntimeQuestPersistence@NQuestManager@@QAE@XZ` and `..QAE@ABVCCharString@@0@Z` (members unchanged), and the setter declarations `?SetQuestScriptPersistScript@..@@QAEXABVCCharString@@0@Z` (only when SAVEGAMESTATE, which already declares it, is off) and `?SetQuestMasterDataPersistScript@..@@QAEXABVCCharString@@0@Z` (FableWin.pdb; Ego_r.pdb 0x004BA540 / 0x004BA660). | 0x004B4E30, 0x004B4F30 |
| engine/CThingDeadCreature.h | `FABLE_RETAIL_CTHINGDEADCREATURE_SETANIMATIONFILTER_OUTOFLINE` (inside NF80's CREATE block): `SetAnimationFilter(const CAnimFilterBase&)` declared only, for its owning row (Ego_d.pdb has it out of line in thing_dead_creature.obj); the `#else` arm is NF80's inline body (Create 0x00835C80 still expands it). Link note: the owning row's strong copy and an includer's pick-any COMDAT would clash only if an includer emitted the inline body out of line; Create expands it. | 0x00835AF0 |

Static asserts (`lane_merge/pin_asserts.py rebuild/include <this dir> <header>` after every edit):
- engine/CSymbolMap.h: pinned 1: `FABLE_STATIC_ASSERT_AT(72, sizeof(CSymbolMap)==0x24)` (moved by the NAMESUSED lines).
- engine/CThingDeadCreature.h: its assert was already `_AT(54, ..)` (pinned 0). engine/CQuestManager.h: all 39 asserts were already
  `_AT(n, ..)` (pinned 0; the 39 assert texts are unchanged). stlport/fable_containers.h and engine/CVectorMap.h have no asserts (pinned 0).
  A final pass over all five headers reports 0 to pin.
