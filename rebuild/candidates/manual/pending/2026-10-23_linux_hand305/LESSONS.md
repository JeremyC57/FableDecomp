# Chunk 305 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **GetSubDef / GetDef stand-in:** a same-TU stand-in body for an out-of-line GetSubDef/GetDef keeps the def in a register and reuses its frame word.
  - Form: `#pragma auto_inline(off)`; the body only writes the out pointer.
  - Fixed 0x005EA400; took 0x0079B6F0 from 739 to 732 bytes and 0x007BD2D1 from 613 to 599.
- **Inline STLport lower_bound in a GetInterface out-parameter helper:** declare `last` before `first`. This gives retail's
  register assignment for the last lookup (`mov edi,[x+48h]; mov esi,[x+44h]`). Fixed 0x007BA550.
- **Uninitialised out-parameter on the miss path:** the never-written local can share the slot of a parameter that is still
  live. Retail reads `deed` / `thing` ([esp+28h], [esp+24h]) on the miss path of 0x007993F0 and 0x00802E00.
- **Loop-invariant member loads:** a loop-local `CThing* parent = ParentThing;` at the top of a `for (;;)` loads the member
  before the by-value CCharString argument is built, kept in a callee-saved register (0x007BA550).
- **Inline accessors and the eax-to-ecx move:**
  - An inline member accessor for a manager pointer (`GetWorld()->GetDefinitionManager()->F(CCharString(..))`) gives
    retail's `mov eax,[eax+10h] ; push ; mov ecx,eax`.
  - The plain member read gives `mov ecx,[eax+10h]` (0x007BA550).
- **CCharString equality:** the inlined operator== needs `__forceinline` in these TUs, or VC7.1 calls it out of line
  (0x00802E00).
- **HasTC:** it needs `__forceinline`. Plain `inline` becomes a `push type ; call` when the TU is large (0x007993F0).
- **Releasing a counted def:** for retail's `mov esi,[eax+4]; mov ecx,eax; add eax,4; dec esi; mov edx,esi; test edx,edx`
  sequence, write the destructor `if (Object) Object->Release();` with Release() as `DecRef(); if (GetRefCount() == 0) Destroy();`.
  - An open-coded `--RefCount == 0` gives `dec [eax+4]` plus a reload (0x007993F0, as 0x00625ED0).
- **Block scope and slot sharing:**
  - A function-scope CDefPointer adds a frame word.
  - Block-scoping it lets VC share a word, but VC picks a dead parameter slot first (0x007993F0).
- **Pushing an argument before `this` is computed:** `ParentThing->GetPlayer()->HasMode(2)` pushes 2 before the
  GetPlayer() call (0x00802E00). Do not read the early push as an argument of GetPlayer.
- **Secondary-base draw callbacks:** CTCRandomAppearanceMorph::OnAppearanceDraw runs with `this` at +0xc and reads
  ParentThing as [this-8]. Model it with real multiple inheritance (CTCBase, then the callback base) (0x00727D00).
- **x87 rounding helper:** `fadd 0.5 ; fistp ; integer compare of the float bits ; dec` in 0x00747DA0 is a hand-written
  rounding helper. It forces an ebp frame. Treat it as a lane gap.
- **Bitmask of bool tests:** for `(Has(1)?1:0) | (Has(2)?2:0) | ...`, VC7.1 reorders the operands of the `|` chain. Neither
  source order gives retail's 1-first call order (0x007BFA90).
- **A syntax error can hang the wine compile:** an unbalanced brace in a probe ran the grader into the 200-second
  timeout with no output. Check the brace balance before blaming wine.

Review at landing:
- Rows graded more than 4 times:
  - 0x007BE0F9 (near-miss, about 10 grades)
  - 0x007BD2D1 and 0x007BD527 (stand-in retries)
  - 0x007993F0 (8 grades; near-miss on one hidden-return slot)
  - 0x0079B6F0
- Local base models next to an existing engine header:
  - CTCTargetingPlayer (retail is 8 lower than engine/CTCTargetingPlayer.h from +0x80).
  - CTCShotDefinitionBase (packed, 4 lower than Ego_r.pdb from HitThingsList).
  - CTCTavernGame and CTCSpotLight use local models.
  - Every matched src/ file models `this` locally. Check them against rebuild/include/engine/ before landing.
- The catalog row 0x0072CF40 holds the function, its jump table and about 20 following functions (SKIP).
- The 0x0072BC60 catalog name ("CancelAllAbilities") was propagated from 0x0072BA40; the real name is CancelAbilitiesForCutscene.
- The 0x00625ED0 catalog name is "CTCQuickAccessMenu::vfunc_26"; it is GlobalUpdate(bool).
