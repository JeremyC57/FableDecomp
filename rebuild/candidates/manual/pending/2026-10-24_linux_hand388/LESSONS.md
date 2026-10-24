# Chunk 388 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Inlined STLport `map::find` lookups whose key temporaries must share one frame slot need a same-TU `#pragma
  auto_inline(off)` + `__declspec(noinline)` body of the find. That body must be the real tree walk and must not pass
  the key pointer on to another call. With only a declaration, each GetUID() temporary gets its own slot (frame 0x10 vs
  retail 8). Seen at 0x006E93C0, 0x006E9680, 0x006E9700, 0x006E9D10.
- An x87 3-term dot product that retail evaluates Z first is written `a.Z*b.Z + (a.X*b.X + a.Y*b.Y)`. The natural
  `X*X + Y*Y + Z*Z` reorders the loads. Seen at 0x006E8E10 and 0x006E7F00.
- A float epsilon test that retail emits as `fcomp dword [eps] ; fnstsw ; test ah,41h` is `(float)fabs(x) > eps` with
  `#pragma intrinsic(fabs)`. The double compare gives `fld eps ; fcompp ; test ah,5`. Seen at 0x006E93C0 (IsRotating).
- A "bool" query that ends `neg al ; sbb al,al ; inc al` (byte result, no eax widening) returns `unsigned char`. The `bool`
  return of `!x->Virtual()` widens through eax. Seen at 0x006E97A0, where the PDB mangling says bool.
- A large static helper that VC7.1 leaves out of line, as a 31-byte call shell, needs `__forceinline`. Seen at
  0x006E9F60.
- Written as `if (type == 5) return true; return false;`, the constant bool returns are threaded straight to the shared
  result arms. A ternary or `return type == 5;` materialises the bool. Seen at 0x006E9F60.
- A `HasTC()`-style bit test that returns the masked word, not a bool, gives `test dword ptr [..],imm`. A bool return
  gives `mov ; shr ; and ; test al`. Seen at 0x006E9F60.
- After a map lookup, keep the found node pointer, not a reference to `->value`. The node gives `lea ecx,[ebx+58h]` at
  the use. The reference adds `add ebx,18h` up front. Seen at 0x006E9700.
- A body shaped `if (cond) { ...; return true; } return false;` pushes every callee-saved register up front. The
  early-return form shrink-wraps the pushes and duplicates epilogues. Pick the form by retail's prologue. Seen at
  0x006E9700.
- To home a temporary in an argument's own slot (`find(.., &speaker)`), use a same-TU noinline `__find` stand-in plus a
  tag type with a user constructor. A tag without one gets a zero byte store. Seen at 0x006E58A0.
- An STLport map iterator walk that calls a member on each value is written `T& c = it->second; ++it; c.Member();`.
  It takes the `lea` before the inline `_M_increment`. Seen at 0x006E60F0.
- A class member whose model is only a `char` array lands at the wrong offset after a bool, for example `lea
  ecx,[esi+35h]` vs retail `+38h`. Model the opaque member with `unsigned long _data[n/4]` to give it 4-byte alignment.
  Seen at 0x006ED3F0 (CombatDialogueManager).
- STLport `__uninitialized_fill_n` / `__uninitialized_copy` rows are `__fastcall` explicit instantiations (/Gr TU). Their
  element is named by the stride and by the caller's landed source:
  - 13 bytes = `pair<C3DVector,bool>` (0x006EFF00).
  - 28 bytes = `CDefendingCombatantInfo::CProng` (0x006F1180).
  - C3DVector is 1-aligned in this module (pack(1); PDB CProng::Prong at +1).
- An STLport vector copy constructor for an element with a user copy constructor is written `_Vector_base(x.size(),
  a)`, then `allocate(n)` (null for n == 0), then `__uninitialized_copy(.., __false_type())`. The copy loop is inline
  when the copy constructor is trivial-bodied (0x006F0810, vector<C3DVector>), else it is a call (0x006F20A0 →
  0x006F14A0).
- A zero-initialised result returned once at the end gives the preloaded `xor eax,eax`. An early `return 0` returns the
  tested value instead. Seen at 0x006E2860.
- `!(gain > 0.0f)` gives `fcom ; test ah,41h ; jne`, where `gain <= 0.0f` gives `jnp`. Seen at 0x006E2B70.
- A chained `GetPItem()->Name.ToUpper()` pushes the result slot first and forms the member address with `add`. Fetch the
  object into a named local first to get retail's order. Seen at 0x006E0E80.

Review at landing:
- No row needed more than 4 grades. Four rows are parked (006e0880, 006e5990, 006e7f00, 006e9f60), each after 4 grades.
- Local models next to existing engine headers. The headers carry Ego_r.pdb (Dinkumware 16-byte container) offsets, and
  retail is STLport 12-byte:
  - engine/CScriptConversation.h has Conversation at +0x18; retail is +0x14 (006e58a0, 006e5990, 006e6d30).
  - engine/CTCCombat.h has UnsheathingWeaponType at +0x78; retail is +0x6C (006e9f60).
  - engine/CThingCreatureBase.h leaves the UID at +0x18 as unnamed pads (the combat rows).
  - engine/CGameScriptInterface.h is method-less: there is no way to place the virtual slots 386/395 (006e7370,
    006e75c0).
  - NParticleEngine::CParticleEmitter: the retail fields sit 0xC below the PDB (006dfda0, 006e0e80).
- 006e97a0 IsCreaturePerformingNonInterruptableSequence returns `unsigned char`, so the mangled name ends `QBEE...`.
  The PDB name set has `_N` (bool). The bytes require the byte return; the symbol differs from the PDB spelling.
- Already-landed sources that name callees wrongly:
  - 0x006E9450, 0x006E9E40 and 0x006EA770 declare 0x006E9200 as `CCombatWheel::BlockAttackerProng(attacker, const
    CAttackingCombatantInfo*)`. The PDB name is `CDefendingCombatantInfo::CCombatWheel::SetAttackerProngAsBlocked(const
    CThingCreatureBase&, CAttackingCombatantInfo&)`. The ABI is the same.
  - The landed `global_SumPrefixWeights006e8850` is `CCombatWheel::GetRingRadius(long) const`.
  - The landed CTCDParticleEmitter::FrameUpdate 0x006E1620 calls 0x006DFDA0 `SetHidden`. It writes the PDB's
    ParticleCreationDisabled bit.
  - The landed 0x00764970 element `CElement12_006f0810` is vector<C3DVector>.
- 006f1ae0 has no call or data reference in the image. Its `_Rb_tree(comp, a)` vs `map(comp, a)` spelling is not
  decided by the bytes. The sibling 0x006F1B20, the same constructor for the 0x18-byte node, is not in this chunk.
- The STLport rows mix `namespace std` (006eff00, 006f1180, 006f1ae0, as the landed 0x0069C970) and `_STL` (006f0810,
  006f20a0, as the landed 0x004D08A0). All leaves are prefix-agnostic and resolve to one symbol each.
- `#pragma pack(push, 1)` on C3DVector (006eff00, 006f1180, 006f0810, 006f20a0) is the real retail layout (PDB
  `pair<C3DVector,bool>` = 13, CProng::Prong at +1), as in the landed 0x006F14A0 / 0x006EB8A0. It is not a layout trick.
