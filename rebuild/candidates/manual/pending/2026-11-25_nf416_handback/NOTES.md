# NF416 hand-back (2026-11-25): T3 nf409 R-NF409-1 (port, lever, debt) and nf401 0x00AA9190 identity

| batch | rows | verdict | one line |
|---|---|---|---|
| b1 | 0x005BF91A, 0x005BF969 | ACCEPT | ??_GCItem, erase, __copy, __destroy_aux and ~CCountedPointer copies EQ; needs one add-only header opt-in |
| b2 | 0x005C1ECC | ACCEPT | y-off dropped, ~CItem implicit, __fastcall _Destroy: every touched copy EQ, leaf-referenced __copy included |
| b3 | 0x00AA7DB0 (owner), 0x00AA7DF0, 0x00AA9190 | ACCEPT | Option S unit lib_chunked_file.obj on engine/CChunkedFile.h; real ??1CChunkedFile@@UAE@XZ = 0x00AA7DF0 |
| b4_hold | 0x00621A20 | HOLD | TRANSFERIN cannot give this /O2 TU the size-mode ??_G; measurement only, lock released |

Each batch has its own folder holding RESULTS, CHECKS, UNITS, TABLE_CHANGES (none) and logs/evidence; b3 also has SELF_SYMBOLS and b1 HEADERS.
The header change is at the agent root: hdr/stlport/fable_containers.h, with the original in orig/hdr_fable_containers.h.
NAME_FIXES.tsv, T3.txt and RULINGS_NEEDED.tsv (empty) are at the root too.

## Task 1: R-NF409-1 (a), 0x005BF91A and 0x00621A20 with FABLE_RETAIL_NINVENTORY_CITEM_TRANSFERIN
- **0x005BF91A.**
  - TRANSFERIN alone turns its ??_GCItem copy EQ at 0x005C1390 (28 -> 31 B, it now calls ~CCountedPointer 0x005C04BF), and the leaf
    still matches.
  - That alone also changed the cdecl __destroy_aux / _Destroy copies into bodies with no retail home: R1 residue, reached through the
    already-NEQ erase copy.
  - Root cause: this TU's erase copy (52 B, NEQ v 0x005C1ECC, leaf-referenced) calls an out-of-line cdecl _Destroy(first, last).
    Retail's erase calls __fastcall __destroy_aux 0x005C18C6 directly.
  - Retail also keeps unreferenced __fastcall copies of the dispatch functions, each filling a gap between catalog functions:
    0x005C1937 __destroy (15 B, ret 4), 0x005C1996 _Destroy(first, last) (13 B) and 0x005C149E _Destroy(CItem*) (8 B).
  - So the TU is /Gr for free templates, with the dispatch expanded. b1 adds:
    - FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL (existing);
    - FABLE_RETAIL_STLP_DESTROY_FASTCALL (existing, NF398);
    - FABLE_RETAIL_STLP_DESTROY_DISPATCH_FORCEINLINE_FASTCALL (new: the DISPATCH_FORCEINLINE text with __fastcall).
  - Result: every bound copy is EQ. ??_G 0x005C1390, erase 0x005C1ECC, __copy 0x005C12FA, __destroy_aux 0x005C18C6 and
    ~CCountedPointer 0x005C04BF.
  - Each NOADDR copy has a unique tier-2 home whose relocation resolves to its bound callee: 0x005C149E / 0x005C1996 / 0x005C1937,
    all unreferenced. The lane's pk_all --addrs agrees (EQ at each). ??1CItem is tier 1 (8 landed definers).
  - The new ??_GCItem copies are byte- and reloc-identical to the landed home row 0x005C1390's copy.
- **0x00621A20: HOLD.**
  - The leaf matches with TRANSFERIN, but the ??_G copy becomes 74 B (this is an /O2 TU, so ~CCountedPointer is expanded) and stays NEQ.
  - The NEQ __destroy_aux<CItem*> copy (vs 0x005C18C6) flips reach n -> y, through the leaf-referenced __destroy_aux<CCategory*>.
  - Not handed back as a row; reworded as T3 debt.

