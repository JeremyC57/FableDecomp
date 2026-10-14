# Batch `2026-10-14_linux_tiny3` — family tiny3 (≤16-byte bodies, `wtiny3`): 88 bodies (76 landed; the 11 that reproduce after three model fixes landed in `2026-10-14_linux_tiny3b`)

Third generic synthesiser for the ≤16-byte tail (`tools/decomp_pipeline/families/fam.py` template
`wtiny3`, 22 instruction-pattern recognisers written from the shapes left unmatched by `wtiny2`).
Each recogniser emits a descriptive class/member model, the method and a behaviour test; the class
comes from the manifest label when it is a plain class name, else the method lands under a
descriptive placeholder.

Shapes landed here (76 bodies over 10 modules): `FreeSelf` 9 (`::operator delete(this)`, size-pragma
`pop ecx` cleanup), `ForwardThroughMember` 6 (unused stack arguments, the call on a member pointer),
`ApplyToMember` 5, `CtorInitUnusedArg` 5 (an allocator-tag constructor), `CallInterfaceSlot` 4
(COM-style `__stdcall` virtual with `this` pushed), `ClearFlags` 4, `CallImport` 4 (`__fastcall` over a
one-argument `__stdcall` import), `CtorZeroThreeAndByte` 3, `ApplyPair` 3, `GetConstant` 3 (pooled
float literal, unread arguments), `PrepareThenFinish` 3 (member call then a tail-jump to a free
function), `GetTargetFlag` 3, `CallSlot` 3, `GetFirstAsHandle` 3 (a non-POD one-pointer wrapper
returned through the hidden pointer), `GetLowBits` 3 (byte bitfield at bit 0), `HasIndex` 3
(`!= -1` as a byte), `GetOut` 3, `DtorReleaseImport` 3, `GetItemAt` 3, `CountShifts` 3.

The first-pass DIFFERs: the `$E` dynamic initialisers of one-member globals (the lander must be
pointed at the `??__E` symbol, not the inline constructor), the `== 1` byte predicates (an
`unsigned char` result, like the `setne` predicates of `wtiny2`), the flag-setting `BeginWith`
bodies (a generator bug in the assignment order), and one `ClearFlags` body whose stores are not
in layout order (left out).

All graded `MATCH` / `RELOCATION_MATCH` + `PASS` in the Linux lane (Wine + DDK 13.10.4035 cl.exe) on 2026-10-14.
