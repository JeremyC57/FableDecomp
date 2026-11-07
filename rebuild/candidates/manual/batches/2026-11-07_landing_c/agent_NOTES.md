# Hand chunk 828 (CMeleeOpponent .. CAIStateGroup_JackOfBladesStage2): 1 of 6 byte-exact (six retries)

All six names were already proven (two sources each, see the parked sources' Identity lines); nothing re-proved. Grades below are
q.py with `FABLE_HDR_OVERLAY=hdr`; "md" = masked diff lines (bin/md.py: relocations masked, frame offsets kept). Every attempt,
with what VC7.1 emitted, is in STATUS.tsv (notes column) and ATTEMPTS.log; probes are in probe/<addr>_<tag>.cpp.

## What matched (1)
- 0x00E2C100 NScript::CV_AmbushScamScript::CAS_Trickster::Main (8945 B) -- RELOCATION_MATCH (q.py and read-only vland: RELOCATION_MATCH
  + PASS; only WARN RELOC_NAME display text for CDefString::operator CCharString). Parked by chunks 816 / 818 at 2 lines (one
  scheduling swap in the shared PauseMyEntitiesPlease return pad 0x00E2E38E) after 22 attempts. Fix (first new attempt, t1): the
  final wait loop `while (!T) { ...; NSF; }` written in the chunk 813 / 824 loop form `for (;; NSF) { if (T) return; ... }`. The
  shared return pad then gets retail's this-first `mov edx,[ebp]; mov ecx,ebp; push 0` schedule. Relocations paired with
  bin/relpair828.py: 64 symbols, one target each; all 24 string literals checked against retail (bin/strchk.py). Test: g++ and VC PASS.

## Parked for Fable 5.1 (5)
- 00d56790 NScript::CQ_GuildTrainingMeleeScript::CMeleeOpponent::Main DIFFER(6150v6151): frame slots from stage 3 on (retail puts the
  Guildmaster / Thunder CScriptThing locals at esp+0x40 and every CCharString temporary 8 bytes higher; ours CScriptThing at esp+0xb0);
  stage-3 counter now load / inc / store as retail but via `add eax,0x48` (long&); stage-6 colouring | tried: 12 (plain-inline GetTime,
  long& / long* / named copy / parent pointer / parent reference / ++ / += 1L / 1 + x increments, row-local inline ~CScriptThing,
  FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY) | next: the declaration / block shape that gives the CScriptThing locals the low slot.
  Needs the hdr/ CMeleeOpponent opt-in (the repo header from chunk 815 has no CMeleeOpponent body: the chunk 810 source SRC_FAILs).
- 00e9e420 NScript::CV_IntroductionToTrophiesScript::CBriarRose::Main DIFFER(6035v5915) (re-grade md 392, was 440): per-exit dtor
  splits (retail calls ~CTimer / ~CScriptThing / movie dtor at the shared nested pad, ours expands one site more) and the prologue
  colouring that follows (retail edi = CScriptThing vtable, ebp = UnusedTimer; ours ebp = 0) | tried: 12 (inline / declared-only
  IsAlive, default ctor, ~CScriptThing; declaration orders; plain returns; for(;!T;NSF) and while(!T) loops; movie OUTOFLINE opt-ins
  dropped with and without the declared-only PImp dtor spec) | next: lane gap (per-site inline choice, chunk 807 class).
- 00d0c410 NScript::CQ_BanditCampBossBattleScript::CBanditForger::Main DIFFER(5255v5373) (md 765): frame 0xa0 v 0xa4, loop 1 rotated,
  movie dtor 6 calls v 5 expansions | tried: 12 (for(;;){if(!NotAttacked) break;..}, loop-2 form, bool done, explicit continue,
  else-continue forms, inverted talk test, named talked bool) | finding: `if (!talked) continue; {..} break;` (f5 / f13) gives
  retail's loop-1 head exactly (mov al,[esp+0x2f]; test; je) but VC7.1 then expands ~7 more return epilogues (+400 B) | next: f5's head
  with the epilogues kept as calls.
- 006c6040 CTCHeroOpinionDeedLog::InformRelevantThingsOfDeed DIFFER(2809v2377) (md 1026 -> 716 with the switch): the vandalism and
  0x35/9 witness arms are not cross-jumped (retail merges them at 0x006C6228) | tried: 10 (switch in two case orders, hoisted / per-arm
  locals, nested conditions, if / else-if order swapped, two plain ifs, chunk 821 loop form, an IS_THING_ALIVE probe) | finding: the
  two copies differ only in the GetTC key slot ([esp+0x28] v [esp+0x30]), eax / ecx in the HasTC bit test and the CSE'd bl = 3 |
  next: make the vandalism arm's GetTC key reuse [esp+0x28].
- 00974aa0 CAIStateGroup_JackOfBladesStage2::Update DIFFER(2928v3080) (md 903 -> 823): frame 0x800 v 0x80c; retail stores each HasTC bit
  to a bool slot [esp+0x23] before the je; action-temporary slots; rest of the first draft | tried: 10 (the facing angle written out
  per arm -- this one split the merged arms as retail; GETTC_EXPANDED / BITSET_INDEX / the 006c6040 TU's opt-ins; const bool& GetTC
  results; `me` locals) | next: a header opt-in GetTC body that stores HasTC first (CThing.h line 121 note), then the action slots.

## Callee names (Jack draft; checked against reloc_bindings.tsv: one symbol, one address)
The draft's spellings follow the PDB; three addresses are bound under a different symbol in rebuild/oracles/reloc_bindings.tsv
(NAME_FIXES.tsv): 0x007E4040 (bound as a free function; PDB: CTCWillResponse static), 0x008B7F80 (bound with pointer / long
parameters; PDB: references + bool), 0x008C33E0 (bound as a stand-in `?GetSubDef@CDefClassBase@@QBE_NPAUCDefPointer_..`; PDB and the
landed row: the template). No relocation identity could be checked for the five parked rows (layouts differ).

## Skipped
None.

## Label corrections
None: all six catalog labels and the NAME_FIXES addresses' manifest labels are right (LABELS.tsv is the header only).

## Header debt
None beyond hdr/ (one opt-in block).
