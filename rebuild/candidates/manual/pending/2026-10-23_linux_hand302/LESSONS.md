# Chunk 302 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- `const T& current = vec[slot]; T previous = current;` fixes a SIB base/index swap on the lea of an indexed element (AssignAbilityPC).
- A value that retail loads before a call (e.g. `unassigned.m_pObject` before `map[k]`) comes out early from a named value local (`CComponent* icon = it.m_pObject; map[k] = icon;`).
- Same-TU noinline stand-ins for vector push_back, the element ctor/dtor and the vector dtor make a local non-escaping. That enables `_M_finish` caching across calls and a strength-reduced counted loop. Escape status also flips the frame order: non-escaping locals go lower.
- An empty predicate passed by value is zeroed (stosb) when value-initialised; a user ctor `CPred() {}` removes the zeroing.
- Constructors: a declaration-order initialiser list reproduces retail's member store scheduling (CTCHeroStats ctor). `resize(n, 0)` with the literal pushes the `const long&` temp before the count's call chain.
- `__forceinline` is needed on an inline derived ctor whose base ctor is out-of-line, and on def-pointer dtors; plain `inline` stays out of line.
- Def pointer release: `if (m_p && --m_p->RefCount == 0) m_p->DeleteThis();` with RefCount a direct member at +4. Do not add an explicit `__vftable` member next to virtual functions (shifts RefCount to +8).
- Tail merging: write identical arm tails out in full (if/else both ending in the same calls); VC7.1 merges them as retail does (0057df85, 0047e9ae).
- `lea ebp,[esp-78h]` frames (EBP biased into the middle of a >128-byte frame) come from a size TU with the default FPO setting, i.e. `#pragma optimize("s", on)` WITHOUT `optimize("y", off)`. With "y" off you get `mov ebp,esp`; speed gives `mov` instead of `and [x],0`.
- A local declared in a `{}` block that ends before a sibling loop releases its frame word, which fixes an 8-byte frame-size miss (0057df85 marker vs the loop's fine/frames).
- `IS_THING_ALIVE<CThing*>(member)` with a member typed as a derived pointer builds the conversion temp in a frame word as retail does; a named `CThing* hero` local takes a different slot.
- Globals ahead of call chains also apply to `g_pPlayerGui->F(GetWorldMap()->GetRegion(r))`: an inline `GetPlayerGui()` accessor loads it first. The same works for `g_pMainGameComponent->TextBank` when retail loads it after the CCharString temp.
- Behaviour tests: x87 (VC7.1 /Od) and SSE (g++ x64) truncate inexact float products differently (`0.01f*500` gives 4 vs 5); use dyadic constants such as 0.125f.
- Lane gaps seen: a ternary string-literal argument becomes a register select hoisted before `push ecx` (retail pushes in the arms); retail packs new-temps into dead argument slots ([ebp+8]); liveness-style slot sharing of a marker pointer with a later inline-param home.

Review at landing:
- 0047e9ae is the 5th SetPosition push ecx/fsubp row; any advisor fix for 0047e651 should be re-graded on it.
- 0057df85 / 0047e9ae differ files carry `// Flags: /GS`.
- 005b4a95 test constants were changed to dyadic floats (VC7.1 FAIL before).
- 00c3dfd0 / 00c41060 skip: confirm the library attribution and drop the CTCHeroStats label.
