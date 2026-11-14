# agentC38 NOTES

## Row 1 0x006096E0 CTCInventoryMap::ConstructLogbookListPC
- Port: draft's 7 dead opt-ins -> today's: CHEROLOGBOOKENTRY_ACCESSORS (+GetLogEntries()), CTCINVENTORYITEM_STARTINGRHSET, CTHINGOBJECTDEF_VALIDATE (has Graphic),
  CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY (retail calls GetDef<CThingObjectDef> 0x0042B040), STLP_LIST_CONSTRUCT_HELPER, STLP_MAP_COPY_OUTOFLINE,
  STLP_TREE_FIND_MEMBER_TEMPLATE_DECL. /d2inlT71 dropped. Plus twin sets (C31 + landed 0x00607330): LIST_BASE_CONVERT_ALLOCATOR + MALLOC_ALLOC_ALLOCATE (no REBIND), RELEASEDATA.
  No new header opt-in needed so far. First grade: DIFFER(12880v12832) 41 real hunks, everything before 0x0060AFA1 equal (frame-masked).
- Only call-list difference without the flag: count loop `++it` (0x0047B3E0 prefix tree operator++) expanded by us, called by retail.
  Trace (tmp/inl_a3): site 577 sz 62 pen 33 c1 89 f 5 sc 72 (T 73: called iff sc>=74). Needs eff. cost +3 or f 2.5.
  v/c1 = `it = begin(); if (it != end()) for (; it != end(); ++it)` (folded guard, f 2.5) -> ++it CALLED, DIFFER(12832v12832) 25 real hunks.
  FITTED (dead guard; same device as landed twin 0x00607330's books loop / `entry && arrow`); working lens only; natural text still to find.
  Retail: first loop iterator homed at [esp+24h] (address taken), second loop iterator a different slot [esp+68h], ++ expanded there.
- e3: title block under `if (!books.empty())` (retail 0x0060B299 je 0x0060B9C6 skips the title; landed twin idiom) -> 1 real hunk (pads) + frame/slot residue
- g1: frame groups pass observe* to AddActionOnSelected and ignore* to AddActionOnInactive (as C31's 0x00601310 frame groups) -> only 2 diffs left: 0x0060C45F, 0x0060C5D5 (log-entry loop wide-string temps)
- h1: log-entry loop as the landed twin (`abbreviatedName = logEntry.GetAbbreviatedName()` for the menu entry, GetName() for the subtitle) -> RELOCATION_MATCH (with the fitted count-loop guard). Saved differ/006096e0.cpp.
- MATCHED (RELOCATION_MATCH, test PASS, vland PASS, addmatch done): src/006096e0.cpp. No header change (hdr/ identical to C35's overlay).
  EVIDENCED: opt-in set = same-TU sibling 0x00601310 (REBIND set gives DIFFER 12736); title block under `if (!books.empty())` (retail je 0x0060B9C6 at
  0x0060B299; landed twin); frame scroll groups observe-on-selected / ignore-on-inactive (retail slots at 0x00609EED.. ; C31 sibling);
  abbreviated name for the menu entry, GetName() for the subtitle (retail member offsets at 0x0060C45F / 0x0060C5D5; landed twin); uniform `++it`.
  FITTED: `CLogEntries::iterator it = entries.begin(); if (it != entries.end()) for (; ...)` -- only way found to make c2 call operator++ 0x0047B3E0.
  Tried without the guard (all expand ++it): const_iterator, iterator declared outside, `!(it == end)`, `it.operator++()`, shared `it` (twin),
  ParentThing-> v GetParentThing(), postfix-through-prefix experiment (postfix itself gets called; header experiment reverted).
  Arithmetic for a natural cure: score 72 -> 74 needs effective cost +3 (sz 62 -> 64, or savings 6 -> 3, or a+g +870 which the group-4
  window [-114,+76] forbids) or f <= 2.5.
- Test: hand883's model test updated to the three corrected behaviours (frame groups reversed, title only with books, abbreviated/full names).

## Row 2 0x00652FA4 NPlayerGui::CDrawQuickAccessIcons::Draw (no draft existed)
- Header plumbing: the matched twin's 13 header diffs (2026-11-13_acctC_c2a/hdrdiff) applied with `patch` to hdr/; new opt-ins (hdr/README.md): CPLAYERGUI_GUILDSEAL,
  CPLAYERGUI_GETINPUTINTERFACE, CUSERPROFILEMANAGER_CONTEXTITEMINPUT, CFADEINFADEOUTBASE_DECLS, CABILITYDEF_SUBDEF (quarantine header), CTCINVENTORY_DPADICONS.
- differ/00652fa4.cpp: first compiling draft written from the acctB outline with the twin 0x00651B04's preamble and idioms. /GS + `optimize("y",off)` + `("s",on)`.
  First grade DIFFER(4879v4653), call COUNT 140/140.
- Descriptive names: HUDRingAlpha 0x01375744 (declared `long` here; the twin c2a declares `unsigned char` -- one of the two is wrong: retail reads it as a dword at
  0x006532B7 and as a byte for ScaleAlpha), QuickAccessNumberOffset 0x01375404. CPlayer::AggressiveMode read in place (PDB ?IsAggressiveMode@CPlayer@@QBE_NXZ has no header decl yet).
- State at hand-back: differ/00652fa4.cpp DIFFER(4734v4653), 8 grades. What is settled:
  * `#pragma optimize("s", on)` ONLY (the dossier's `optimize("y", off)` is wrong: it gives `mov ebp,esp`; the biased `lea ebp,[esp-74h]` frame comes
    from the size pragma alone on a large frame, as landed 0x0064ED4F / 0x0064499C) + `// Flags: /GS` (cookie).  Removing the y-pragma: 4879 -> 4734 bytes.
  * loop conditions compare `long i` with `size()` UNSIGNED (retail jb / jbe): no `(long)` cast on size().
  * CGameDefinitionManager hides the template: `World->PeekDefinitionManager().CDefinitionManager::GetDef(item, &def)`.
  * call list equal by count (140/140) and by target multiset (tools/calls.py: only unbound-name rows differ).
  Open (next steps, in order): (1) `this` in edi / `def` in esi (retail esi / edi) -- first hunk 0x00652FBD; the named `ringsPos` reference of the twin is
  byte-neutral; try the twin's declaration order facts (retail stores &PrimitiveHandles to [ebp+0] and keeps hero in ebx until 0x0065311A); (2) frame +8:
  one extra 8-byte temp (candidates: the `C2DVector(pos)` sprite position copies -- retail shares [ebp+40h] for the ring sprites; the tooltip `text` vectors);
  (3) retail's two `fst` (value kept on the x87 stack and stored twice: `fade` assignments) ; (4) the ability-cross condition is written with the twin's
  conditional expression -- check retail's shared `test al,al ; jne 0x00653CD4` at 0x00653C1E against it; (5) flash switch: retail `sub 0Dh / dec / dec / dec`
  with `default: goto dpad` (the draft initialises flashGraphic = 0 instead: needs the outline's shape); (6) effect loop `e < size && e < 12` v `if (e >= 12) break`.
  No behaviour test written yet (twin test: 2026-11-13_acctC_c2a/test/).

## Row 3 0x0058807D CTCInventory::OpenPCInventoryMenu -- NOT OPENED (context). Twins listed in the task: landed 0x0058F649 / 0x00589A30 / 0x0058AF10, matched c24a 0x0058F9BC.
