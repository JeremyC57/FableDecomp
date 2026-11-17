# NF85 header overlays

Four overlaid headers. Each base copy is in `../orig/` and is byte-identical to the repo header as of hand-back (`cmp` at the end of the
run). Every edit is an `#ifdef` opt-in with a new member-specific macro name. None of these macro names occurs anywhere in
`rebuild/include` or `rebuild/src`.

**Token identity.** With no NF85 opt-in defined, every overlay is token-identical to its base. `work/tokid.log` holds the evidence:
`cpp -P -D_MSC_VER=1310` was run over 60 translation units, once with `-I orig` and once with `-I hdr`, and the whitespace-normalised
outputs were compared. The 60 units are one bare `#include` per header plus 15 landed includer rows per header (fewer where fewer
exist). 0 of 60 differ.

**Asserts.** `pin_asserts.py /home/user/FableDecomp/rebuild/include <dir>/hdr <header>` ran after each edit. Results:
- `fable_retail_banks.h`: the first edit pinned 2, `FABLE_STATIC_ASSERT_AT(66, sizeof CContainedBank==0x14)` and
  `_AT(67, sizeof CRetailBank==0x28)`. Later runs pinned 0.
- `stlport/fable_containers.h` and `engine/CVectorMap.h`: pinned 0 on every run.
- `engine/CQuestManager.h` (39 asserts): pinned 0 on every run.

No `FABLE_STATIC_ASSERT` was moved.

## fable_retail_banks.h
- `FABLE_RETAIL_NBANKFILEMANAGER_CCONTAINEDBANK_CTOR`
  - Adds the inline default ctor of the retail `NBankFileManager::CContainedBank` (`DataVersion(0)` .. `Alignment(0)`).
  - Evidence: FableWin.pdb `??0CContainedBank@NBankFileManager@@QAE@XZ`.
  - User: 0x009AC530, map<CCharString, CContainedBank>::operator[]. Its `__default_constructed` value fixed the frame-size DIFFER.

## stlport/fable_containers.h
- `FABLE_RETAIL_STLP_SORT_COPY_BACKWARD_AUX_INLINE`
  - Gives the SORT_INTERNALS `__copy_backward` signature an `#ifdef` arm that is plain inline. The `#else` arm is the repo text.
  - User: 0x005E0FE0 (copy_backward).
- `FABLE_RETAIL_STLP_SORT_HEAP_BODY`
  - STLport 4.6 `sort_heap` as an out-of-line `__fastcall` body.
  - `pop_heap`, `__pop_heap_aux` and `__pop_heap` are inline. `__adjust_heap` is declared unless HEAP or DECAL_HEAP already carry it.
  - Users: 0x0077F190, 0x009B8170, 0x00A41960.
  - An unused nested opt-in was removed before hand-back.
- `FABLE_RETAIL_STLP_POP_HEAP_AUX_FASTCALL_BODY`
  - Out-of-line `__fastcall __pop_heap_aux` with `__pop_heap` inline.
  - Not active together with SORT_HEAP_BODY or POP_HEAP_AUX_PASS22.
  - Users: 0x009B6D80, 0x00A41260.
- `FABLE_RETAIL_STLP_MAKE_HEAP_FRONT_BODY`
  - `__fastcall make_heap`, forwarding to `__make_heap(first, last, comp, (T*)0, (ptrdiff_t*)0)`.
  - User: 0x004E36C3.
- `FABLE_RETAIL_STLP_FINAL_INSERTION_SORT_BODY`
  - `__fastcall __final_insertion_sort` with threshold 16 and an inline `__unguarded_insertion_sort`.
  - The other helpers are declared unless their bodies are switched on.
  - User: 0x004E415D.

## engine/CVectorMap.h
- `FABLE_RETAIL_CVECTORMAP_LOWERBOUND_COUNT_FIRST`
  - In the non-const noinline LowerBound, `size()` is taken before `begin()`. The `#else` arm is the original order.
  - This is the non-const twin of NF82's CONST_LOWERBOUND_COUNT_FIRST.
  - Users: 0x004B9750, 0x004B39B0.
- `FABLE_RETAIL_CVECTORMAP_UPPERBOUND_COUNT_FIRST`
  - Applies to the body of the existing PODIUM_UPPERBOUND: adds `__declspec(noinline)` and takes the count first. The `#else` arm is the repo text.
  - Users: 0x004B9850, 0x004B39B0.
- `FABLE_RETAIL_CVECTORMAP_ERASEFROMKEY`
  - Inline `EraseFromKey(key)`: LowerBound, a test against end(), UpperBound, then `erase(lower, upper)`.
  - Evidence: FableWin.pdb `?EraseFromKey@?$CVectorMap@VCCharString@@VCPersistInfo@..@@QAEXABVCCharString@@@Z`; the Ego_d locals are
    key, lower and upper, with upper in an inner block.
  - Needs PODIUM_UPPERBOUND.
  - User: 0x004B39B0.

## engine/CQuestManager.h
- `FABLE_RETAIL_CQUESTMANAGER_CLEARQUESTPERSISTSCRIPT`
  - Adds the inline `CRuntimeQuestPersistence::ClearPersistScript(quest_name) { PersistScripts.EraseFromKey(quest_name); }`.
    - Evidence: Ego_r.pdb 0x004B9CE0 and FableWin.pdb `?ClearPersistScript@CRuntimeQuestPersistence@NQuestManager@@QAEXABVCCharString@@@Z`.
  - Adds the public declaration `CQuestManager::ClearQuestPersistScript(const CCharString&)`.
    - Evidence: Ego_r.pdb 0x004B9D90 and FableWin.pdb `?ClearQuestPersistScript@CQuestManager@@QAEXABVCCharString@@@Z`.
  - The first addition sits inside the typed LOADGAMESTATE + ACTIVATEMULTIPLEQUESTS branch.
  - User: 0x004B39B0.
