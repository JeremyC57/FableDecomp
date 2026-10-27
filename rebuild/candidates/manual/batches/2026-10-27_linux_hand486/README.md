# Chunk 486 (agent486, 2026-10-27): 35 of 40 single functions byte-exact

Second queued 40-row chunk of single functions under 128 bytes (address order). One Opus 5.5 agent at high effort.

- **Landed:** 35 rows with `vland.py --land` (verify.log: 14 MATCH, 21 RELOCATION_MATCH, every test PASS, no
  RELOC_IDENTITY).
- **Parked (3, pending/2026-10-27_linux_hand486/differ/):** 0x004AD810 GetAllThings<const CThing, ..., CThingFilter_None>
  (122v125: retail keeps a constant `xor al,al ; jne` after the InLimbo test and homes the pushed pointer in the dead
  argument slot), 0x004C7870 (15v15, `xor al,al` before vs after the null test), 0x0051EA30
  CThingManager::FillRenderListsWithAllEntities (86v88, register allocation).
- **Skipped (2):** 0x004DDAA4 (three packed functions) and 0x004E0ADC (four) need re-splitting in the catalog.
- **Identities:** 27 proven (in ghidra_out/labels_chunk486_2026-10-27.tsv), 6 probable, 4 descriptive (NOTES.md).

Label corrections (NOTES.md): 0x004C7AE0 is CThing::GetPWorldSeed (not CViewBase::GetDisplayEngine), and landed
0x004C7AA0 / 0x004C7AB0 are CThing::GetWorld / PeekWorld; 0x00545840 is ~CSpellContainer (not ~CZoomingComponent);
0x004C2C00 is `__unguarded_insertion_sort_aux<CMeshInfo*, ..., CSortPrimaryMeshesLast>`; 0x0052EFE0 / 0x0052F180 are
CComponent::GetType / IsBastardChild; 0x0041E5F2 is NUISystem::CManager::GetInstance; 0x004C3BF0 is `__pop_heap_aux`;
0x00512CE0 is `_Rb_tree<unsigned long>::operator=`; 0x004C7590 is bound under three spellings in landed rows.
Header debt: CObserver, CClass, CRegion, CQuestManager kept local models (flat generated headers).
