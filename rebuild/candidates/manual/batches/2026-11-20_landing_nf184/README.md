# Landing nf184 (Account A, 2026-11-20): returned STL / persist entries, the three held pick-any drafts (5 rows)

Agent NF184 (round 10) took returned entries: NF180's held 0x0083EEE0, NF177's held 0x009A80B0 and NF173b's reverted 0x007502D0. It
handed back early (context) with 28 entries: 6 done, 2 done-already, 2 rejected, 2 deferred and 16 open. Its files are in
`naming/2026-11-20_agentNF184/`. The open entries go back to the queue unclaimed, and so do q11760 / q11873: NF181 locked their row and
has since landed.

- 5 rows, all RELOCATION_MATCH, every test PASS.
  - 0x0083EEE0 LHHttp2::GetUntilCRLF (q4190): a self rename carried from NF180, with no header change. The row defines two existing
    stlport/fable_containers.h opt-ins (FILL_CHAR, ORIGINAL_COPY_TRIVIAL_BACKWARD_PASS22). Its `_M_fill_insert<char>` copy then becomes
    289 bytes and grades OK against the owner 0x004110D0 (it was 266, which held the row at nf180).
  - 0x009A80B0 CBankFileManager::BankExists (q4391).
  - 0x009A7D40 GetRetailBankFilePath (q4393).
  - 0x007502D0 CTCChest::InitClosed (q6391 / q6462): onto engine/CAnimInit.h; Set -> Init, and its local CAnimInit / CAnimFrameInfo
    models are gone.
  - 0x0065F37A (q6143): CCountedPointer<CVillageTaskHorn> from engine/CCountedPointer.h; leaf-only self rename, binder-free.
- Headers (add-only, `orig` equals HEAD, default paths unchanged, pin_asserts pinned 0):
  - stlport/fable_map.h: LESS_CCHARSTRING_SIZE_BODY, STLP_TREE_INSERT_UNIQUE_DECL_ONLY.
  - fable_retail_banks.h: CBANKFILEMANAGER_CREATEDRETAILBANKS, CBANKFILEMANAGER_CREATERETAILBANK.
  - engine/CAnimInit.h: CANIMINIT_CTOR_SIZE_BODY.
  - The coordinator added one comment line to each size block: the owner sets the same pragmas, and a user row must not carry a
    row-head optimize pragma.
- Advisor decisions (owner delegated; EXTRA_RULES 2026-11-20, new section):
  - Size-pragma scaffolding. FABLE_RETAIL_STLP_LESS_CCHARSTRING_SIZE_BODY and FABLE_RETAIL_CANIMINIT_CTOR_SIZE_BODY wrap the inline body in
    `#pragma optimize`, so the pick-any copy gets its owner's body. Accepted on three checks:
    - With the macro removed (`logs/scaffold_off_*.txt` vs `scaffold_on_*.txt`), both rows still grade RELOCATION_MATCH, and the only
      changed symbol is the one COMDAT: `less<CCharString>::operator()` 41 vs 43 bytes, the CAnimInit ctor 49 vs 59.
    - The owners set the same pragmas themselves: 0x0042AAB3 `optimize("s", on)`; 0x005B33F4 `("y", off)` + `("s", on)`.
    - The block closes with `optimize("", on)`.
  - 0x009A7D40 moves its 0x009AAEB0 binding from the fake `?insert_unique@?$set@VCWideString@@...` stand-in to the STLport header
    spelling `?insert_unique@?$_Rb_tree@VCWideString@@V1@U?$_Identity@...`. The count stays at 3. The self row's body is STLport 4.6
    `insert_unique` on that tree. The PDB name sets hold no STL instance names, so the header spelling is the attested form
    (descriptive-by-library). The self row (whose Leaf line is truncated) and 0x009A7A20 are queued to converge.
- Pick-any (`logs/pickany_check.txt`):
  - OK against their owners: the CAnimInit ctor (49 = 49, 0x005B33F4), `_M_fill_insert<char>` (289 = 289, 0x004110D0), less<CCharString>
    (41 = 41, 0x0042AAB3), CBasicString operator< (88, 0x00429950), and the list iterator helpers (0x0041C780 / 0x0041C7A0 /
    0x0041C7C0).
  - The MISSING lines are owners whose Leaf is absent or a stand-in. 0x00405BA0 / 0x00405BC0 / 0x00412880 / 0x004122D0 were graded
    by the agent with the row source at each owner address (RELOCATION_MATCH, NOTES q4190). 0x0041C820 / 0x0041C830 are the folded
    iterator comparators (MATCH; also nf177 / nf180).
- Link (`logs/link_landing.log`): 2 LNK2005 against 1 at HEAD (`logs/link_control_head.log`). The extra one is
  `??0CAnimInit@@QAE@EKJW4EAnimUpdateMode@@_NKJ@Z`: an identical 49-byte copy against 0x005B33F4's strong NODUPLICATES definition
  (the nf181 0x00485400 case). The owner's port onto the header opt-in is queued.
- Rejected: q5226 / q5234. The 44-byte bodies 0x0042918C / 0x0065D5A0 are CCountedPointer ReleaseData (FableWin.pdb). The 5-byte jmp
  thunks 0x00429BF9 / 0x0065F92A are the destructors. A family fix of 9 rows per address is queued.
- Recheck (`logs/recheck.log`): rebind multi-check 1 (baseline); split gate 0 created. Includers and referencing rows 986 good, 0 bad
  (stlport/fable_map.h is widely included); owner recheck good.
- tablecheck (`logs/tablecheck.txt`): six addresses shrink (0x004110D0 3 -> 2, 0x008F8210 3 -> 2, 0x009AB4F0 / 0x009AB560 / 0x009AB5D0
  2 -> 1, 0x008F99A0), none grows. No out-of-image oracle line added.
