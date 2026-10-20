# Retry 10 -- lessons (fold into GOTCHAS at landing)

- The "inline-once destructor band" is not a cost band: VC7.1 (RTM 3077 and QFE alike) expands an inline destructor at
  EVERY site while the object has two destructor sites, and at exactly ONE site once it has three or more live sites
  (unreachable sites do not count). For a by-value parameter the expanded site is the first one in source order (the
  `return` inside the wait loop) and the later sites call the out-of-line copy and tail-merge; for a block-local (the
  vector in 0066bd00) the expanded site is the block end and the earlier returns call out of line. So a script thread
  with "both exits inlined" (00d17090, 00e32200) is written with a THIRD exit: `if (IsActiveThreadTerminating())
  return;` after the loop instead of folding that test into the final `&&` / `if (!...)`. Retail's tail (`jne` past the
  flag store straight onto `call ~CScriptThing`) is the merged sites 2 and 3.
- Per-callee, not per-caller: inline accessors, extra sibling functions in the TU, and wrappers around the other callees
  never changed the destructor decision; a dead member store in the callee can (vector `_M_start = 0` -> once,
  CCountedPointer `Info = 0` -> never), but it perturbs the register colouring (zero register kept for the null test).
- A bool function whose result is a comparison: `return a >= b;` emits `xor eax,eax ; cmp ; setge al` (int-width);
  retail's bare `setge al` is a NAMED bool (`bool ok = a >= b; return ok;`), `if (..) return true; return false;` or
  the `? true : false` ternary (0059da10).
- A long-returning inline helper (`return 1; / return 0;`) negated into a bool result: `!(unsigned char)Helper()`
  gives retail's `mov eax,1 ; ... ; test al,al ; sete dl` (byte test of the full-register constant); `!(bool)Helper()`
  tests eax, a bool helper emits `mov al,1` (007525c0).
- The inline `CCharString == ""` family (sete materialised): write the null-block `strcmp("", "")` and the block compare as
  two tests joined by `goto` into the SAME arm (the landed 0x0048F7C0 idiom), keep the non-empty arm in line and put the
  empty arm at the label; a default literal passed to a `const CCharString&` is an implicit conversion
  (`SetAnimationName("LIT")`), not a named local -- the named local is homed in a different dead argument slot
  (008bb130, 008bb200, 008bb6f0, 00892cf0).

## CAnimInit passed as a temporary (008d6780, 008d6cb0, 008d6e80, 008d6f20 -- NCreatureMode::*::AnimUpdate)
`anim.SetMainAnim(CAnimInit(type, 0, entry->BankIndex, 1, looping, flags, 100), blendIn, 0);` -- the record is a
temporary bound to the `const CAnimInit&` parameter, not a named local. With a named `CAnimInit init(...)` VC7.1 treats
the inline ctor's `anim` parameter as a variable: the BankIndex load is hoisted and coloured into a callee-saved register
(esi/ecx) and `&init` goes through the leftover scratch register; as a temporary the load is a scratch temp (edx, or eax
loaded lazily after the flags push) and `lea eax,[esp+14h]` follows. Same instructions, different registers -- the
"register colouring only" family note for these rows was this idiom. Also: the slot-4 virtual `GetAnimFilter()` of
CModeBase takes NO argument; the `push 0` before `call [eax+10h]` is the third (long) argument of
CThingCreatureBase::GetAnimFromName(const CCharString&, const CAnimFilterBase*, long) @ 0x00662B10.

## CDefPointer release through a `long&` (00591e8c, and 5-12 bytes closer on 20 more release-family rows)
`--Object->RefCount == 0` written directly makes VC7.1 either re-read the pointer slot after the `dec` (member form:
`dec [eax+4] ; mov ecx,[slot] ; mov eax,[ecx+4] ; test`) or materialise the count in a register (local-copy form:
`mov edx,[eax+4] ; add eax,4 ; dec edx ; test edx,edx ; mov [eax],edx`). Decrementing through a reference,
`static __forceinline bool DecRefToZero(long& count) { return --count == 0; }` (or `long& count = p->RefCount; if (--count == 0)`),
gives retail's `dec dword ptr [reg+4] ; jne`. With a local copy of the pointer in the destructor the remaining gap on the
other rows is only `mov eax,[slot] ; test eax,eax ; mov ecx,eax` versus retail's `mov ecx,[slot] ; test ecx,ecx`.

## Call result used directly as the object of a virtual call (00543d30, 00543df0 -- CParticleEffect::CreateEmitter)
`CFrontEndManager::GetInstance()->GetLayer(Depth)` inside the argument list, not `CFrontEndManager* frontEnd = GetInstance(); ... frontEnd->GetLayer(Depth)`.
With the named local VC7.1 hoists the vtable load (`mov edx,[eax]`) right after the call, which lets the temporary
CRightHandedSet's constructor stores sink past the next push; written inline the vtable load stays at the call site
and the stores land between `push 1` and the GetLayer call as in retail. Same family as the CAnimInit lesson: a named
local for a value used once changes scheduling/colouring, a temporary or inline expression does not.

## STLport vector model: the end-of-storage pointer is an _STLP_alloc_proxy (00458c19, 00457da9, 004e00bc, 004570d3)
A 12-byte vector modelled as three plain pointers zeroed in one ctor (`first(0), last(0), end(0)`) makes VC7.1 schedule
the next member constructor's `lea ecx,[this+N]` one or two zero stores too early ("24i lea-scheduling lane gap").
Model it as STLport does: `T* _M_start; T* _M_finish; CAllocProxy<T> _M_end_of_storage;` with
`template<class T> struct CAllocProxy { T* p; CAllocProxy(T* v) : p(v) {} };` -- the third store then comes from a
member-object constructor and the lea lands between the second and third store exactly as retail.

## Zeroing a vector member through an inline member function (00724f80 -- CTCPhysicsControlled::OnCreate)
`ImpulseVelocity.Set(0.0f, 0.0f, 0.0f)` with `void Set(float x, float y, float z) { Z = z; Y = y; X = x; }` reproduces
retail's Z,Y,X store order AND keeps the following bit-field loads (`mov cl,[esi+3Ch]`) after the stores: stores through
the member function's `this` pointer may alias, so the loads are not hoisted. Direct member stores
(`ImpulseVelocity.X = ... = 0.0f`) let the lane hoist the first flag load above them.
