// CPSCSingleSprite::DatabaseSerialise(...) -- serialise every single-sprite particle component field by name (41 string keys), including bitfield flags
// Grade: DIFFER(-v2032) -- triage only, no source drafted: contains the same `xor cl,[mem]` bitfield write-back that keeps the parked twin 0x00B1A870 at DIFFER (same length, one instruction); lane gap until that is solved
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B079B0, 2032 bytes. Decoded from dis_all.txt; see NOTES.md for the next step.
