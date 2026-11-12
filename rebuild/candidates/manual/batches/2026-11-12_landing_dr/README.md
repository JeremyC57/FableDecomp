# 2026-11-12 landing dr -- chunk Q24: 2 rows landed, 1 held back

Source: `pending/2026-11-12_linux_handQ24/`. Landed (RELOCATION_MATCH, test PASS under VC7.1, LANDVERIFY_STRICT_HEADERS=1):
0x00584B86 CTCInventory::AssignThingPC, 0x009289B0 CAIStateGroup_BanditPlot::Act.
Held back: 0x00622A00 OpenInventory, which needs a stand-in body of another retail function (ConstructQuestList 0x0061B610, reserved by
the second account); see the pending README.

Headers (7, three-way merged against d71c14ffa, all unchanged upstream, additions only; hdrcheck2 default=same, every DIFF macro new):
CTCInventory.h, CTCInventoryQuests.h, NInventory_CTCInventoryBase.h SELECTEDCATEGORYINDEX, CTCHeroCentre.h GETQUESTCARDS,
CAIStateGroup_BanditPlayDice.h, CAIStateGroup_BanditPlot.h, CParticleSimulation.h READFROMFILE.

Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011dr):
- 0x00929550 is CTCDiceGame::SetCurrentPlayer (landed `_unattributed::AssignTarget_00929550`).
- 0x009285D0's Leaf is QAE, but the PDB has protected virtual MAE.
- 0x005BC4B2 GetSelectedCategoryIndex and 0x0066E799 SelectScreenOfType.
- RELOC_NAME claims: 0x00429160 binds `?ReleaseData@?$CCountedPointer@VCAction@NUISystem@@@@QAEXXZ` but is landed as
  ~CCountedPointer<NUISystem::CAction>; 0x0042ABCA binds `~_List_base<CComponent*>` but is landed as `~list`.
