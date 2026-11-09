# TRACE -- chunk 890 (agent890): every experiment, hypothesis -> probe -> evidence -> conclusion -> change

Paths: `$A` = `$S/w22/agent890`. Grades: `$A/work/grades/<stem>.txt`, objects `$A/work/objs/<stem>.obj`, diffs `work/d.sh <addr> <stem>`
(work/dd.py: strict = registers + frame offsets kept, loose = both masked), call sequences `work/calls.py`, frame symbols
`work/fas.py` (/FAs listing: `_name$ = -off` per frame symbol), allocator `c2prio.py --trace` (outputs `work/c2/*.trace`),
inline decisions `work/inl/run.sh` + `work/inl/sites.py` (outputs `work/inl/*.json`; per site: r = inlined, f = c2's static
frequency of the call site, sc = score, accepted iff sc < T).

## Extents (verified with $S/w17/xdis.py)
- 0x00551EA0: `ret` at 0x00552DA8, int3 0x00552DA9..AF, next function 0x00552DB0 (`mov eax,ecx; mov [eax],0; ret`) -> 3,849 bytes. The
  catalog gap 4,512 holds the small functions from 0x00552DB0 on (not this row).
- 0x0056AD40: early `ret` at 0x0056AE99 (inside the body, no padding after), final `ret` 0x0056BDFB, int3 0x0056BDFC..FF, next 0x0056BE00
  -> 4,284 bytes.
- 0x00498490: `ret 0Ch` at 0x0049987A, int3 0x0049987D..7F, next 0x00499880 -> 5,101 bytes.

## Row 0x00551EA0 NUISystem::CTable::ConstructSpritesToDraw

### E1 (v1) first draft
- Hypothesis: the body is 4 corner blocks + 4 edge lines (CTable virtuals 143/144 = ConstructHorizontalLine / ConstructVerticalLine,
  rtti-vtables 0x0124A224) + 2 separation loops; the line calls take `sep != end ? sep->second : CCountedPointer<CComponent>()`.
- Probe: grade + dd.py + fas.py. Evidence: work/grades/r1_v1.txt, work/fas/r1_v1.asm.
- Result: DIFFER(4025v3849), loose 80.5%; frame 0x68 v 0x58. /FAs shows `$T9886` (4 bytes, -96) = a conditional-temporary flag
  (`or $T9886,2` / `test` / dtor) for the `?:` null arm, plus a second 8-byte slot (-24) for the first loop's position.
- Conclusion: retail has no flag and constructs both arms' argument in the call's own slot, with two call sites (cross-jumped
  when their tails are identical, separate when the registers differ: 0x5526ED/0x55272B v the merged 0x5528A3) -> if/else calls.
- Change: v2 = `if (sep != end) F(it->second, sep->second, ...); else F(it->second, CCountedPointer<CComponent>(), ...);`

### E2 (v2) if/else line calls
- Result: DIFFER(3820v3849), strict 82.2% / loose 91.0%; frame 0x58 = retail. calls.py: retail calls the CCountedPointer(T*) ctor
  COMDAT 0x00429C15 at corner sites 2-4 (inline at site 1), ours expands all four; retail calls tree::find 0x00553520 three times
  (sections 1, 2 and the SEPARATION_TOP lookup), ours twice; the last ConstructVerticalLine pair is cross-jumped in ours.
- Probe: inline trace (work/inl/r1_v2.json, sites.py). VC7.1 regime-0 decision (c2 0x1078D1AA): cost X (c1, +27 for tree::find),
  score = (X/f + 2X)/3 (k = 3), accepted iff score < T; f = c2's static frequency of the call site: function entry 1.0, an `if`
  halves it, a loop body x5, and after a `for` loop the current frequency stays at the loop body's (joins add the not-taken
  share). Ours: sections 1/2 each double f (1 -> 2 -> 4), so the corners run at f 4: CCP sites 2-4 at f 1 (sc 70, inlined) and
  the SEPARATION_TOP find at f 2 (sc 70, inlined). Retail's decisions need the corner code at f 2 (CCP 2-4 at 0.5 -> sc 80
  rejected; SEPARATION_TOP find at 1 -> sc 85 rejected; push_back at 0.5 -> sc 64 still accepted).
- Micro probe (work/micro/f1.cpp, work/inl/m1.json): `for (i = 0; i < n; ++i)` inflates the following code (fA: 1 -> 2 -> 4),
  `unsigned long i = 0; while (i < n) { ...; ++i; }` does not (fB: f back to 1.0 after the loop).
