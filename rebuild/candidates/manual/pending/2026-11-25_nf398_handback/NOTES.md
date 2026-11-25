# NF398 hand-back (2026-11-25): T3 nf396 0x004DFB8F, the map<unsigned __int64, CGroupMemberData> tree unit

## Summary
- Verdict: unit b1 is gate-clean throughout; one ruling question decides it.
  - 0x004DFB8F (_M_erase, respelt self): ACCEPT.
  - 0x004E1BD2 (clear; NF396's b3 text unchanged): ACCEPT.
  - 0x00813460 (erase(first, last), ported onto stlport/fable_map.h, respelt self): HOLD_RULING Q1. It is gate-clean and becomes ACCEPT on a yes.
    - Q1 asks whether the size-pragma rule's conditions 1 and 2 are met for the _Destroy / ??_G /Os specialization.
    - On a no, land batches/b1/held/00813460_q1_no.cpp instead. That text is already measured: RELOCATION_MATCH, and its only NEQ is the 33-byte ??_G copy that HEAD already has.
    - The other rows land only together with 0x00813460, because it witnesses the line replace.
  - 0x004E350F (~_Rb_tree): ACCEPT, but separable, and outside the original claim (covered by the lock only).
    - Caveat: the compile that decides it (~CTCCreatureGroup on its header) grades DIFFER 25v19 at the reference function, because it adds a vftable store retail lacks. The Members call in that compile names ??1?$_Rb_tree.
    - If the coordinator rejects it, drop this row and the one `add ?clear -> 004e1bd2` line. The rest of the unit does not depend on it.
- Q2 and Q3 are confirm-only; neither blocks.
- TABLE_CHANGES (apply_table --dry: 0 misses, multi-target 1 -> 1, no new targets):
  - replace the 4-argument `_M_erase -> 004dfb8f` line with the STLport spelling (witness 00813460). This settles NF396 Q1 the coordinator's way: one address, one name.
  - replace `?Destroy_004dbe39@CInner@@QAEXH@Z` with `??_G?$pair@$$CB_KVCGroupMemberData@@@std@@QAEPAXI@Z` at 004dbe39 (witness 004dfb8f).
  - add `?clear -> 004e1bd2` (witness 004e350f).
  - The respelt self of 0x00813460 already has its table line (witness 00813640), so it needs no add.
- Two add-only, default-off header opt-ins (HEADERS.tsv; orig/hdr_* in both the batch and the agent root):
  - stlport/fable_containers.h: FABLE_RETAIL_STLP_DESTROY_FASTCALL.
  - engine/CGroupMemberData.h: FABLE_RETAIL_STLP_GROUP_MEMBER_TREE_CLEAR_SIZE_OWNER, plus the nested _DESTROY_SIZE_OWNER.
  - NF399 b2 also edits fable_containers.h (a different #elif, about 300 lines further on). There is no overlap; merge both.
- No CompilerFlags change, so there is no FLAGS.tsv. Tests are unchanged.
- About 54 VC7.1 compiles:
  - 6 for the first HEAD/new grades;
  - 5 evidence probes (inc, df, v2, v3, dtor);
  - 2 for the 0x004E350F grade;
  - 13 census objects;
  - about 16 for the vland pair;
  - 4 for the overlay recheck of the two equiv test rows;
  - 8 more census objects for the names of 0x004E350F (real-map-branch rows).
- Restart: the container restarted while equiv_check was running. That was the only partial step and it was rerun complete. Locks were intact. HEAD sources, tests, headers and the table were re-verified unchanged afterwards.

## Identity (two sources per respelt self)
- **0x004DFB8F.**
  - Source 1: NF396's probe (RELOCATION_MATCH).
  - Source 2, my own reading of its three direct callers:
    - the recursion at 0x004DFBA1;
    - clear 0x004E1BD2, whose only caller is the tree destructor 0x004E350F, which ~CTCCreatureGroup 0x004E5C71 calls on +0x18 (Ego_r.pdb Members, +24);
    - erase(first, last) 0x00813460, at 0x0081348D inside its inlined clear.
  - The per-node destroy is `push 0; call 0x004DBE39`. That target is ??_G of the pair: ~CBaseIntelligentPointer on +8, then delete when bit 0 of the flag is set.
- **0x00813460.**
  - Source 1: its only caller, 0x00813640, already binds this STLport spelling here (table line, witness 00813640).
  - Source 2: the body is STLport `erase(first, last)`, with clear(), erase(position) and _M_increment inlined.
- **0x004E1BD2.** NF396's evidence. My re-read: its callee 0x004DFB8F now has the same spelling as its new self.
- **0x004E350F** (NF389 pair rule: reference sites decide).
  - The probe batches/b1/evidence/probe/dtor_004e5c71.cpp compiles ~CTCCreatureGroup on engine/CTCCreatureGroup.h. VC7.1 inlines the implicit ~map, calls `??1?$_Rb_tree` at the Members call, and emits that tree destructor as exactly 23 bytes (`call clear ; free(header)`).
  - The same compile emits ~map as `jmp ??1?$_Rb_tree`. Retail's unreferenced 0x004E409B is `jmp 0x004E350F`.
  - So 0x004E350F is ~_Rb_tree. The probe leaf is DIFFER 25v19 only by the vftable store that retail's ~CTCCreatureGroup lacks; that is queued in T3.

## Extra COMDATs (rulings R1 / R10; evidence/scan_u2.tsv, tools/tier398.py)
- **Tier 1:** byte- and reloc-identical in 00813010 (shared headers):
  - ~CIntelligentPointer<const CThingAICreature>, ~pair, ~CGroupMemberData, the two iterator constructors, ??8 _Rb_tree_base_iterator, begin, end;
  - also ??9 _Rb_tree_base_iterator (definer 00783a30).
- **Tier 2, unique home:**
  - ??_G pair at 0x004DBE39;
  - _Rb_global::_M_increment at 0x004292C0;
  - clear at 0x004E1BD2 (the owner row in this batch);
  - the __fastcall _Destroy at 0x004DD6F3. This is a hand check (evidence/destroy_home_004dd6f3.txt, tools/destroyhome.py): 49 masked hits, exactly one whose call resolves to 0x004DBE39. It is not a catalog start (RULINGS Q3).
- **Tier 2, not unique:**
  - node deallocate (20 bytes /Os, 18 bytes /O2; two bodies under one name, RULINGS Q2);
  - ??E post-increment, erase(iterator), ~_Rb_tree_base.
- **Small:** the 1-byte ~alloc_proxy and ~allocator.
- No residue. scan_u2.tsv still prints RESIDUE for _Destroy only because the tool counts catalog or 16-aligned starts; see evidence/scan_u2.NOTE.txt.

## Opt-in decisions (each probe-measured; the leaf is unchanged with and without)
- **0x004DFB8F.** The default cdecl _Destroy (12 bytes) has no retail copy (residue, the NF396 Q2 class). Retail kept a __fastcall copy at 0x004DD6F3, which is the /Gr _construct.h shape. FABLE_RETAIL_STLP_DESTROY_FASTCALL gives that copy byte for byte; only that COMDAT changes.
- **0x00813460** is /O2. Without opt-ins its object adds:
  - a new NEQ _M_increment (54 v 44 bytes);
  - a new NEQ clear against the owner 0x004E1BD2 once that row is respelt (51 v 41 bytes);
  - a residue cdecl _Destroy;
  - the HEAD-existing 33-byte ??_G NEQ copy at 0x004DBE39.
- The fixes, all size-pragma scaffolding whose owners set optimize("s"):
  - FABLE_RETAIL_STLP_RB_INCREMENT_SIZE_OWNER_PASS27, an existing opt-in;
  - the new clear specialization;
  - DESTROY_FASTCALL with the /Os _Destroy specialization. This also turns ??_G into the 31-byte owner body (condition 2 is RULINGS Q1).
- **0x004E350F:** FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE and MERASE_DECL_ONLY, so it emits no clear or _M_erase copy.

## Gates
- **vland --recheck-landed**, HEAD and new at one LANDVERIFY_WORK path (FABLE_HDR_OVERLAY=hdr on both sides):
  - all four rows RELOCATION_MATCH PASS on both sides; behaviour tests PASS;
  - no WARN LITERAL_DIFF;
  - the new side loses HEAD's RELOC_CONFLICT and SHARED_HEADER_AVAILABLE warnings. STANDIN_LEAF and RELOC_NAME are the catalog labels; relabel is queued.
- The vland objects are ALL_EQ to the objects all other gates used (tools/comdats.py).
- **pk_all + pickany_vs_retail**, HEAD v new, on the repo table and on the TABLE_CHANGES table (work/troot):
  - 0 NEQ in the new objects; reach flips 0.
  - 13 census objects give identical pk results on both tables (the existing NEQs there are pre-existing debt, unchanged).
- **/FORCE link**, 4 rows + 13 census objects: duplicates 0 -> 0; unresolved 41 -> 36; nothing HEAD resolved becomes unresolved.
- **Census by header reach** (tools/reachset.py; the regex covers CGroupMemberData and every old stand-in name):
  - 229 rows, clang prefilter (evidence/clcensus.txt);
  - VC7.1 of every definer of a new name in this family, plus the generic _M_increment / ??9 definers and the only clang-fail row 008127f0. That is 13 objects: 00813010 00813640 008136a0 00813530 00813580 008137e0 008127f0 006c5d70 0070a570 00766550 00783a30 00ac8840 00ad4810.
  - The first census watch list was built before 0x004E350F joined the unit. I reran it with that row's names (clang over the same 226 rows,
    then VC7.1 of the 8 rows that take the real map branch of CTCCreatureGroup.h: 00682810 00783830 00783840 00784140 00784150 007841f0
    009341c0 00934410). No row defines or uses ~_Rb_tree, ~_Rb_tree_base, ~_STLP_alloc_proxy or ~allocator<node> of this tree.
  - New selves are all Any; none has another landed definer.
  - Removed names keep their definers or are referenced by no other row:
    - ?BuildTreeArray@LTextTreeWalkThrough@@QAEXPAUCTextTreeNode@@@Z: 5 other NoDuplicates definers; HEAD's conflict is now gone.
    - ?OnKill@@YIXPAUAutoTinyOwnedList@@@Z: many definers.
    - @OnReadFinished_2@4: definer 004dd159.
    - The 4-argument erase / clear / _M_erase and Destroy_004dbe39 / CActiveFile_helper / AutoTinyFreeListAllocation stand-ins: only these rows used them.
- **equiv_check** for the header edits, over all 57158 sources and tests: 10 differ.
  - 8 differ only by the scratch tree path in __FILE__.
  - 2 tests (0x0064E978, 0x0064AE80) differ by their fixtureAbort(__LINE__), 1905 -> 1915. Both pass vland --recheck-landed with the overlay (evidence/vland_eq.log).

## Deviations
- The first equiv_check used an unmatched scratch path, which gave 1369 path artifacts. I kept that output as work/equiv_headers_firstrun_pathartifact.tsv and reran with matched work/eq/old and work/eq/new.
- A shell heredoc ran backtick text once and blanked two snippets in T3.txt. I rewrote them.
- Census row locks were claimed (all missing or released beforehand) and released after the compiles. The four unit rows stay locked to NF398.
- No git command was run and nothing was written to the repo. vland was dry (--recheck-landed, no --land), run from work/rsroot. equiv_check was run from the repo root (it reads rebuild/build_candidates.ps1 relative to the cwd); it writes only to my folder and $S/stubinc_equiv.
