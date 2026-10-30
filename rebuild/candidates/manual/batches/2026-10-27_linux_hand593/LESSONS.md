# Chunk 593 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **STLport `_M_fill_insert` over a refcounted element (0x00AF2010):** `fill` needs a same-TU `__declspec(noinline)` body
  even though retail calls it out of line. Declared only, VC7.1 splits x_copy's inline release into `mov/dec/mov` around
  the call (330v318). With the body, it keeps retail's single `dec [eax+4]` release.
- **Where retail homes the empty tag decides helper bodies (0x00B4A2C0 vs 0x00BDFF50 / 0x00AF2010 / 0x00AF7570):**
  - Tags stored on the dead `x` argument slot (`finish - n` stored there and re-read): the tag-taking helpers
    (`__uninitialized_copy`, five-argument `__copy_backward`, and `__uninitialized_fill_n` when out of line) need same-TU
    noinline bodies.
  - Tag in its own frame byte (0x00B4A2C0, `esp+13h`): every helper stays declared only.
- **`__rotate_adaptive` (0x00B85CD0):** copy / copy_backward and their `__copy_ptrs` / `__copy` /
  `__copy_backward_ptrs` chains must be `__forceinline`. Plain `inline` leaves all six calls out of line in this large
  body (151v320).
- **`__chunk_insertion_sort` (0x00B85B90):** STLport verbatim matches. VC7.1 expands the loop's `__insertion_sort` and keeps
  the tail call out of line by itself; `__linear_insert` needs to be `inline __fastcall`.
- **LZO static chains (0x00C06F30 swd_init):** declare swd_initdict / swd_insertdict / swd_init / init_match as same-TU
  `extern "C" static __declspec(noinline)` bodies, and give init_match a dummy extern caller. VC7.1 then picks retail's
  private registers (s in ebx, dict_len in edx); the leaf is `_swd_init` (static, scl 3).
- **A virtual method's leaf mangles `UAE`, not `QAE` (0x00C11AC0).** A leaf matching no symbol makes q.py silently grade
  the largest function in the object, so a wrong leaf looks like a large DIFFER.
- **A named local for a nested call argument (0x00C11AC0):** `CEntry* entry = table->GetEntry(g, i); long state =
  GetEntryState(tag, entry);` gives retail's evaluation order and registers; the inline nested call does not. Read the
  parameter order from the retail stack offsets (here (tag, key), not (key, tag)).
- **A `void*` out-parameter copied into a typed local (0x00C1A870):** `void* value; Find(&key, &value); T* file = (T*)value;`
  frees the slot and keeps the record in a register. `(void**)&file` keeps the record in memory (343v321 -> 325v321).
- **Lane gap candidate (0x00C1A870):** retail homes the trace CString in the dead Find out-slot. VC7.1 here never packs
  them: tried a sibling block, an inline typed Lookup helper and a count local. Same family as chunk 589's
  "several short-lived EH locals homed in one dead slot". Park after 2 grades.

Review at landing:
- 0x00C16750 is bound three ways (`??0CDebugBreak@@QAE@XZ`, `??0CAssertTrace@@QAE@XZ`, `??0CTrace@@QAE@XZ`). The parked
  0x00C1A870 uses CDebugBreak (the chunk 589/590 trace model). Pick one.
- The open-file record's +4 word is `long m_pendingRequests` in the landed QueueRead 0x00C1A110. ReleaseFile's assert
  tests it unsigned (`test eax,eax ; ja`) and calls it a "Reference count"; the parked model spells it
  `unsigned long m_refCount`. Settle on one name and one type.
- 0x00C1AE50 Find is landed as `PKCTRRedblackMap_00c1ae50::Find` with `// Leaf: Find`; its callers bind
  `?Find@PKCTRRedblackMap@@QAEJPBKPAPAX@Z` and `?Find@COpenFileMap_00c1a070@@...` (NAME_FIXES.tsv).
- Every catalog label in this chunk was wrong (NOTES.md "Label corrections").
- 0x00BDF600 carries two catalog names (`_Move_backward<pair<unsigned __int64, CPersistInfo>*>` and the family-pass-5 `__copy_backward<C3DBoundingSphere*, ..., int>`). This row's match proves the C3DBoundingSphere caller; check for an ICF-folded body before picking.
