# Chunk 500 (agent500, 2026-10-27): 40 of 40 single functions byte-exact, every name proven

Queued 40-row chunk of single functions under 128 bytes (address order, 0x00D56750..0x00E04AE0: NScript quest and
entity scripts). One Opus 5.5 agent at high effort; 36 rows matched on the first grade.

- **Landed:** all 40 rows with `vland.py --land` (verify.log: 9 MATCH, 31 RELOCATION_MATCH, every test PASS, no
  RELOC_IDENTITY, no warnings). Nothing parked or skipped.
- **Identities:** 40 proven (ghidra_out/labels_chunk500_2026-10-27.tsv): 30 entity-script virtuals (Init slot 2,
  OnPredicateFail slot 5, one Main slot 1) and 6 quest-script Init overrides (slot 3), plus others in NOTES.md. Each
  name is the vtable slot of the vtable stored by the class's already-named Alloc or ctor, and the exact mangled name
  in both Ego_r.pdb and FableWin.pdb. Two member names (+0x40 `Game`, +0x44 `PMasterData` on the quest classes) are
  descriptive.

Label corrections (NOTES.md): all 40 `sub_<addr>` placeholders named; 0x00D73220 is the CHTG_Executioner ctor (not
CACompressor::CACompressor), 0x00D5DC50 the CQ_GuildTrainingWillScript ctor, 0x00DBF190 the CQ_OakValeBanditRaidScript
ctor. Review: landed entity GetParentScript bodies (0x00D69ED0, 0x00D6A070 and siblings) are `T::Get` over pad structs
where the PDB has `?GetParentScript@<Entity>@<Quest>@NScript@@UBEPAVCScriptBase@@XZ`; landed 0x00D83F20 and 0x00DCD1D0
use global non-virtual spellings; 0x00F35B30 (`?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ`) has about 40
other spellings in reloc_bindings; landed 0x008ADF90 spells its vector with one template parameter.
