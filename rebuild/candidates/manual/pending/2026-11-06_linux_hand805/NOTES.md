# Hand chunk 805 (CAppleGirl .. CEndGameJackOfBlades): 2 of 6 byte-exact

Six gap-scan entity Mains. Every name is proven by slot 1 of the entity vtable its landed Alloc stores plus the Ego_r.pdb
and FableWin.pdb name sets (LABELS.tsv). The two matched rows are RELOCATION_MATCH: every relocation is bound to the same
symbol and every string is equal. Their behaviour tests pass under g++ and VC7.1. Four rows are parked, each with a test
that passes. No row uses asm or flags. Header changes are opt-in only (hdr/README.md); the 13 landed includers recheck
clean (SIBLINGS.tsv).

## What matched (2)
- NScript::CQ_GuildTrainingScript::CSkillTarget::Main 0x00D41D00 (4662 bytes, RELOCATION_MATCH, 14 attempts): the archery dummy.
  It slides from its position to the static marker, waits for hits in static mode, then slides between the markers and scores
  each arrow. Three spellings made it match:
  - Each slide step is a NAMED per-site vector built member-wise: `C3DVector step(D.X*(float)i, D.Y*(float)i, 0.0f);` then
    `C3DVector(P.X+step.X, P.Y+step.Y, P.Z)`. The four 12-byte steps give retail's 0x19C frame. The member-wise ctor keeps
    (float)i alive past the Y product, as retail does (`fld; fmul st(2)`, then a pop after X). `D*(float)i` through
    operator* gives the frame but consumes the scale (`fxch; fmul mem`). Inline temporaries `(D*(float)i).X` give neither.
  - The score ladder is `damage<0.25 / <0.5 / <0.75 / else` (fcomp; test ah,5; jp), not `==`. The static-mode test is
    `damage>0.5 -> break` first (test ah,0x41; je).
  - `i=0;` comes before EntitySetTargetable(Me,false), so VC reuses the zeroed ebx for the push.
  - CTimer ctor and dtor stay out of line (0x00CD4450 / 0x00CD4470).
- NScript::CV_LostTraderScript::CLostTraderTrader::Main 0x00EA36B0 (4804 bytes with its 5-entry switch table, RELOCATION_MATCH,
  2 attempts): the lost trader's five-state brain (yell for help, yell at the hero, introduce, follow, reunited).
  - The key spelling is a plain inline CTimer ctor body. VC7.1 still calls 0x00CD4450, but with the body visible it keeps
    TimerIndex in ebp across the calls. Declared-only, it reloads the index (58 lines).
  - Plain-inline ~CTimer gives retail's mix of expanded and called exits.
  - The helpers TraderYellForHelp / TraderYellAtHero / MakeRandomComment come from the LOSTTRADER_TRADER opt-in.

