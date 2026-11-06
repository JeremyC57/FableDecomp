# Landing 2026-11-05u: chunk 794 (3 of 6 rows landed, 3 parked)

One expert agent authored the rows (sharing the lane with the chunk 795 agent). All three landed rows are plain C++ and grade
`genuine`.

| Address | Name | Grade |
|---|---|---|
| 0x008304A0 | CThingAICreature::Save | RELOCATION_MATCH PASS (2 grades) |
| 0x007039A0 | CThingObject::OnHit | RELOCATION_MATCH PASS (13 grades) |
| 0x0079ACC0 | CTCSummonSpell::CreateMinion | RELOCATION_MATCH PASS (12 grades; regraded after the coordinator respell below) |

- **Key spellings:**
  - 0x008304A0: the remapped UID is a named local first.
  - 0x007039A0: the BARREL branch's village-member lookup uses a non-const GetTC, so it binds LowerBound 0x0040F020 at 0x00703D13
    as retail does (a const PeekTC spelling also graded RELOCATION_MATCH but bound 0x004365B0 there). The agent checked every
    relocation's symbol against its retail target.
  - 0x0079ACC0: same-file `__declspec(noinline)` bodies for GetSubDef<CSpecialEffectsDef>, GetEffectFromString and
    GetDef<CBrainDef> (retail 0x0079BD80 is in this file); FableWin's inline CTCEffectOnDie setters; Normalise's squared length
    spelled `X*X+Y*Y+Z*Z` behind the new FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ opt-in.
  - **Coordinator respell:** the row bound 0x0138306C as a new global `GSummonCreatureGlobalDefNum`. Two landed CTCSummonSpell rows
    (0x0079A6E0, 0x0079B650) already bind it as the class static `?SummonCreatureGlobalDefNum@CTCSummonSpell@@2JA`, so the row now
    declares and uses that static (regrade RELOCATION_MATCH PASS, `logs/recheck_0079acc0_respell.log`).
- **Headers** (`hdr_README.md`): 21 headers gain opt-in branches and CTCHeroOpinionDeedLog.h is new. The default path changes
  only by the forward declaration `class CBrainDef;` in CThingAICreature.h; hdrcheck2 reads `default=same`. Two existing opt-ins
  read DIFF because sub-macros were nested inside them (FABLE_RETAIL_CTHINGAICREATURE, FABLE_RETAIL_CTCCREATUREOPINIONOFHERO): the
  coordinator rechecked all 21 landed sources that define them, 21/21 RELOCATION_MATCH PASS (`logs/recheck_macro_users.log`).
- **Names.**
  - Labels: `ghidra_out/labels_landing_2026-11-05u.tsv` (the six rows plus unlanded GetValidTeleportPos 0x0079AA90,
    GFFloatToCharString 0x009A08B0, CTCCreatureOpinionOfHero::InformOfDeed 0x00682810, CThingFilter_IsEligibleWitness::operator()
    0x006C8E90).
  - 12 queue lines (tag `land0511u`): GetDef<CBrainDef> 0x0079BD80, CHitParameters::IsMeleeStrike 0x00868860 (vland RELOC_NAME),
    GetValidTeleportPos, GFFloatToCharString, CTCParticleAttacher::DetachParticles 0x007F8860, CTCWallMountEffects::OnPutOnMount
    0x007DF3E0, and four multi-spelling targets this row exposed (0x0138306C, 0x004BE2C0 HasDummyObject, 0x006AEDC0 AddFollower,
    0x0076D270 AddAllyThing: the PDB spelling is the one 0x0079ACC0 binds; the older stand-ins are queued).
  - Descriptive only: GDeedUnknownThreatWitnesses (0x01375732), CGameDefinitionManager::PHeroStatsDef (+0xDC), NumMountedTrophies.
- **Landing:** `vland --land` (`logs/land.log`); labels applied; the manifest sync dry run prints 0.

## Parked (`pending/2026-11-05_linux_hand794/`)
| Row | Grade | Residue |
|---|---|---|
| 0x007C4430 CTCHealLife::StartHealingEffect | DIFFER(1980v1952), 9 attempts | retail calls the loop's float push_back out of line (0x00568850); a probe shows one extra `if` around it makes VC7.1 do the same, so the original likely has a condition not yet found |
| 0x006C6040 CTCHeroOpinionDeedLog::InformRelevantThingsOfDeed | DIFFER(2793v2377), 5 attempts | retail merges the two village-witness branches into one tail and keeps the list node in ebx |
| 0x007DE710 CTCWallMount::AttachObject | DIFFER(2574v2578), 6 attempts | x87 operation order inside the inlined matrix multiply |

Two parked rows got fewer than the brief's 10 attempts (5 and 6); they are candidates for a retry pass.
