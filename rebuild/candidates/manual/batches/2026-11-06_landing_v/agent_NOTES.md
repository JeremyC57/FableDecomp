# Hand chunk 821 (CEndGameScythe .. CTCTavernGameSpotTheAddition): 2 of 6 byte-exact

All six rows were retries of parked sources (chunks 789 / 800 / 805 / 808). Every parked source was first re-graded unchanged with
the current repo headers: all six gave the same byte counts as when parked. Names were already proven; no label or spelling was found
wrong. No shared header was changed (hdr/ is empty). Full attempt log: STATUS.tsv notes (and ATTEMPTS.log, one line per grade).
Masked diff lines below use bin/md.py (relocations and memory operands masked), so they can be a little lower than the parked counts.

## What matched (2)
- **0x007D8E20 CTCTavernGameSpotTheAddition::FrameUpdate** -- RELOCATION_MATCH (a1, first new attempt). The frame-slot residue
  (board-bounds CDefPointer at [esp+0x14] v retail [esp+0x18]) is scope packing: retail puts the bounds def in halton's slot, which
  VC7.1 only does when halton's scope has ended. Fix: `float halton`, the halton position loop and the PositionMesh loop in a sibling
  block `{ }` before the bounds-def block (chunk 818's sibling-block lever). vland: RELOCATION_MATCH + PASS.
- **0x00D2ED20 NScript::CQ_EndGameFocalSitesScript::CEndGameScythe::Main** -- RELOCATION_MATCH (a14). The diagnostic /d2inlS15
  matched first (S14 = default), so the speech-exit MovieBase dtor was one inliner step short. The plain-C++ fix is c2's frequency
  rule (docs/pipeline/INLINE_DEPTH_REVISIT.md): wrapping the whole SAY block `if (GetHealth > 0) { Speak; wait; T }` in one
  `do { } while (0);` makes both speech-wait terminate returns inner-loop exits, so VC7.1 expands the implicit dtor at both and
  tail-merges them (`jmp 0x00D2F58B`) as retail. Line 2 carries the `// Inline-split:` note (construct unproven). Wrapping individual
  T checks instead moved the movie's frame slot (a5-a8), wrapping Speak/wait inside the if expanded one exit only (a13), and wrapping
  the inner T checks as well was too hot (a15). vland: RELOCATION_MATCH + PASS.

## Parked for Fable 5.1 (4)
- 0x00CFE030 NScript::CQ_AwakeningTheOracleScript::COracleTablet::Main DIFFER(3575v3575): two spawn_points[i] addresses in the CreateCreature arms are `[edi+ecx]` / `[edi+eax]` in retail, `[ecx+edi]` / `[eax+edi]` here (0x00CFEC29/2C, 0x00CFECD7/DA), 8 masked lines, size exact | tried: row-local `inline CScriptThing::~CScriptThing() { }` (183 lines: final exit also expanded); final wait as for(;;)-return / continue-return, a block round the tail, TRetVoid, do-while(0) on the loop / post-loop / every T; then the final DiggingPoints erase loop as a `for (;;) { if (it == end()) break; ...; ++it; }` (or `while (it != end())`) loop, which stops the post-loop frequency leak, so only the in-loop-1 / post-loop exits expand: 23 -> 8 lines; index spellings `*(begin()+i)`, `(&front())[i]`, a `CScriptThing&` local | next: an index spelling whose IL adds the strength-reduced offset first, else a register-operand-order lane gap.
- 0x00D0A830 NScript::CQ_BanditCampBossBattleScript::CBanditKing::Main DIFFER(2700v2720): the merged terminate exit of the "over" speech calls the MovieBase COMDAT 0x006E7B80 where retail expands it (`call 0x006E7AB0` ; `mov [esp+0x7c],0x0126008C` ; `call 0x0099A430`); 20 masked lines, the rest is the jump table | tried (16): no TRetVoid; every T as do-while(0) (too hot); the 0x00D2ED20 SAY-block do-while with and without the movie-scope do-while; Speak/wait inside a do-while; each speech T wrapped, the post-loop T alone; a for(;;) wait with the post check inside (byte-identical to the parked shape); the health-bar tail in a second do-while; /d2inlS12-20 and /d2inlT72 (no diagnostic match: S shifts other timer inlines) | next: the exit is below c2's frequency threshold for the implicit dtor in this body; a construct that heats the post-loop return without heating the in-loop one (the parked a0 source kept, with its existing Inline-split note).
- 0x00D2D930 NScript::CQ_EndGameFocalSitesScript::CEndGameJackOfBlades::Main DIFFER(5005v5094): the four cut-scene scope-end MovieBase dtors call the COMDAT where retail expands them; 15 masked lines | tried (6 + diagnostic): /d2inlS15 gives RELOCATION_MATCH (diagnostic only; GOTCHAS chunk 805 said "no value", it only tried S14 and S16), so the sites lose to c2's caller-size inline penalty; do-while(0) movie scopes, implicit movie ctor, implicit STB op= (chunk 819 lever), a trimmed dead tail, no inline CScriptThing ctor, plain-inline GetPos: none lifts it | next: lane gap (inliner budget, documented blocker); parked early with this note.
- 0x00DE8370 NScript::CQ_RansomVictimScript::CRansommer::Main DIFFER(4894v4894): in the rotated IsPerformingScriptTask tests of the two block-1 not-enough-gold SAYs retail does `mov eax,[esi+14h]; mov edx,[eax+0ACh]; lea ecx,[eax+0ACh]`, ours `mov ecx,..; mov edx,[ecx+0ACh]; add ecx,0ACh` (2 x 3 instructions, size exact) | tried (10): chunk 816 loop bottom, for(;;)-break waits (both forms), a handle reference, gold and answer tests reversed, do-while(0) round the SAY / the in-loop T, sibling blocks, plain returns at those two sites | next: register colouring, lane gap; park.

## Callee names
No new callee was declared: the four parked sources and the two matches keep the parked sources' declarations (names proven by
chunks 789 / 800 / 805 / 808). vland's RELOC_NAME warnings come from labels that were already in the catalog, not from these rows:
0x005E9A60 (bound here as CTCTavernGameSpotTheAddition::KillThing, labelled std::_Dest_val<...CFlash>), 0x008AB1E0 (bound as
CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::operator=, labelled with the descriptive _global::CCountedPointer_Set_008ab1e0),
0x00CDBFB0 (bound as std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map, labelled CRetailTree_erase...; chunk 584 already
reported it). These look like bodies identical code folding shares between several templates, so one name per address is a catalog
decision; not listed in LABELS.tsv because this chunk did not prove a single correct name.

## Skipped
None.

## Label corrections
None (LABELS.tsv and NAME_FIXES.tsv carry headers only).

## Header debt
None.
