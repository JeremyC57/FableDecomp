# Batch `2026-10-14_linux_hand18` — hand-authored CChunkedFileSuperChunk .. CCombatAbilityUseProjectileWeaponDef functions: 40 bodies (38 landed)

Delegated hand-authoring pass (compiler in the loop, Wine lane) over the nineteenth chunk of
class-labelled open functions of 17–96 bytes: `CClass` / `CClassFactory` registration bodies
(propagated labels corrected from the callers), `CClickable` / `CHoverable` event handlers, a
`CCombatManager` fighting-thing query over an STLport `map<UID, CCombatantInfo>`, no-RTTI script
module bodies (`CVtable_<addr>::Main`, a `WaitFor` pump), the networking URL holder's by-value
host / path getters, def-class constructors (`/Os` TUs: `or dword ptr,-1` for `CDefString`,
`fldz ; fstp` for a 0.0f member), the `CEngineSceneMap` constructor, `N3DPrimitive` vertex-shader
input constructors and a `CScriptScreenText` copy constructor. Engine headers are modelled as
`<Class>_Retail` / `<Class>_Calls` where they disagree with the retail layout (`CHoverable` 0x16c,
`CClickable` 0x194, `CDraggable` 0x1ac, `CClassFactory` with a 12-byte vector at +4).

Lessons recorded by the pass: VC7.1 has only the unnamed return-value optimisation (a named
local is copied out through the copy constructor); scalars assigned in a constructor body are
stored after the init-list member constructors in body order; a virtual call sequences before
`this`-side address arithmetic only through a named local; four byte channels loaded before any
store are an inline `Set(r,g,b,a)` (`CRGBColour` is D3DCOLOR order); `arg == node->field` loads the
argument and compares with memory; `while (!IsFinished()) Pump();` is the rotated test plus
do/while. Two bodies differ by block order (`CCombatAbility::IsValid`) and a by-value iterator
tag's frame slot (`RemoveAllSubChunks`).

All landed bodies graded `MATCH` / `RELOCATION_MATCH` + `PASS` on 2026-10-14.