## Parked for Fable 5.1 (4)
- 00d3d150 NScript::CQ_GuildTrainingScript::CAppleGirl::Main DIFFER(4252v4252): in the MANY_MORE_APPLES / NO_MORE_APPLES arms (0x00D3DCA2..0x00D3DD5A) retail loads the Game vtable into eax and the line's CCharString address into ecx, where ours keeps the ONE_MORE_APPLE arm's edx / eax | tried: else-if / inverted / nested / switch chains, one shared Conversation, ApplesGiven at function scope, int type, plain return, `>0` | next: find what keeps ApplesGiven's eax lifetime open past `test eax,eax` in our IL (lane gap: register allocation)
- 00ec9d30 NScript::CV_SickChildScript::CMansLover::Main DIFFER(4708v4708): block layout only. Retail falls through from `cmp edi,1; jne` into the threaded answer==1 -> give-letter block and routes those checks to the second identical exit copy 0x00ECA325; ours lays the NOT_SEEN else arm first | tried: GiveLetter flag (false / true by default / uninitialised), duplicated give-letter code (6225 bytes), answer and outer if/else swaps, a flag-free `answer==1` join, `ShownLetter||GiveLetter` join, CCharString== forceinline | next: a source shape where the answer==1 arm owns the give-letter code without duplicating it
- 00d706e0 NScript::CQ_HangingTreeGoodScript::CHTG_Guard::Main DIFFER(4755v4755): the SetPredicate(CIsThingAlive(Me)) temporary. Retail expands ~CIsThingAlive (vtable 0x012C32F0 in ebp, vptr restore) around an out-of-line ~CScriptThing 0x004AA840; ours calls ~CIsThingAlive out of line, so ebp is pushed later and the first terminate check has its own exit copy | tried: inline / __forceinline / implicit ~CIsThingAlive, with and without the row-local inline ~CScriptThing, definition order, definitions after Main | next: lane gap: the CIsThingAlive copy-inline case of INLINE_DEPTH_REVISIT.md (retail expands ~CScriptThing at the local sites but not inside ~CIsThingAlive)
- 00d2d930 NScript::CQ_EndGameFocalSitesScript::CEndGameJackOfBlades::Main DIFFER(5005v5094): the implicit MovieBase dtor at the four cutscene scope exits (0x00D2E028 / 0x00D2E2DC / 0x00D2E768 / 0x00D2EB30). Retail expands it (call 0x006E7AB0; GROBase vtable store; call 0x0099A430); ours calls the COMDAT 0x006E7B80 | tried: switch, do-while(0) on the checks and on the movie scopes, full MOVIEBASE_FORCEINLINE (adds a MovieBase vtable store per site), /d2inlS14/16/20 as a diagnostic (S14 = default, S16+ flips other inlines) | next: lane gap: the long-Main MovieBase dtor inline budget (chunks 797 / 800 / 804)

Row notes:
- AppleGirl: helper declarations are row-local (IsDistanceBetweenThingsUnder `__fastcall`, matching engine/fable_script_distance.h);
  an inline CTimer ctor/dtor as in LostTrader.
- MansLover: FadeOutAndKillEntity 0x00CBE716 is spelled with the PDB signature (const CScriptThing&, float, bool, bool) of
  engine/fable_script_fade.h (`?FadeOutAndKillEntity@@YIXABVCScriptThing@@M_N1@Z`). The `_NM1` spelling is chunk 804's
  NAME_FIXES item and is not repeated here.
- JackOfBlades: CScriptThing::operator= is __forceinline over the declared-only CCountedPointer<CScriptThing>::operator=
  0x00704580, so all four MarkerHero sites expand and tail-merge as in retail. The STB operator= comes from hand800's Scythe
  template.
- Guard: IsDistanceFromThingToPositionOver 0x00CBE45C stays a row-local declaration (no shared header has it).

## Skipped
None.

## Label corrections
- 0x00D2D930 is NScript::CQ_EndGameFocalSitesScript::CEndGameJackOfBlades::Main, not CPermanentBarrier::Main. The chunk table
  and oracle module say CPermanentBarrier; the labels file already has the right name, and CPermanentBarrier::Main is the
  landed 0x00D2D830.
- 0x00EA3490 is NScript::CV_LostTraderScript::WatchForQuestFailure (bound by landed 0x00EA2E80; both PDBs). The manifest label
  'CV_BeardyBaldyScript::WatchForQuestFinished' is a byte-identical-body propagation. The vland RELOC_NAME warning seen in the
  includer recheck comes from it.
- 0x00704580 is CCountedPointer<CScriptThing>::operator= (FableWin.pdb ??4?$CCountedPointer@VCScriptThing@@@@QAEAAV0@ABV0@@Z, the
  spelling src/00d41d00.cpp binds). The label '_global::CCountedPointer_Set_00704580' is a placeholder (vland RELOC_NAME warning).
  reloc_bindings.tsv also binds this one address to `CCountedPointer<CScriptThingImp>` (landed 0x00CF8970) and to a non-template
  `CCountedPointer` (landed 0x00CFD6C0), which breaks one-symbol-one-address; NAME_FIXES.tsv lists both.
- vland's other warnings are the usual RELOC_ALIAS `_rand` -> import thunk MSVCR71.DLL::rand 0x00BFEB16 (both rows), not a fault.

## Header debt
None. The four headers carry opt-in branches only.
