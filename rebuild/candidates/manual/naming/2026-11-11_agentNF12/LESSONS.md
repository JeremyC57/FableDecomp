- VC7.1 shares a frame slot only between locals of disjoint blocks: putting a named CSoundPair in its own `{ }` after the GetTC
  lets it reuse the def handle / lookup-key word as retail does (0x0083DF20, 0x2c -> 0x28 frame) -- no callee stand-in needed.
- A same-TU inline body of a callee that also has an out-of-line copy (CThingPhysicalSwitch::PeekDefinition) reproduces retail's
  "first call expanded, second call made" split on its own (VC7.1 inline budget).
- CTCTrapBase overrides are protected virtual (MAE) in Ego_r.pdb; vtable slot order 28..31 comes from the derived CTCMeleeTrap vtable.
- `ParentThing->PeekBaseDef().GetSubDef(&p)` = retail's in-place `mov ecx,[eax+70h]`; `ParentThing->GetPBaseDef().Get()` = the
  inc/dec pair before GetSubDef (0x0078809B) -- confirmed again on four rows.
- A CThingCreatureDef / CThingAICreature member block added in an opt-in that wraps the existing branches in its `#else` keeps the
  overlay diff additions-only.
- CGameDefinitionManager hides CDefinitionManager::GetDef unless FABLE_RETAIL_CGAMEDEFINITIONMANAGER is defined (C2664 otherwise).
- GETDEF_GLOBAL_BODY / GETDEF_NAME_BODY need no complete T (SetDynamicCast / operator= only move the pointee), so a forward-declared
  def class (CBrainDef) is enough.
- A retail GetTC whose map test is `cmp [eax],key ; jg` (component into eax first) is not any PDB-named CThing template body on the
  shared headers; park such rows (0x005B37F7 / 0x005CD767 TCI_SKELETAL_MORPH).
- Always rebuild a row list before redirecting a loop's output into the same file (`cut file > file` empties it).