- Change: v3a = section 1's loop as the while form (v3b = section 2's instead: same decisions, same bytes).

### E3 (v3a / v3b) while loop in one section
- Inline trace work/inl/r1_v3a.json: every decision now equals retail (tree::find rejected at sections 1, 2, SEPARATION_TOP; CCP
  inlined at site 1 only; push_back inlined everywhere).
- Result: DIFFER(3817v3849), strict 95.8% / loose 97.5% (v3a and v3b identical).

### E4 (v4) positions declared first; edge totals written inline
- Evidence from v3a (work/d.sh 00551ea0 r1_v3a --hunks; allocator trace work/c2/r1_v2.trace): ours `xor ebp,ebp` at +0xb and stores
  the C2DVector zeroes through ebp (one zero register shared by the integer and float zero stores), retail stores the floats as
  immediates (`mov [esp+..],0`) and keeps `mov ebx,ecx` first. c2 assigns the shared zero to a register only when the integer
  locals (declared first) start the zero run; with the four vectors first, their stores are emitted as immediates. Separately, the
  named `float totalWidth/totalHeight` locals were stored (`fstp`) and reloaded (`fld`) where retail keeps the converted product on
  the x87 stack (`fst` + `fadd`) and reuses it as a CSE for the later `-=`.
- Change: vectors before integers; `topRightPosition.X += horizontalSize * nbHorizontal` (and the `-=`) written inline.
- Result: DIFFER(3849v3849) -- size equal; strict 96.8% / loose 98.4%. First diff: zero-store order (`[esp+0x24],edi` v
  `[esp+0x20],ebp`) and the imul destination at 0x00552084.

### E5 (v5) integer declaration order
- Observation (c2 trace work/c2/r1_v4.trace, d.sh): the order of the integer zero stores follows declaration order, and in
  `horizontalSize * nbHorizontal` / `verticalSize * nbVertical` the later-declared symbol becomes the imul destination. Retail:
  `imul ebp,[nbVertical]`, stores nbHorizontal, nbVertical, horizontalSize, verticalSize in that order.
- Change: declare nbHorizontal, nbVertical, horizontalSize, verticalSize.
- Result: DIFFER(3849v3849), strict 99.2% / loose 99.4%. Remaining: 0x00552162 -- ours `lea ecx,[esp+0x34]` + key store before the
  push for `GetStates()[0]`, retail pushes the key's address after storing it later (scheduling around the map operator[] call).

### E6 (v6) FABLE_RETAIL_STLP_MAP
- Hypothesis: with the map<unsigned long,CUIState>::operator[] body invisible (only a declaration), c1 materialises the key
  temporary before the argument setup; with the body visible (still rejected for inlining, called out of line 0x0042D5B1) the
  temporary's store is scheduled after `mov edx,[esi]`, as retail.
- Change: `#define FABLE_RETAIL_STLP_MAP`. Result: DIFFER(3849v3849), strict 99.8% / loose 100.0%: the only residue is two frame
  offsets at 0x005527A8 (`fld [esp+0x28]` = bottomLeftPosition.X in retail, ours loads the copied position.X `[esp+0x40]` first).

### E7 (v7a/b/c) the separation line's X
- Hypothesis: retail computes `position.X` from bottomLeftPosition.X (the copy source) plus the converted width, not from the
  copy itself. Three spellings, one grade each: v7a `position.X = bottomLeftPosition.X + horizontalSize;` -> RELOCATION_MATCH;
  v7b `position.X = horizontalSize + position.X;` and v7c `position.X += (float)horizontalSize;` -> the E6 residue unchanged.
- Conclusion: the load source is the only decision; v7a keeps the vertical loop's `position.Y += verticalSize` form (that one
  matches as written).

### E8 (v8) owner decision D1 -- the tree find as a member template
- vland reloc identity: retail calls 0x00553520 at three sites; landed under a placeholder spelling (`?find@?$_Rb_tree@UCUnprovenKey_00553520@@...`).
  Owner D1: STLport's `_Rb_tree::find` is a member template (`??$find@W4ETableSprites@NUISystem@@@?$_Rb_tree@...`).
- Change: hdr/stlport/fable_map.h opt-in FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE (`template<class KT> iterator find(const KT&)`).
- Result: RELOCATION_MATCH (bytes unchanged; objdump -r shows the `??$find@W4ETableSprites...` symbol at the three call sites).
  Final text src/00551ea0.cpp (adds the header comment and a `SpriteIterator` typedef) re-graded RELOCATION_MATCH (work/grades/r1_final.txt);
  test/00551ea0.cpp PASS under g++ (tall.sh) and VC7.1 (tall.py).
