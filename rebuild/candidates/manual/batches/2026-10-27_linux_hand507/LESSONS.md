# Chunk 507 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A by-value CCharString argument built from a getter result (`f(Me.GetDataString())`) only takes retail's in-slot shape when the user copy ctor 0x0099EC30 is declared. Without it VC7.1 bitwise-copies through an extra temp (0x00D6BAC0: 195 -> 174 bytes).
- A ternary of two string literals feeding one by-value CCharString argument gives a select. Retail's two pushes into one call are an if/else of two whole calls, which VC7.1 tail-merges (0x00DCF000).
- `x * 2.0f` compiles to `fadd st,st`, not `fmul` (0x00D29320).
- STLport vector operator[] must be `*(begin() + n)`, not `_M_start[n]`. The latter changes register allocation (0x00D374A0).
- Quest-script Init is public in the PDBs (`?Init@<Q>@NScript@@UAEXXZ`). A protected/private Init mangles `MAE`/`EAE`, and the Leaf then names no symbol (0x00CFADD0, 0x00D2A8A0). Entity-script virtuals are private (`EAE`).
- CBaseDispatch::GetTypeInfo inlined into GetIDsOfNames: the static-local scope number (`?N@` vs `?O@`) depends on brace scopes before it. The first CheckPointer `if` must be braceless (0x00CACE70).
- strmbase CBaseAllocator has two ctors (TCHAR = WCHAR, and CHAR). They differ only in which CUnknown overload they call (0xCACB80 PBG vs 0xCACC80 PBD). Pick the Leaf by that callee (0x00CABAE0 / 0x00CAC570).

Review at landing:
- Landed `CUnknown_00cacc80.cpp` line 1 says WCHAR, but the bindings (and the landed CBasePin comment) treat 0x00CACC80 as the char overload `??0CUnknown@@QAE@PBDPAUIUnknown@@@Z`. Check and fix the landed comment.
- 0x00704580 is labelled "CCountedPointer_Set_00704580". It is `CCountedPointer<CScriptThing>::operator=` (vland RELOC_NAME on 0x00D374A0).
- The vland SHARED_HEADER_AVAILABLE CBaseObject warnings on the 4 strmbase rows are a name collision with Fable's CBaseObject. Keep the local strmbase model.
- 0x00DCDED0's class CGuardTeamMember is not in either PDB. The name is probable, and its members borrow CCrateTeamMember names.
- No row needed more than 4 grades (max 3: 0x00DCF000).
