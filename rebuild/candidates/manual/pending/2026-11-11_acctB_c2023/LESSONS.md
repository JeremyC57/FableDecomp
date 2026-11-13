# Chunk 2023 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- VC7.1 evaluates an inline by-value accessor ahead of out-of-line calls in the same expression: `EA()*EB() + p->GetR()` loads `p->R`
  before calling EB/EA, while `q->GetP()->R` (member lvalue) loads the pointer first and reads the float last (`fadd [esi]`). Retail's
  "pointer early, float late" (`mov ebx,[eax+60h]` ... two calls ... `fadd dword ptr [ebx+34h]`, 0x0094C1F2 / 0x0094C216) is a member
  read, not GetRadius().
- A float sum whose integer product sits in a named local (`long m = a*a; if (d > m + p->GetRadius() + 0.5f)`) comes out as
  `fld [p+34h] ... fild ; fadd st(1) ; ... fld [d] ; fcompp ; fstp st(0)` once the then-arm builds by-value CIntelligentPointer arguments
  with the inline constructors; the same code with an out-of-line constructor or a plain call keeps `fild ; fadd [mem] ; fcomp [d]`.
- Inline split thresholds measured from the c2 trace for one TU (0x0094C050): an 89-byte inline member (StayInShop, c1 86) is called
  below static frequency 1.71, the expanded inline LowerBound (c1 52) below 1.86 (score 74 at f = 1.82 is "called": inline iff < 74).
- Static frequency of the code after a loop = entry x (normal exits / all exit edges); each `||` operand of an in-loop `if (...) return`
  is its own exit edge (`if (a || b || c) return` = 3 edges). `bool waiting = true; while (waiting && t >= now) { if (!x || !y) {...;
  return;} waiting = ...; }` has 2 normal of 4 edges (0.5) and the following `if (!waiting)` halves again (0x0094C050: the reply loop at
  3.65, the thanking loop at 1.22). A `for` over a list keeps the post-loop frequency at the body's 5x; a `while` drops back to 1.
- A trailing `RefuseCustom(); Terminate();` after `if (!waiting) { ...; return; }` makes every identical in-loop
  `{ RefuseCustom(); Terminate(); return; }` cross-jump into the function's last block (0x0094C77D, falling into the single epilogue);
  without the trailing copy the merged block is placed after the next unconditional jump.
- `std::list<T>::const_iterator it; for (it = l.begin(); ...)` (iterator declared before the loop) changed the colouring of the whole
  function start: the GetTC out pointer stayed in memory until its first use and the loop's element compare went through ecx with a
  `mov ebx,ecx` copy in the then-arm (0x0094C0B8..0x0094C0BF); with `for (iterator it = l.begin(); ...)` the element took ebx for the
  whole body, the out pointer got an ebx piece with a reload block and the loop head was aligned (+16 bytes). The same edit fixed the
  frame slot order and two scheduling spots elsewhere.
- `end_frame >= CWorld::GetFrame()` compiles to `cmp [esp+N],eax ; jl` (retail 0x0094C3C5); `CWorld::GetFrame() <= end_frame` gives
  `cmp eax,[esp+N] ; jg`.
- `waiting = DoNothing() == 4 && dist2 > max * max;` gives the loop-back `test ah,5 ; jnp head`; `if (DoNothing() != 4 || dist2 <= max*max)
  arrived = true;` gives `test ah,1 ; jne`.
- A by-value CIntelligentPointer argument copied from a member needs the user copy constructor (FABLE_RETAIL_CINTELLIGENTPOINTER_COPY:
  base ctor, vptr, GetPItem, SetPItem); the implicit copy writes the vptr and PData directly (0x0094C51E).

Review at landing:
- hdr opt-in FABLE_RETAIL_CTCPHYSICSBASE_RADIUS_ACCESS (CTCPhysicsBase::Radius public) has byte evidence only; check the PDB access.
- 0x0094BEF0 is CAIStateGroup_WorkBase<CAIStateGroupBase>::StayInShop (IBE_NXZ), not IsInWorkHours: the landed row and two bindings are
  misnamed (NAME_FIXES.tsv); src/0094c050.cpp emits the body and it grades RELOCATION_MATCH against 0x0094BEF0.
- 0x0094C7A0 (10 bytes, `mov [ecx+0Ch],arg ; ret 4`) is an uncatalogued function between the SellToBuyer and GameMaster TUs.
- 0x0094C050's wait-loop source (the `waiting` flag plus the trailing refusal) is the shape that reproduces retail's inline split and
  layout; retail's exact construct is unproven (same bytes).