- NAME_FIX: 0x00553520 -> the member-template spelling (NAME_FIXES.tsv).

## Row 0x0056AD40 CTCBoastUI::Activate (catalog: CQuestManager::UpdateUI_QuestMenu)
### A1 identification (no grade)
- Hypothesis: the catalog label is a forge-autoname guess; `this` is not CQuestManager (the quest manager is read from
  [0x013B89FC] separately, and `this` has a map at +0x0C cleared inline, a bool at +0x18 that ends the call, a byte +0x2A cleared
  when nothing is offered).
- Probe: reloc_bindings.tsv (landed caller 0x0056BE00 binds 0x0056AD40), fablewin/egor_pdb_names.tsv, struct_layouts_egor.tsv.
- Evidence: 0x0056BE00 calls it as ?Activate@CTCBoastUI@@QAEXXZ; FableWin.pdb 0x0239BB40 and Ego_r.pdb 0x005E17E0 have that
  symbol (thiscall, no arguments = plain `ret`); Ego_r CTCBoastUI: BoastGroups (map<CMenuEntry*,CBoast>) +0x0C, Active +0x18,
  Components +0x1C, ActivationRequested +0x2A -- every offset the body touches.
- Conclusion: the row is CTCBoastUI::Activate (LABELS.tsv; NOTES label correction).
### A2 decode (no grade)
- work/r2.ann (annotated listing, work/ann.py) and work/r2.cc (destructor runs collapsed): full decode in differ/0056ad40.cpp.
- Not drafted against headers: the UI list classes behind PC_BOAST_LIST / PC_BOAST_SUB_LIST (virtual slots 144, 151, 152, 156) are in
  no NUISystem header, CTCBoastUI has no retail branch, CQuestManager lacks ActiveQuests / GetBoastsForScript /
  GetActiveQuestCardFromScriptName, and three local tree instantiations are out of line (0x0056D930 / 0x0056D9F0 / 0x0056E020 inserts,
  0x0056D130 find, make_pair 0x0056CB40, pair converters 0x0056CB60 / 0x0056DAB0). Session budget ended here: 0 grades.

## Row 0x00498490 CProgressDisplay::DrawRetailDisplay
### B1 identification and layout (no grade)
- Evidence: FableWin.pdb 0x018AEEB0 / Ego_r.pdb 0x004A2EC0 ?DrawRetailDisplay@CProgressDisplay@@AAEXABVCCharString@@M_N@Z (12
  argument bytes = `ret 0Ch`); the landed caller 0x00499880 binds a non-PDB spelling ?DrawProgress@... (NAME_FIXES.tsv).
- Layout probe: Ego_r member order with STLport vectors (0xC) and 8-aligned doubles gives sizeof 0x88 and puts every offset the row
  touches on a member (+0x04 / +0x10 / +0x1C vectors, +0x28 LoadingScreenIndex, +0x4C TextTag, +0x50 LastTextDisplayed, +0x54
  GuildSealFrameIndex, +0x58 StageCount, +0x70 LastProgressUpdate, +0x7A..+0x81 bools); no header exists (engine/ has only CAProgressDisplay).
### B2 partial decode (no grade)
- 0x00498490..0x00498972 decoded (differ/00498490.cpp: stage total, clamp by reference, installer progress, loading-picture
  advance, the auto-save frame / background split); the rest summarised from work/r3.ann. 0 grades.

## Session 2 -- row 0x00498490
### B3 (v1 -> v2) first draft against the shared headers + hdr/ opt-ins
- v1 grade: DIFFER(4826v5101), strict 41.1% / loose 87.4% (work/grades/r3_v1.txt).
- First diverging instruction: 0x00498496 `sub esp,5Ch` v ours `sub esp,64h`; then 0x0049849C `mov ebx,ecx` v ours `mov esi,ecx`
  (retail this=ebx, display=edi spilled [esp+18h], renderer=esi; ours this=esi, display in eax spilled [esp+2Ch], renderer=edi).
- calls.py-equivalent (objdump -dr relocs v r3.ann): the call multisets are identical (GFAXToVX 25/25, GFAYToVY 22/22, ~CWideString
  11/11, ~CCharString 9/9, Draw2DScaledGraphic 10/10, ...) except ours has 2 extra calls to ?EndsWith@CCharString@@QBE_NABV1@@Z
  (retail expands both: the strcmp loop at 0x0049941A and the `_PC` site).
