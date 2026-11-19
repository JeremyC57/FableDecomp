# b4 (hand3c2, Account B, 2026-11-19): 0x00AABF60 CSoundEmitterLH::PlaySample -- READY

- Grade: RELOCATION_MATCH (g.py + strict vland `RELOCATION_MATCH PASS`, LANDVERIFY_STRICT_HEADERS=1). Purity: genuine (plain C++).
  ATTEMPTS.log: 2 grades this session (DIFFER(620v620) first port, RELOCATION_MATCH) + 3 no-ATTEMPTS probes (probe/bf60 va/vb/vc). Flags: none.
- Leaf ?PlaySample@CSoundEmitterLH@@UAEKAAVCASoundBank@@J_NABVCSoundInit@@1@Z, 620 bytes, thiscall `ret 14h`. (The draft's leaf
  spelled the last bool `_N` where VC7.1 back-references it as `1`; fixed. The Ego_r forwarder has the same `@@1@Z` tail.)

## Identity
vtable 0x0129E6EC (RTTI CSoundEmitterLH) slot 3 = the engine/CASoundEmitter.h slot PlaySample (Anniversary PDB order), and the
Ego_r.pdb / FableWin.pdb forwarder ?PlaySample@CThingSoundEmitter@@QAEKAAVCASoundBank@@J_NABVCSoundInit@@1@Z calls slot 3.
No Ego_d record (the LH sound emitter is not in the PC debug build); parameter names from the header.

## What closed it
1. Port onto the landed sibling PlaySoundEvent 0x00AABD50 (same includes, opt-ins and descriptive audio classes; the earlier
   "LTCG" label was wrong -- VC7.1 hoists the invariant FindFirstSetBit call of the pitch loop by itself, as in the sibling) plus the
   engine-bank lookup (slot 4), the bank-sample play (slot 14) and the sample query (CSampleQueryLH 0x00C08750): DIFFER(620v620),
   one hunk: the EmitterId ternary's block order.
2. FITTED (`// Inferred:` line in the source): `(init.OverrideID != -1) ? init.OverrideID : EmitterID`. Probes: `== -1` ternary and
   `if (== -1)` DIFFER(620v620); this ternary and `if (!= -1) .. else ..` RELOCATION_MATCH. The sibling 0x00AABD50 matches with `== -1`.

## Overlay header (hdr/engine; copy of today's repo header + NEW opt-in; default path unchanged; no asserts in this header)
| Header | New opt-in | What |
|---|---|---|
| LHA_CEventPlayOptions.h | FABLE_RETAIL_LHA_CEVENTPLAYOPTIONS_PARAMSETTERS | declarations SetLoopCount(int) 0x00C0A860 / SetInterruptable(bool) 0x00C0A880 (descriptive) |
No overlay of this header in ../hand3c/hdr; no landed row defines the new opt-in.

## Test (test/00aabf60.cpp)
Links the REAL row object (the counted-pointer ctor / dtor, list push_back / node creation, DeleteData COMDATs run as compiled).
Raw native-layout fixtures for every offset the row reads; the out-of-line callees with retail semantics (CEventPlayOptions ctor
0x00C0A790 every default, the setters, SetFlag16 / SetFlag through 0x00C0AE70's index bound); interface slots as __fastcall fakes.
An independent model predicts the event list (receivers, arguments, the full 0x5C-byte option image at each play call), the return
value, Playing and the tracked-sound list node (Data, refcount 1, deleter). 25 targeted cases (every boundary: sample 100000 /
100001 / -1, NoPlays -1 / 0, override -1 / 0 / -2, FindSample -2 / -1 / 0, play handle -1 / 0, gain bits 16 / 31 ignored, the
pitch loop's hoisted -1 index) + 2000 random runs. Prints `00aabf60_TEST PASS`.

## Mutants (mutants.tsv / mutants.log): 47 / 47 KILLED
Every condition (both early-out tests, the bound `>` / value / signedness, loop count ternary, loop / Is3D, HeadRelative, both
fader loops, position branches, EmitterId ternary, FindSample result boundary, handle >= 0 boundary), every stored value and
argument (query group / sample, SetInterruptable, PlayBankSample group / index order, CActiveSound arguments), Playing and the
return value. One equivalent mutant not listed: `pitchBits ^= 1 << bit` -> `pitchBits = 0` (retail's pitch loop only terminates
for 0x80000000, where both agree).

## Names (NAME_FIXES.tsv; all descriptive -- the LH audio library has no RTTI, no PDB names)
0x00C0A860 SetLoopCount, 0x00C0A880 SetInterruptable (both landed as CAnimateThing::SetAsFinished, a propagated label),
0x00C08750 / 0x00C08780 CSampleQueryLH ctor / dtor (labelled CVtable_012b1a54 / SetMemberConst_00c08780). Slot names (FindSample,
PlayBankSample, PlayEvent, GetEventManager) are descriptive virtuals of the row-local classes, as the sibling's.

## Files
src/00aabf60.cpp, test/00aabf60.cpp, hdr/engine/LHA_CEventPlayOptions.h, LHA_CEventPlayOptions.h.diff, payload.json, oracle.tsv,
mutants.tsv, mutants.log, vland.log, NAME_FIXES.tsv.
