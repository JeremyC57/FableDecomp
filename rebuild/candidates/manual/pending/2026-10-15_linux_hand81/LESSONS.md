# Chunk 81 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **A no-argument accessor called while ecx still holds `this` is a thiscall member that ignores `this`.**
  In 0x0078BFC0 the accessor 0x00686A80 (game singleton + 0x24) is called with ecx = `this`, and the
  index is loaded into edx. Modelled as a global `__cdecl` function, ecx becomes free and the lane
  loads the index into ecx (`mov ecx,[ecx+44h]`). A member declaration (`CWorld* GetWorld();` on the
  component) reproduces retail.
- **A member store that follows inline member constructors in a constructor is a body statement.**
  In the CTCBetting constructor (0x007E57C0), `Active = false` is stored after the two inline vector
  constructors. Written as an initialiser it is stored before them.
- **A first word written twice in a constructor (8 at the start, 10 at the end) is an inline base
  constructor followed by the derived body.** See 0x007554C0. A base constructor that calls `malloc`
  is kept out of line by VC7.1, so it must be `__forceinline`. The grade also needs the derived
  constructor as the leaf (`// Leaf: ??0Derived`), because the out-of-line base is the larger symbol.
- **A destructor's member destructor running before a later member store means the temporary was in
  an inner block.** In the SetPTarget family (0x007A4350 / 0x007A7150 / 0x007A9AB0), the
  CIntelligentPointer temporary's destructor runs before the trailing byte store. That happens only
  when the temporary lives in an inner `{ }` block and the store follows the block.
- **A `const&` key that retail copies into a stack slot needs a real conversion.**
  - A qualification conversion binds directly with no copy: `CThing*` to `const CThing* const&`
    passes `lea edx,[ebp+8]`.
  - A derived-to-base pointer conversion (`CThingCreatureBase*` to `CThing* const&`) creates the
    temporary, and so does an inline accessor that returns the pointer by value.
  - VC then homes the temporary in the caller's dead argument slot. A discarded hidden-result
    iterator of a later call reuses the same slot (`lea eax,[ebp+8]`).
  - Seen in 0x0064639C, and in 0x007E9360, where the key lands in the dead bool slot.
- **A member pointer loaded above `sub esp,N` / the first push is a named local read first.** In
  0x00783C20 / 0x00783C50 / 0x00783C80, the `mov eax,[ecx+4]` sits above the frame set-up. Reading
  the member into a local first also fixes the eax/ecx choice for a later argument reload.
- **The `if` / early-return form decides which arm comes first.** For
  `if (locked) { flag = 1; } else { work; flag = 0; }` with the work arm first in retail, write the
  negated test with the work in the `if` arm (0x007AA480). The early-return form places the short arm
  first.
- **A plain record deleted through a helper gives a `??_G` that the lander can pick.** A record whose
  members include a list with an inline `clear(); free(node)` destructor, deleted through
  `void Kill(T* p) { delete p; }`, gives the retail non-virtual `??_G` (0x00A833B0, `// Leaf: ??_G`).
- **Lane gap (3 rows: 0x007E93C0, 0x007E9410, 0x007E9360).** Two 4-byte iterators with user copy
  constructors are passed by value from a pair local. Retail loads `first` into edx BEFORE reserving
  `last`'s slot. The lane loads it after copying `last`, or, with inline forwarders, hoists both
  loads and addresses the slots with `mov ecx,esp`.
- **Lane gap (0x0064639C).** A conversion temporary homed in the dead argument slot is stored two
  instructions earlier than in retail.

Review at landing:
- **Local base models next to existing engine headers.**
  - `CTCBase_Calls` (engine/CTCBase.h) appears in 9 sources (plus the CTCBase-shaped bases inside the 3 parked differ/ rows), because the header's vptr is a plain
    word and these need virtual calls, constructors or destructors.
  - `CTCBerserk_Retail` (engine/CTCBerserk.h): retail PDef is at +0x68 and IsInCutscene at +0x72,
    where the header has +0x6c / +0x76 (DependentThings is a 12-byte vector in retail).
  - `CTCBoastUI_Retail` (engine/CTCBoastUI.h): retail Active is at +0x18 and the flags at
    +0x28..+0x2b, where the header has +0x1c / +0x30..+0x33 (12-byte map and vector in retail).
- **Already-landed sources to check.**
  - `rebuild/src/compiled/00/65/unattributed_CVector00655cdc_00655cdc.cpp` models 0x00655CDC as a
    thiscall vector range constructor. Its callers (0x0064639C, 0x0064A2DB) pass a hidden result in
    ecx and the value reference in edx, so it is the SGI `find(first, last, value)` (__fastcall,
    forwarding to `__find` @ 0x0065575F with an input-iterator tag). The bytes are identical; the
    identity and name are wrong.
  - `CTCHeroCentre_OnHeroWalkedThroughDoor_0077db90.cpp` declares 0x00686A80 as a `__cdecl`
    `GFGetHeroCreature()`. 0x0078BFC0 uses the same accessor as the world: its result feeds
    `CWorld::GetPRandomSeed`, and only the thiscall-member form matches there. Reconcile the name and
    the convention.
- **No row needed more than 4 grades.** 0x007E93C0 used exactly 4; its twins were parked on the same
  evidence.
- **00656bd8's parameter types are a guess.** It is the second CQuestInfoElementCounter constructor,
  and its parameters are given `unsigned long` only so the two overloads differ. Both sources declare
  one constructor each and grade independently.
- **No catalog row holds several functions.**