- Inline trace (work/inl/r3_v1.json, sites.py): EndsWith r=0 sz=140 c1=129 sc=405 (both sites, f=0.062); GetVStringExtents r=0 sz=76
  sc=126 (retail calls 0x00437210: correct); every other helper r=1. A /d2inlT threshold that accepts 405 would also accept 126, so a
  per-row flag cannot give retail's split -> ladder step 3 is the only device; the header precedent is StartsWith (FABLE_FORCEINLINE,
  chunk 784) and Ego_r.pdb has no out-of-line EndsWith (EXTRA_RULES: Lionhead header helper with no release copy = __forceinline).
- /FAs frame table (work/fas/r3_v1.asm): ours 0x64 with three 16-byte box slots (-48 $T11791/$T11788, -32 box/vbox, -16 ctor temps) and
  _display$ = -68; retail 0x5C: boxes at F+0x48 / F+0x58, 4-byte slots F+0x10..F+0x3C, the EndsWith bool at F+0x0F.
- `total`: ours `neg al ; sbb ; and ; add` from the if/else; retail `mov [F+10h],82h ; jne ; mov [F+10h],1D1h` = initialise then override.
- Change (v2): hdr/fable_string.h EndsWith FABLE_FORCEINLINE (opt-in only); `long total = 130; if (!g_bUseCompiledWorldFiles) total = 465;`.
- v2 grade: DIFFER(5051v5101), strict 45.6% / loose 93.3%. EndsWith now expanded at both sites (call multiset = retail's); this=ebx,
  renderer=esi as retail; first diverging instruction still 0x00498496 (frame 0x64 v 0x5C), then 0x004984A3 (retail keeps display in
  edi and spills it to [F+18h]; ours goes through eax and spills to [F+30h]).
### B4 (v2 -> v3) three call-site shapes (work/micro/m2: m2.cpp / m3.cpp, /FAs)
- Bar pieces (0x00498AE1 etc.): retail `call PeekGuiDef ; mov eax,[eax+0A30h] ; mov ecx,[PClass] ; mov edx,[ecx+14h] ; push 1 ; push 0`;
  ours loads PClass->Graphics into edi before the PeekGuiDef call (args `Get().PeekGraphicBank(), PeekGuiDef().X`). Micro m2: f1 (both
  in the argument list) = ours, f2 (`const CPlayerGuiDef& def = PeekGuiDef();` first) = retail byte order.
- First backdrop tile (0x00498686..0x004986C0): retail builds a C2DBoxF temporary at F+58h and copies it dword-wise into box F+48h;
  ours constructs box in place. Micro m3: `C2DBoxF box(...)` and `C2DBoxF box = C2DBoxF(...)` construct in place; only
  `C2DBoxF box; box = C2DBoxF(...);` makes the temporary + copy.
- Text window (0x00499761..0x00499793): retail evaluates BRX (dims.X - left), then 300*scale, then 100*scale (fxch); ours BRX, 100*scale,
  300*scale. The two-vector ctor C2DBoxF(C2DVector(left, 100s), C2DVector(right, 300s)) evaluates br.Y before tl.Y (right to left, the
  tiles show C2DVector(x, y) builds y first), which is retail's order.
- Change (v3): the three spellings above.
- v3 grade: DIFFER(5085v5101), strict 75.4% / loose 97.2%; frame now 0x5C (the tile temporary moved the slots); first diverging
  instruction 0x004984A3 (display: retail edi + spill [F+18h], reload 0x00498FDF / 0x0049930B; ours eax + spill [F+30h], loads into
  edi / esi at each use).
### B5 (v4) EndsWith block layout
- Retail 0x00499400: `sub eax,ecx ; js <false at the end>`, then the strcmp loop with `test eax,eax ; sete [F+0Fh]` in both exits and
  `mov byte [F+0Fh],0` last. Ours (if (offset < 0) return false;): `jns ; mov byte [..],0 ; jmp` first, one merged sete.
- Change (v4, header only): `return offset >= 0 && strcmp(...) == 0;`.
- v4 grade: DIFFER(5092v5101), strict 82.3% / loose 98.1%. display now edi (spill slot [F+20h] v retail [F+18h]); EndsWith: `js` to
  the false block at the end as retail, but `&&` makes a branchy bool (`mov [F+0Fh],1 ; je`) where retail keeps `sete` in each strcmp exit.
- Change (v5, header only): `if (offset >= 0) return strcmp(...) == 0; return false;`.
- v5 grade: DIFFER(5101v5101) -- size equal; strict 84.1% / loose 99.8%. Loose residue: (1) the BlackScreen box (retail stores
  TLX/TLY zeros before the colour bytes at 0x0049900C, ours after), (2) `mov ecx,edi` one slot later at 0x00499094. Strict residue:
  frame slot assignment only (same frame 0x5C).
### B6 frame packer (no grade): work/s2/slotmap.py, work/s2/fx.sh, work/pk/{pk,ord}.sh (agent887's c2 packer probe, paths changed)
- slotmap (ours /FAs symbol -> retail slot at aligned instructions): retail groups A 0x10 (8: total, tag, position), B 0x18 (display
  spill, left), C 0x1C (scaleX, title, ...), D 0x20 (8: scaleY, text, fillPosition), E 0x28 (8: fraction, formatted, textBank),
  F 0x30 (8: scale, barPosition, sealPosition, the CCharString temps), G 0x38 (16: sealX, extents, formattedWindow, the CWideString
  temps), H 0x48 box/vbox, I 0x58 box temps. Ours: formattedWindow (16) joins A (A = 16 bytes, everything above shifts +8) and
  F/G are partitioned differently (ours F' = scale + sealX + all string temps, G' = 8-byte positions + extents).
- ord.sh: c2 processes the frame items by size ascending, weight (reference count) descending; formattedWindow is the very last item
  (16 bytes, weight 1) and takes the first non-interfering group, A. Groups are laid out in creation order.
- fx.sh experiments (compile only): declaring formattedWindow before formatted / before vbox / before WordWrap_SetCallback -> no change
  (e1, e2, e7, e8, e9); a function-level-in-the-text-block formattedWindow (e3) -> A = 8 bytes and 10 of 16 locals at retail's slots,
  but formattedWindow gets its own group at 0x60 (frame 0x64); extents as a temporary (e5) / one shared FullScreen box (e6) -> no change.
- Open: what in retail makes formattedWindow interfere with group A but not with G; not resolved.
### B7 (v6) BlackScreen colour
- Hypothesis: the 3-argument CRGBColour(r, g, b) (FableWin ??0CRGBColour@@QAE@EEE@Z, body Initialise(r, g, b, 255)) changes the IL order
  so the box zeros are stored first. Change: FABLE_RETAIL_CRGBCOLOUR + `CRGBColour(0, 0, 0)` at the BlackScreen site only.
- v6 grade: DIFFER(5101v5101), unchanged (rejected; the colour ctor does not move the zero stores).
### B8 (v7) BlackScreen box as a named local
- Hypothesis: retail's zeros-before-colour means the box is built before the colour in IL order (a named local constructed in the
  statement before the call); the loading-screen site (colour first, then zeros) is the argument temporary. Change: `C2DBoxF screen(...)`.
