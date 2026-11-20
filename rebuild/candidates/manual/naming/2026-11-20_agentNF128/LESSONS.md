# NF128 lessons (one line each)
- A "blocked: port" row that only needed a define set another landed row already uses (0x008D8CE0 with 0x00665860's FABLE_RETAIL_CTHING_ORIGINAL_VTABLE) can match first time: grep for a landed row that already uses the virtual or member before calling the class unportable.
- Map a stand-in helper to its PDB name by its signature shape: GetAnimSet / world seed / CAnimBank were GetAnimFilter / GetWorldSeed / NCreatureMode::GetRandomAnimationFromCreature; the self rows of the helpers already carried the PDB leaves.
- An inline member expanded with ONE null test for both length and text (retail 0x008E9A30) wants a named CCharString local, not a temporary bound to a const& (the temporary reuses the ctor's returned `this` and keeps two tests).
- A temporary whose derived class has no user destructor is destroyed through the base dtor: use the header's IMPLICIT_DTOR opt-in instead of creating a new ??1Derived spelling.
- Grep both PDB name sets by the signature suffix (`@CThingCreatureBase@@QBEMXZ`) to find real names for descriptive callee stand-ins (GetDefHover*Speed).
- Compile every other REFS row of a family target once (bindchk.sh) before handing back a self leaf change; it turns "waits for all" into a proof.
