# Hand chunk 804 (six entity Mains: three arena fans, the singing-stones door man, the graveyard gate man, the ghost fisherman): 1 of 6 byte-exact

## What matched
- 0x00D8ABF0 NScript::CQ_HerosOldHouseScript::CGhostFisherman::Main -- RELOCATION_MATCH (src/00d8abf0.cpp, 4 attempts).
  - relpair: all bound-same except the compiler float `__real@00000000`. strchk: 40/40 strings in order.
  - test PASS under g++ (tall.sh) and VC7.1 (tall.py).
  - Shape:
    - seh_me plus the StartScripting wait, then `CScriptThing Booty = GetThingWithScriptName("HiddenBooty"); CTimer GhostTimer; GhostTimer = 0;`.
    - Loop `while (!ParentClass.WifeAttacked && !GhostGoing)`.
    - The talk block owns seh_hero, the cast map `std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>`, the movie and
      the pause.
  - Key spellings:
    - The declared-only `template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();` before the movie
      header. Retail expands the implicit movie dtor around a call to 0x006E7AB0.
    - The STB (ScriptedThingBase) assign and dtor stay inline (no OUTOFLINE opt-ins). The CTimer ctor/dtor/operator=/GetTime are
      inline through `pDSTGame`.
    - FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR and FABLE_RETAIL_STLP_MAP.
  - Callee names: all from the PDB name sets or landed rows (source header).
  - Label: the oracle and catalog spell it `sub_00d8abf0`. The PDB name and the vtable slot prove CGhostFisherman::Main.

## Parked for Fable 5.1
- 0x00CF18D0 NScript::CQ_ArenaHoldingScriptScript::COakvaleFan::Main DIFFER(4676v4539): 388 lines. The VC7.1 caller IL-size inline cap leaves the
  movie dtor as a call (??1CScriptGameResourceObjectMovieBase) at the second-loop and exit sites, where retail expands every site.
  - Proof: the same file is RELOCATION_MATCH with the diagnostic /d2inlS20.
  - tried: implicit movie (1314 lines), full MOVIEBASE_FORCEINLINE (531; one extra MovieBase vtable store per site), do-while(0)
    frequency levers (worse), no pause FORCEINLINE_CTOR (worse), FadeOut written in both arms (fixes the FadeOut schedule).
  - next: a plain-C++ spelling that lowers the caller's IL under the cap without changing bytes, a movie-dtor opt-in that expands with
    no vtable store, or the owner's ruling on the cap.
- 0x00CF2B70 NScript::CQ_ArenaHoldingScriptScript::CBowerstoneFan::Main DIFFER(4454v4298): 1523 lines, the same cap (9 movie dtor calls).
  - Proof: RELOCATION_MATCH with /d2inlS15 or /d2inlS20; /d2inlS13 is not enough.
  - tried: FORCEINLINE ctor+dtor (77 lines, the fewest), scoped movie blocks (1467), FadeOut 1/1 spelling, value-initialised movie,
    no pause FORCEINLINE_CTOR (489 lines but 12 calls), `continue` in the hit arm.
  - next: as for Oakvale.
- 0x00CF3D20 NScript::CQ_ArenaHoldingScriptScript::CHookCoastFan::Main DIFFER(4464v4298): 708 lines, the same cap.
  - Proof: RELOCATION_MATCH with /d2inlS20; S10 and S12 are not enough. Diagnostic cap1 (first loop removed) expands the later sites.
  - tried: full FORCEINLINE (33 lines, the fewest, with 10 MovieBase vtable stores), `== false`, do-while(0) levers (w1/w3/w4/dw1),
    FadeOut spellings (fx/f2).
  - next: as for Oakvale.
- 0x00ED17D0 NScript::CV_SingingStonesScript::CManWithDoorName::Main DIFFER(4149v4279): 224 lines, 4 event hunks.
  - Residue:
    - SetPredicate(CIsThingAlive(Me)): retail expands the predicate ctor but calls the CScriptThing copy 0x004ABE90 inside it
      (GOTCHAS 1913/2131).
    - Per-site movie split: retail expands the bribe-site movie ctor and calls 0x006E7B60/0x006E7B80 at the other five.
    - Retail expands two exit cascades that ours calls, so ours is 130 bytes short.
  - tried: implicit movie (752 / 790), OUTOFLINE ctor+dtor (301), a named predicate (340), no inline copy (350), STB OUTOFLINE_DTOR,
    nested or folded conditions, PresentedItem scoped (297), do-while loop (228 / 224).
  - next: a per-site movie spelling plus the open GOTCHAS 1913 predicate-copy recipe.
