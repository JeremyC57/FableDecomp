# Chunk 101 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A static member function called through an object expression (`GetWorld()->GetWorldFrame()`) still evaluates the
  object call and drops its result: `call GetWorld ; call GetWorldFrame` with no `mov ecx,eax` (0x007EEBB0, as 0x008EC7E0).
- A by-value CDefPointer temporary read through `->` (`GetDef()->Smashable`) keeps the flag load ahead of the release only
  when the release is a member of the DEF (`if (--m_refCount == 0) Destroy();`) called from the pointer's destructor; the
  same code inline in the destructor schedules `dec eax` above the flag load (0x007EECF0).
- Two floats initialised from one non-immediate constant (`fld [2.0f] ; fst ; <vtable> ; fstp`) are a CHAINED body
  assignment `b = a = 2.0f`, while a lone `5.0f` is an immediate dword store (0x004D3B41).
- A raw data pointer zeroed AFTER its owner's vtable store in an inlined constructor is a member object with its own
  inline constructor (`CIVCountedPointer() : m_pObject(0)`); a plain `Data(0)` or a body assignment stores it before the
  vtable (0x00BFBBC0, 0x004DC737). Under optimize("s") both inline constructors need `__forceinline`.
- The out-parameter GetInterface template (0x007504E0 shape) is emitted OUT OF LINE in a caller that makes two further
  calls on the component; `__forceinline` restores the inline body (0x0072AC90).
- `GetWorld()->Member->Call(arg)` pushes the argument before calling GetWorld in the lane; a named local for the
  GetWorld result restores retail's call-then-push order (0x00747090; the wave-24l rule, also for a virtual call on a
  fetched manager, 0x00787550).
- `(long)(p ? p->f * g_int : 1.0f)` gives the per-arm `__ftol2` with `fild [g] ; fmul [p+f]` (0x007470E0).
- A 40-bit thing id passed as `unsigned __int64` (`and ecx,0FFh` on the high word only) is `UniqueID & 0xFFFFFFFFFFui64`
  (0x00787580 family).
- A persist hook (`ret 8`, slot 1) of a STATIC member transfer the static directly and never read `this`
  (0x0072AB40).
- Lane gap seen six times (0x0072A970 family): after an explicit `CCharString name("..", -1)` the retail loads the AddTC
  object (`mov ecx,[esi+N]`) BEFORE `push 0 ; push 1 ; lea eax,[name]`; the lane pushes first and uses ecx for the lea.
  optimize("s") reproduces the order but adds a frame pointer and narrows the bit test; accessor/local/helper forms do not.

Review at landing:
- 0066b040 used all four attempts; the CTCSummonSpell-style release with the null store inside the arm was not tried.
- 0072a970/0072a9b0/0072aa00/0072aa50/0072aaa0/0072c680: one shape, parked as a lane gap -- try the retail (RTM)
  compiler if the 5.1 lane has it. Their names (SetupForcePush/SetupHealLife/SetupMultiArrow) come from the TC strings.
- 004dc737 (CTCStockItem constructor) uses a local polymorphic model next to engine/CTCStockItem.h (the header is a
  flattened POD with `void* __vftable`); 008abbf0 includes the header and views PrimHandle through a cast accessor.
- 00bfbbc0 is attributed to CLocalDetailLayer::CObject from the member layout only (no callers found by xref.py):
  confirm the owner (the 76-byte PDB CObject) before landing under that name.
- 00c25cb0 class name CParticleHandle is descriptive; the landed callee 0x00C1EA40 is named CParticle::IncrementLifetimer
  but its body DECREMENTS the word at +0x1e (a reference drop) -- that landed name looks wrong.
- 0x00C35C30 lands under _unattributed (free __stdcall); its callee 0x00C43980's manifest name is unverified.
