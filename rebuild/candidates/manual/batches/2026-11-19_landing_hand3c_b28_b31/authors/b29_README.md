# b29 (hand3c chunk 1, Account B, 2026-11-19): 0x008A8260 CGameScriptInterface::GetHeroSummonedCreaturesList -- READY

- Grade: RELOCATION_MATCH 466v466. Strict vland `RELOCATION_MATCH PASS` (LANDVERIFY_STRICT_HEADERS=1), no warnings. 3 grades in
  ATTEMPTS (2 on the old form, residue = merged early-exit epilogue; the IS_THING_ALIVE form matched at its first grade). Flags: none.
- Leaf ?GetHeroSummonedCreaturesList@CGameScriptInterface@@UBE_NAAV?$vector@VCScriptThing@@V?$allocator@VCScriptThing@@@std@@@std@@@Z.

## What closed it (measured with no-ATTEMPTS probes, probes/)
Retail keeps a private early-exit epilogue at 0x008A8428 (`pop esi ; mov al,bl ; pop ebx ; add esp,1Ch ; ret 4`) for the null / dead
hero; `phero && !phero->IsDead()` merges it into the main epilogue (456v466). Early `return`s duplicate the epilogue but constant-fold
the value (`xor al,al`, 453v466); an `else { added = false; }` gives 466v466 with the same `xor al,al`. A retail scan for duplicated
`pop esi ; mov al,r8 ; pop ebx` epilogues (probes/epscan.py) found the landed sibling CGameScriptThing::MsgGetThingsKilled 0x008D3DD0
with the same shape: `bool found = false; if (IS_THING_ALIVE(PThing)) {...} return found;`. The shared engine/ThingAlive.h
IS_THING_ALIVE (inline, two returns) on the hero gives retail's bytes.

## Shape and evidence
- IS_THING_ALIVE for the hero and both minion tests. FableWin.pdb has the instantiations
  ??$IS_THING_ALIVE@PAVCThingPlayerCreature@@@@... (0x018FFDA0) and ??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThingAICreature@@@@@@...
  (0x01B5F900). Only the hero test decides the bytes: hero-only, hero + summon, and all three all match. The debug size picks the
  all-three form: VC7.1 /Od /EHsc gives 418 bytes for it against Ego_d / FableWin's 417 (hero-only 478, hero + summon 455, the old form
  488; /Od alone, /RTC1 and /GZ do not come close). Recorded in the source header.
- Locals and names: Ego_d.pdb (thing_list, added, phero; ptc_summon_spell, ptc_ghost_sword; minion; minions, i).
- New opt-ins in the row: FABLE_RETAIL_IS_THING_ALIVE and FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT (both existing repo opt-ins,
  as in 0x008D3DD0); ThingAlive.h included. No overlay headers.

## Test (test/008a8260.cpp)
Links the real row object (its TC-map lower bound, vector copy constructor and CIntelligentPointer COMDATs run as compiled). Thiscall
doubles bound by /alternatename to the row's callees (GetMainPlayer, GetPControlledCreature, CBaseIntelligentPointer ctor / dtor /
GetPItem / SetPItem, GetScriptThing, ~CScriptThing, vector<CScriptThing>::push_back); malloc / free counted over the process heap.
Native-layout fixtures with offsets cited to instructions. Reference model: pushes and result; also balanced intelligent pointers, one
~CScriptThing per push, the copied minion storage allocated and freed once, the ghost-sword list untouched. 15 targeted + 2000 random
cases. Prints `008a8260_TEST PASS`.
Mutation check (tools/mut.py; header mutants on a temporary overlay copy of the repo ThingAlive.h): 16 one-decision mutants, 16 KILLED.

## NAME_FIXES
None.
