# Chunk 219 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Out-of-line converting assignment: `ptr = thing;` (an IMPLICIT conversion temporary through an out-of-line
  `CIntelligentPointer<const CThing>(CThing*)` ctor, 0x004A96F0) gives retail's `lea eax,[temp] ; push eax` before the
  out-of-line operator= (0x004A9760); `ptr = CIntelligentPointer<const CThing>(thing)` reuses the ctor's returned eax
  (0x0094B8B0; also fixes the argument in 0x0093C2F0 / 0x00946400). If both a same-type and a converting operator= exist,
  make the same-type pointer ctor `explicit` to avoid the ambiguity.
- IMPLICIT CCharString conversion in a derived action's base mem-initializer (`: Base(owner, "PUT_EMPTY_JUG_ON_BAR",
  0x29)`) builds the string before any argument push and destroys it before the derived vtable store; an explicit
  `CCharString("...")` argument is built between the pushes (0x00967A20).
- An action temporary passed BY POINTER to an inline helper (`SetPerformAction(output, &Action(...))`) keeps the
  virtual Clone call on the ctor's returned this and the memory `or [flags],8`; a named action local devirtualises the
  call and does a load/or/store (0x0094DC40, the landed 0x0097DA40 brain-output model inlined).
- `fcomp st(1) ; fnstsw ; fstp st(0)` = both compare operands are NAMED float locals (`float d2 = ...; float r2 = r*r;
  if (!(d2 < r2))`); the inline `delta.LengthSq() < r*r` form gives fcompp (0x0090F320). `jnp`/`jp` after `test ah,5`
  tells which side of `<` holds.
- Separate `if (r != 2) { Terminate(); return; }` exits tail-merge with the LAST one placed inline (and a later exit can
  jump past a shared `mov ecx,esi`); an `a != 2 || b != 2` condition moves the Terminate block to the end
  (0x0096F050, 0x0090F320, 0x00916910, 0x0095D200).
- `if (x) Call(x); else Call(G());` sets `this` for Call in both predecessors (`mov ecx,ebx` before the jne) -- the
  `if (!x) x = G(); Call(x);` and ternary forms put it after the join (0x0094B8B0).
- Component register at the use: `if (HasTC(n)) { T* p; GetInterfaceV(&p, n); p->f(arg); }` gives `mov eax,[eax+4] ...
  mov ecx,eax` (the second bit test folds away); `HasTC + FindEntry(n)->component->f()` loads straight into ecx; the
  bool out-parameter GetInterface in an `if` can flip `this` from edi to esi for the whole function (0x00921870,
  0x00967A20, 0x0094B740).
- The bitset-test bool materialised to a frame byte (`shr ; and cl,1 ; mov [esp+x],cl ; je`) is a GetInterfaceT form
  `bool has = test(); if (has) {...} return has;`; a `bool reached` declared later can share that byte (0x0090F320,
  0x00916910).
- A member of a CALL RESULT used as an lvalue (`GetShop()->m_flags |= 1`) compiles to `mov dl,[eax+d] ; add eax,d ; or dl ;
  mov [eax],dl`; put the result in a named local first (0x00926A10). Likewise `GetX()->member.Method()` gives `add eax,d ;
  mov ecx,eax` where a named local gives `lea ecx,[eax+d]` (0x0090C1A0).
- A creature argument through an inline `GetCreature()` accessor (vs a direct `PBrain->PCreature`) changes the
  eax/ecx/edx roles of the argument load -- try it whenever a component should land straight in ecx after the
  argument load (0x0094B8B0).
- A frame-rate global read before a seed store: write the load between GFROR13 and `*seed = rotated` (or use
  `--flags /Oa` with the natural order) (0x0094B740). Locals read before a call keep declaration order INVERTED in
  load order (declare the 64-bit UID before the fps copy to load fps first, 0x00937C20).
- `__forceinline` is needed for small inline helpers that retail inlines into big bodies (CDefPointer dtor/release,
  the shop sync body, the range test).

Review at landing:
- Rows past 4 grading attempts (kept moving with new lessons): 0x00921870, 0x0094B8B0 (matched), 0x0090F320 (matched),
  0x00930EE0, 0x00946400, 0x0094D680, 0x00962C10, 0x00957DB0, 0x0093D270, 0x0095D3A0.
- Local base models next to existing engine headers (the generated headers flatten the CAIStateGroupBase base): every
  src/ file models the class locally with the PDB member names (LightHomeCandle, MinionFollowThing, OpinionReactionRoot,
  StayNearSchool/SitInPub (no header for UsesSeat), TrollBase, TavernServer, MinionHasInformation).
- 0x00938280's catalog row includes its 24-byte jump table after the code (q.py grades the whole 248 bytes).
- Recurring lane-gap families seen here: (a) C3DVector copy into a compare temporary loads all three dwords before
  storing in retail (0x0090C1A0, 0x00937B10, 0x00937C20, 0x00936A60); (b) two inlined interface lookups whose key slots
  retail shares (0x0093C830, 0x0093C2F0, 0x00962C10); (c) a lookup phi that retail merges straight into ecx with an
  argument push in between (0x0090C670, 0x0095D3A0); (d) member-address CSE hoisted above a temporary's GetPItem
  (0x0093D270, 0x00946400). Worth a targeted probe session.
