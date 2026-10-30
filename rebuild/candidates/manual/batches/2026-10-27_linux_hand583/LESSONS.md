# Chunk 583 lessons

- A by-value C3DVector argument copied from a const reference needs a memberwise user copy ctor (`X(v.X), Y(v.Y), Z(v.Z)`): the implicit copy loads Z before storing Y (0x00ED7E00).
- Model script members that are CScriptThing / CTimer with the real types, not byte arrays: the arrays lose 4-alignment and shift every later member by 4 (0x00F14300).
- Shared CScriptThing.h plus an inline-everywhere copy ctor matches rows whose copies are all by-value inline (0x00ED7E00). Rows that need a mixed inline / out-of-line copy still need the local tag model of the landed 0x00F05F30 (0x00D2D1A0).
- A CLevelScriptBase OnPersist with an enum member uses the two-argument `Transfer(name, value)` over `static __fastcall GetDefaultVal<T>()` (0x00E1DCF0); float defaults are written `0.0f`.
- Lane gap (parked twice): VC7.1 picks inline vs out-of-line destructor expansion per exit site.
  - Retail sometimes inlines a local's destructor at one return and calls it at the others: ~ScriptedThingBase in 0x00D2D1A0; std::set in 0x00EE91E0, which has three forms in one function.
  - None of these change the lane's choice: explicit or `__forceinline` dtors, `inline_depth`, the real STLport allocator chain, visible callee bodies, the QFE 4035 toolset, /GL+/LTCG.
  - Park on sight.
- A set<unsigned long> local's ctor is the out-of-line 0x00A394D0 even though STLport defines it in-class. Declare it without a body; a visible body inlines it.

Review at landing:
- Local CSpawnedFunc<T> over the shared CSpawnedFuncBase (00ee47b0, 00ebef80). CSpawnedFunc<T> has retail per-instantiation vtables, and no shared header exists yet.
- Local CPersistContext (00edaf30, 00e1dcf0), and local opaque CScriptDef / CQ_SunnyvaleMasterData data models.
- Parked 00d2d1a0: a local CScriptThing stand-in (pre-defined FABLE_ENGINE_CSCRIPTTHING_H guard plus a tag copy ctor), and local polymorphic CBaseObject / CScriptGameResourceObject(ScriptedThing)Base / CSpawnedFunc1 models.
- Parked 00ee91e0: local polymorphic CBaseObject / CScriptGameResourceObjectBase / CScriptGameResourceObjectScriptedThingBase, and a local STLport set / _Rb_tree.
- NAME_FIXES.tsv covers 12 addresses:
  - Notable: 0x0099F830 is GFIntToCharString. The landed bindings PrisonerNumberString and CCharString::FromLong are wrong.
  - Notable: 0x004CFF70 is landed as CTCCarryable::OnKill, but it is the set<unsigned long> tree's clear.
  - 0x004D0740 / 0x004D0E20 are ~set and ~_Rb_tree, with the order unproven.
