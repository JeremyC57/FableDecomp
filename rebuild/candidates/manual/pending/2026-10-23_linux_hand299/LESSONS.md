# Chunk 299 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Shared tail after if/else: if retail schedules `mov ecx,edi` differently in the tail, duplicate the tail into both arms;
  VC7.1 cross-jumps the copies back together with retail's scheduling (0x00729CF0).
- Frame-slot packing in a bool function: give the `else` arm its own `return true/false` instead of falling through to a
  shared return (0x00686B00).
- CCharString c_str: `!p ? "" : p->Data` gives retail's layout (0x00686B00).
- `&CGameDefinitionManager::Get()` passed as the base-class pointer (derived-to-base conversion, not a conversion operator)
  pushes the default arguments before the Get call (0x00686B00).
- STLport `erase(begin(), end())` on a vector: an inline static `copy(first, last, result)` that starts with
  `if (first == last) return result;` gives retail's `cmp eax,eax` plus the memmove path.
- Same-TU stand-ins: a template stand-in needs `__declspec(noinline)` (`#pragma auto_inline(off)` does not stop template
  inlining), and a stand-in must WRITE its out-param with a value read from somewhere, or VC7.1 propagates the
  caller's zero (`xor ecx,ecx`) (0x007E0F20).
- A 256-entry static pointer-table fill loop compiles to `rep stosd`; `Init() == true` on a bool gives `cmp al,1` (0x00A6C170).
- CRGBColour from a COLORREF: per-byte stores (`R=(uchar)c; G=(uchar)((ushort)c>>8); B=(uchar)(c>>16); A=0xff`) give
  al/ah/shr stores; a ctor temporary copied through the frame does not (0x009A5490).
- Clamped box copy: copy a 6-int box member by member (`box.Left = area.Left; ...`); `C3DBoxI box = area;` loads the
  fields in another register order. Build a CPixelFormat in its own `{}` block over the D3D description so the description
  shares the frame words of the later box (0x00A67A20).
- Masked diff tool: diffing with relocation immediates AND esp offsets masked (probe/rd2.py) separates frame-placement
  residue from real instruction-order residue quickly.
- Parallel q.py runs on the SAME address race in `$S/wine/q/<addr>` and give corrupted grades (an objdump failure gave
  it away). Set `QTAG=<tag>` per variant or run them one after another; recheck any earlier parallel result.
- A particle-emitter dead flag tested as `mov cl,[eax+0x40]; shr cl,5; test cl,1` is an `unsigned short` bitfield
  (`Flags40:5; Dead:1; High:10`). Every unsigned char / bool / explicit-shift form folds to `test byte,0x20`
  (0x0079C880, 0x0079D7E0).
- `or byte ptr [eax+0x44],0x10` comes from `unsigned char Flags44; Flags44 |= 0x10;`. A bitfield write gives load/or/store.
- VC7.1 calls these out of line in big functions unless they are `__forceinline`: C3DVector ctor and operators,
  CrossProduct, the CRightHandedSet ctor, the CThingAICreatureInit ctor, GFRandom, WrapAngle, HasTC, IsDead and
  GetLengthSquared (CTCAssassinRush rows).
- Single-exit nested ifs with a final `return false` give retail's shared fail epilogue; multiple returns duplicate it.
- An inline `GetDef() const { return m_pDef; }` accessor fixes the load order before GetSubDef.
- Copy an out-parameter into a second local (`CThing* found; Get(..., &found); CThing* hero = found;`) to let VC keep
  hero in ebx instead of pooling const 1 in bl.
- Bitfield writes to creature flag bytes are emitted in source order; write InCrowd (+0x92), NoLOD (+0x1c9) and
  IsCrowdMember (+0x15c) in the order that gives retail's load order.
- An out-of-line call to one instantiation of a template ctor next to an inlined one in the same function
  (0x0079D7E0: CIntelligentPointer<CParticleEmitter>(T*) at 0x00655318, CIntelligentPointer<CThing> inline) is an
  explicit member specialisation declaration:
  `template<> CIntelligentPointer<CParticleEmitter>::CIntelligentPointer(CParticleEmitter*);`.
  Put the temporary in a named `{}` block so the GetPItem call reloads `lea ecx` rather than reusing the ctor's eax.
- Seven `def->Vec[m_nLevel]` reads compile to retail's CSE'd `shl eax,2; [eax+base]` only when operator[] returns
  `const float&` and m_nLevel is read in place. Reading it into a local gives `[base+eax*4]`. The ints-by-value pushes
  (`mov edx,[eax+edi]; push edx`) also come from the reference return.
- A temporary returned by hidden pointer (CDefStringTable::GetString) and evaluated before the other arguments are pushed
  is a named local in its own `{}` block: `{ CCharString s = ...GetString(id); LoopSound = PlaySound(s, true); }`.
- `void* key = Fn(...); Member = key; Use(key);` gives retail's `mov edi,eax; mov [esi+..],edi`. Storing to the member and
  then reusing it reloads the member.
- A HasTC bit test that VC7.1 compiles to `mov edx,[esi+0x2c]; shr edx,0x11; test dl,1` (from `return (bits & mask) != 0;`)
  becomes retail's `test dword ptr [esi+0x2c],0x20000` when written `if (bits & mask) return true; return false;` (0x00814260).
- `CVectorMap::Add` / `Sort` must be `__forceinline` in a big function, or VC7.1 calls them out of line (0x00814260).
- A CCharString("") temporary that retail reloads with `lea` after its ctor (instead of using the ctor's eax) is a named
  local in a `{}` block around the call (0x00814260, 0x0079D7E0).

Review at landing:
- 0x009A6610 is matched with real C++. The older landed asm_bake file for it stays an oracle only.
- Close parks for the advisor / near-miss sweep:
  - same length: 0x00814260 (scratch registers only), 0x0079C880 (frame slots, ~15 grades), 0x007E5230 (frame), 0x0056BE00, 0x006C0C90, 0x006C1250, 0x007E0F20;
  - within 5 bytes: 0x00783380 (one 4-byte re-test, ~13 grades), 0x0079D7E0, 0x00492973, 0x00492BAB, 0x0072A090,
    0x007CD9A0, 0x007CDD10.
- 0x00A67A20's line 1 names CVolume::ClearArea while the catalog says CSurface; the leaf `ClearArea` matches the TU symbol.
- /GS rows (all parked): 0x00492BAB, 0x0072A090, 0x007E4E30, 0x009F4590.
