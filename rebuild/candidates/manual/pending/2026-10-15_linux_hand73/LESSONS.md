# Chunk 73 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Retail vector push_back inline element copy (the landed PushBack family, `InsertGrow(last, value, SScalarTag(), 1, true)`):
  N straight dword moves = the element has an explicit memberwise copy constructor; dword 0 moved, then both pointers
  re-based (`add edx,4 ; add eax,4`) and the rest moved = a pair-style `{long; POD}` record copied with
  `first(o.first), second(o.second)` where `second` must stay a plain POD (a memberwise copy constructor on it folds
  the offsets away).
- `add ecx,N` then dword moves through edx = a plain POD record assigned whole (`CoordAxis = axis`).
- In a constructor with no vtable store, a scalar stored LAST as an immediate after the member objects' zero stores
  is a body assignment (in the initialiser list it is stored first; 0x00CB5C70, both forms checked).
- Inline map Find: retail caches `end()` in a local right after LowerBound (`mov esi,[esi+4]`); calling `end()` at
  each use compares against memory and forks the return tails.
- CScriptThing pimpl forwarders: `if (!PImp_Data) return g_static; return PImp_Data->Get();`, the static copied through
  the out-of-line CCharString copy constructor 0x0099EC30 or an inline C3DVector memberwise copy; `CScriptThing()`
  0x008CFF40 and 0x004C7E50 (CThing's CScriptThing factory) are separate out-of-line callees.
- An engine header's opaque `CIntelligentPointer` member (e.g. `PThing`) reached through a local pointer view
  `(CIntelligentPointer_Retail*)PThing` is byte-identical to a member model, so the header can still model `this`.
- An inline `operator+` building a named local field by field computes x, y, z in that order; a three-float value
  constructor evaluates z first (0x00851E70).

Review at landing: 006a0c50, 00903710, 00905330 model their bases locally although engine headers exist (CBaseObject,
CCreatureActionBase, CScriptGameResourceObjectScriptedThingBase -- flat records that cannot serve as polymorphic bases
with out-of-line copy constructors); the comments name the header paths, which also silences the lander's
shared-header warning. Confirm the layouts match the headers' offsets before accepting.
Label corrections (23 propagated labels) are listed in NOTES.md.
