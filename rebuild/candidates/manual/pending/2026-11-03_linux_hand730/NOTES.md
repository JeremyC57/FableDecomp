# Chunk 730 (w22): CTriangleStripifier2 x4, NScript::CV_AmbushScamScript x3

Result: 0 matched / 7 DIFFER (parked) / 0 skipped. No payload rows (`payload.json` has an empty `authored` list), so vland was not run.
Every row got at least 10 compiled attempts on the full header mirror (LOG.tsv; FindBestStartVertex 20, Main 11).

## Rows (best source in differ/<addr>.cpp and src/<addr>.cpp, with its Grade line)
| address | row | best | grade | residue |
|---|---|---|---|---|
| 0x00F3BDD0 | CTriangleStripifier2::FillInTriStrip | f6 | DIFFER(2976v2998), dl 149 | the score/poly registers are swapped (edi/ebp); everything else follows from that |
| 0x00F3D980 | CTriangleStripifier2::BuildRings | b7 | DIFFER(2301v1947), dl 445 | push_back 0x009B4A20 and the CArray<ushort> copy ctor 0x00A87090 are out of line in retail; level*12 IV (0xBFFF4) |
| 0x00F3E120 | CTriangleStripifier2::FindBestStartVertex | a17 | DIFFER(1411v1411), dl 199 | register allocation only |
| 0x00F3EA50 | CTriangleStripifier2::GenerateTriangleListAndStrip | g4 | DIFFER(2603v2379), dl 658 | per-site _M_insert_overflow; zero register |
| 0x00E2B090 | NScript::CV_AmbushScamScript::Main | m4 | DIFFER(2183v2357), dl 152 | mixed inline/out-of-line ~CScriptThing |
| 0x00E2EC30 | NScript::CV_AmbushScamScript::GetTrickster | c6 | DIFFER(1053v1336), dl 297 | mixed dtors / copy ctor / GetDataString |
| 0x00E2F170 | NScript::CV_AmbushScamScript::SpringAmbush | s4 | DIFFER(2163v2781), dl 1318 | mixed inline/out-of-line ~CScriptThing |

## The blocker shared by all three NScript rows and two stripifier rows
VC7.1 decides inline expansion per call site. Retail expands ~CScriptThing (and push_back / _M_insert_overflow / the CArray
copy ctor) at some sites of one function and calls the out-of-line copy at others. With plain `inline` the decision is
function-wide (GOTCHAS 513: all or nothing); `__forceinline` expands every site (SpringAmbush 3743 bytes, Main 2394). The
known lever is in-body `#pragma inline_depth(0)` regions (GOTCHAS 629), which this lane's EXTRA_RULES forbid (owner,
2026-11-02: no new row lands with it). These rows join the INLINE_DEPTH_REVISIT family; no pragma-free shape was found.
Which sites retail expands: Main -- the exits of the two rotated `for(;;){ if (IsActiveThreadTerminating()) return; ...}`
loops, the inner MsgOnRegionLoaded wait and the after-wait test (four copies); every later exit shares one out-of-line
cleanup at 0x00E2B9AB. SpringAmbush -- the top-of-loop exits of both rotated loops and both post-loop paths; the ambush
exits and creature temporaries call 0x004AA840. GetTrickster -- GetDataString expanded at the Good site, called (0x004AA900)
at the Evil site; ~vector<CScriptThing> 0x008AC970 called at one exit.

## Shapes established (all in the parked sources)
- Main: entity bindings via `new CEntityScriptBinding<CV_AmbushScamScript>(name, this, &CAS_X::Alloc, 1)` (ctor forced
  inline by FABLE_RETAIL_CENTITYSCRIPTBINDING_FORCEINLINE_CTOR); morality test `GetHeroMoralityCategory()==2 || ==1 || ==0`
  -> evil quest, else good; spawned functions with an explicit `CCharString("")`; the closing
  `DeactivateQuestLater(GetActiveQuestName(), 0)` (slot 655 returns CCharString by hidden pointer).
- SpringAmbush: two named ambushers + four discarded CreateCreatureNearby per morality branch, the conversation, the
  WatchForAllAmbushersDead spawn, then the rotated wait loop (AllAmbushersDead -> break; trickster dead -> 
  TricksterKilledAfterAmbush). Health test `<= 0.0001f`.
- FillInTriStrip: scores 2..19 over the unused masked polygons of each cache-ordered vertex (how many corners are in the strip,
  whether the last two strip vertices are a swap pair, the winding); the sixth (bool) argument is never read.

## Header debt (overlay hdr/, mirror = rebuild/include + hdr)
- engine/CTriangleStripifier2.h: one `#ifdef FABLE_RETAIL_CTRIANGLESTRIPIFIER2` branch adding the PDB member set
  (GenerateTriangleListAndStrip, ReorderTriangleListInPlace, and the private helpers with demangled-PDB signatures).
  hdrcheck2: default=same. Landed includer 0x00F3D320 regraded on the full mirror: RELOCATION_MATCH (SIBLINGS.tsv).
- engine/CV_AmbushScamScript.h (new): NScript::CV_AmbushScamScript : CLevelScriptBase<CQ_SunnyvaleMasterData>, layout
  Ego_r minus 0x14 (QuestFinished +0x48 .. LeaderDead +0x70, HitCount +0x74, BuddyCounter +0x78, RandomSpeechTimer +0x7C,
  RandomSpeech[5] +0x80, sizeof 0x94 asserted), the nested CAS_Ambusher / CAS_Trickster / CAS_Buddy entity scripts.
  CAS_Buddy carries the three PDB bools (+0x1C..+0x1E); C3DVector BuddyStartPos is left out: the Ego_r dump says offset 31,
  natural alignment gives +0x20 with the same 0x2C size, and no row here proves it.

## Names (NAME_FIXES.tsv, LABELS.tsv)
- 0x00F3EA50 is GenerateTriangleListAndStrip (3 x CArray<ushort>&), not "GenerateStrips"; manifest module is still _global.
- 0x00E2BAC0 is AssignMoralityEtcForGoodEnding (catalog says EvilEnding twice); 0x00E2BB10 stays EvilEnding.
- Suspicious, not this chunk: WatchForQuestFinished is also labelled at 0x00D66E90 and 0x00E50100; ResetRandomSpeechTime is
  labelled at 0x00E1FEB0, outside this class's TU.

## Tests
test/00e2ec30_test.cpp (GetTrickster selection) and test/00f3e120_test.cpp (FindBestStartVertex closed-fan walk) print PASS
with g++; no matched rows, so no payload tests.
