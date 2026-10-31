# Chunk 615: lessons

- Respelling with the PDB names is the first lever, not the last. It won 4 of 4 here (and cut 00a24d70 from 39 to 7
  lines) after the parked waves' local tweaks had stalled. A stand-in call shape (wrong argument list, wrong owner class,
  a member read instead of the inline accessor) changes VC7.1's IR, not just the symbols.
- Count the pushes against the PDB signature. In 00a24d70 the parked PeekData(handle, device) "used" two pushes that
  really belonged to the next call (LoadAnimFileCompressed(data, length, pool)) around a zero-argument GetData().
- A wrongly named virtual hides a real member. "SetSpin(m_spin)" was ModifyHealth(-MaxHealth). Check the slot's owner
  in the Ego_r name set and the retail member offset in the engine header.
- The out-of-line callee name sets settle the namespace and nesting: NCreatureMode::CModeKnockedDown and
  C3DAnimationInfo::CAnimationData. Confirm with rtti_bases.tsv.
- Frame size: a pointer out-parameter that dies early has to sit in its own block, or it takes an extra slot
  (00867010: frame 0xc -> 8).
- An inline wrapper that takes the index as a parameter (CMeshDataBank::GetAnimationType(index)) evaluates the argument
  before `this`, and its switch becomes a jump table. Retail had the if-chain in the caller.
- The PDB marks static members SI (`__fastcall`). Spell inline StatGetInterfaceType as `static ... __fastcall` so the
  COMDAT symbols match. The bytes do not change.
- tall.py needs wine/env.sh sourced. Without it, verify_and_land creates a literal `D:\Documents\FableTLC` directory
  under the cwd.
