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

**Review closed -- advisor decision (owner delegated, 2026-11-19: "have the expert advisor decide"):** the Inferred row(s) here stay landed.
Scope / lifetime rows (0x006B2960 inner-block local, 0x00A50C20 block scope) are required constructs whose wording alone is inferred (frame size /
slot sharing fix them) -- accepted outright under the 2026-11-12 Inferred-statements rule. Fitted tie-breakers (0x00F3ABF0 reference local,
0x006B5890 by-value operand) accepted as Inferred on the 0x006E89B0 precedent.
Bracket check after the owner's bracket rule (coordinator, q.py): `(block_buffer[n]) = ...`, `= (ReadVarSizedUInt(...))`, a bracketed address
argument, and the whole assignment bracketed -- all DIFFER(605v605); no bracket replaces the reference, so the row is unchanged.