- 0x00DC4D70 NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageMan::Main DIFFER(4275v4319): 87 lines.
  - Residue:
    - CIsThingConscious(Me): retail calls the nested copy 0x004ABE90 and dtor 0x004AA840 inside the expanded predicate
      (GOTCHAS 1913/2258). Knock-on: retail keeps ebp == 0 (cmp reg,ebp).
    - The first movie ctor is expanded and the second called.
    - Exit split.
  - tried: the parked CSecretPassageWoman source (970), named/scoped predicate (429/87), dropping inline bodies (155/141),
    __forceinline predicate ctor (87), MOVIEBASE_OUTOFLINE_CTOR (134 lines, SDIFF 27), no PImp spec (565).
  - next: spm_a7 plus a first-loop-only expanded movie ctor; the GOTCHAS 1913 recipe.

## Skipped
- None. All six rows are single functions (no int3/nop plus a second body inside the row length).

## Label corrections
- 0x00D8ABF0: `sub_00d8abf0` -> NScript::CQ_HerosOldHouseScript::CGhostFisherman::Main.
  - Evidence: PDB name set; vtable 0x012D50C0 slot 1; labels_anniversary_2026-11-04_waves23.tsv. NAME_FIXES.tsv, LABELS.tsv.
- 0x00CBE716 FadeOutAndKillEntity: the PDB spelling is `?FadeOutAndKillEntity@@YIXABVCScriptThing@@M_N1@Z` (const CScriptThing&, float, bool, bool).
  - The `_NM1` (bool, float, bool) spelling is bound in reloc_bindings.tsv for 0x00E0F4E0 and declared in 13 landed rows (list in
    NAME_FIXES.tsv). Same machine code, wrong symbol.
- 0x00CDBFB0: the manifest's `CRetailTree_erase00cd344a::~CRetailTree_erase00cd344a` is the
  std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> dtor (chunk 799 hands back the same fix).

## Per-row details
Every source carries an Identity / Callees / Layout / Shape header block.
- Names: all six are proven. Each is slot 1 of its entity vtable, has a single .rdata reference, its ctor store sits inside the
  class's Alloc, and the Ego_r.pdb / FableWin.pdb name set has the name. None is descriptive. Locals (speech_movie, HaveTalked,
  SaidWellDone, SpokenLeaving, FriendEscape, ...) are descriptive.
- reloc_bindings check: every bound callee symbol is bound at the same address in rebuild/oracles/reloc_bindings.tsv. The one
  exception is FadeOutAndKillEntity, which has two spellings at 0x00CBE716 (above).
- Fan shape (all three):
  - Take seh_me, then a first loop `while (!PlayerAttackedFans && !ArenaOver)`. Each line is spoken in its own movie + pause
    block.
  - Then `if (PlayerAttackedFans)`: wait FansCanLeaveNow, re-take seh_me and run. Else: walk to FanLeavingMarker answering talk
    and hits.
  - `if (T) return; FadeOutAndKillEntity(Me, 1.0f, true, true);` is written at the end of BOTH arms. Retail tail-merges them
    (`push 1; mov ecx,edi; mov dl,1; push 1.0f`), which one shared copy does not reproduce.
  - The per-fan flags differ:
    - Oakvale: Introduced + GivenHelmet; WELL_DONE gives OBJECT_HERO_HELMET_CHAINMAIL.
    - Bowerstone: one Spoken flag for both first-loop arms, plus SpokenLeaving.
    - Hook Coast: Introduced + SaidWellDone, with WELL_DONE_10.
- Header debt:
  - The codex pending candidate 00cf0d20 (CEntranceGuard2::Main) needs CEntranceGuard2 in engine/CQ_ArenaHoldingScriptScript.h
    (hdr/README.md).
  - The landed CV_SingingStonesScript::Main 0x00ED10A0 and the landed fan Alloc/GetParentScript rows could move to the new headers.
