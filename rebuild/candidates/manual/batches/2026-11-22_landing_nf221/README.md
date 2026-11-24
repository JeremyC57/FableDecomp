# Landing nf221 (Account A, 2026-11-22): q12403, the four NUISystem::CComponent ??1 binders (4 rows)

Agent NF221 (owner: "we can start a new agent when one lands / finishes"). Its whole hand-back, including the 42-row ReleaseData family that
does not land yet, is in `pending/2026-11-22_nf221_handback/`.

- 4 rows land (all RELOCATION_MATCH + PASS, as at HEAD): 0x0042D45D, 0x0058F016, 0x005C8459, 0x005C9A95. NF221 compiled all 115 landed
  callers of 0x004291DE / 0x00429C53 at HEAD (`pending/.../work/hb_cc.tsv`). These 4 were the only ones that bound
  `??1?$CCountedPointer@VCComponent@NUISystem@@@@QAE@XZ` at 0x004291DE (47 sites). They now bind
  `?ReleaseData@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXXZ` there (FableWin.pdb), as the other landed callers do.
  - 0x0042D45D: `FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY`. 0x0058F016: the same, in place of `DTOR_DECL_ONLY`.
  - 0x005C8459 / 0x005C9A95: `FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_NOINLINE` (`_DECL_ONLY` DIFFERs there).
- Recheck (`logs/recheck.log`, HEAD control `logs/recheck_head.log`): the 4 rows, owners 0x004291DE / 0x00429C53, and witnesses
  0x00563920 / 0x00570332 / 0x0041DED2. All 9 RELOCATION_MATCH + PASS on both trees.
- Pick-any copies v Fable.exe (`logs/pickany_vs_retail.txt`): every `??1<CComponent>` copy is now the 5-byte thunk, equal to
  0x00429C53. At HEAD these copies were 44 B, a ReleaseData body under the ??1 name. The two `ReleaseData<CComponent>` copies now equal
  0x004291DE (HEAD: 5 diffs).
- Link (`logs/link_check.txt`, 9 objects): LNK4006 duplicates 0 at HEAD and 0 at landing.
- reloc_bindings: the `??1<CComponent>` line moves from 0x004291DE (stale witness 0x00570332, which binds ReleaseData 9x and has no
  ??1 site in its function) to 0x00429C53, witness 0x00563920 (??1 15x). That removes the recheck's RELOC_CONFLICT and closes the
  0x00429C53 item of the p26 "stale table lines" queue entry.
- No header change.

## Not landed (queued, tag land1122nf221: q12538 q12539 q12540 q12541 q12542 q12543 q12544 q12545 q12546 q12547 )
- **The CCountedPointer<T>::ReleaseData family (q12226-q12229, q12402).** NF221 ported 42 rows, all RELOCATION_MATCH + PASS, with 23 self
  renames onto FableWin.pdb names. They are saved unapplied in the hand-back. Under the family rule (a family hands back whole or stays
  blocked), these wait on:
  - 0x00A5F1C0 / 0x00A634F0 (AcquireFont): they still bind Reset at 0x0041947A through local stand-ins (recipe in NOTES "Open");
  - 0x005BFF07: held. NF221's in-place respell makes its object define pick-any `??1<CGraphicDataBank>` (owned by 0x004194B5) and
    `??1<void>` from a local-model body, which fails condition (2) of the in-place rule (advisor 2026-11-20).
- NF221's unapplied NAME_FIXES: AcquireFont QBE (x2), three DeleteData self rows, and the remaining GetFont / OpenGraphicBank stand-ins.
