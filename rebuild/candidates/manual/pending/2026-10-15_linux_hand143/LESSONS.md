# Lessons from hand chunk 143 (for GOTCHAS)
- /GS parameter caching is the same lane gap as GOTCHAS 23e. Under /GS the lane loads a by-reference parameter used
  after the tests into a callee-saved register at entry (`push ebx ; mov ebx,[esp+38h]`), where retail re-reads its
  slot. The same source without /GS matches retail minus the cookie. An unnamed CGameEvent temporary drops the caching
  but also the cookie (a temporary's char array gets no GS buffer). Four CInputProcess rows were parked on this.
- `(x + 15) >> 4 << 4` folds to `and eax,-16`. Retail's `shr 4 ; shl 4` is the unsigned `(x + 15) / 16 * 16`. When a
  start/end pair is aligned in place (`add eax,0Fh` on the same register), write unsigned locals and modify them.
- A vector clear whose last-pointer store retail emits before a following load must be an inline member of the
  vector. Written in the caller's body, the store sinks past the next load.
- A hoisted constant register is decided by use counts. Retail CHeightMap::Clear keeps 0 in ecx for loop guards and
  stores, while the lane picks 0xff and `test`. No source form found (parked).
- A row whose code runs into a catch funclet (`mov eax,<continuation> ; ret`) before the function's epilogue is a
  prefix row: skip it like the 28-byte family-scan prefixes.
- Six of 40 labels were other functions (CWeaponDef dtor, CMiniMapDisplay ctor, a CTCLook stack-record dtor, an
  insertion-sort step, C3DPrimitive2::FreeData, the event-key comparators). The vtable store, or the only caller,
  settled each one.
- Out-of-line copies of inline destructors differ per module: CHHUndead calls the CIsThingAlive destructor
  0x00E241B0 out of line where CHTE_Prisoner inlines it. Check for `lea ecx,[esp+N] ; call` with no vtable store
  before reusing a family model.
- An out-of-line copy constructor copying a C3DVector word by word through eax/ecx/edx needs C3DVector's memberwise
  user copy constructor. The POD member copies through `lea` plus a re-based pointer.