## Task 2: the lever (R-NF409-1 b), 0x005BF969 and 0x005C1ECC
- **Measured.** The y-off pragma frames only end-of-TU template instances and compiler-generated members. Comparing COMDATs with and
  without it (no CompilerFlags in either row), the leaf and the erase copy are byte-equal; __copy, ??_G, _Destroy and __destroy change.
  - Moving the pragma to the end of the row changes nothing: VC7.1 takes template-instance mode at the end of the TU.
  - An early scaffolding helper that emits ??_G before the pragma does work for 0x005C1ECC. It was not used: it is a new form of
    scaffolding that would need a ruling, and the simpler fix below makes it unnecessary.
- **The fix is to remove the pragma, not move it.** The retail evidence for that is in the same retail TU:
  - The landed same-TU row 0x005BF91A has no y-off.
  - Retail ??_G 0x005C1390 and __copy 0x005C12FA are frameless.
  - Retail erase 0x005C1ECC is framed, and our erase leaf is framed without the pragma too.
- **0x005BF969 (b1):** drop the pragma and add b1's four opt-ins. Each was checked alone with the others in place (nf398;
  evidence/leave_one_out_b1.txt). Keeping the pragma re-frames __copy and ??_G.
- **0x005C1ECC (b2):** drop the pragma, make ~CItem implicit (delete the local declaration) and spell the local _Destroy(T*) __fastcall.
  Each change alone leaves a NEQ (evidence/leave_one_out_b2.txt).
  - IMPROVED: __copy (leaf-referenced), ??_GCItem, ??4CItem and ??4CCountedPointer.
  - ShareData goes NOADDR -> EQ 0x005C04EB.
- **Link order.** 0x005BF91A / 0x005BF969 / 0x005C1ECC now all carry EQ ??_GCItem and erase copies, so an address-ordered link no
  longer picks a wrong body for them.

## Task 3: nf401 0x00AA9190 identity
- **Where the real ~CChunkedFile is.** It is 0x00AA7DF0, its own 74-byte body (it is not inlined everywhere). It was landed under the
  wrong spelling ??1CChunkedFile@@QAE@XZ (non-virtual, row-local CBase model).
- **Name, two sources.** FableWin.pdb 0x03469490 and Ego_r.pdb 0x00C1C4F0, both in lib_chunked_file.obj (Ego_r index 31, right after
  the ctor at index 30).
- **Retail.**
  - A raw immediate scan for vftable 0x0129E4E8 finds three stores: the ctor, 0x00AA7DF0 and the ??_G. 0x00AA7DF0 is the only
    non-deleting one.
  - The C3DMeshFile / C3DAnimFile ??_G 0x00A9D2A0 / 0x00A9A730 call it directly.
  - It is virtual: slot 0 of the vftable is the ??_G 0x00AA9190.
- **Why the rows had to change together.**
  - HEAD 0x00AA9190's object strongly defined ??1CChunkedFile@@UAE@XZ, so respelling 0x00AA7DF0 alone would duplicate it.
  - HEAD 0x00AA7DB0 (ctor, a stand-in self) emitted a 30-byte NEQ ??_G, which an address-ordered link meets first.
- **b3.** One Option S unit, owner 0x00AA7DB0 (lowest address, newly authored owner), on engine/CChunkedFile.h. It holds a
  five-line TU: the ctor and an empty ~CChunkedFile.
  - The thin rows 0x00AA7DF0 and 0x00AA9190 include it.
  - All three leaves grade RELOCATION_MATCH. Each thin object is section-identical to the owner object.
  - pk_all on the troot (repo table plus SELF_SYMBOLS): ctor EQ 0x00AA7DB0, ??1 EQ 0x00AA7DF0 and ??_G EQ 0x00AA9190, from every leaf.
  - The two CCountedPointer<CChunkedFileSuperChunk> COMDATs are tier 1 (identical in 9 landed definers / 0x00AD1E80).
  - ??_7 has slot 0 -> ??_E (weak, defaulting to ??_G) = retail 0x0129E4E8's 0x00AA9190. HEAD 0x00AA7DF0's object had a stand-in
    ?VSlot0 there.
  - s_unit_check: OK.
