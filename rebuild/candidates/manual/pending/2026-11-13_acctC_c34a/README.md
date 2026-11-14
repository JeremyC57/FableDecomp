# acctC hand-back c34a (agentC34, 2026-11-14) — supersedes c32open

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x006A1A00 | CTCCarrying::GetCarriedThingInterpolatedPosAndOrientation | 864 | RELOCATION_MATCH | PASS |
| 0x007FC9D0 | CTCKraken::FrameUpdate | 704 | RELOCATION_MATCH | PASS |

Verified by the coordinator with `lane/coordverify.sh` (read-only vland, strict headers): `coordverify.log`.

Review items:
- 0x006A1A00: the second string site is `bool found; { CCharString secondary_name = def->SecondaryDummyPosName.GetString();
  found = ...; } if (found)` — FITTED shape (plain C++, default CDefString header); it is the only spelling found that calls
  GetString 0x00415DD0 as retail does (VC7.1 never expands the conversion forwarder while calling GetString). The up-vector
  spelling is evidenced by the store schedule at 0x006A1B73. `g_bTurnPrimarySlotCarriedThing` (0x0137574C) is a descriptive name.
- 0x007FC9D0: new additive opt-in `FABLE_RETAIL_CTCKRAKEN_EMPTY_UPDATES` (inline empty UpdateUnderwater / UpdateTentacles /
  UpdateTentaclesAndHead; real 16-byte functions in Ego_r, no out-of-line copies in retail); `case 0: UpdateUnderwater();`
  gives retail's 4-entry jump table. The named `effect` string in its own block is evidenced by the frame (0x14).
- RELOC_NAME warning: 0x0129BA3C is labelled g_NormaliseEpsilon; the shared C3DVector.h spells it DELTA_NOTIONAL_ZERO.
- 0x007AED10 is CTCBoss::GetBossPhase (reloc_bindings binds two wrong names): NAME_FIXES.tsv.

`open/007bc38a.cpp`: CTCShotDefinitionThrowPredictive::InitMovement DIFFER(880v880), one instruction (successor agentC36).
