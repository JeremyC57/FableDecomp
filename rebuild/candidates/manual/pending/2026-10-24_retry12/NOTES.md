# retry12 notes (2026-10-24)

## 0x00987700 audit (priority 1)
- Catalog row `ReturnParam_Identity_00987700`, 3 bytes `8B C1 C3` (`mov eax,ecx ; ret`). Retail at 0x00987700 IS exactly those 3 bytes
  (int3 padding to 0x00987710). The landed 2-line `return this` stub is byte-correct for its span and the span is a real function.
- The "large matrix routine" is NOT this row: it lives in a manifest gap. `rebuild/manifest/functions.tsv` has no rows between
  0x00987700 and 0x00987BF0 (CQuaternion::operator*). That gap holds six uncatalogued CMatrix44-family functions:
  0x00987710 (17 B, `float operator()(row,col) const`: `fld [ecx+(col+row*4)*4]`), 0x00987730 (`float& operator()(row,col)`, `lea`),
  0x00987750 (55 B, SetIdentity: one dword store per element), 0x00987790 (96 B, build a 4x4 from a transposed 3x4: rows of
  [eax+0/0xC/0x18/0x24] ..., last row 0,0,0,1), 0x009877F0 (112 B, ctor from 16 floats, `ret 40h`),
  0x00987870 (442 B) and 0x00987A30 (442 B): two 4x4 multiplies `this = a*b` / `this = b*a` (argument order swapped), each with an SSE
  path guarded by the byte at 0x013D2880 (`movaps/shufps/mulps/addps` over 4 rows; the operands are first spilled to the home slots
  `[esp+4/0xC/0x10]` and reloaded -- the signature of a retail `__asm` block referencing C variables) and an x87 fallback loop
  (4 iterations, `dec esi`; the per-column `fld/fmul/faddp` term order differs between the two, the VC7.1 reassociation already noted
  for 0x00988A50). These are `retail_asm` candidates (owner-gated: the SSE blocks cannot come from VC7.1 C++), not reconstructible here.
- Label correction: 0x00987700 and 0x00987670 (also `mov eax,ecx ; ret`) are referenced only as `push <fn>` immediates at 0x00A5CBCF /
  0x00A5CBE7 (and 0x00A5D11x) followed by `push 4 ; push 40h` / `push 4 ; push 10h` into `vector_constructor_iterator`
  (0x004038C0: ptr, size, count, ctor). They are therefore empty DEFAULT CONSTRUCTORS of a 0x40-byte class (CMatrix44, consistent with
  the neighbouring 0x9877xx helpers) and a 0x10-byte class (a 4-float vector / CQuaternion), building `T[4]` arrays in that caller --
  not "identity return-param" callbacks. Suggested names: `CMatrix44::CMatrix44()` @ 0x00987700, `C3DVector4/CQuaternion::ctor()` @
  0x00987670 (the 0x10 class is unproven: check the caller 0x00A5CB80's locals). The stubs themselves compile to the same bytes either way.

## Other label / catalog findings
- 005eadc0 (debakeA, "CTCTavernGame::SetStateAfterYesNo", 22 B): the catalog row is the /GS prologue prefix of a 354-byte function
  (0x005EADC0..0x005EAF22, `ret 1Ch`, four by-value CWideString parameters destroyed at [esp+40h/44h/4Ch/54h], a CCharString literal
  at 0x0125603C, callees 0x005EA950, 0x00492973 (9 args), 0x009F1760, 0x0041649C, `__security_check_cookie`). It needs
  `rebound_landed.py 005eadc0 354` and a fresh reconstruction under `/O2 /Oy /W3 /GS`; the manifest's 0x005EAF30/0x005EAF46
  "SetStateAfterYesNo(Alt)" rows are the small siblings that follow it. Not attempted (RE of four callees out of budget).
- 00698c30 (landed PushDownManualCameraList, blank manifest status columns) still grades RELOCATION_MATCH against the exe (218 B).
- 004ad700 / 006d4600 and 0080e5d0 / 0080e530 are byte-identical retail twins (modulo list id / callee / functor size); the parked
  models differed from the landed twins only in shape, so porting the twin matched at once. `twins.py` here scans every parked row
  for an identical or near-identical (<=4 normalised instructions) catalog twin: only 0080e5d0 (exact) and 00698b50 (near) remain.

## GOTCHAS one-liners (proposed)
- Before re-deriving a parked row, look for a landed retail twin: normalise the bytes (immediates/rel32 masked) and compare against
  the catalog; a twin's source ports with a constant change (004ad700 <- 006d4600, 0080e5d0 <- 0080e530, 00698b50 <- 00698c30).
- A vector push_back of the loop local itself (even through a cast) coalesces the homed argument temporary with the local and demotes
  it from ecx to edx; a by-value `ThingCast<T>(thing)` temporary keeps them distinct (004ad700).
- The inline lower_bound walk that gives retail's entry copy `mov esi,eax` declares `last`, `first`, the count and then `it = first`
  (a distinct loop variable); `first = m_pFirst; len = m_pLast - m_pFirst` folds the copy away (0080e5d0).
- A same-TU `__declspec(noinline)` CVectorMap::LowerBound stand-in (so the key never escapes) plus STLport `Find(const K&)` with no
  key copy gives retail's `this`->esi / hash->edi tie (006b93d0); do not use `#pragma auto_inline(off)` for it -- the pragma also
  catches template members instantiated at the end of the TU (`Get()` went out of line).
- Reverse list walks are the real STLport reverse_iterator (copy the base iterator, decrement the copy, compare `base()`); a
  hand-folded `current->_M_prev` walk swaps the iterator/event registers (0095d920, 0096bc20, 0094e530; chunk 337's model).
- VC7.1 allocates callee-saved registers by use weight (esi, edi, ebx, ebp in that order of preference); moving one use of a value
  onto another expression flips a tie (0095d920 v2/v3), so a register-tie residue is a use-count problem, not a declaration-order one.
- A `?:` or `&&` whose last operand is a bool call gives `test al,al ; setne al`; a plain `return call();` does not (008af6e0).
- VC7.1 folds a plain member load into `cmp` whichever side it is on; retail's `mov ecx,[m] ; mov eax,[n] ; cmp eax,ecx` for two plain
  members has no source form found yet (00698b50: named locals, accessor, pointer difference all inert).

## Residues worth a later look
- 008af6e0: the first fail block (`pop esi ; xor al,al ; pop ebp ; ret 8`) sits inline after the IsSpeechAllowed test in retail and is
  reached backwards by the GetInterface miss; every form sinks it past the 4-pop block.
- 007eac70: `mov ecx,[esi+4]` before the three pushes; a local before the string ctor keeps it in esi (62 B), after the ctor it sinks.
- 009bb0d0: SIB `[eax+ecx]` (index as base) vs `[ecx+eax]` in every indexing form.
- 00738a80: Info/Data esi-edi tie survives every Bind3 argument shape.
