# Landing nf377 (b1, b2; 2026-11-24): NF374's follow-ups, ten rows onto exact PDB spellings

Hand-back: `pending/2026-11-24_nf377_handback/` (NOTES.md, STATUS.tsv). Per row: HEAD and new RELOCATION_MATCH, test PASS at one path;
objsec ALL_EQ except the intended renames (`.drectve` byte-equal except 0x00BFC770); both pick-any checkers HEAD = new, 0 reach flips;
/FORCE link 0 duplicates, nothing HEAD resolved left unresolved; every new spelling an exact line in both PDB name sets; census by
compiling (14 HEAD compiles). land_scripted rechecked all ten on the landed tree.

b1 (9 rows): 0x0046B0D6 TransferBinaryIn(CDataInputStream&); 0x00786700 C3DSkeletalMorphParameter ctor (const reference);
0x00752640 calls the PDB CAnimationSet::GetAnim; 0x007FDF60 SetPItem(CBaseObject*); 0x0052AC10 CThingPhysical ctor (unsigned long);
0x00A9B740 ~CTemporaryBuffer virtual; 0x00950C30 AIAction_DoNothing as the inherited protected CAIStateGroupBase member; 0x00BF8C80
(the TreeNode ctor call and its own SplitBranch ctor take their PDB argument types; its caller 0x00BF8030 already bound the PDB name);
0x00BFD0B0 self renamed SaveVerts<CWaterFanBaseVertex> (at HEAD it and 0x00BFCF90 both defined the CFanBaseVertex name; now only
0x00BFCF90 does). Table: land_scripted applied 3 lines; `TABLE_CHANGES_hand.tsv` (5 rewitness lines to rows NF377 compiled binding the
old spelling) applied by hand after a dry run (0 misses).
b2 (0x00BFC770, landed after b1): its four SaveVerts calls use the PDB private by-reference spelling; 4 table replaces. Its `.drectve`
keeps 6 lines in the same order and size; the four `/alternatename` lines change because the left operand is the row's own rename and
the right is the compiler's non-template fallback, renamed with it (defined nowhere). HEAD's link resolved none of the four calls; the
new link resolves each to the address the table bound the old name to (0x00BFCF00, 0x00BFCF90, 0x00BFD020, 0x00BFD0B0). Accepted under
the 0x00939E70 ruling (advisor decision, owner delegated).
self_symbols: 0x00BF8C80 and 0x00BFD0B0 moved. T3: nf374 0x00BFD0B0 closed (`../2026-11-24_landing_nf377_b2/T3_CLOSED.tsv`); nf374
0x00A9B740 set aside for the landing and restored in place (its open work is binder 0x00A25D20's port). Queued: NAME_FIXES for the
0x00BFD0B0 catalog / file names and the five 0x005DC340 GetAnimation declarers; two QUEUE_PROPOSALS lines.
