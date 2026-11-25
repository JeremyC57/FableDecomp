# NF435 drafts held (not batches)
- 006e72a0 CGameScriptInterface::Validate: q.py RELOCATION_MATCH with the add-only CClassFactory.h opt-in FABLE_RETAIL_CCLASSFACTORY_VALIDATE
  (hdr/engine/CClassFactory.h; original ../hdr_CClassFactory.h). name_gate FAIL: RELOC_NAME, the catalog labels the tail-jump target 0x00A36FE0
  `CListOwner_00a36fe0::CallAll` (landed self `?CallAll@CListOwner_00a36fe0@@QAEXXZ`, a made-up list-walk class). The real name is
  ?Validate@CClassFactory@@QAEXXZ (FableWin /Od callee + Ego_r publics; NAME_FIXES). The respell needs a port of the 0x00A36FE0 row onto
  engine/CClassFactory.h (ClassList walk calling a CClass virtual, slot 1) -- not done this round. Land this draft after that port; no test yet.
- 0089dff0 CGameScriptInterface::EntitySetAlwaysBlockAttacksFromThing: the CTCCombat.h polymorphic branch did not compile in two distinct macro
  sets (plain: ECombatAbilityDataType undeclared + size assert line 177; with FABLE_RETAIL_CCOMBATABILITY / _CCOMBATABILITYDATA: cl.exe hung
  >150 s, killed by PID). No #error in CTCCombat.h / CCombatAbility*.h. Loop rule: stopped after 2. The CTCCombat.h opt-in
  FABLE_RETAIL_CTCCOMBAT_ALWAYSBLOCK (AddThingToAlwaysBlock 0x007352D0 / RemoveThingToAlwaysBlock 0x00734190, Ego_r publics) is in hdr/.
