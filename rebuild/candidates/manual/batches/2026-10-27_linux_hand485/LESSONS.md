# Chunk 485 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A switch whose cases each `return false` on a mismatch and otherwise fall to `return true` takes its merged tail from
  the source case order. Write the cases in retail's compare order (0x00405DB0: 3, 2, 1, each as
  `if (a != b) return false; break;`). `return a == b` per case emits sbb/setcc code instead.
- A packed, unaligned function address is a /Os TU. The /O2 build gives away the wrong flags through register choice
  (edx instead of ecx), integer zero stores for float members, or `cmp mem,imm`; one `#pragma optimize("s", on)` fixed
  0x0041CD13, 0x00428C38 and 0x0046A130.
- An out-of-line implicit destructor of a non-virtual class (no vtable store) comes from the deleter that uses it.
  Instantiating `CCountedPointer<T>::DeleteData` (`delete (T*)data`) emits `??1T` byte-exact (0x00441B90, 0x00442190).
  A free `void f(T* p) { delete p; }` emits the non-virtual `??_GT@@QAEPAXI@Z` (0x00442E60).
- Gap-scan COMDATs next to an inlining user are often the out-of-line STLport pieces of the same container:
  _Vector_base(n, a), _M_allocate_and_copy, __copy_backward with its iterator tag. The tag is homed in a dead argument
  slot (`lea ecx,[esp+N]` of n's slot) and the `ret` count includes the tag and distance pointers.
- `Catch@<addr>` rows that use `[ebp-..]` and end `mov eax,<continuation> ; ret` are EH catch funclets. Skip them: they
  only exist as the catch block of their parent.
- FableWin.pdb's name set has members Ego_r.pdb lacks (`CWorld::SetStartTime`, `GetGameStartTime`). Check both sets
  before calling a setter descriptive.
- To add a method to a class that has a generated header, use
  `#define C DonorC / #include "engine/C.h" / #undef C / class C : public DonorC { ... };`. The method still mangles as
  `C::f` and the verifier's SHARED_HEADER warning goes away (0x0047D478, 0x0049AE70, 0x0049CA20, 0x0049D8E0).
- In a test, `long` is 8 bytes under 64-bit g++. Use `int` for 4-byte fields when the test asserts a sizeof
  (0x0040DB10 test).

Review at landing:
- `$S/w22/calls.py`, named in the brief, does not exist. Call targets were checked with q.py relocations and vland
  (RELOC_NAME warnings only, all of them wrong callee labels listed in NOTES.md).
- 0x0047D478 is descriptive. The unassigned PDB members `CTCInGameMenu::OpenInGameMenu` / `ActivateBackOption` have
  the same signature, and a caller-side search may pin one of them.
- Leaf `?Callback_malloc@CIME@NUISystem@@SIPAXI@Z` (0x00404A60) differs from the existing stand-in binding
  `?Callback_malloc@@YIPAXI@Z`. The same applies to the three Def Alloc rows and the two NPlayerGui destructors:
  rebind them at landing (one address = one name).
- Leaf `?SetStartTime@CWorld@@QAEXN@Z` comes from FableWin.pdb only. Confirm that retail has no
  HasInitialisedStartTime write elsewhere for this setter.
