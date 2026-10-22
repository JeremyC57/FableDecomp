# Chunk 280 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A perform/action result compared inline (`== 1`) next to a temporary's destructor materialises a bool (`neg ; sbb ; inc`). Use a named `long result` (0x0096FCD0).
- Unrotated loops with an in-loop Terminate: write `for (;;) { x = Get(); if (!x) break; if (dead) continue; r = ...; if (r != 1) continue; Terminate(); return; }` (0x0096FCD0).
- Two out-of actions sharing one frame slot and one destructor come from an if/else of two perform calls, tail-merged; retail pushes the shared `push 1` before the branch. A conditional (`?:`) of temporaries makes two slots with flags (0x0096FF10).
- An empty filter tag with NO user ctor shares the byte slot with a named bool (`bool has = (bits >> b) & 1`); with a user ctor it takes its own dword (0x00942EE0). `T()` value-initialisation of the empty filter stores a zero byte; use a named, uninitialised filter.
- Same-TU noinline stand-ins for the search templates (FindThings 0x00943830, GetPBestThing 0x009107B0, GetAllThingsInRoughArea 0x006B2720) and for GetDef<CSoundThemeDef> 0x006B25E0 keep the local vector/def pointer from escaping. The start/end then stay in callee-saved registers, and the def release reads its slot once (`mov ecx,[slot] ; cmp ecx,ebx ; je ; dec [ecx+4]`). The second fix took 0x006B1F30 from a reload to a full code match.
- Name a GetPItem pointer before the action calls to get retail's GetPItem-before-pushes order (0x009474F0).
- A DIFFER with no visible diff lines is a float immediate hidden by nd.py; compare raw immediates. The 0x3B360A13 look speed is 0.0027777f, not 1/360 (0x3B360B61 at 0x01238E00 IS 1/360) (0x009474F0, 0x00727220).
- `(float)GetWorldFrame() + (float)fps * def->X` with fps read into a local first gives `fild frame ; fild fps ; fmul ; faddp`; a named long frame gives fiadd.
- A struct copy of a {vector; set} sub-struct gives the `lea edi,[mesh+0x2c] ; ... ; add edi,0xc` CSE (0x00A90AE0). The STLport set is 12 bytes: header, count and an empty key_compare.
- A default-value temporary for resize with an empty ctor stores nothing. Releasing a counted pointer through a local copy `old` avoids a reload.
- Two-way interface-0x4d collision checks are one bool: `bool ok = true; if (a->HasTC(0x4d)) { ok = GetTC(a)->CanCollideWith(b); if (!ok) return; } if (b->HasTC(0x4d)) ok = GetTC(b)->CanCollideWith(owner); if (ok && ...)`. This gives retail's `mov al,1` before the first flag test. A `__forceinline` helper used twice caches the owner in ebp and pushes an extra register (0x00727220). In big bodies, HasTC must be `__forceinline` (otherwise VC7.1 emits a call).
- An XY length of an offset whose Y was CSE'd into a frame copy: the `offset.GetXYLengthSquared()` member call (X*X + Y*Y) restores retail's X-first order where the inline expression reads the Y copy first in BOTH operand orders (0x00726340, 0x00727220).
- A `GetVelocity()` result assigned into an existing vector (`offset = Owner->GetVelocity(); offset *= 0.1f;`) keeps its own hidden-return slot, as retail does.
- `float sign = (frame % 2 == 0) ? -1.0f : 1.0f` gives retail's `mov [s],-1.0 ; je ; mov [s],1.0`; the `? 1 : -1` form inverts the jump. A random angle computed as `turns = angle * (1/360); Rotate(turns * sign)` keeps retail's multiply order.
- VC7.1 /O2 cross-jumps identical `fstp [this+N] ; jmp end` tails of switch cases into the last case's store (0x00C72200). Retail kept them separate, so this may be a QFE/RTM difference; it was not tested because the row over-captures anyway.
- Catalog over-capture check: when the oracle is longer than the built function and the retail tail shows `int3` padding and then a new prologue past a jump table, the row cannot match until the catalog is split (0x00C72200 includes the getter at 0x00C724F0).
- A hidden-return temporary of a by-value virtual call shares the dead slots of later block locals; declaring the consumer local (e.g. the result vector) BEFORE the call gives the temporary its own slot, but VC7.1 then hoists the local's zero stores above the call (AdvB3 on 0x006B1F30).

Review at landing:
- Rows that needed more than 4 grades: 0x0096F770 (10), 0x00942EE0 (8), 0x009474F0 (6), 0x0096FF10 (5), 0x00A90AE0 (4+), 0x00727220 (12), 0x00726340 (9), 0x006B1F30 (8).
- 0x006B1F30 (frame-only diff) went to advisor AdvB3: no match after 12 more forms. Retail's 0x7c frame holds about 44 dead bytes above +0x60. Untried forms are listed in probe/advisor/006b1f30/NOTES.md.
- 0x00C72200 has a two-level jump table (dword case table plus byte index table) and over-captures the function at 0x00C724F0. Land it only after the catalog split.
- /GS row: 0x00C5D7F0 (parked).
- Skipped as the known inlined-lower_bound lane gap: 0x00966DA0, 0x00967D40, 0x00945060, 0x00944940. Skipped as the inline-asm fistp gap: 0x00716B30.
- LowerBound overloads: 0x00727220 calls the 0x0040F020 copy (non-const); 0x00726340 calls 0x004365B0 and declares it const. Keep them as two overloads if both ever land in one TU.
- Local layouts are used where engine headers do not fit (functor/TC/thing views in 0x007272xx, 0x006B1F30, 0x00C72200). The offsets were cross-checked against the retail disassembly and landed siblings (0x00726BE0, 0x006A8300, 0x006B1960, 0x00C72AF0).
