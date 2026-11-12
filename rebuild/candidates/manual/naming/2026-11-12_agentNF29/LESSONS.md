# Lessons (agentNF29)
- vt.py checks only `<addr>_TEST PASS`; rerun a test whose catalog PassPattern differs (OK_..., PARITY_OK) through behaviour_of with that pattern before calling it a failure.
- Overloaded virtuals: MSVC lays them out in reverse declaration order, so a local model naming slots 134/135 both GetState declares the slot-135 overload first (as the shared header does), and a call from a non-const method picks the non-const overload.
- A protected callee reached from an unrelated class (CWorldMap -> CMap::CopyPos) needs a `friend class` in the local model to mangle IAE.
- A non-template stand-in body for a template instance becomes `template<> ... IS_THING_ALIVE<T*>(...)` after declaring the primary template; overload resolution still prefers the exact non-template overloads beside it.
- engine/CD3DFormatInfo.h pulls in Direct3D9Types.h: drop a row's local _D3DFORMAT / _D3DDISPLAYMODE copies (same field types) and put the include where the enum was; when the field types differ (_D3DPRESENT_PARAMETERS_), stop.
- A grade that times out (empty output) is usually an #error/hang from a missing opt-in prerequisite; prefer a declaration-only overlay opt-in over pulling a member-layout opt-in into a tiny row.
- Moving a global stand-in struct into its real namespace makes it a local model of the header class; when the header lacks the needed member or opt-in, leave the row as a gap rather than adding that model.
