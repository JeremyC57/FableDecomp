# Chunk 532 (agent532, 2026-10-28): 3 of 10 re-queued large functions landed (520-637 bytes)

Re-queue chunk of 10 rows of 520-637 bytes (0x0052A2D0..0x009EF4D0). One Opus 5.5 agent at high effort.

- **Landed:** 3 rows with `vland.py --land`, all RELOCATION_MATCH, tests PASS under g++ and VC7.1: 0x0052A2D0 and
  0x009EF4D0 (STLport `_Rb_tree::insert_unique(iterator, const value_type&)` for the NPC name map and `map<char,
  CCharString>`), 0x008334F0 CThingAICreature::GetGatherPosInBuilding.
- **Parked (7, differ/):** 0x005458A0 CSpellContainer::AssignSpell (inline budget), 0x0055E2C0 CryptoMatic (register /
  frame permutation), 0x005FCBE0 CTCInventoryMap::ConstructPastQuestsList (CDefPointer release shape), 0x00753630
  CTCLook::InitialiseTurningViaAnimation (x87 order), 0x007634E0 NShape::CBasicShape::UpdateSubdivisions, 0x0077DC40
  CTCHeroCentre::OnInitialActivate and 0x00834530 CombatStressTestDebug (LTCG register / slot sharing).
- **Landed name fix applied:** the crypto pairs were landed swapped. Ego_r.pdb orders every pair Encrypt before Decrypt
  (Buffer, StdString, CCharString String) and retail keeps that order; 0x0055F090 also sizes its output size()+32. Rows
  0x0055F090 (now StdEncryptString) and 0x0055F140 (now StdDecryptString) were swapped -- files renamed, names and the
  inlined EncryptBuffer 0x0055E4F0 / DecryptBuffer 0x0055E510 respelled -- and rechecked RELOCATION_MATCH with tests PASS;
  labels in ghidra_out/labels_cryptoswap_2026-10-28.tsv (supersedes labels_rename1's 0x0055E4F0 / 0x0055E510).

Other name fixes (NAME_FIXES.tsv, the rest deferred): CUnproven `_Rb_tree_iterator` leaves at 0x00527480 / 0x00527AC0 /
0x009EE1F0, 0x0052B8B0 is the map's `operator[]`, 0x009EF0D0 `_M_insert` is AAE, 0x0041D21B CManager::CreateComponent,
0x00661ED0 GetCombatManager returns a reference, 0x0077F440 std::sort (bound GuiVarBoolHelper). All 9 catalog labels for
the chunk rows were wrong.
