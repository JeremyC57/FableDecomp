# Chunk 678 (NA023, 257-512 band) -- 9 of 10 landed (2026-11-01)

Nine RELOCATION_MATCH rows landed (`verify.log`; bindings multi-check 0): the implicit quest-script destructors
~CQ_SecretPassageScript 0x00DF10D0 and ~CV_BookCollectingScript 0x00E57AA0 (identity by the ??_G slot-0 caller, ctor, Alloc and
"S_" registration string), ~CDisplayManager 0x009C01C0, CTCPhysicsRigidBody::Move 0x0075AEC0 (slot 87), ~C3DAnimFileLoader 0x00A996D0,
~CBankFile 0x009CE180, libvorbis 1.0.1 res1_forward 0x00CA1060 (residue1 export bundle slot 6), CTCDExperienceOrb::OnCollected
0x0077A180 and CTCContainerRewardHero::RewardHeroWithItems 0x0074EE90 (23 probes). Five catalog labels were wrong and are fixed by
`ghidra_out/labels_chunk678_2026-10-27.tsv`.

Decisions at landing (coordinator, under the owner delegation):
- 0x0074EE90 compiles a partial same-TU `auto_inline(off)` stand-in for the unlanded GetRewardItems 0x0074E860 (retail's TU order put the
  real body ahead of it). The stand-in is never landed: the verifier bound the call as an ordinary relocation to 0x0074E860 under its
  PDB symbol, and the stand-in text is not a candidate for that address. Same for the same-TU LowerBound / GetSubDef<T> copies: COMDAT
  duplicates of landed text, bound by address, nothing re-landed.
- 0x0074EE90 spells 0x013B86A0 `g_pMainGameComponent` (the GOTCHAS-era spelling); the established spelling changed to the PDB static
  member `CMainGameComponent::PMainGameComponent` on 2026-11-01, so a c678 queue row respells it at the next naming wave rather than
  touching the landed byte-exact text now.
- Header debt (local models next to method-less flattened headers for CBankFile, CTCPhysicsRigidBody, CTCBase, CPlayer, CPlayerGui ...)
  stays for migration wave 10; `rebuild/include/fable_string.h` should gain the PDB default `long length = -1` on CCharString(const char*, long).

Parked: CBankFile::OpenReadOnly 0x009D06F0 (DIFFER 385v385, 16 probes, block order only: retail places the FindContainedBank-failed exit
right after the find test and the version check jumps back to it; VC7.1 here merges the two exits). Next try: same-TU noinline bodies for
FindContainedBank and ~CCountedPointer<CRetailBank>, or a whole-TU build. Best source in `../../pending/2026-10-27_linux_hand678/differ/`.

NAME_FIXES.tsv: 37 rows (queued as c678). Lessons in LESSONS.md.
