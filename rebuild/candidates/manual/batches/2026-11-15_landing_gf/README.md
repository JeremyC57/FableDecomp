# 2026-11-15 landing gf -- 0x00675E20 CCreatureDef::Transfer (1,912 bytes; agentP6F, Fable model)

RELOCATION_MATCH, behaviour test PASS, strict headers, no header change (the header profile Codex's source needed was merged in
landing gd). Codex pass 22 parked it at 177 differing lines: retail gives eax to every stream push of the first inlined traits arms
(0x00675E43, 0x00675E50, 0x00675E5B, 0x00675E80, 0x00675E8D) and rotates scratch registers only from the vptr load at 0x00675EA7; ours
rotated from the first arm.

Fix: the existing opt-in FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOR_NODEFAULT (TransferVector's switch ends in the NODEFAULT block
`default: { __assume(0); }`, MSVC-only, already used by landed 0x006D8F10). Retail's `sub eax,2 ; je` with no `dec eax ; jne` at
0x00675EC1 shows the switch had that unreachable default. An `__assume` anywhere in the function stops VC7.1 forwarding inlined
reference arguments and return values, so those five temporaries become allocator candidates instead of rotation temporaries.
Three RELOC_NAME warnings (GFSerialiseMapBinaryIn / Out instances bound under catalog labels).
