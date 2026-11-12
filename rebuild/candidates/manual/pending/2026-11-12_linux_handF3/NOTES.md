# Chunk F3 notes

Matched 2 of 2 (payload.json / oracle.tsv; tests PASS under the lander's VC7.1 check; both regraded with LANDVERIFY_STRICT_HEADERS=1;
overlay $S/w22/chunkF3/hdr, unchanged from Q31 -- no new opt-ins).

- 0x008031E0 CTCTargetingPlayer::GetPBestThingForMeleeAttack -- RELOCATION_MATCH (38 grades here, +1 /Od probe).
  What made it match: the parent is a reference local with a SECOND use after the AreThingsEnemies call, in a compare whose two
  outcomes are the same statement (`if (p_thing == &parent) return p_thing;` in front of the function's `return p_thing;`). VC7.1
  keeps a 2-use variable in the dying `this` register and copies it to ecx; the compare and its branch are deleted after register
  allocation when the two arms are merged. INFERENCE, flagged in the source header: the bytes prove a code-free second use of the
  parent; its exact wording is not provable (Ego_r's build of the function is 34 bytes longer, so the arm most likely held code
  that TLC lost). Needs: the Q28/Q31 overlay opt-in FABLE_RETAIL_CTCTARGETINGPLAYER_MELEE (hdr/README.md); no flags.
  Ruled out here (all 150v152 unless noted): parent pointer reassigned after the call; dead store to `targeted` between load and
  call; parent through pointer-to-local / 2-element array; pointer wrappers with a thing-only null test (3 forms), both-null tests
  (2), `__assume` (177 / no change), parent used on the dead thing==0 path (3); by-value functor copies (3); GetTC ORIGINAL_CHAIN /
  default GetTC; CTCBase::ParentThing as a reference member; /Oa (CSE across calls: 152 bytes but another shape), /Ow, explicit
  /Og..; two-def parent (162), for(;;) wrapper; empty-bodied `if (p_thing == &parent) {}`; `parent.IsDead()` identical arms (164);
  the compare on the `return 0` side; smashable+enemy tests duplicated per GetTC outcome (2 forms); positive enemy test (160).
  A pointer wrapper with a null test on the parent (`a == 0 || b == 0`) gives retail's registers plus `test ebx,ebx ; je` (160):
  that showed the mechanism (second use); the identical-arm compare removed the residue.
- 0x006B05A0 CTCPhysicsBase::CreatePPhysicsMeshInfo -- RELOCATION_MATCH (20 grades).
  What made it match: the tail (HasPhysicsMesh / SetPhysicsMeshBankIndex / PhysicsDef / SetInteractionFlags) written out in BOTH
  arms of the graphic-type test (`if (Type == 5) { info = PeekStaticMesh; tail } else if (Type == 4) { info = PeekAnimatedMesh;
  tail }`) instead of once after a shared `info`. VC7.1 emits both copies and merges them from the end; the merged code has the
  `mov edx,[esi] ; mov ecx,esi ; push eax` order. Needs: the Q28 opt-ins FABLE_RETAIL_CTCPHYSICSBASE_CREATEPPHYSICSMESHINFO and
  FABLE_RETAIL_C3DMESHINFO_PHYSICSMESH; no flags. The local two-line `class CPhysicsMeshInfo : public CMeshDataBankEntry {}`
  stays (RTTI base, no engine header).
  Ruled out (all 110v110, both pushes): /G4 /G5 /G7 /GB; `CTCPhysicsBase& self`, pointer locals + if/else, early return on IsNull,
  inverted HasPhysicsMesh, value locals, one shared value variable, a value variable with trivially identical `return` arms.
  Written but not graded (fg.py stops at the first match): only the mesh half duplicated (v/j2.cpp), the switch form (v/j3.cpp).
  How it was found: the this-first order occurs at 12 of 1,942 register-object virtual call sites; the landed script rows that have
  it show the same statement with both orders, so block context decides; a learning grade with the call in two identical arms of
  a compare (v/i1.cpp) flipped site 1 (the common `mov edx,[esi] ; mov ecx,esi` is hoisted above the branch, the arms keep the
  push), and the one natural source of identical arms at BOTH sites is the per-type tail.

Made-up names: none (locals only: parent, info, bank). Probes: 1 (008031e0: /Od size of the draft vs FableWin's debug copy; not
usable, FableWin.pdb is a later-compiler build). Out-of-line real bodies in TUs: none. Header overlay: unchanged (a trial
engine/CTCBase.h with a reference ParentThing was removed again). v/ holds every graded variant.
