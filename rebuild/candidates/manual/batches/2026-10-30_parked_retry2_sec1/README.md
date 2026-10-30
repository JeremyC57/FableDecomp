# Parked-row retry sweep, wave 2 section 1 (2026-10-30): 14 of 110 attempted parked rows landed, 7 improved; 244 rows not attempted

Section 1 = retail 0x00404D20-0x00646F67 (the 312 "other"-range + 42 engine rows of the six-way split). The agent touched
110 of 354 rows and stopped on its own judgement ("remaining residues are register colouring / scheduling / frame slots");
the 244 untouched rows are re-queued as section 1b with a no-early-stop mandate (coordinator). All 322 parked sources with a
grade still DIFFER when regraded unchanged.
Landed (14, all PASS): the CreateMenuDesc / CreateMenuDescPC family 0x005820E7 / 0x00582673 / 0x005B358C / 0x005B3613 and
ConstructWieldEntry / ConstructWieldEntryPC 0x005C323A / 0x005C3444 (the shared `engine/CCountedPointer.h` replaces the parked
local stand-in; PDB class keys; the text bank is `NGameText::CDataBank`); `CPersistContext::Transfer<unsigned char>`
0x00405A60 and `<unsigned __int64>` 0x0044BB00 (over-long catalog rows graded over the true extent, 308 -> 208 and 261 -> 224,
BOUNDS_FIX.tsv); 0x005DB5F0 (CCharString length accessor as two returns with an `unsigned long` result; Ego_r says `long`,
which reintroduces the diff -- the inline emits no symbol); `NUISystem::CIconText::GetAlignment` 0x00543B10 (renamed from
GetTextAlignment; the landed CText::GetAlignment shape); 0x005361D3 (`unsigned size() > 0` keeps the `sar`; callees
HasState / HasStateInChildren); 0x004AFC60 (guarded do-while with the element pointer declared inside the guard);
0x00404D20 (four-argument CRGBColour ctor fed from `high = colour >> 8`); 0x0053CB20 (same-file non-inlined
`_List_base(const allocator&)` body that never reads the allocator tag: the "RTM lane gap" label was wrong).
Improved (../../pending/2026-10-30_parked_retry2_sec1/differ/): 0x0044C6C2 matches only through a local def-pointer stand-in
(the four members at +0xD0..+0xDC are CDefPointer handles per the dtor 0x00450BFF; needs `CDefPointer(ENullDef)` in
engine/CDefPointer.h: migration wave 10, same blocker as section 2's 0x00698A30); 0x0053848F / 0x005384DD 73 of 78
instructions; 0x004B3CE0 / 0x00625AF0 / 0x00627400 the shared header cut the differing instructions 304 -> 144, 188 -> 131,
42 -> 13; 0x0058301A header debt paid. `../../pending/.../hdr/` holds 62 more sources moved onto the shared header with
their residue unchanged (use them as the starting point when those rows are retried).
Held 89 with residue: 13 inlined-stream-Read rows (one cdq/idiv register swap); the CDefPointer rows 0x004084B0 /
0x00408500 / 0x00408550 / 0x004085A0 (84v67); float-slot `push ecx` before `fsubp` in the ChangePosition family (0x0047D296
/ 0x0047DD9D / 0x0047ED76; /G5 /G6 /G7 /O1 /Oy- and named locals do not move it); counted-pointer release re-reading its
count after the decrement (0x004C5B90 / 0x00554030: needs a faithful STLport insert_unique body, a trivial stub drops the
release); 12 one-or-two-instruction register / slot choices; 31 rows got one batch grade (inline ternary accessors as two
returns: no change).
Per-lever tally: shared CCountedPointer.h 6 matched + 3 improved; over-span true extent 2; same-file non-inlined _List_base
1 + 2; one each two-return accessor / landed twin's shape / `size() > 0` / guarded do-while / CRGBColour ctor 5.
NAME_FIXES.tsv: 8 rows (0x00405940 CPersistTraits<unsigned char>::TransferBinaryIn; 0x005D9630 CTCInventoryItem::GetDetails
(was GetItemDefDescription); 0x00993EB0 CDataOutputStream::WriteSLONG; the CCountedPointer<NUISystem::CComponent / CAction>
ctor / copy / dtor 0x00429C15 / 0x00428928 / 0x004291DE / 0x00429160 (bodies shared across every T: canonical-name decision);
0x0053C94A the _List_base ctor spelling) + 1 review row. Descriptive: g_NewLineWideString 0x013B8BA8,
g_imeCompositionAlpha_013750b0. 0x0051D870 (held) is vector<CNavigationLayerUpdate>::resize.
Header debt: SHARED_HEADER_AVAILABLE for CManager, CTCInventory, CTCInventoryClothing, CQuestManager, CFontBank,
CDataOutputStream (the generated headers lack these members). Lane note: gall.sh was copied without its QTAG and wrote
untagged work folders under `$S/wine/q/<addr>` for the 14 src rows until fixed.
Coordinator: vland --land 14/14, multi-check 0.
