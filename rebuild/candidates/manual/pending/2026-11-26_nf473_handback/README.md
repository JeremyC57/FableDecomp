# NF473 hand-back (2026-11-26): seven more CAIStateGroup vftables, with their slot rows respelt

`README_agent.md` is the agent's own report.

## Landed: b0_hdr (1 header) + bS1a (25 rows, 7 units), one commit after equiv_check
- **Header:** `CAIStateGroup_MinionKidCalledToMother.h` gets a nested add-only opt-in declaring the private `FindCallHomeEvent` (Ego_r + FableWin). One assert is pinned.
- **Units:** StandStill, MinionKidCalledToMother, WanderAroundInitialPos, Freeze, KrakenTentacleIdle, KrakenTentacleRise, KrakenTentacleAttack.
  - Each unit respells its misspelt slot target onto the PDB virtual. Then its ctor emits the full 56-byte `??_7`, equal to retail, with the `??_G` home as a thin row.
  - MinionKidCalledToMother also respells the `CIntelligentPointer<const CMessageEvent>` vftable 0x0128B0C0 and its second binder, 0x009009EA.
- **T3 closed (`REL=nf319,nf322,nF325`):** the three 0x0090EEB0 lines.
- **Coordinator, after landing:**
  - 7 relabels and 25 SELF_SYMBOLS lines;
  - bindings_multi 1; funclets 1,733, 0 broken; s units 188, 0 failed; descriptive_check 2,030, 0 FAIL.
  - **Tracker:** +17 to agree_pdb (15 rtti_identified, 2 split_unknown), nothing worse.

## Held
- **CreatureGoUnderground** (0x008FE30C, 0x0090101E, 0x00920710): r473b declined, see below. T3 nf324 / nf308 stay open, and the h_920710 table line was not applied.
- **MinionWander** (0x0092BF30, 0x008FEB3D, 0x008FEB4F): the header port pulls `fable_containers.h`, and with it libcp, through CThing.h (r473d).
- **bU3 / 0x0091A570:** r473e declined, see below.

## Rulings (coordinator, on precedent)
- **r473a: granted.** The respelt rows may keep their unrelated HEAD local models; edits to those are only for access / const (r457d, the NF466 C1 precedent). No new local models were added.
- **r473b: declined.** `reinterpret_cast<const CThing&>` to dodge a header is the r460d form.
- **r473c: granted.** 0x013BB68C is retyped from unsigned char to bool (PDB IsRunnable returns bool; objproof ALL_EQ). The name stays the existing stand-in, still open.
- **r473d: waits on NF468.** The lane-wide `/D_USE_ANSI_CPP` proof is owner approved and running; if it is applied, libcp goes away for these rows. Otherwise the route is a container-free CThing view.
- **r473e: declined.** Variant A would add NEQ pick-any copies of `~WorkBase` and of two `??_G` names that retail binds elsewhere. VC7.1 keeps the first copy, so that risks wrong code. bU3 stays held.
