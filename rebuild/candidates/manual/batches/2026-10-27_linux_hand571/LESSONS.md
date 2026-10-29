# Chunk 571 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **A local STLport vector looks aliased when its out-of-line `operator=` has no body (0x00BDBB70).**
  - Symptom: a local vector passed to an out-of-line `vector::operator=` setter is treated as aliased. You get extra zero
    stores and member reloads after malloc (952v909).
  - Fix: give `vector::operator=` a same-TU `noinline` body (as for `__uninitialized_copy`). VC7.1 then sees the
    callee does not keep the local's address.
  - `/Oa`, the QFE compiler and `__forceinline` did not help.
- **An implicit `operator=` can be emitted out of line where retail inlines it (0x00ECE460).**
  - The implicit `CScriptGameResourceObjectScriptedThingBase::operator=` (a CBaseObject base plus a PImp member) was
    emitted out of line: 777v908.
  - Fix: write it as `__forceinline` (base `operator=` + `PImp = other.PImp`).
- **Inline `CCharString::operator!=(const char*)` is `_stricmp(GetAsCharArray(), s) != 0`.**
  - The call goes to the IAT thunk 0x00BFEBA8 (MSVCR71 `_stricmp`).
  - GetAsCharArray is `PStringData == 0 ? "" : PStringData->PData`.
- **Spawned-function blocks: write the real template `CSpawnedFunc1<TClass, TPar1>` (0x00F08E90).**
  - Members: Func +0x34, PClass +0x38, Par1 +0x3c.
  - The ctor must be `__forceinline`; plain inline leaves a call.
  - `~CScriptThing` must be declared out of line (0x004AA840). The forceinlined ctor otherwise expands the by-value
    parameter's destructor inline, where retail calls 0x004AA840.
  - CSpawnedFuncBase is exactly 0x34 bytes including its vptr. One extra dword shows up as `push 0x4c` instead of
    `push 0x48`.
- **A C3DVector passed by value needs the memberwise copy ctor (0x00F08E90).**
  - Write `C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }` to get retail's `ld/st, ld/st, ld/st` interleave.
  - The implicit trivial copy schedules both loads before the stores.
- **Entity-script `Main` / `Init` are `private virtual` (EAE) (0x00EE3B20, 0x00F08E90).** Declare them
  `private: virtual void Main();` / `virtual void Init();`. A non-virtual declaration gives a leaf mismatch: q.py warns
  "matches no symbol".
- **A CScriptThing local's destructor can be partly inline without any pragma (0x00EE3B20).** With a trivially-bodied
  inline `~CScriptThing() { }`, VC7.1 by itself calls 0x004AA840 at the shared exits and expands it inline at one
  return. That return is the one inside the second iteration of a rotated wait loop. Try the plain form before adding
  `inline_depth(0)`.

Review at landing:

- **Landed CAT_Sentry::Main 0x00F045E0 has wrong member names.**
  - It names CAT_Sentry +0x1c `Attacked`; Ego_r.pdb calls it `SentrySeenSpy`.
  - It names pGlobalScriptDef +0x9F4 `ArenaSentryScareDistance` ("PDB name not resolved"). By the def's 0xA0 shift
    this is Ego_r 0xA94 `AmbushTradersSpySentryProximity`.
  - Its Leaf `?Main@CAT_Sentry@@` lacks the `CQ_AmbushTradersScript@NScript@@` scope.
- **Landed CAT_Phase1Guard::Init 0x00F083A0 uses invented types.**
  - Types: CScriptThingRuntime / CScriptThingStored, CSpawnedFuncEnemyDeath, CSpawnedFuncBaseRuntime, and
    `CQ_AmbushTradersScript::Add` for CScriptBase::AddSpawnedFunction.
  - 0x00F08E90 shows the same block matches with the real `CSpawnedFunc1` template, `CSpawnedFuncBase` and
    `CScriptBase::AddSpawnedFunction`. NAME_FIXES.tsv carries the symbol side.
- **NAME_FIXES.tsv rows for fleet-wide helpers may hit ICF aliases.** Rows such as CCharString ctor/dtor/operator=,
  CScriptThing copy/dtor, CBaseClass and CBaseObject list many wrong spellings. Check none is an /OPT:ICF-folded alias
  before respelling.
- **Header debt is listed in NOTES.md:**
  - flattened CVertexShaderConstantLayout / CEngineVSConstantLayoutBasic / CSpawnedFuncBase headers;
  - the pGlobalScriptDef type, probably `CDefPointer<CScriptDef const>` but unproven.
- **No row needed more than 4 grades.**
