# agentNF14 lessons (one line each)

- Model a def lookup that retail calls out of line as a `__declspec(noinline)` explicit specialisation of the const member template (`GetDef<T>` / `GetSubDef<T>`); declared-only specialisations bind the exact Ego_r symbol with no stand-in body.
- Prove which GetSubDef<T> an address is by the StatGetTypeName literal its body pushes (`work/str.py <addr>`), not by the caller's type.
- `CDefPointer<const CDefClassBase>` needs its own specialisation for the GetPDefFromGlobalIndex / GetPDefFromInstantiationName return (clearing or temporary-release dtor); a dtor-less one models hand-released out slots.
- GetPDefFromGlobalIndex has byte-identical QBE (0x009AD9E0) and IAE (0x009ADA10, protected non-const) overloads; check which one the caller binds before choosing const.
- `engine/CTCBase.h` under FABLE_RETAIL_CTCBASE needs `engine/ETCInterfaceType.h` included first.
- A retail header that includes CDefPointer.h without FABLE_RETAIL_DEF_CLASS_BASE (+ CCOUNTEDPOINTER / CINTELLIGENTPOINTER) hits its `#error`, and under Wine the compile hangs instead of failing: define them up front.
- The CTCVillage TU is /Os: a CTCVillage row that is one instruction long and DIFFERs under /O2 matches with `// Flags: /Os`.
- Pass a `const char*` to a `const CCharString&` parameter by implicit conversion; an explicit `CCharString(...)` temporary moves the construction and changes the bytes.
- A free `__fastcall` stub row that wraps GetWorld is usually a small member function; find it from the Ego_r donor order around its retail neighbours and from vtable slots (`work/vt.py`, `work/xref.py`).
- Retail reading `[component]+0x28` then `+0xBC` is PeekDisplayEngine().IsScreenFadeOut(), not a player record; check the shared headers' member at each offset before naming a stand-in.
- `[0x013B8A1C]` is CThing::PCurrentThingManager (CTCBase::PeekComponent reads its +0x1C); `+0x24` is the thing manager's World reference.
- A VC7.1 test must not use `long long` / `ULL`: typedef `__int64` under `_MSC_VER`.
- `<list>` in a VC7.1 /Od test can fail to compile (TCC_FAIL); hand-roll the container in the portable test.
- syn.sh (g++ -m32 syntax gate) reports MS forward-declared enums and missing 32-bit libc headers; grade those rows with FORCE=1.
