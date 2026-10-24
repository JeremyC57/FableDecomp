# Chunk 331 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- `mov ecx,[esi]` (the callee object loaded from a member) BEFORE the argument `lea/push` = the object pointer is a named local: `CDefinitionManager* manager = context->PDefinitionManager; manager->Get(name);` -- the direct `context->PDefinitionManager->Get(name)` pushes first and loads ecx last (00a3e0d0, 00a3e180).
- A loop-carried value read through an inline `Read(&local)` that retail keeps in a register, preloaded from its home slot before the loop (`mov ebx,[esp+0Ch]` with no visible store), comes from an inline by-value `long ReadLong() { long v; Read(&v, 4); return v; }` -- the unset-local path makes VC7.1 cache the slot (006778a0).
- Two parsed longs homed in two dead argument slots (parser, then context) need an inline by-value helper `long TransferInLong(ctx, parser) { long v; TransferIn(ctx, parser, &v); return v; }`; two named locals in the body give the second one a fresh frame slot (`push ecx`) (00755f40).
- A result copied out of a callee's by-value return through a GPR plus `fld/fstp` (`fld [esp] ; ... ; fstp [eax] ; mov [eax+4],ecx`) is `T local = Base::Get(); return local;` with a user copy constructor (0055c5d0); the same mixed float copy comes from `return C2DVector(x, y);` over two ReadFloat() locals (00995030).
- CCharString inline accessors decide arm order: `if (!PStringData) return 0; return PStringData->Length;` reuses the zero pointer for the empty length and lays the empty arm first; the `?:` form inverts both arms (006ba030). `operator==("")` written with nested ifs keeps retail's `je/jmp` arms; `return strcmp(...) == 0` gives `sete` (00a3e180 vs 00755f40 -- both shapes exist in retail).
- A by-value function result used as an assignment source whose address retail re-forms with `lea` (not `push eax`) goes through an inline wrapper: `CCharString CDefString::GetString() const { return g_DefStringTable.GetString(TablePos); }` (00a3e180).
- POD-vector persist with `push 0Eh ; pop ecx ; cdq ; idiv` = a packed 14-byte element (CEngineGraphic is 0x0E in the PDB: `#pragma pack(1)` reveals, not hides, the layout); in a size TU the vector's `size()`/`data()` must be `__forceinline` or data() stays out of line (004ea26b).
- An empty-traits call on a `this` homed at `[ebp+0Bh]` (1-byte temp) vs retail `[ebp+0Ch]` stays open (0046c3b9); a 4-byte traits temp lands in the first dead slot instead.
- `#pragma optimize("", off)` list family at 0x009F79xx also holds end() (`ret 4`, hidden iterator) and back() (named head local, iterator local, `--`, inline `m_pNode->m_value`) (009f7970/009f79a0/009f7c20).
- A destructor whose only body is inline Clear() calls on a member array, followed by `eh vector destructor iterator`, needs `/EHsc`; without it VC7.1 unrolls the element dtor calls in a loop (00c75440).

Review at landing:
- No row needed more than 4 grades to match; 0046c3b9 used 5 compiles (2 sources + 3 probes) before parking.
- Local models next to existing engine headers: 006c52d0/006c5310 model CThing (only UniqueID +0x18) and CBaseIntelligentPointer locally (no engine header for the pointer; engine/CThing.h absent); 00bf5a30 derives a method carrier from the data-only engine/CWaterPatchMesh.h.
- 00995030's class is a CDataInputStream method carrier (`CDataInputStreamReader`), module CDataInputStream on line 1.
- The landed CPersistContext_TransferHelper 0x004E9550 states CObjectAugmentationParticleSet is 0x2C bytes in retail; 004e959d confirms 12-byte vectors at +0x14/+0x20.
- 00bf5a30 has no direct callers (an out-of-line copy of code inlined in LoadForegroundPatch); its name CWaterPatchMesh::CreateFromStream is descriptive.
