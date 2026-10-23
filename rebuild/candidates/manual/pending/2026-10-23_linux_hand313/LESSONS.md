# Chunk 313 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- HasTC word form `unsigned long word = m_presentBits[t >> 5]; return ((word >> (t & 31)) & 1) != 0;`, used in GetInterface as `const bool& found = HasTC(t); if (found) {...} return found;`, gives retail's `mov ecx,[eax+0x30]; shr ecx,0x10; and cl,1; mov [esp+N],cl` bool form. The byte-narrowed form loads `cl,[eax+0x32]` instead. The mask form (`test dword [eax+off], mask`) is the separate `HasTC(t) & mask` then GetTC spelling; one function can use both.
- A same-TU `#pragma auto_inline(off)` LowerBound stand-in lets the lookup-key temps share frame slots with component out-vars, as retail does. Add a separate const body for 0x4365B0.
- Scoped intelligent-pointer temp `{ CIntelligentPointerThing previous(x); member = previous; }` gives retail's `lea ecx,[tmp]` re-address for operator=.
- Per-call address-taken bools: declare `bool reached;` inside each loop body or if arm (each arm gets its own byte: [esp+0x12] / [esp+0x13]). Use a separate function-scope bool for a later call.
- Action object sizes: AddRealObjectToInventory (0x843700) 0xb0; AlertToPlayer (0x842150) 0xa8; CCombatActionBase-derived 0x114; 0x843670 / 0x842c70 actions 0xd4.
- Def release without re-read: `~CDefPointer(){ T* p = P; if (p && --p->RefCount == 0) p->DeleteThis(); }` plus a same-TU GetSubDef stand-in. When retail has no null test (`mov ecx,[def]; dec [ecx+4]`), drop the `p &&`.
- A single-case `switch (x) { case 1: ... }` gives retail's `dec eax; jne`.
- A float argument stored in the frame: name it (`float radius = target->Radius;`).
- STLport vectors returned by value: `CVec v = shop->GetFreeSaleItemPlaces(true);` with inline dtor `if (_M_start) free(_M_start);`. VC7.1 has no std::min, so use a local `Min(a,b){return b<a?b:a;}`.
- `(places.begin() + i)->Position` keeps the per-iteration `lea esi,[edi+edi*8]` (no strength reduction).
- A squared sum emitted Z,Y,X comes from a C3DVector temp built by operator- (ctor args evaluate right to left). For `fstp st(3)` and the pops before fsqrt, write `float l = (a - b).LengthSquared(); sqrt(l)`.
- x87: `!(d > x)` gives `test ah,0x41; jne`; `d >= x` gives `test ah,1`; `a < b` gives `test ah,5; jp`.
- Hoisting the shop-door IsPointInside `this` (GetPItem before the arg lea): name the building pointer per arm (`CThingBuilding* b = shop.GetPItem(); b->IsPointInside(front)`).
- Nested success structure (`if (result == 2 && alive) { ...; while (...) { if (DoNothing() != 4) { Terminate(); return; } } } Terminate();`) reproduces retail's two merged exit blocks. VC7.1 then calls the vector dtor out of line at some exits and inline at others (00926ed0, 0x004DF88A), with no source change.
- `#pragma pack(1)` classes (an IP at +0x6B or +0x1D in OpinionReactionRoot and VersusScorpionKing): model them with `#pragma pack(push,1)`. A pointer-to-member table with an MI representation needs `#pragma pointers_to_members(full_generality, multiple_inheritance)`; `__multiple_inheritance` on a single-inheritance class is error C2292.
- Fast normalise: reuse the landed 0x13CE558 Normalise recipe (007a6e20). The VersusRockTroll/ScorpionKing copies cache x*x and y*y for a later 2D distance.
- Advisor levers that did NOT help register-role swaps:
  - a by-value `__forceinline` FindInterface (made 009647c0 a whole-function swap);
  - a null-initialised out-param;
  - ternary, if/else or default-inverted retry counts (00927f30);
  - a scoped `reached`;
  - helper, const-ref cast, early-return and reference-local reshaping of a temporary action (009658b0).
  A named action object fixes 009658b0's registers but moves the bool push before the ctor.

Review at landing:
- 009658b0: retail esi holds the loop `result`, edi holds `this`. Under R&D's weighted-count rule the result must outweigh `this`. The named-object form achieves this but costs 7 bytes. Worth one more R&D pass.
- 00926ed0: the wait condition uses retail's `neg al; sbb eax,eax; inc eax; test eax,eax` logical-not value. No spelling tried (inline helper, `== false`, ternary) reproduced it.
- Label corrections are in NOTES.md. Many CAIStateGroupBase helpers (Terminate 0x8FCF40, GetPThingWorld 0x8FCFA0, GetPosition 0x4C73D0) still carry propagated wrong names.
