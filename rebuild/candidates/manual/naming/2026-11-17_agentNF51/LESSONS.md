- A derived row that redeclares a base virtual (Update, IsRunnable, IsStillValid) becomes virtual once the base comes from the retail header: check the PDB access (U/M/A, const) and set it, do not stop at UAE.
- A stale `// Leaf:` line still grades RELOCATION_MATCH (q.py falls back to the largest function); grep the grade output for `matches no symbol`.
- Same-TU stand-in bodies of a callee (0x009425F0) shape the caller's frame; keep the body and respell it to the header signature.
- Replacing a row helper `GetCreature() { return PBrain->PCreature; }` by a direct member read can move registers (0x00980770); use the header's inline accessor.
- A stand-in class name is not evidence: the 8-byte "CAction" of 0x0059DE04 is a CDeletion (retail CAction() is elsewhere, slot 62 is SetDeletion).
- Including a NUISystem leaf header (CDeletion, CActionParamSLONG, CActionParamULONG) drags in engine/NUISystem_CComponent.h: a row with
  a local NUISystem::CComponent cannot take the binder fix alone; one diagnostic grade shows it, then blocked/port.
- Some local same-TU stand-in bodies exist only to give VC7.1 escape knowledge (GetProfileNames 0x005955AB): a shared-header port with
  the callee declared only reproduces the documented non-retail form (540v546), so read the row's Shape comment before porting.
- A copied `#pragma optimize` block from a landed row may sit inside `#ifdef _MSC_VER`: copy the guard too (an `#endif without #if`
  makes q.py print nothing).
- A private member read from another class (CTCInventoryAbilities +0x144 in CDrawAbilityButtons) points at an inline accessor that only the
  debug PDB names (FableWin GetHUDAssignedAbilityLevel); look there before adding a friend.
- Always end Bash calls that probe files with `|| true`: a grep over a missing file exits 2.
