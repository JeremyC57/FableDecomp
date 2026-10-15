# Chunk 906 (skipped-row ledger: the two functions of catalogue row 009576c0)

- 009576c0 -- CAIStateGroupBehaviourBase::OnActivate(): vtable 0x0128C444 slot 2 (also in the vtable at 0x0128D11C).
  Zeroes TriggerTime, ThisSpeechDuration and RandomWaitTime with the integer literal 0 (shared zero register) and
  tail-jumps to its own virtual slot 16, the per-behaviour activation hook (empty `ret` in the base). MATCH, first grade.
- 009576d0 -- no callers, no slot; named IsTargetSeenWithinCloseRange: the perception component must have a running
  awareness timer, direct awareness and line of sight, and the creature-to-target distance on the ground plane must be
  under 5 units (25.0f squared). Int-width result. Four grades: the compare is `<` (`test ah,5 ; jp` is VC's ordered
  less-than), and the squared sum must be accumulated one statement at a time to square dx before dy.
- Label corrections: 0x00744430 is CTCPerceiveThing's DirectAwareness getter (manifest "CTCInventoryBase::
  GetCanItemBeQuickAssigned" is propagated); 0x004C73D0 is CThing::GetPosition (manifest "CVertexBufferWin32::DoSizeof").
