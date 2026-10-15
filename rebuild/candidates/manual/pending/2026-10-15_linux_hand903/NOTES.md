# Chunk 903 -- skipped-ledger pieces (8 bodies from 2 multi-function rows), hand-authored by the coordinator (Fable 5.1)
- 004debf7 (48-byte row, six bodies): CTCTargetingSpirit vtable 0x012419A4 slots 10 (forward to slot 9), 29 / 31 / 32 (constant 0 with 0 / 3 / 1 arguments), 22 (a default-constructed hidden-result object whose constructor is 0x004D5853 -- its label CActionDoCreatureAction::GetActionName is propagated; size TU) and 8 (constant true). 8 of 8 matched.
- 0061b530 (96-byte row, two bodies): CTCInventoryQuests slots 38 / 39 = ProcessInventoryInputUp / ProcessInventoryInputDown (the +0x157 quest-list flag routes to the hooks 0x0061AA80 / 0x0061A9D0; otherwise the +0x1c menu mode switches between the landed CTCInventoryBase handlers 0x005BDD8C / 0x005BDDB8 and virtual slots 72 / 71).
## Review at landing
- 0x004D5853 is landed as CActionDoCreatureAction::GetActionName; its use here as a hidden-result default constructor says that label is propagated.
- The landed sibling CTCInventoryQuests_ProcessButtonBReleased_0061b5f0.cpp models the class as __fastcall free functions with `m_flag`; the +0x157 bool is the quest-list flag read here too.
