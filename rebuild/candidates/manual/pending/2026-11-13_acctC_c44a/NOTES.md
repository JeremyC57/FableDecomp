# agentC44 NOTES

## Row A 0x0058807D CTCInventory::OpenPCInventoryMenu -- RELOCATION_MATCH (4997 v 4997) on grade 4 (src/0058807d.cpp)
Authored from the acctB outline (ref/0058807d/notes/OUTLINE.md) + the landed twin OpenPCSkillsMenu 0x00587BFF + the landed item
row 0x005A8C66. Grades: 1 SRC_FAIL (opt-ins), 2 DIFFER 5070 (`optimize("y",off)` gives `mov ebp,esp`; retail has the biased
`lea ebp,[esp-78h]` = no y-off pragma, C38 lesson), 3 DIFFER 5051 (57 structural hunks), 4 MATCH. What closed grade 3 -> 4:
- gold block: `CThingPlayerCreature* creature = ...GetPControlledCreature(); CTCHeroStats* stats = 0; creature->GetTC(...)` (retail
  zeroes the out slot AFTER the creature call, 0x00588202) and `stats->GetMoney()` (the landed 0x005A8C66 accessor; `mov eax,[eax+3Ch]`
  before the vtable load, `mov edx,eax` after the pushes).
- card arms in retail block order obj / hair / tattoo: `if (hair.IsNull() && tattoo.IsNull()) {obj} else if (!hair.IsNull()) {hair}
  else if (!tattoo->IsInvalidCustomTattoo()) tattooCount += ...` ; `mod` declared BEFORE `if (!def.IsNull()) def->GetSubDef(&mod);`
  (store of the null at 0x00588AF8 before the `je`).
- menu entries: `description = entry->FindChild("..._DESC"); viewport->AddFakeChild(description);` (viewport vtable loaded after the
  FindChild call, 0x00588C4A).
No header change, no new opt-in. Only `#pragma optimize("s", on)` (as the lane's matched 0x0058F9BC; no /GS here: no cookie in retail).
vland 0058807d RELOCATION_MATCH PASS; test PASS; addmatch done (payload.json / oracle.tsv).

## Row B 0x00652FA4 CDrawQuickAccessIcons::Draw (start = prev_C41/differ K5 = wb/k5.cpp; my tools: tools/va.sh replace-all variant + signature, tools/bk.py block K/this/def)
- FOUND (EVIDENCED, two PDB sources + size): the copy of the sprite position is `NPlayerGui::GFToScreen(const C2DVector&)` -- Ego_r 0x00686AC7 /
  FableWin 0x020D908C ?GFToScreen@NPlayerGui@@YI?AVC2DVector@@ABV2@@Z, FableWin /Od 27 bytes / 12 insns = `return pos;` (identity copy), never
  bound in retail (always expanded), ALREADY in the shared header engine/NPlayerGui_Graphics.h (landed 0x0064ED4F uses it). So the
  sprite sites are `sprite(0, GFToScreen(pos), ...)` (was the FITTED `C2DVector(pos)`), DrawIcon's `GFToScreen(position)` (was
  `C2DVector(position.X, position.Y)`; same bytes) and flash / d-pad `GFToScreen(GFAnchorToBottomLeft(ringsPos) + def.DPadMiddleOffset)`
  (replaces `.ToC2DVector()`; FABLE_RETAIL_C2DVECTOR_TOC2DVECTOR no longer needed). wb/s4.cpp: 4653v4653, d-pad Y now RIGHT; wrong: flX flfst dpX + swap.
  The cross sprite (`pos + def.QuickAccessAbilityCrossOffset`) has NO copy (GFToScreen there = 4659).
- s4 state: priority this 655 v def 656 but ids now this 32 / def 26 (tie -> later id = this): `this` needs only +1 (or def -1).
  Ruler (type b) k=4 placed before the effects-kill loop or before `if (FlashButton != 0) {` -> only flfst wrong (k=1,2,3,5 not). So ~4 symbol
  units are missing between the loop-2 end and the flash block, plus one `this` reference.
