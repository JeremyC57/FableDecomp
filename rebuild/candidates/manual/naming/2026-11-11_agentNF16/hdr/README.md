# agentNF16 header overlay

One header, a full copy of the current repo file with one `#ifdef` opt-in added. Nothing changes for a row that does not
define the macro.

## engine/CCreatureActionBase.h: `FABLE_RETAIL_CCREATUREACTIONBASE_GETTHINGMANAGER`

- Adds `class CThingManager;` to the forward-declaration block.
- Adds the protected member `CThingManager& GetThingManager();` (out of line, 0x00692980) to the retail branch
  (`FABLE_RETAIL_CCREATUREACTIONBASE`). It sits just before the `FULL_MEMBERS / PRECEEDING / PCREATURE / INTERRUPTION`
  member chain.
- **Evidence:**
  - Ego_r.pdb and FableWin.pdb both have `?GetThingManager@CCreatureActionBase@@IAEAAVCThingManager@@XZ` (protected,
    returns a reference).
  - Retail 0x00692980 is `add ecx,8 ; call 0x00A01B50 ; mov eax,[0x013B8A1C] ; ret`. That is PCreature's GetPItem, then the
    private static `CThing::PCurrentThingManager`.
  - Caller: 0x0084A7E0 `CCreatureAction_AlertAllNearbyToThing::PerformAction`.
- **Users:** rows 00692980 (the definition) and 0084a7e0 (the caller). Both also need `FABLE_RETAIL_BASE_OBJECT`,
  `FABLE_RETAIL_CINTELLIGENTPOINTER` and `FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE`. Without
  `FABLE_RETAIL_BASE_OBJECT` the 0xa8 size assert fires.
