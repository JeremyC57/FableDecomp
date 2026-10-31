# Chunk 621 -- 1 of 10 landed (2026-10-31, after the c621 respell pass)

Cut from the parked-row pool. CPersistContext::TransferMap<std::pair<long,long>, CCharString>(const char*, std::map<K,V>&,
const V&) 0x00756D00 is RELOCATION_MATCH 1116v1116 + test PASS (the fix: Ego_r's CPersistTraits<std::pair<long,long>> is 8 bytes
with DefaultVal at +0, so the traits object doubles as the value and is transferred in place). The first version used descriptive
stand-ins; the agent respelled it to the real STLport template over std::map with every callee at its PDB spelling. Those PDB
spellings differ from what ~25 landed callers bind to eight callee addresses (PersistError 0x009BAA90 vs Error, SetAsFoundItem
0x009BAB40 vs EntryTransferred/Deactivate_2, CleanOutLine 0x009BB0D0 vs CleanText, SkipPastWholeString 0x009B9C00 vs SkipToItem,
the std::list<CStringParserCommentBoundaries> dtor 0x004046B0 vs three stand-ins, CPersistTraits<CCharString>::TransferIn/Out
0x0040E510/0x0040E630 by reference vs pointer forms, 0x00755A30), so landing it now would break one address = one name. It is
HELD in `pending/2026-10-27_linux_hand621/name_review/` (src + test) until the callers are respelled and rechecked (queue rows c621).
Nine rows DIFFER in `pending/2026-10-27_linux_hand621/differ/`: 00aeeba0 (operand order in 8 of 18 matrix products), 00a8a220 /
00a8a5f0 (268->202 / 256->204 lines via x87-trace-generated float sums; operand order inside products left), 0088b0c0
(compiler-generated ~CStringParser never inlines at one early return), 0063d0a0 / 0063d4a0 (scheduling), 00972f30 (slot layout),
009482a0 (edx vs eax), 009289b0 (1133v1131 register share). 11 NAME_FIXES + 5 review rows queued (c621). The agent's x87sim.py /
gen3x4.py (symbolic x87 trace -> stepwise float sums) are copied to lane_tools/. Labels: ghidra_out/labels_chunk621_2026-10-27.tsv.

Respell pass (2026-10-31, `$S/w22/naming621`): the eight callee addresses were respelled to their PDB names across 67 landed
sources (bodies 0x009BAA90 PersistError, 0x009BAB40 SetAsFoundItem -- rewritten on the Ego_r CPersistContext layout, was the
CTCCreditsUI Deactivate2 stand-in --, 0x009B9C00 SkipPastWholeString -- now binds 0x009B9AA0 SkipUntilWholeString and 0x009B9570
CalcInQuotes --, 0x004046B0 the std::list<CStringParserCommentBoundaries> dtor with the allocator argument, 0x0040E510/0x0040E630
reference forms, 0x00755A30 non-const pair&; 37 callers lose the CStringParserCommentList/CCommentList stand-ins). 11 binding rows
replaced (8 collapsed onto rows already carrying the PDB spelling), catalog labels for the three bodies renamed, files renamed.
All 67 recheck RELOCATION_MATCH (009bab40 MATCH) + PASS; then 0x00756D00 landed (vland --land RELOCATION_MATCH + PASS,
multi-check 0). Open from the pass: 0x00463871 list copy-ctor spelling in 0046381b, 0x009BA330 ReadAsStringUntilString in
0040e510, 0x009B9570 SerialiseElements row from 009b9ba0, 006d3880's own QBE/const pair* symbol (no PDB proof), 0x00755F40 REVIEW.