- **Binders.**
  - The table already bound the ctor's real spelling. Its 8 references in the census link did not resolve at HEAD; they now do.
  - Four landed loaders (0x00A99420 / 0x00A99510 / 0x00A9B640 / 0x00A9B740) reference ??1CChunkedFile@@UAE@XZ only from a 5-byte
    pick-any ??1C3DAnimFile / ??1C3DMeshFile `jmp` thunk. Their leaves do not reference it, and retail has no such thunk (no E9 to
    0x00AA7DF0). The name now has one strong definer, the unit.
  - A census pk over 24 CChunkedFile objects, HEAD table v troot, shows no verdict change.
  - vland under LANDVERIFY_STRICT_LEAF=1 passes all three S members.

## Gates (per batch, details in CHECKS.tsv)
- **vland --recheck-landed.** HEAD and new at one LANDVERIFY_WORK path (tools/runpair.sh v1, one call for all six rows).
  - Every row: RELOCATION_MATCH PASS on both sides, 0 LITERAL_DIFF.
  - New grades with FABLE_HDR_OVERLAY=hdr (the header, and the thin rows' owner include resolving to the agent copy src/compiled/...;
    a grading device, not a hand-back file). HEAD grades with the repo tree.
  - The unit's SHARED_HEADER_AVAILABLE warnings are gone. The remaining warnings are present at HEAD too: 0x005C1ECC SHARED_HEADER
    CItem plus two RELOC_NAME catalog labels, and 0x00AA9190 STANDIN_LEAF (catalog label).
- **Tests.** Fixtures follow the rows: implicit ~CItem through a counting PPerItemData member, and a virtual ~CChunkedFile deleted
  through a base pointer. Six PASS, and six mutation controls FAIL (logs/mutation_controls.txt).
- **objsec.** Compared by COMDAT symbol (evidence/objsec). Leaf sections are EQ, or differ only by renames for the unit.
- **pk_all + pickany_vs_retail, HEAD v new (tools/pkflip.py).** 0 REGRESS, 0 NEW_NEQ, 0 REACHFLIP in all four objects; 10 copies go NEQ->EQ and 2 go NOADDR->EQ.
  The only bound NEQ left is ReleaseData at 005c04bf in 0x005C1ECC, unchanged and booked as T3 multiname.
- **Census /FORCE link.** 67 HEAD objects against 65 new (the unit counted once; 0x005C1390 and 0x005C18C6 included).
  - Duplicates 16 = 16, the same set, none on these names.
  - Unresolved: nothing new, and ??0CChunkedFile (real spelling) went from 8 unresolved to resolved.
  - Census: a clang prefilter over the 755 rows that reach the NInventory headers (38 rows), plus the 27 rows that mention CChunkedFile
    / C3DMeshFile.h / C3DAnimFile.h. All compiled with VC7.1 (work/hobj).
- **Other gates.** apply_table --dry: 0 misses (there are no table changes). reserved_check (RESERVED_REFS=""): 0 reserved. The one
  mention is 0x005BFD5E, a caller cited in 0x005C1ECC's existing comment.
- **Header.**
  - Add-only: no row or header defines the new macro.
  - clang -E of the header is identical with and without the change under every existing destroy-opt-in combination.
  - The new branches use FABLE_FORCEINLINE, so a guarded #include "fable_compiler.h" is added at the top. That is the
    BOOL_FILL_OWNER pattern already in the file.

## Locks
- Kept NF416 (ACCEPT): 005bf91a, 005bf969, 005c1ecc, 00aa7db0, 00aa7df0, 00aa9190.
- Released (HOLD): 00621a20.
- Census compiles were read-only HEAD compiles of other rows.

## Deviations / notes
- Nothing was written to the repo, git was not run, and wineserver was never killed. No cl hung.
- The lane's pk_all and objsec tools were used as-is. Helpers added under tools/:
  - rdis.py: retail disassembly.
  - immscan.py: raw 32-bit immediate scan.
  - mscan.py: reloc-masked retail scan with the decoded targets.
  - probe.py: lock-checked VC7.1 probe compile.
  - trun.py: behaviour test / mutation control.
  - pkflip.py: pk_all HEAD v new verdict / reach diff.
  - mkrows.py, pack.py, tsvs.py: packaging.
- Descriptive names: none. Every name is a PDB spelling, or the repo's STLport spelling of a PDB-typed instance.
