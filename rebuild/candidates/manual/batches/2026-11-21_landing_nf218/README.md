# Landing nf218 (Account A, 2026-11-21): nf217 follow-ups and the UnassignString family (8 rows)

Agent NF218 (owner: "we can start a new agent when one lands / finishes"). Hand-back, state files and drafts:
`pending/2026-11-21_nf218_handback/`.

- 8 rows land (1 MATCH, 7 RELOCATION_MATCH, every test PASS). The 42-row recheck (`logs/recheck.log`) covers the rows, their owners and
  same-TU copies, and every landed caller of 0x00C442F0 / 0x0041D165 / 0x0099E9B0 / 0x00C576E0; all good.
  - q12385: 0x00C442F0 is re-authored as `CircularQueue::GetInfo` (`?GetInfo@CircularQueue@@QAEAAUSQueueInfo@@AAU2@@Z`, descriptive, libpk
    has no PDB) on the landed 0x00C437C0 sibling model (MATCH); its test now links the row. Catalog / file name move from the placeholder
    `CMarkedBuffer_00c44550::GetMark`. 0x00C43810's local declaration returns `SQueueInfo&`.
  - q12384: 0x0041D165 is `?OnCloseMenus@CManager@NUISystem@@SIXXZ` (both PDBs; the self row and header already use it). 0x0047D78B
    (ResetMenus) and 0x00621720 (free CloseMenus) are respelled in place. 0x00621C80, whose stale NF217 lock was released by the
    coordinator, uses NF218's graded draft. Now 1 spelling (was 3).
  - q12386: 0x00C576E0's SourceUnqueueBuffers is `inline` with the nf86 / nf88 address helper: the owner's copy is pick-any, bytes
    unchanged, and the two SourceUnqueueBuffers link duplicates are gone. 0x00C57C90 / 0x00C73050 are untouched.
  - q12377: the self row 0x0099E9B0 is ported off its July local model onto fable_string.h (existing opt-ins), leaf `QAIXXZ` -> the
    existing protected `?UnassignString@CCharString@@IAEXXZ` (both PDBs), and its test is rewritten on the header. 0x0099EC30 moves from
    ReleaseData to UnassignString. The census compiled 25 callers: 23 were already on IAEXXZ. No row binds the old self leaf, so the rename
    lands (nf200 / nf204 rule). The remaining stand-in at the address (0x0122A5E0, `Invoke@AutoTinyMethodTarget`) is a split to queue:
    0x0099E9B0 goes 5 -> 4 spellings in reloc_bindings.
- No header change.
- Link (`logs/link_check.txt`, 42 objects): duplicates 28 at HEAD -> 27. SourceUnqueueBuffers loses 2; UnassignString gains 1. The ported
  owner is now strong beside the strong same-TU copies in 0x00859040 / 0x0099ED10 / 0x0099EE20 (all 87 B, equal bytes and targets; the
  accepted nf96a TU-mate kind; queued).
- reloc_bindings (`logs/tablecheck.txt`): 5 stale witnesses removed, 1 added; none grows. 0x0047D78B's `clear@CGlobalList` /
  `g_UIList` (HEAD too) are left out because they would grow pre-existing splits; queued.
- Not landed / queued (tag land1121nf218): the 0x01223EC0 + 0x0122A5E0 static-initializer / atexit pair (its graded probe is in the
  hand-back), 0x00C44550's placeholder name, the 0x0099B4D0 catalog name (CWideString), the UnassignString one-definition form, and the
  0x004855A3 / 0x013B8A98 list spellings.
