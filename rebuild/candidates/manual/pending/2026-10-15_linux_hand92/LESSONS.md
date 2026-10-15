# Chunk 92 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **VC7.1 (no /GL) uses interprocedural facts about callees DEFINED EARLIER IN THE SAME TU.** Three shapes that look
  like whole-program codegen reproduce once the real callee body sits above the caller in the source:
  (a) register preservation -- `this` kept in EDX across the call (`mov edx,ecx ; call ; ... mov ecx,edx`) because the
  callee is known not to touch edx (0x005EEF84 with GetAbilityMaximumLevel 0x005EDEA3 above it; declared-only: 53v51);
  (b) no memory writes -- a loop's size() hoisted into a callee-saved register and `this` dropped (ecx reused for the
  element) around calls to a callee that writes nothing (0x005BC9C8 with CCategory::OnKill 0x005BC754 above it);
  (c) read-only callee -- the element pointer strength-reduced from ONE read of `first` while size() is still
  re-evaluated (0x005BCA87 with CCategory::GetSelectedItem 0x005BC99C above it; declared-only: 73v71). Per-function
  pragmas (`optimize("y", off)` ... `("y", on)`) can wrap the callee. Re-try the parked "this kept in ecx across a call"
  (00c20cb0) and "hidden return pointer kept in edx" gaps with the callee defined in the TU.
- VC7.1 does not delete empty loops: a loop whose body is an inline empty member call keeps the entry test, a
  preheader re-computation of the hoisted size() and `inc ; cmp ; jb` (0x005BC754).
- Constructor store order: init-list members are stored in declaration order; a word that retail stores BEFORE two
  earlier-declared fields is a body assignment (`: a(0), b(0), c(0) { rate = 44100; channels = n; encoding = 0; }`,
  0x00C2D920).
- An 8-byte record cleared with one base load and two stores (`mov eax,[esi+4] ; mov [eax+edi*8],ebx ;
  mov [eax+edi*8+4],ebx`) is an `__int64` element `= 0`; two `long` member stores re-read the array pointer between
  them. A slot indexed at every use (`a[i] != 0`, `cur = a[i]`, `a[i]->f()`) gives retail's `lea eax,[eax+edi*4]` CSE;
  a `T*&` reference local makes the lane strength-reduce `i*4` into a fourth register (0x00C443D0).
- Size-TU loop entry tests: `i != size()` = `test edx,0FFFFFFFCh ; je` + `jne` back edge (0x0057AA1B); unsigned
  `i < size()` = `sar edx,2 ; je` + `jb` (0x005EE146); signed `long i = 0; i < size()` = `test eax,0FFFFFFF8h ; jle` +
  `jl`; `i = 1; i < size()` = `and eax,0FFFFFFF8h ; cmp eax,8 ; jle` (0x005EE0E7). The speed build reuses `first` from
  the size computation and aligns the loop head; retail re-reads it after the entry test.
- Under `optimize("s")` a template converting constructor (`CDefPointer<T>(const CDefPointer<U>&)`) and a small
  inline find (`CVectorMap::Find`) are called out of line; `__forceinline` both (0x005EE207 62v68 -> match,
  0x005EE31D 51v72 -> match).
- `Min(level, MAX)` over a `static const long MAX = 3` in a frame-pointer size TU: `cmp eax,3 ; mov eax,offset MAX ;
  jg ; lea eax,[ebp+8] ; mov eax,[eax]` -- the clamped local lives in the dead argument slot because `Min` takes it by
  reference (0x005EDEA3).
- `if (c) a->push_back(Get(x)); else b->push_back(Get(x));` hoists the shared argument pushes above the branch, keeps
  one Get call per arm, loads each arm's object into ecx and merges push_back + the temporary's destructor (0x005EEFB7).
- A derived-to-base pointer argument to a `T* const&` parameter builds the conversion temporary in the argument's own
  slot (`mov eax,[esp+4] ; lea edx,[esp+4] ; push edx ; mov [esp+8],eax`): no long->pointer trick needed (0x008631B0).
- An in-place `__fastcall` conversion of a by-value struct argument (`F(&pos, &pos)`) is `lea edx,[ebp+8] ; mov ecx,edx`
  (0x005BCE49); a `fld1 ; push ecx ; fstp [esp]` float argument is the literal 1.0f.
- A frame-pointer `/GS` body (cookie at [ebp-4]) is `// Flags: /GS` plus `optimize("y", off)` + `("s", on)`; the flag
  alone gives an esp-based frame (0x005BC527, 105v95).
- Behaviour tests built by tall.py use `/Od`: VC does not elide by-value copies there, so a counted-pointer test model
  needs a counting copy constructor (g++ elides and passes without it; 0x005EE207).
- Tooling: gall.sh greps every `// Flags: ` / `// Leaf: ` line, so explanatory comments must not start with those
  prefixes (use `// GS note:` / `// Leaf note:`).

Review at landing:
- Three sources define a same-TU callee next to the graded function (needed for the bytes): 005eef84.cpp also defines
  CTCInventoryAbilities::GetAbilityMaximumLevel (0x005EDEA3, landed separately from 005edea3.cpp), 005bc9c8.cpp defines
  CCategory::OnKill (0x005BC754, also its own row), 005bca87.cpp defines CCategory::GetSelectedItem (the landed
  0x005BC99C body). Line 1 / `// Leaf:` select the right symbol; check the lander does not double-land the callees.
- Local `this` models sit next to existing engine headers whose PDB layouts do not fit the shipped classes:
  CTCInventory.h (00587749), CTCInventoryAbilities.h (all CTCInventoryAbilities rows), CTCHeroStats.h (0057aa1b) -- the
  shipped vectors are 12 bytes and the CTCInventoryBase base is 0x14 bytes narrower. Strict header mode will warn.
- Already-landed naming errors noticed: 0x005BC99C "CCategory::GetHighlightedItem" reads +0x20 (SelectedItemIndex) ->
  GetSelectedItem; 0x00AC40C0's CArray model names {data, count, capacity} while the CArray<C3DGroup2> push_back
  (0x00AE83D0) treats the words as {first, last, end} (bytes unaffected). 0x006404B0 "CTCInventoryAbilities::
  AddLearnedExpression" is the same list-remove shape as 0x008631B0 (CCreatureInteraction::RemoveCreature) and is
  probably propagated too (unverified).
- No row needed more than 4 grades; no catalog row holds two functions.
