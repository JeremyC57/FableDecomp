# chunk 720 continuation (agent720b): CTCInventoryWeapons + CTCVillage

Grade every row with `FABLE_HDR_OVERLAY=$S/w22/agent720b/hdr` (g.sh does). Payload: 2 rows, both RELOCATION_MATCH, call
targets checked against retail one by one (ck.py: our relocation symbol per retail call, in order).

| addr | symbol | grade | attempts |
|---|---|---|---|
| 005c552c | ?RemoveWeaponsFromSheathes@CTCInventoryWeapons@@QAEXXZ | RELOC | 6 |
| 005c36c2 | ?PutSheatheObjectInCarrySlot@?A0x..@@YIXAAVCThing@@0AAM@Z (anonymous namespace; leaf matched by prefix `?PutSheatheObjectInCarrySlot@`) | RELOC | 2 |

## The TU model (reusable for the rest of CTCInventoryWeapons)
- `#pragma optimize("y", off)` + `("s", on)` (frame pointer, size), as every landed sibling.
- Real engine headers instead of the siblings' local stand-ins: engine/CTCBase.h (FABLE_RETAIL_CTCBASE), engine/CThing.h
  (FABLE_RETAIL_CTHING, overlay), engine/CDefPointer.h + CDefClassBase.h + CThingBaseDef.h (FABLE_RETAIL_DEF_CLASS_BASE),
  engine/CIntelligentPointer.h (FABLE_RETAIL_CINTELLIGENTPOINTER), engine/CArray.h, fable_string.h (FABLE_RETAIL_CCHARSTRING),
  engine/CTCInventoryItem.h (FABLE_RETAIL_CTCINVENTORYITEM, overlay). `this` is modelled locally (no CTCInventoryWeapons.h
  exists; struct_layouts has only padding for it): CTCBase + pad to +0x134, packed PActiveMeleeWeapon +0x134 / PActiveRangedWeapon +0x141.
- Lever 1: CVectorMap::Find must be __forceinline in this TU (new opt-in FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND). Under
  optimize("s") the header's plain-inline Find is called out of line, so the `it == end || key < it->first` test vanished
  from the caller. Keep the header's same-TU noinline LowerBound: declared-only LowerBound makes VC re-read the key temp
  (`mov ecx,[ebp-10h]; cmp ecx,[eax]`) instead of retail's folded `cmp [eax],ebx`.
- Lever 2: same-TU noinline bodies of the TU's COMDAT callees -- IS_THING_ALIVE<CThing*> (0x0040DCE0), the explicit
  specialisation IS_THING_ALIVE<CIntelligentPointer<CThing> > (0x004CC340) and the CDefClassBase::GetSubDef<T> template body
  (0x0042B1A6 / 0x005CC10E; the landed GetSubDef<T> shape). With them VC keeps the sheathe pointer in esi, reuses its slot
  for the next lookup key, and releases the out-parameter CDefPointer as `dec [ecx+4]; jne` with no null store (declared-only:
  `add eax,4; dec [eax]; cmp [eax],0` + `and [ebp-10h],0`).
- `!weapon->GetPBaseDef().IsNull() && weapon->GetPBaseDef()->GetSubDef(&def) && ...` gives retail's two flag-guarded
  CDefPointer temporaries (`or [ebp-4],1/2`, the second nulled after release) exactly.
- Retail's sheathe-object test is `if (x > 0) .. else if (x != 0) sheathe = weapon` (a `je` after the `jle`): a negative
  SheatheObject means "the weapon itself", 0 means none.

## Header overlays (hdr/, hdrcheck2: default=same for each)
- engine/CVectorMap.h: FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND (Find / const Find __forceinline).
- engine/CThing.h: chunk 718's overlay (GetTypeFlags) plus, in the FABLE_RETAIL_CTHING branch, GetTCGraphicAppearance()
  (both PDB overloads), GetPlayerNumber() (QBEKXZ, `movsx`), SetInLimbo(bool) 0x004C8CF0. Merge with 718's copy at landing.
- engine/CTCInventoryItem.h: FABLE_RETAIL_CTCINVENTORYITEM: `: public CTCBase` (the default flattens the base), static
  StatGetInterfaceType() = TCI_INVENTORY_ITEM, ReturnToInventory() 0x005D8E50; members at the default's offsets (sizeof 0x28).
- The chunk 719 FABLE_RETAIL_CTCVILLAGE branch was not touched (AddThingToVillage not reached).

## Local models / header debt
- CTCCarrying, CTCCarryable, CThingObject (Create only), CWeaponDef (+0x38 SheatheObject), CCarryableDef (+0x44/+0x48),
  NGraphicAppearance::CTCGraphicAppearance (MainGraphic +0x44, CEngineGraphic::RenderSizeX +8): no engine headers exist;
  member names and offsets are Ego_r's corrected for STLport sizes. The +0x4C read is MainGraphic.RenderSizeX (SetScale's own
  model, landed 0x004BFA50); which inline accessor retail used for it (GetFullScale?) is unproven, so the member is named directly.

## Callee names
All PDB spellings (Ego_r / FableWin), see LABELS.tsv; NAME_FIXES.tsv lists the bound spellings proven wrong (notably
0x005D8E50 = CTCInventoryItem::ReturnToInventory, bound as CTCShowableItem::OnShowEnded; the CTCCarrying const/reference
forms; CThingObject::Create; vector<CThing*>::push_back 0x00436C80).

## Not attempted (budget; analysis for the next agent)
- 0x0065B854 AddThingToVillage (1502): switch on the thing's type byte (+0x10) after interface 0x23 lookup and two village
  calls (0x007EE540/0x007EE270 on that component); each arm builds a CIntelligentPointer<T> temporary (ctors 0x004A96F0,
  0x0065E21E, 0x0065E15D, 0x0065E1DB, ..., dtor 0x00A01C10) and push_backs it onto the matching village list (0x004BA4E0
  VillageDummies +0x74, 0x006605BB VillageMarkers +0x70, 0x00660FC9 VillagePatrols +0x48, ...); task arms `new` 0x1C-byte
  tasks (0x0065EE66, 0x0065EEFF) wrapped in CCountedPointer. Build on 719's FABLE_RETAIL_CTCVILLAGE branch.
- 0x00659800 CalculateSecuritySweepTasks (1074): find_if-style algorithms with by-value empty functors over VillagePeople
  (+0x2C; `push [ebp-1Ch]` = the uninitialised functor byte), the GFFastOneOverSqrt expansion (table 0x013CE558) and a
  CRightHandedSet-style rotation 0x00A14040 in a do-while over SecuritySweep steps.
- 0x005C6B26 ConstructWeaponDescription, 0x005C90FC ConstructWeaponDescriptionContainer, 0x005CACB9 RefreshWeaponDescriptions:
  CWideString-heavy UI code; reuse this TU model (pragmas, CThing GetTC with FORCEINLINE_FIND, same-TU GetSubDef<T>).
