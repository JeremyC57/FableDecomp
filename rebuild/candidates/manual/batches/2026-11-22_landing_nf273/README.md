# Landing nf273 (Account A, 2026-11-22): the private Clone self rows and 0x00ADCE60's CVertexGroupBlock dtor copy (3 rows, 2 tests, 1 header)

Agent NF273 (entry 1 done; entry 2 split: 0x00ADCE60 done, 0x00AD94F0 rejected on measurement; q12659 not started). Hand-back:
`pending/2026-11-22_nf273_handback/`.
- **Entry 1, 0x006A0E50 / 0x006A0D20:** both PDBs spell the leaves `?Clone@CCreatureAction_UnsheatheItemFromInventory@@EBEPAVCCreatureActionBase@@XZ`
  (Ego_r 0x006C59B0, FableWin 0x02211670) and `?Clone@CCreatureAction_UseObject@@EBEPAVCCreatureActionBase@@XZ` (Ego_r 0x006C5980, FableWin
  0x022129F0), and the two copy ctors. 0x006A0D20 on `engine/CCreatureAction_UseObject.h` (0x006A0D50's define set minus its two copy-ctor
  opt-ins); 0x006A0E50 on 0x006A0150's define set plus a new add-only opt-in `FABLE_RETAIL_CCREATUREACTION_UNSHEATHE_COPYCTOR` in
  `engine/CCreatureAction_UnsheatheItemFromInventory.h` (copy ctor 0x006A0E80; 1 assert pinned; whole-tree equiv_check 0 of 57,125 differ).
  Each object defines only its leaf (QAE -> EBE), relocations identical to HEAD, no vtable, no pick-any copy. Each address 2 -> 1 spellings;
  slot 11 of 0x0069F0F0 / 0x006A0150 / 0x006A0D50 SAME; link: the EBE Clones unresolved with the HEAD objects, resolved with the new, 0
  duplicates. Tests: the HEAD standalone host models replaced by tests linking the rows (private leaf through a test-only `/alternatename`
  alias, copy ctor probe, test operator new for the allocation-failure path); PassPatterns kept.
- **Entry 2:** retail never calls 0x00A96DA0 from 0x00AD94F0 / 0x00ADCE60 / __destroy_aux 0x00ADC190 (whole-exe scan): it expands the dtor
  inline there; 0x00A96DA0 is the out-of-line COMDAT the linker kept. HEAD: 0x00A96DA0 the only strong definition; 0x00AD94F0's pick-any copy
  EQ (84 = 84); 0x00ADCE60's NEQ (19 v 84). **0x00ADCE60:** one declaration `~CVertexGroupBlock();` in its existing local model: leaf
  RELOCATION_MATCH with identical relocations, the NEQ copy gone, its `??_G` copy NEQ -> EQ against owner 0x00ADBE50 (30 = 30), __destroy_aux
  NEQ as at HEAD. **0x00AD94F0:** the same form grades DIFFER(420v536) (the leaf would call the dtor where retail expands it): REJECTED; its
  copy is a legitimate EQ pick-any copy. Link: 2 LNK4006 at HEAD -> 1 (0x00AD94F0's EQ copy against the owner's strong definition).
- **Table:** no change (Clone binders are vtable slots; the dtor line already binds 0x00A96DA0).
- **Recheck** (`logs/recheck.log`, landed tree): the 3 rows + 0x006A0D50, the header's includers 0x0069F0F0 / 0x006A0150 / 0x005CA68E /
  0x006666A0 / 0x00694430 / 0x006DEEE0 / 0x006DF800 / 0x006DF860, and 0x00A96DA0 / 0x00AD94F0 / 0x00ADBE50 (15 rows): 15 RELOCATION_MATCH PASS, 0 DIFFER, 0 RELOC_CONFLICT.

## Coordinator rulings on NF273's questions
1. Entry 1 lands (both Clone rows, the opt-in, the two linking tests).
2. (a) 0x00ADCE60's one-line declaration lands as a stopgap: it is inside the row's own local model (no shared-header line changes), removes
   a NEQ copy and makes its `??_G` copy EQ, leaf unchanged. It does not match retail's TU shape (retail expands the dtor inline there), so (b),
   the owner-side fix (0x00A96DA0 emitting its dtor pick-any via an inline-dtor header opt-in and the nf86 / nf88 scaffolding, then a census
   and recheck of the 33 rows with local CVertexGroupBlock models), is queued; when it lands, 0x00ADCE60 goes back to the inline dtor.
3. q12659 to the next agent.
Rule slips (one `rm -f` on a variable path inside its evidence folder; GOTCHAS read in full only after entry 1's grades; a static exe scan
during entry 1's equiv_check): noted; the next brief repeats the `rm` rule.
