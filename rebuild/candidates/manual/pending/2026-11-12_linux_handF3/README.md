# Chunk F3 (Fable 5.2, rapid fire): the two rows three earlier agents could not close

Both matched, in 58 grades and about 62 tool calls. An independent evidence verifier (Opus) passed both.
- **Landed (landing ed): 0x006B05A0 CTCPhysicsBase::CreatePPhysicsMeshInfo.** The tail (HasPhysicsMesh / SetPhysicsMeshBankIndex /
  PhysicsDef / SetInteractionFlags) is written out in both arms of the graphic-type test; VC7.1 merges the copies from the end and
  the merged code keeps retail's `mov edx,[esi] ; mov ecx,esi ; push eax` order at both virtual calls.
- **HELD for the owner's decision (held_for_owner_decision/): 0x008031E0 CTCTargetingPlayer::GetPBestThingForMeleeAttack.** It is
  RELOCATION_MATCH with test PASS, but only with an INFERRED statement: `if (p_thing == &parent) return p_thing;` after the
  AreThingsEnemies call, whose two outcomes are the same return. The bytes show the parent had a second use that left no code
  (`mov ebx,[ebx+4] ; mov edx,esi ; mov ecx,ebx`); the exact wording of that use is not provable (Ego_r's build of the function is
  34 bytes longer, so the arm probably held code TLC compiled out). It needs hdr/engine/CTCTargetingPlayer.h (MELEE opt-in, parked
  here) when it lands.
