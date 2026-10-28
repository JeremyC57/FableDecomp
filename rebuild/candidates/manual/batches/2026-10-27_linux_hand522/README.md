# Chunk 522 (agent522, 2026-10-28): 9 of 20 re-queued single functions landed (161-495 bytes)

Re-queue chunk of 20 rows of 161-495 bytes (0x004FA0A9..0x00663990): NUISystem components, inventory tabs, creature
actions, player GUI and def persistence. One Opus 5.5 agent at high effort.

- **Landed:** 9 rows with `vland.py --land`, all RELOCATION_MATCH, every test PASS under g++ and VC7.1.
- **RELOC_CONFLICT settled:** 0x0057846F (~CTCInventoryExperience) binds std::vector<CEnginePrimitiveHandle>::~vector to
  0x00440C40, where landed 0x00BE25F0 had bound the same symbol to the byte-identical 0x00AF7210. Ego_r.pdb types the
  game members as `vector<CEnginePrimitiveHandle>` and the engine members (Primitives, ParentHandleList) as
  `CArray<CEnginePrimitiveHandle>`, and `??1?$CArray@VCEnginePrimitiveHandle@@@@QAE@XZ` is a PDB symbol; two retail
  addresses cannot carry one COMDAT name. 0x00AF7210 is respelled the CArray destructor in 0x00BE25F0 (rechecked
  RELOCATION_MATCH, test PASS).
- **Parked (11, pending/2026-10-27_linux_hand522/differ/):** register allocation or frame-slot residues only (0x004FA0A9
  the TransferVectorOfSubComponents eax/edx tie; 0x005E1CB0 ebx/edi swap; 0x00654438 slot order in the inlined ceil, a
  retail_asm candidate; the rest register / spill-slot choices).
- **Identities:** all 20 row names proven; ~30 callees proven.

Name fixes (NAME_FIXES.tsv, 65 entries, deferred to the naming pass), e.g. 0x00663990 is static
CThingCreatureBase::CreateWeaponForThingFromDef, 0x006924B0 CCreatureActionBase::GetRandomAnimFromName (not
GetTableAnimation), 0x00533CC0 NUISystem::CComponent::AddChildDynamically, 0x005E0E20 `std::pair<ulong,CAnimationEntry>::
operator=`, CRetailVector stand-ins are STLport std::vector, 0x004FA0A9's element is CCardDef.
