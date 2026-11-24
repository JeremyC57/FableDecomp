# Landing nf254 (Account A, 2026-11-22): the vector<pair<long,CCharString>> family (7 rows), L111, L114, L120 (11 rows)

Agent NF254 (L4 / L5 + 0x0074FCE0, L111, L114, L120 done; L28 / L57 planned; L37 / L129 census + plan). Hand-back:
`pending/2026-11-22_nf254_handback/` (owners.tsv for pk.py; 0074fce0_f1.cpp = form 1, the no-header alternative of decision 1).
- **L4 / L5 + 0x0074FCE0 (one unit):** 0x0074FCE0's local `FableVector_0074fce0` model (pair<int>) -> STLport
  `?_M_insert_overflow@?$vector@U?$pair@JVCCharString@@@std@@...IAEX...I_N@Z`, an explicit instantiation on stlport/fable_containers.h (3
  forms; int -> long from both PDBs (J) + all four binders' element type). 0x0074F4F0 / 0x0074F530: stand-ins -> STLport
  `__uninitialized_copy` / `__uninitialized_fill_n` for `pair<long,CCharString>*`. Binders: 0x008A9AE0 off CBoastVector (its own leaf QBE ->
  `?MsgOnBoastsMade@CGameScriptInterface@@UBE_N...` (both PDBs, vtable slot 34), `virtual` in place on its local model); 0x008ED1F0 off
  CContainerItemVector; 0x008ED440 (right name, 53-byte stand-in body) now EQ the owner (249 = 249). 0x0074FCE0 4 -> 1, 0x0074F4F0 /
  0x0074F530 2 -> 1. New add-only `FABLE_RETAIL_STLP_DESTROY_AUX_PAIR_LONG_CCHARSTRING_SIZE_BODY` at the end of fable_containers.h (size-
  pragma scaffolding: the owner 0x004DD782 sets optimize("s") itself; the `__destroy_aux` copy is then EQ its owner).
- **L114:** 0x00A978C0 (catalog `CCountedBufferPair_00a978c0`) -> `??1CDeviceIndependent@CAnimatedBlock@N3DPrimitive@@UAE@XZ`, 0x00A978A0 ->
  its `??_G`; both emitted by an anonymous-namespace harness under the define / include block of TU-mate 0x00A90AE0 (whose HEAD object
  already has both copies EQ retail); first form; 2 -> 1 each.
- **L111:** 0x00A427A0 -> `?Update@CLipSyncDataBank@@UAEXXZ` (both PDBs, vtable slot 7); its tail call binds `?Update@CBankFile@@UAEXXZ`
  (0x009CEC60 2 -> 1). New add-only `FABLE_RETAIL_CLIPSYNCDATABANK_UPDATE` (5 asserts pinned).
- **L120:** 0x00B620A0 -> the header's private `BuildBuffers` (AAE) with its four builder calls on PDB spellings; new add-only
  `FABLE_RETAIL_CENGINESKYRENDERER_BUILDBUFFERS_CALLEES`; 0x00B61DD0 2 -> 1, 0x00B620A0 2 -> 1; 0x00B61B30 QAE -> AAE in place, 2 -> 1;
  0x00B61CD0 / 0x00B61FF0 stay at 2 (count-neutral; their own rows' ports queued).
- equiv_check (all three headers) over 57,125 rows: 0 differ (a first mid-file placement shifted `__LINE__` in two tests; moved to the
  end). Link of the 11 objects + 7 TU-mates: 0 LNK2005 / LNK4006 (control link with a duplicate: 2). Table per TABLE_CHANGES.tsv (2
  deleted, 7 replaced, 1 added). Catalog / files respelled for 0x0074FCE0, 0x0074F4F0, 0x0074F530, 0x00A978C0.
- **Recheck** (`logs/recheck.log`, landed tree): the 11 changed rows + the TU-mates / binders 0x004DD782, 0x0074FE50, 0x009CEC60, 0x00A90AE0,
  0x00B61CD0 / 0x00B61DD0 / 0x00B61FF0 / 0x00B625E0, 0x00D17A80 (20 rows): 20 RELOCATION_MATCH PASS, 0 RELOC_CONFLICT.

## Coordinator rulings on NF254's questions
1. **D1 (`??_G?$pair@JVCCharString@@@std@@QAEPAXI@Z`, 33 B v owner 0x004DA68A 31 B): (a), land, queued as debt.** Unlike nf250's L86 (whose
   NEQ copy was new under the real name), landed 0x00D17A80 already defines this identical 33-byte body under this name at HEAD, so the
   link already holds a NEQ copy of it; the compiler-generated body is decided only by the end-of-TU optimize mode, and forcing that mode
   breaks the leaf (measured). The `__destroy_aux` copy counts as fully equal only once D1 is resolved (nf212 condition 3).
2. Catalog renames: applied.  3. Table lines: applied.  4. Locks: released with this landing (L37 census rows included).
Owed: the 11 tests are still HEAD host-model tests; the 0x00B61CD0 / 0x00B61FF0 ports.
