# Batch `2026-10-14_linux_hand33` — sub-agent hand-authoring chunk 33 (CEngineInternalPrimitiveMeshBase .. CEngineLightingManager): 37 of 40 landed

Chunk 33 of the class-labelled 17–96-byte pool, authored by the single sub-agent with the Wine lane and re-graded by the
lander: CIVCountedPointer releases / assignments (a release, `operator=(0)`, `operator=(const&)`, a two-element member
array destroyed through the compiler's counted loop), CCountedPointer block releases, the particle-group / particle-mesh
primitive bodies over the retail layout (0x14 bytes narrower than the PDB), CParticleSimulation's destructor pair and
~CEngineLandscapePatch, node-chain and map-owner delete loops, a `__fastcall` global-allocator free, a line segment
midpoint, a single-action anim-set lookup, a 0x00C3 `__stdcall` dispatcher, CPersistContext::TransferFloatRange,
CEngineLightingManager bodies and the landscape-map / pool-surface releases. Pragmas: `optimize("s", on)` on 004d959d
and 00458f2b only; no extra flags.

Label corrections (details in the agent's NOTES): 00724950 is a plain line-segment GetMidpoint; 00c52c40 is a Cyrix CPU
name lookup (parked); 00693250 / 008f4aa0 are a `__fastcall` free GetSingleActionAnimSet over a vector<CAnimSet*>;
004d959d is a game-module pool-surface release (named by address); 009a4350 is the `__fastcall` free
GFFreeFromGlobalAllocator (0x00BD9B90 is the member twin FreeVirtual); 00ae0630 / 00ae6070 are CParticleSimulation's
`??1` / `??_G` (vtable 0x0129F0C4) and 00bf3a40 is the real ~CEngineLandscapePatch; 00bdd8e0 / 00bdd930 / 00cb1210 /
00b282b0 / 00b36b20 are owner-class bodies named by address; 00c35c50 is a `__stdcall` free dispatcher; 00458f2b is
CPersistContext::TransferFloatRange; 00bbbf00 / 00bd9af0 / 00bd9c40 / 00b67140 are members with an unused `this`.

Lessons (also in GOTCHAS, wave 24h): CIVCountedPointer `operator=(0)` is a release plus a second unconditional zero
store; `dec [eax] ; mov eax,[esi+8] ; cmp [eax],0` is `--m_pBlock->m_count; if (m_pBlock->m_count == 0)` (the member is
re-read); a `__stdcall` 0x00C3 body loading its object into ecx with eax/edx argument loads calls thiscall members on it;
`delete g` through a class `static void __fastcall operator delete(void*)` gives `mov ecx,[g] ; test ; mov esi,ecx ;
call dtor ; mov ecx,esi ; call`; a 64-bit size guard wants the call as the `if` body with `return 0` as the fall-through;
`g->bank->Get(x)` needs `bank` in a named local to load the global before the argument; two inline CArray accessors
re-read the first pointer; a user-constructed 8-byte struct copied bitwise gives the integer-move hidden-pointer return
where a user copy constructor goes through the x87.

Parked (evidence in the agent's NOTES): 00c52c40 (register choice in a compare chain), 00bc25b0 / 00b9d1e0 (the inline
by-value getter's 12-byte frame temporary the lane elides).
