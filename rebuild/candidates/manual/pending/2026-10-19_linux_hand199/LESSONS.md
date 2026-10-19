# Chunk 199 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A GetSubDef out-slot that the caller copies into a named raw pointer right after the call and releases by hand at the
  end (`test edi,edi ; dec [edi+4] ; call [edx+4]`) is a plain slot struct with NO destructor plus a manual release; a
  CDefPointer with a destructor re-reads the slot from the frame after every call (0x008B9F20, 0x008B9E50, 0x008EC320).
- An implicitly converted `const char*` argument (`def->GetEffectFromString("TELEPORT_IN", type)`,
  `thing->AddTC("CTCHaste", true, 0)`) builds the CCharString temporary BEFORE any argument is pushed; an explicit
  `CCharString("...")` argument is built after the later arguments are pushed (0x008B9F20, 0x006A02B0).
- `PCreature.GetPItem()->Method(constants...)` pushes the constants before GetPItem; a named `CThing* x = PCreature.GetPItem();`
  restores retail's "GetPItem first" order -- also for the second call in the same body (0x008EC320).
- Two `CIntelligentPointer` arguments: the one passed through an inline `operator CThing*()` is fetched before the one passed
  as a plain `GetPItem()` (0x00845270, CEventShownItem(creature, item)).
- Clearing an array element with two separate zero registers (`xor edx,edx ; mov [..],edx ; xor esi,esi ; mov [..+4],esi`) and
  `inc [size]` is a zeroed local struct copied into the element at an index read once (0x00B8D6C0).
- A 2D vector in an FPU expression wants a C3DVector local with only X/Y assigned (frame 0x10 = string + 12-byte vector), and
  `lengthSq > limit * limit` evaluates the length first: `test ah,5 ; jp` with r^2 in st0 (0x008B8BC0).
- A head-point copy whose X/Y are forwarded to the FPU but whose Z goes through an integer register needs a user
  C3DVector copy constructor (0x00BCA6B0).
- The STLport tree in retail is 12 bytes (header pointer, count, a dword for the empty comparator): a map member modelled as
  8 bytes shifts everything after it by 4 (0x005954AD).
- DirectShow base classes are linked into the exe: a 0x48-byte local with `bFixedSizeSamples`/`lSampleSize` = 1 and pUnk/
  pbFormat at +0x3C/+0x44 is CMediaType; such bodies need `/EHsc /GS` (0x00CAC4A0).
- In the HUD size TU a small pair/ctor used inline needs `__forceinline` (plain inline stays out of line) (0x0064E0DA).
- Register swaps of `this` against the first member pointer (esi/edi, ebx/ebp) survive named locals, inline helpers and
  pragmas in nine rows of this chunk; treat them as a lane gap after two tries.

Review at landing:
- Name conflicts in the landed tree: 0x00669D30 (`CCRC::CalcAndLookup` vs `NativeEffects::GetEffectFromString`; this chunk uses
  `CSpecialEffectsDef::GetEffectFromString(...) const`, the PDB class), 0x00CB1CC0 (landed `CComHolder_00cb1cc0::Release`,
  really ~CMediaType), 0x00449970 (`GetMainPlayer` 34x vs `GetPlayer` 17x), 0x00487DD0 (`GetControlledCreature` vs
  `GetControlledThing`), 0x004292C0 (`TreeNext_004292c0` vs `Successor`), 0x00833A10 (five SetSuspendAnimation spellings).
- IS_THING_ALIVE at 0x0043DBF0 is a different instantiation from 0x0043DBD0 (<CThing*>); the parked rows call it
  IS_THING_ALIVE<CThingPhysical*> (T is a guess, the manifest's <CThingBuilding*> is a byte-twin label).
- The landed CObserver deleting destructor (0x0052E2F0) destroys its +4 member through 0x0052E1E0, while ~CFrontEndManager's
  inlined CObserver base tail-jumps to 0x0052E050 for the same member: one of the two member types is mislabelled.
- 0x00BDD2F0: the lane folds a signed `/16` of a zero-extended ushort into `shr`; retail kept cdq/and/add/sar -- check against
  the QFE toolset before calling it a source form.
- No row needed more than 4 grades on its src/ file; probes were used for side experiments (pragma sweeps, variants).
- Local base models next to an existing engine header: TeleportIn/Out, TrollThrowProjectile, UnsheatheWeapons (the PDB
  headers put the members 0x18 higher than retail), CDataOutputStream (header lacks the vtable), CEngineLightingManager.
