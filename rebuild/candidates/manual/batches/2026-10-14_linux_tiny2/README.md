# Batch `2026-10-14_linux_tiny2` — family tiny2 (≤16-byte bodies, `wtiny2`): 132 bodies (102 landed; the other 12 that reproduce landed in `2026-10-14_linux_tiny2b`)

Second generic synthesiser for the ≤16-byte tail of the manifest (`tools/decomp_pipeline/families/fam.py`
template `wtiny2`): every un-landed body of at most 16 bytes is disassembled and matched against ~24
instruction-pattern recognisers, each of which emits a descriptive class/member model, the method and
a behaviour test. The class comes from the manifest label when it is a plain class name, else the
method lands under a descriptive placeholder; member names are descriptive (the layouts are opaque).

Shapes landed here (102 bodies over 27 modules): `GetByteThrough` 8, `ForwardMember` 8,
`CtorVtableNull` 7, `RegisterWith` 7, `CallWith` 7, `GetCount` 6, `DtorFree` 6, `GetFlagBit` 6,
`ForwardConstant` 5, `GetFloatThroughArg` 5, `SetGlobal` 4, `TailVirtual` 4, `DecrementThrough` 4,
`GetCountThrough` 4, `CallWith2` 4, `SetGlobalByte` 3, `GetItemOfMember` 3, `IsKind` 3,
`ForwardWithConstant` 3, `SetFirst` 3, `ReturnArg` 2.

The 12 first-pass DIFFERs were three model errors, fixed in the template and landed as
`2026-10-14_linux_tiny2b`: a byte predicate (`mov al ; test ; setne al` with no `xor eax,eax`) returns
`unsigned char`, not `bool`; a `shr al, n ; and al, 1` getter reads a one-bit bitfield of an
`unsigned char` storage unit (byte arithmetic) rather than shifting a byte member in a dword; and the
`__stdcall` block-release wrapper is a delete-expression (`mov eax,[esp+4] ; push eax ; call operator
delete ; pop ecx`) -- a plain `free` call pushes the stack slot directly.

All graded `MATCH` / `RELOCATION_MATCH` + `PASS` in the Linux lane (Wine + DDK 13.10.4035 cl.exe) on 2026-10-14.
