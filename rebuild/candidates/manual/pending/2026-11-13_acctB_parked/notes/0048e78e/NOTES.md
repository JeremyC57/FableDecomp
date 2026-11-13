## bwf1 author (2026-11-12) progress log
- g10: best l2 = 35 hunks 2662v2662 (from c39 55 / c67 50). Moved it: (1) pos.Y offset as `if (in_text_box) {3x +=InBox} else {3x +=OffY}` with decl order 1,2,3 (sibling DrawButtonText 0x0048FAB6 shape; cross-jumped into the lea select) fixed the pos.Y init; (2) layer `if (DisplayFlags & 0x800)` ahead of the bools/box3 fixes flags=ecx / 0x45=edx (but splits the block: drawn stores + je order now differ); (3) arm A offsets in TLX,BRX,TLY,BRY order -> address CSE in eax.
- did not move: default-ctor + Y assignment chains, copy chains on the new base, timer through reference/pointer, flags local copy, layer before half/box3-inner decl.
- g~25: best s1 = 26 hunks 2662v2662. Moved it: FableWin-proven inline CSubtitleRenderer::IsFlagSet(ESubtitleFlag) (hdr opt-in FABLE_RETAIL_CSUBTITLERENDERER_ISFLAGSET, -1 hunk + tail F0B7 fixed), an outer `{ }` from button1_box through `return drawn;` (FITTED; fixed the tail movsd/xor order, -8).
- did not move: arm A offsets XYXY/YXYX/C2DVector ref/float refs/box ref/a=a+b (X offset address never CSE'd in block 1; not flags pressure: DIAG without arm flag tests same); single-block (layer if after box3) still gives flags=eax/0x45=ecx for every decl order, mixed raw/IsFlagSet, ternary, if/else layer.
- g~70: best pm11 = 14 hunks 2662v2662 (arm A block 1 statements BRX,TLY,TLX,BRY). Arm A address CSE (X eax + Y ecx) appears for every order EXCEPT ones starting `TLX ; TLY` (t1 = retail store order fails: only one address CSE); pm03 TLX,BRY,BRX,TLY also gets both CSEs (15 hunks, Y stores swapped). Not register pressure (DIAG with no flags / no shared 0x45: still one CSE). Operand order (off + box) on any subset: no effect.
- g~95: still best pm11 (14). Single-block form (layer if after box3) + pm11 arm A order (af1, 23 hunks) gets 0x45=edx right; left: flags eax/InCutscene cl swapped v retail ecx/al. Tried there: InCutscene local copy, mixed raw/IsFlagSet, arm C condition order, const flags local (no: retail reloads [esi+7Ch] after IsTextReadyForClickPast, so it's the member). Arm A `TLX;TLY` first never gets the X address CSE (24 orders, operand flips, scoped adds, refs, huge_pos between, DIAG without the top timer section).

## bwf1 author hand-back (2026-11-12): 0x0048E78E CSubtitleRenderer::DrawButtonGraphics -- DIFFER(2662v2662), 14 hunks (start: 55)
Best source: differ/0048e78e.cpp (= c/pm11.cpp + header). Overlay: hdr/ (README lines added; STALE headers re-based or dropped, old copies in hdr_stale_backup/).
What moved it (in order): (1) pos.Y offsets as `if (in_text_box) {3x += InBoxOffsetY} else {3x += OffsetY}`, positions declared 1,2,3
(same shape as the unlanded sibling DrawButtonText 0x0048FAB6; VC cross-jumps the arms into retail's lea select) -> fixes the pos.Y
init completely; (2) layer `if` ahead of the bools/box3 -> flags ecx / 0x45 edx / InCutscene al; (3) FableWin inline
CSubtitleRenderer::IsFlagSet(ESubtitleFlag) for every DisplayFlags test (new opt-in FABLE_RETAIL_CSUBTITLERENDERER_ISFLAGSET; fixed the
tail 0x0048F0B7 operand order); (4) an outer `{ }` from button1_box through `return drawn;` (FITTED; fixed the tail movsd/xor/lea order);
(5) arm A first offset set ordered BRX,TLY,TLX,BRY (both offset addresses CSE'd as retail).
Exact residue: (a) retail is ONE block from the half subtracts to the 0x800 je (drawn/button1_drawn = false stores interleaved early, je after
box3); every single-block spelling (layer if after box3) gives flags=eax / InCutscene=cl (c/af1.cpp, 23 hunks, otherwise right incl. 0x45=edx);
(b) arm A: retail stores TLX,TLY,BRX,BRY with lea eax,[ebx+5FCh] / lea ecx,[ebx+600h]; VC7.1 drops the X address CSE whenever the
statements begin `TLX ; TLY` (all 24 orders graded: pm03 TLX,BRY,BRX,TLY and pm11 BRX,TLY,TLX,BRY are the only 14/15-hunk ones).
Tried, no effect: Y default-ctor/assignment chains, copy chains, InCutscene local, const flags local (impossible: retail reloads
[esi+7Ch] after IsTextReadyForClickPast), early `bool behind`, layer ternary / if-else, bool decl positions / top decls, arm C/E condition
restructures, raw v IsFlagSet mixes, record rulers (1..24 dead statements at the top / before the arms), timer via reference/pointer,
arm A operand flips / refs / scoped adds / C2DVector& corners / huge_pos placements, box decl order, subtract order.
Best next idea: both residues look like ONE missing source fact (FableWin's /Od row is still ~26 instructions longer than ours,
c/pm11.cpp 1178i v 1205i): find the extra statement(s) -- e.g. a named extents local / C2DExtentsF temp shared by the boxes, GetTLPos-style
accessors, or the 0x800 test feeding `layer` through an inline helper -- that make c2 allocate flags before InCutscene with the if after box3,
then re-run the 24 arm-A orders on that single-block base (c/af1.cpp is the single-block template).
Names: no landed name proven wrong beyond prior NAME_FIXES.tsv. Made-up names: parameter/local names only (descriptive, as prior notes).
