# Landing nf189 (Account A, 2026-11-20): returned STL / counted pointer / string template instances, second pass (18 rows)

Agent NF189 (round 10) handed back early (context) with 77 entries: 9 done, 2 done-already, 7 rejected, 5 blocked and 54 open (40 not
started, 14 census-only; census in `naming/2026-11-20_agentNF189/APPLIED.tsv` and `work/probe.tsv`). The open entries and the partial
q4774 / q6206 / q6207 go back to the queue unclaimed (57 entries).

- 18 rows land: 1 MATCH (0x00A99FF0) + 17 RELOCATION_MATCH, every test PASS.
- Self renames (6): 0x00A99FF0 CCountedPointer<CChunkedFileChunk>::DeleteData, 0x00579EA3 the EHeroTrainableStatType tree's `clear`
  (was the bare leaf `OnKill`), 0x00A80A20 `_List_base<CCountedPointer<CNavRegionConnection>>::clear`, 0x00AADD80 / 0x00AADE80
  vector<short> ctor / operator= (were vector<unsigned short>), 0x00AADFF0 CCountedPointer<CWaveFile>::Reset.
  - 0x00579EA3, 0x00AADD80 and 0x00AADE80 each leave one stand-in binder (0x00579FAA; 0x00AAD5D0's CArray stand-ins). The old self
    leaf goes and the new one equals a spelling a binder already uses, so each address narrows from three spellings to two. tablecheck
    counts referencers only, so it cannot show this; the split gate reads 0 created. Accepted (advisor decision, owner delegated; nf183
    0x00416268 precedent).
- Other rows respell callees in place: CTCWife / CTCInventoryStats map finds (q4385 / q4386), the four C3DAnimFileLoader
  Register*SubChunkType rows inside their existing local CCountedPointer template (symbol sets equal to HEAD), the three
  CEnginePrimitiveManagerParticle*Group ctors, 0x005EC990 (its local CRetailHandleVector now derives from the shared
  std::vector<CEnginePrimitiveHandle>), 0x004C00B0.
- Header: stlport/fable_map.h (`orig` equals HEAD) gets two size-pragma scaffolding opt-ins (EXTRA_RULES 2026-11-20):
  FABLE_RETAIL_STLP_TREE_FIND_LONG_LONG_SIZE_BODY (owner 0x005937E1) and FABLE_RETAIL_STLP_MAP_FIND_ULONG_LONG_SIZE_BODY (owner
  0x005D8548). The default path is token-identical, no landed row defines either macro, and pin_asserts pinned 0.
  - Checks (`logs/scaffold_off_*.log`, `logs/scaffold_on_off_symbols.txt`): with the macro removed both rows still grade
    RELOCATION_MATCH, and the only changed symbol is the one COMDAT (find<long> 20 vs 21 bytes; map<unsigned long,long>::find 20 vs 22).
    Both owners set `optimize("s", on)` themselves.
  - Advisor decision (owner delegated): 0x005D66BA carries its own `optimize("y", off)` / `("s", on)`, placed after its include of
    fable_map.h, so the header's `optimize("", on)` reset cannot cancel them. The rule now reads "no row-head optimize pragma before the
    include of the opt-in header": EXTRA_RULES and the comments of all four size blocks (fable_map.h x3, engine/CAnimInit.h) are
    reworded in place, with no line count change.
- Pick-any (`logs/pickany_check.txt`):
  - DeleteData<CWaveFile> equals its owner 0x00AADFC0 (34 = 34). DeleteData<CChunkedFileChunk> in 0x00A9CF40 is the copy of this
    landing's own self row 0x00A99FF0 (agent cmpbody: identical).
  - find<long> and map<unsigned long,long>::find equal their owners 0x005937E1 / 0x005D8548 (20 = 20; agent q.py at the owner address).
  - `??8_Rb_tree_base_iterator` (MISSING): the table's 0x0041C820 is the folded comparator (MATCH there, nf177 / nf180).
  - 0x006D0630's `_M_find<long>` copy differs from its owner (DIFFER 56v56), but it is byte- and relocation-identical at HEAD and after
    the port: pre-existing, queued as debt.
  - 0x00579EA3 now binds 0x00579D97 as the tree `_M_erase`: an undefined external, not a copy. The address's own row still has the junk
    name LTextTreeWalkThrough::BuildTreeArray; its rename is queued (tablecheck GREW 0 -> 1 there: the stand-in
    AutoTinyFreeListAllocation_00579d97 gives way to the STLport spelling, count unchanged).
- Advisor decisions on the rest:
  - q5223 / q5991 (53-byte counted-pointer bodies 0x004057A0 / 0x00CE1000, no thunk): no rename. The exe holds 976 identical copies,
    so bytes cannot tell destructor from ReleaseData; the referencer classification check is queued before anyone moves q5991's 155 rows.
  - q12200: no respell of 0x005D8548 (no referencer, no discriminating evidence); done-already.
  - Rejected q5224 q5225 q5227 q5228 q5233 q5235 q5236: each 44-byte body is ReleaseData (FableWin.pdb), the 5-byte E9 thunk is the
    destructor; the corrected names are in NAME_FIXES.
- Link (`logs/link_landing.log`): 3 LNK2005, the same symbol set as HEAD (`logs/link_control_head.log`). The owner / copy pairs above
  link with no new clash.
- Recheck (`logs/recheck.log`): rebind multi-check 1 (baseline); split gate 0 created. Includers and referencing rows 21 good, 0 bad;
  owner recheck good.
- tablecheck (`logs/tablecheck.txt`): five addresses shrink (0x00A99FF0, 0x00443F90, 0x005937E1, 0x005D83B3, 0x00B6CBD0, each 2 -> 1);
  one grows, 0x00579D97 (above). No out-of-image oracle line added.
