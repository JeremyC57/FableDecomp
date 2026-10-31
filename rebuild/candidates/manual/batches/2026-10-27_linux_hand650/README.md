# Chunk 650 -- 2 of 10 landed, stopped early at the owner's request (2026-10-31)

Cut from the fixable bucket of the re-ranked pool; the agent was told to stop and save after two rows matched (8 parked, 6 of them untouched
copies of the brief's best attempts). Landed via `coordinator/land_chunk.sh 650` (vland --land 2/2 RELOCATION_MATCH + PASS, multi-check 0;
new bindings STLport __push_heap 0x0097ADF0, the anonymous-namespace comparator operator() 0x0097ABF0 -- its `?A0x` hash differs per TU,
the twelfth spelling of that address: the anonymous-namespace binding policy is still an open owner decision --, RotateLeft 0x00A646E0):
std::__adjust_heap<CThing**, int, CThing*, CThingCompare_InDirectionOfAndNearestToHeuristicBitch> 0x0097AF70 (catalog `_Adjust_heap` is the
wrong library's spelling) -- the landed sibling 0x0097B0E0 __make_heap model reused: C2DVector memberwise user copy ctor, the comparator
operator() compiled in the same file as a noinline stand-in under `#pragma auto_inline(off)`, so the comparator copy loads two words ahead of
the stores; CMemoryAllocatorVariableSize::RemoveFromTreeFixup 0x00A64910 -- RotateLeft / RotateRight as plain members with no `inline`
keyword defined before the caller: VC7.1's automatic inlining then inlines five of the six calls and keeps the one retail keeps out of line
(the old attempt forced it with `#pragma inline_depth(0)`, 756v757); the null-or-black test stays one inline helper (written out at each site:
761); CMemoryArea nested in the class, protected members, `this` on engine/CMemoryAllocatorBase.h. Eight rows stay DIFFER in
`pending/2026-10-27_linux_hand650/differ/`: 00af56e0 DeleteAllParticles (bottom iterator store from ebx v edi, 11 probes; next: the real
STLport list iterator operator!= / clear bodies), 007e4530 IsThingLightningable (CTCTargeted pointer in edx v eax, 6 probes; still carries an
inherited `#pragma pack(push,1)` to strip), and untouched 00973dc0, 00f38ff0, 008de080, 008e03b0, 00e83230, 006a03a0 (next idea: the
automatic-inlining lever and same-file stand-ins for opaque callees). Lessons: a callee retail inlines everywhere except one call is an
ordinary non-inline function defined earlier in the same file -- let the compiler decide, never force it with a pragma; a test repeated at
several sites stays one inline helper; for an STL algorithm family reuse the landed sibling's model including its comparator stand-in.
3 NAME_FIXES + 3 review rows queued (prefix c650): NParticleEngine::CParticleSystem::ClearParticle (two CRetailList bindings),
DeleteAllParticles (bound as `?Stop@CParticleSystem@@QAEXXZ` at 0x00AF5870), 0x0097ADF0 landed as global `_Push_heap`; label 0x0097AF70;
header debt (no CMemoryAllocatorVariableSize.h, base header flattens the vptr); IsBlackArea descriptive. Labels:
ghidra_out/labels_chunk650_2026-10-27.tsv.
