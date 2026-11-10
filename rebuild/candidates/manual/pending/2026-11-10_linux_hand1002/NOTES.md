# chunk 1002 -- CTCInventoryWeapons (agent1002, 2026-11-10)

Result: 8 of 18 rows RELOCATION_MATCH with passing tests (src/, test/); 1 DIFFER parked (differ/005c71aa.cpp); 9 not attempted.
Grade every src/ and differ/ file with `FABLE_HDR_OVERLAY=$A/hdr` exported (gall.sh as shipped does not export it; run it as
`FABLE_HDR_OVERLAY=$A/hdr bash gall.sh`).

## Identities (Ego_r.pdb + FableWin.pdb name sets, retail order, vtable 0x01253FF4 checked in Fable.exe)
- 0x005C4671 GetFreeAugmentationSlotForWeapon(long); 0x005C66B2 / 0x005C8176 ConstructWeaponItemEntries(PC) (private, AAE);
  0x005C71AA ConstructWeaponDescriptionNoDamageNorWeight(CWideString&,long,float,bool) const; 0x005C7452 ConstructWeaponDescriptionDamage
  (CWideString&,long,float) const; 0x005C8101 PutWeaponsInSheathes(); 0x005CB128 RemoveItemFromInventory(long,long) (vtable slot 79);
  0x005CB2B0 SetThingAsActiveWeapon(long,CComponent*); 0x005CB974 OnInitialActivate() (vtable slot 6).
- Callees named from the PDB in LABELS.tsv; wrong bindings in NAME_FIXES.tsv (notably 0x005C552C RemoveWeaponsFromSheathes, bound as
  TakeWeaponsOutOfSheathes / PrepareWeaponSheathing, and CTCHero +0x0C WeaponsUsable, spelled m_weaponsSheathed in landed 0x005C9962).

## Header debt (overlay in hdr/, documented in hdr/README.md; all opt-in, default paths unchanged)
- engine/CTCInventoryWeapons.h `FABLE_RETAIL_CTCINVENTORYWEAPONS_METHODS` (all remaining PDB members, declarations).
- engine/NInventory_CTCInventoryBase.h `_CREATEOBJECT`, `_PERITEMDATA_CTOR`, `_ONINITIALACTIVATE`.
- engine/CTCCarrying.h `FABLE_RETAIL_CTCCARRYING_REMOVETHING`; engine/CTCInventoryItem.h `_DETAILS`, `_ENGINEGRAPHIC`;
  engine/CTCWeapon.h `FABLE_RETAIL_CTCWEAPON_MEMBERS`.
- New headers: engine/CTCObjectAugmentations.h, engine/CObjectAugmentationsDef.h, engine/CEngineGraphic.h, engine/CTCCarryable.h.
- The rows also use existing opt-ins (TU headers are at the top of each src file): CTHING_GETTC_EXPANDED, CVECTORMAP_FORCEINLINE_FIND,
  STLP_VECTOR_RESIZE_OUTOFLINE, CDEFPOINTER_RELEASE_POINTEE, CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY / _VISIBLE_CALLED_CTOR,
  CDELETION_BODIES, CINTELLIGENTPOINTER_CTOR_OUTOFLINE / _ASSIGN_OUTOFLINE, CTCCARRYING_PUTTHING; GFFloatToLongNear (shared-header helper).

## Next steps per unmatched row
- 0x005C71AA (DIFFER 680v680, 8 lines): only the TEXT_GUI_MENU_NONE block's argument order differs. Try placing the NONE append in a
  helper-free spelling that keeps one join point (e.g. `if (none) { const CWideString& t = bank.GetTextBySymbol(...); text += t; }`) or
  moving the `bool none` test before the names loop's end; the nested-else form fixes the block but loses the copy-loop count spill.
- 0x005C6B26 ConstructWeaponDescription: reuses the 0x005C7452 / 0x005C71AA bodies inline; needs headers for CTCVolumeContainmentTracker
  (TCI 0x75, intelligent pointer at +0x0C), CTCShop (TCI 0x53) IsValidStockItem 0x007723D0 / GetCurrentBuyPrice(long,long) 0x007742C0,
  CStockItemDef (+0x28 value, GetSubDef 0x0059350C), texts WEAPON_VALUE 0x01254538 / KILLS 0x012544E0 / WEAPON_WEIGHT 0x0125451C.
- 0x005CA68E SetupActiveWeaponsOnCarrier: needs CCreatureAction_UnsheatheItemFromInventory (ctor 0x006A0150 (creature, thing, 0, 0x96),
  dtor 0x005CC091, frame object at [ebp-74h]), creature action queue call 0x006644F0, PerformAction 0x0069F9E0,
  CTCMultiStrike (TCI 0x87) RemoveEffectOnWeapon 0x007CC9E0, TCI 0x88 component, CTCCarrying primary-slot getter 0x006A1440.
  Melee (+0x134) and ranged (+0x141) intelligent-pointer halves are twins.
- 0x005C4F42 -> 0x005C574E / 0x005C5F11; 0x005C90FC -> 0x005CACB9; 0x005C3E91 / 0x005C47D5: not attempted (UI builders, NUISystem slots
  noted in the src comments of 0x005C66B2: CComponent slot 60 AddChildDynamically, 62 SetDeletion, CSprite 145 SetGraphicIndex,
  CList 157 SetDefaultChild).

- 2026-11-10 later: chunk 1002b landed 6 more (batch `2026-11-10_landing_t`); 0x005C71AA re-parked in `pending/2026-11-10_linux_hand1002b/differ/` with the c2prio diagnosis.