- v7 grade: DIFFER(5101v5101), strict 84.1% / loose 99.9%: the BlackScreen stores now match (named `C2DBoxF screen`), frame unchanged.
  Remaining loose residue: `mov ecx,edi` one slot early at 0x00499094 (seal block, before the second GetRenderTargetDimensions).
  Remaining strict residue: the frame packer's slot groups (B6). c2 packer reading (c2.dll 0x10790CD6..0x10790D85): items walk the
  slot list in creation order; a slot is skipped when the item is larger than [slot+4] or the item's bit is in the slot's set, else
  0x1077B3C9 tests the interference set; no fit -> 0x10791426 (new slot / last-slot path). Not resolved this session.
### B9 packer walk (no grade): work/pk/pk.sh on probe/fx/e10.cpp -> work/s2/e10.order (item, $T number = sym word 10, weight,
our slot, retail slot via work/s2/v7.slotmap)
- c2.dll 0x10791426 / 0x10791A6A: the placement scans slots from the most recently created one down, joins the first whose size is
  >= half the item's and whose member set does not interfere (0x1077B3C9), else opens a new slot at the end.
- Ours and retail agree on every item up to T11925 (tile 1 TL.Y -> slot 0x30). T11920 (tile 1 BR.Y, `GFAYToVY(256.0f) * scaleY`)
  then joins slot A (total, tag) in ours but opens retail's 0x38 slot; the later tile / bar / seal temporaries, sealX and the
  CWideString temps follow it into 0x38 in retail, and T12022 (tile 4 BR.Y) still joins A in retail. Unexplained: what makes A
  interfere with T11920 but not T12022 in retail. Next probe: hook 0x1077B3C9's result per (item, slot) to read the interference
  bits for T11920 v A in ours, then look for a source shape that adds a tile-1-only reference to total or tag's slot.
