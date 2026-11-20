# 2026-11-20 landing nf146: CAFile / CWideString remainders and nf141 follow-ups (13 rows)

Records: `naming/2026-11-20_agentNF146/` (selfcheck 0 problems). 5 entries: q4283, q4285, q11812, q11818 done; q11817 blocked here on
0x00BF3B60's strong CMemoryDataInputStream ctor (NF148 re-ported 0x00BF3B60; the rename lands with nf148).

## Rows (13): 3 MATCH + 10 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
- q4283: 0x009F2DD0 onto engine/CSurface.h + CAFile.h + fable_string.h; its own leaf takes the PDB
  ?LoadFromBitmapFileToPreallocatedSurface@CSurface@@QAE_NABVCWideString@@K@Z and it binds CAFile::ConvertPathToFullPath. 0x00885F50 /
  0x0055FC90 respelled in place (void -> bool on their existing local CSurface models; defined symbols as at HEAD).
- q4285: 0x009C7A60 off its row-local NGameText::CWideString model onto fable_string.h; own leaf -> the PDB ConstructStringInternal; the
  made-up stand-ins 0x009C7B70 / 0x009C7B80 re-authored as the two PDB ConstructString overloads; 0x0072E710 / 0x0072E940 / 0x004A0370
  respelled in place. Every address in the family ends on one spelling.
- q11812: 0x00964360 leaf-only self rename to ?FindPStealableItem@CAIStateGroup_StealStealableItem@@ABEPAVCThing@@XZ (its referrer already
  binds it).
- q11818: 0x0041C780 / 0x0041C7A0 / 0x0041C7C0 (std::_List_iterator_base) moved to the nf86 / nf88 pick-any scaffolding (HEAD gave 3 x
  LNK2005).

## Header
None changed.

## Checks
- 143 rows rechecked (gate referencing rows + renamed rows), 143 good. Gate 0; bindings_multi_check 1.
- Table: no set grew; 15 shrank (the CWideString family, 0x00999110 2 -> 1, 0x009C7A60 3 -> 1, 0x009F2DD0 2 -> 1, the two literals).
- One definition (`logs/symbol_diff.txt`): own leaves, TU-local scaffolding helpers, and GFIntToWideString pick-any (owner 0x00405F00 is
  pick-any since nf143; compiled into the context link). Landing link (13 objects): none. Context link (150 objects): 13 LNK2005, none
  involving a landing object or the owner.
- Catalog: an earlier run of the flags helper rewrote 0x0055FC90 / 0x00885F50's CompilerFlags to `/GS`; reverted to HEAD's
  `/O2 /Oy /W3 /GS` (equivalent: the lane normalises both), and the helper now skips a catalog entry that already carries every flag.
