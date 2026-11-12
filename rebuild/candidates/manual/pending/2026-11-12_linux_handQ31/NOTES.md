# Chunk Q31 notes

Matched (payload.json / oracle.tsv; tests PASS under g++ and VC7.1; regraded with LANDVERIFY_STRICT_HEADERS=1; overlay $S/w22/chunkQ31/hdr):
- 0x008F70F0 CMap::AddThemeDefIndexToPalette -- RELOCATION_MATCH. Source facts: the claim of the free entry is written in BOTH arms
  (`if (index != 0) {claim} else {ZeroAll; index = FindUnused(); if (index != 0) {claim} else return 0;} return index;`), and each
  claim is `unsigned long i = index; iterator it = begin(); it += i; it->ThemeDefIndex = k; ThemePaletteEntryList[i].ThemePaletteIndexUsed = true;`.
  Why: (1) VC7.1 only reuses the vector start held in edi from the inline searches when the store goes through a VARIABLE set to bare
  begin() (probe: `p = v.begin()` / `it = v.begin(); it += n` do, `v[n]`, `(v.begin()+n)->`, `v.begin()[n]`, `&v[n]` refs do not);
  that makes the two arms different, so they are not merged; (2) the widened index keeps `[edx+ecx*8]` address modes (a direct
  `v[uchar]` CSEs `shl ecx,3`); (3) the if/else order gives retail's block layout and the shared `xor al,al` exit. Out-of-line real
  body in the TU: ZeroAllUnusedThemePaletteEntries (landed 0x008F6FC0, Q28's port). The spelling is unusual but every piece is
  required by the bytes (20 grades, 3 probes; ATTEMPTS.log).
- 0x00A69B70 CTextureManager::AllocateTexture -- RELOCATION_MATCH. Source facts: Reset is the ADOPTING form already proven for
  CIVCountedPointer (chunk 850: `Release(); Object = object;`), so the caller AddRefs, and it does so through operator->:
  `texture.Data.Reset(texture_data); texture.Data->AddRef();` (operator->'s copy gives retail's `mov esi,ebx` and the load/inc/store
  interleaved with the pops; `texture_data->AddRef()` gives `inc [ebx+4]`). The texture size is a named local
  (`unsigned long size = CalculateTextureSize(...); GetPoolIndex(size)`): nested in the argument the callee-saved registers come out
  permuted. Reset must still be forced inline (overlay: FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_FORCEINLINE nested under _RESET_ADOPT):
  VC7.1 inlines the plain body in small toy callers but not in this caller (probe 3: dropping either the -1 early return or the
  CreateTexture call lets it inline). Q28's non-adopting forced Reset (+_CASTTOBASE) is removed from the overlay.

In progress (hand-back on budget): 0x008031E0 CTCTargetingPlayer::GetPBestThingForMeleeAttack, DIFFER(150v152), 21 grades + 2 probes this
chunk; best source differ/008031e0.cpp (Q28's, unchanged), residue and every idea tried on its line 2. The only difference: retail loads
the parent into the dying `this` register and copies it to ecx (`mov ebx,[ebx+4] ; mov edx,esi ; mov ecx,ebx`) -- the IL must hold the
parent in a variable defined after the smashable test that VC7.1 neither forwards into ecx nor loads early. Found: the PDB inline
CTCTargetingPlayer::IsThingAnEnemy is retail 0x00802380 (15 bytes = Ego_r's 15-byte copy) and passes (thing, parent), the other order,
so it is not this call. Next ideas not tried: an inline the PDB lists for CThing / CPlayer / CTCTargeted whose `this` is the parent;
the else branch's three tests spelled through CTCTargeted / CTCBase accessors that change the IL around the call.
Untouched: 0x006B05A0 CTCPhysicsBase::CreatePPhysicsMeshInfo (Q28 best and residue in differ/006b05a0.cpp).

Made-up names: none (locals only: i / it / size). Probes: 008f70f0 3, 00a69b70 3, 008031e0 2.
