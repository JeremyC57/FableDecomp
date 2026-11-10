# 2026-11-10 landing z -- chunk 1007b, retry of the CTCInventory parks (1 row)

Source: `pending/2026-11-10_linux_hand1007b/`. vland --land: 1 RELOCATION_MATCH, test PASS (VC7.1 and g++).

- 0x00589A30 CTCInventory::OpenPCMapMenu: one named local per child component instead of reusing `scroll` / `clickable`; the new candidate
  ids put `this` before `i` in the reload order after the inline `repe cmpsb` (c2prio).

Held (`held/`): 0x0058F649 OpenPCInventory, RELOCATION_MATCH (separate entry / arrow locals; FABLE_RETAIL_CDELETION_BODIES), but it calls
CASoundManager slot 46 as SetGroupVolume, a descriptive name engine/CASoundManager.h marks not landable until the owner accepts it -- held
with 0x0056F30B CloseInventory. Closer (differ/): AssignThingPC (34 difflines, block layout), GetStyleCardsListUIComponent(PC) (hair-card
arm placement), ConstructItemCategoryList DIFFER(1584v1587). The hdr/ overlay (FABLE_RETAIL_CTCINVENTORY_ASSIGNPC, new CAbilityDef.h) serves
only the AssignThingPC draft and is not applied.
