# Landing 00f3abf0 (2026-11-19, hand2b): CRangeCompressor::CalcCompressionScript (605 bytes) -- INFERRED, flagged for owner review

vland --land, strict headers: RELOCATION_MATCH + PASS (11-scenario behaviour test).

**Advisor decision (owner delegated), 2026-11-19:** land with the fill loop's named reference `unsigned long& value = block_buffer[n];
value = ReadVarSizedUInt(...);`, marked `// Inferred:`. The reference is unsourced (Ego_d / FableWin record no such local; their line 749 is
one statement). Every sourced reading was exhausted first (74 grades; FableWin listing line by line, callee signatures, include order, CArray
indexer models, VC6 for-scope loops, QFE-4035 / TU flags). With the donor's statement every instruction matches except an ebx/ebp swap: the
named reference keeps the element address a c2 candidate in the fill-loop head and join blocks, so `this` drops from register priority 58 to
54 and loses to the strength-reduced temp (56), as retail (this = ebp, temp = ebx, 0x00F3AC00 / 0x00F3AC0C). Same behaviour and portability;
precedent: the owner's 0x006E89B0 Inferred landing. This reverses the earlier coordinator rejection now that the sourced readings are
exhausted. OWNER REVIEW: if not accepted, revert this one commit and the row returns to UNMATCHED_ROWS.
