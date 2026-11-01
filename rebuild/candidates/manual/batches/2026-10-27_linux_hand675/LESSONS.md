# Chunk 675 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- zlib under the lane's C++ build: a `?:` over two `uch` arms needs `(int)` on both arms to keep C's int promotion (`movzx` per arm), 0x00C052C0 build_tree; the landed trees.c TU 0x00C03F50 can take the same fix and host build_tree.
- libvorbis statics with a same-TU caller: `static __declspec(noinline) int __stdcall` gives retail's callee-pop `ret N` (cdecl gives caller-pop and a moved frame), 0x00CA20A0 accumulate_fit.
- A not-inlined STLport ctor in retail (vector<bool>(n,val,a) 0x00451583) is written `__declspec(noinline)` in class WITH its real body; giving the real `_Vector_base(n,a)` body too lets VC7.1 see the local does not escape and keep its members in registers, 0x00AD8ED0.
- A named const_iterator result from `map::find` makes VC re-read `end()` for the compare (retail shape), 0x00AD8ED0 HasGroup.
- set::insert inside a loop: a block-scoped `unsigned long index = *it;` before `insert(index)` fixes the argument homing, 0x00AC8840.
- `(unsigned char)(flags >> N) & 1` gives retail's `shr reg,N ; test reg8,1` where `(flags >> N) & 1` folds to `test ch,imm`; a ternary call argument evaluated before an already-loaded register argument is pushed = the ternary in a named local, 0x00C72E00.
- GetTC on the absent path: `CTCShop* tc; if (HasTC(k)) tc = GetTC(k);` reproduces retail reading the dead slot, 0x00946F90; the real same-TU template body of GetAllLeafThings and STLport `_M_insert_overflow` keep the result vector unescaped.
- Compiler-generated destructor of a class with STLport containers: define the empty PDB ctor so the TU emits vtable/??_G/??1, and restate members at STLport sizes (deque 0x28 with a declared out-of-line ~deque), 0x00443E20.
- An inlined member call on an embedded member (`frame->Data->ManagedTexture.Touch()`) produces retail's dead `add eax,20h` after the folded `[eax+24h]` load, 0x00BEBCA0.

Review at landing:
- 0x00BEBCA0 parked after 22 probes: the double graphic-list size computation is shared by PreloadDiffuseTexture 0x00BEC000 and PreloadBumpTexture 0x00BEC470, whose landed sources grade DIFFER (319v1124, 467v526) in this lane -- check their landing status.
- 0x00AD8ED0 needed 15 probes (more than 4 grades): STLport text levers listed above.
- Landed callers bind wrong spellings: 0x00AD8FE0/0x00ADA240 (CountRemainingGroups QBEKXZ), 0x00947280 (`SelectRandomShop()`), 0x004942A2 (free fastcall ??_G stand-in calling LBP_dtor), 0x004427D0 (deque<CUnprovenElement_004427d0>), 0x00451583/0x00459AF3/0x00450CCC (U?$allocator), 0x00772110 (CShaderPreParser::GetConstantCount), 0x008FD000/0x008FCFD0 (derived-class spellings) -- see NAME_FIXES.tsv.
- 0x0143BDB0 has two global spellings in landed code (`g_AudioContext` vs `?g_audioContext@@3UCAudioContext@@A`); this chunk used `g_AudioContext`.
- Local base models next to engine headers: CThingSearchTools, CTCShop, CVertexGroupBlockOptimiser (Ego_r headers carry Dinkumware sizes).
- Landed 0x00C72AF0 CAudioSource::InitializeProperties names CAudioContext +0x50 `pfnSourcei` resolving "alSourcei" @ 0x012BEE14, but that string is "alSourcef" (landed CAudioContext::Sourcef 0x00C72510 agrees); the real alSourcei slot is +0x5C (0x0143BE0C, string 0x012B8E58), as 0x00C72E00 uses. vland caught the mismatch as RELOC_IDENTITY when the +0x50 model was reused.
- vland SHARED_HEADER_AVAILABLE warnings (not fixed, engine headers carry Ego_r Dinkumware sizes or lack members): CFadeInFadeOutBase (0x00443E20), CTCInventoryItem (0x005D8EF0), CAIStateGroupBase (0x00946F90), COptimisedPrimitive (0x00AC8840).