- BREAKTHROUGH 2 (wb/a3.cpp + scratch overlay hdrx/: PROBE_ARROW macro in CIntelligentPointer.h): CIntelligentPointer<T>::operator-> with TWO named
  locals `CBaseObject* item = GetPItem(); T* pointer = static_cast<T*>(item); return pointer;` -> no SWAP (this takes esi), flX and dpX right, `other 0`:
  DIFFER(4653v4653) with ONE instruction out of place: `fst [ebp-4]` at 0x006540C3 (retail after the 3rd `push ebx`, ours after the 2nd).
  One named local (`T* pointer = Get(); return pointer;`) already fixes the swap but not the X phases. Evidence for a non-trivial operator->:
  FableWin /Od ??C?$CIntelligentPointer@..@@QBEPAV..@XZ is 336 bytes / 92 insns for all 17 instances (Get() is 19 b / 9 i): a body with asserts;
  the two-local text is FITTED to the phase (cannot be size-proven: asserts compile away). Draw has exactly two `->` on Effects (kill loop, Effects[9]).
- differ/00652fa4.cpp (on hdr/ with the new opt-in FABLE_RETAIL_CINTELLIGENTPOINTER_ARROW_CHECKED_LOCALS): DIFFER(4653v4653), `WRONG: flfst | other 0`:
  the ONLY difference to retail is `fst [ebp-4]` one push early (ours 0x006540C2, retail 0x006540C3). Same-TU callees from this source at their own
  addresses: GFAnchorToBottomLeft 0x00643CDA, DrawIcon 0x00645292 (now `GFToScreen(position)`), DrawEffectAtPos 0x0064546F = RELOCATION_MATCH (/GS).
- flfst analysis: retail's region 0x0065409E..0x006540CD has the fp ops at the SAME slots as the d-pad region (fld st(1) slot 11, fstp X 13, fst Y 16);
  ours has fst Y at slot 15 in the flash only. Not moved by: flashAlpha type (const / char / signed char / long / static / reference / two-step
  assignment / float intermediate / `255.0f * x`), flash position as `.ToC2DVector()`, named flashPos (changes other bytes), `def.DPadRingsPos`
  instead of ringsPos, inline rulers inside the flash block (k=1..4 at two places), `(x) * 1.0f` copy. Untried: flash arguments spelt through named
  constants / a CRGBColour, the switch as another construct, FlashFade.Close()/IsOpen order, GetFadeInOutAlpha result through an inline helper.
- Other measurements: named-result operator+ (FABLE_RETAIL_C2DVECTOR_NAMED_RESULT_ADD, the FableWin-size text 26i/68b) = 4701 bytes here: retail's
  expansions behave like `return C2DVector(...)`. Natural seal spellings with `ringsPos` keep sealY wrong at every phase (C41's spelling stays).
  GFFloatToLongTrunc (PDB inline) around the alpha conversion moves the ftol into the argument list: not it. Function-level named locals shift the TOP
  phase (break tip / loop-1 sites); named locals inside inline expansions count at their textual position.
- flfst diagnostics (last): with `ScaleAlpha(0xff)` in the flash (all-register pushes) ours gives `fld P X P Y P F P F`; the d-pad (one more push) gives
  `fld P X P P Y P F P F`: c2 schedules this region BOTTOM-UP from the call (F P F P Y P [surplus P] X P fld). Retail's flash is `F F Pmem Y P P X P fld`
  read bottom-up, ours `F F Pmem P Y P X P fld`: the surplus push sits between Y and X in retail, between the pops and Y in ours. The memory push
  (`push [ebp+30]`, flashAlpha) keeps the two pops adjacent in both. A spilled dword local in its place (`ScaleAlpha((unsigned char)flashGraphic)`)
  behaves like ours. So the missing fact is something that makes the Y store of the GFToScreen copy later (or the third 0xff push earlier) in c2's
  dependency order for the FLASH statement only; the d-pad statement with the same text is right.
- Scratch overlay for header-text probes: copy hdr/ to hdrx/, wrap the header line in `#ifdef PROBE_X ... PROBE_X ... #else`, define PROBE_X in the
  variant source, grade with HDRO=$W/hdrx (tools/g.sh honours HDRO). hdrx/ was removed at hand-back.
