# Header overlay (agentNF30 name-fix pass, 2026-11-12)

One header, a full copy of the current repo file with one `#ifdef` addition (no layout change):

- `engine/CThing.h` -- `FABLE_RETAIL_CTHING_PEEKWORLD_FRIEND_CREATUREACTIONBASE`: `friend class CCreatureActionBase;` next to the
  protected `GetWorld()` / `PeekWorld() const` declarations. Used by row 0x00692950 CCreatureActionBase::PeekWorld() const, which
  forwards to the protected CThing::PeekWorld 0x004C7AB0 (Ego_r.pdb ?PeekWorld@CThing@@IBEABVCWorld@@XZ), as the landed twin
  CCreatureActionBase::GetWorld 0x00692940 does through its local friend. Access only.

All other changed rows use opt-ins that already exist in the repo headers.
