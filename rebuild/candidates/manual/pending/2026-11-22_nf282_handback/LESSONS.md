# NF282 lessons (2026-11-22)

- The generated CCountedPointer Reset / (T*) ctor rows (one free `?Destroy_<addr>@@YIXPAX@Z` each) are a one-line binder respell:
  include engine/CCountedPointer.h under FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY, forward-declare T, store
  `&CCountedPointer<T>::DeleteData`. 60 of 60 objects stayed byte- and relocation-identical apart from the one symbol name (objsec ALL_EQ),
  so the header include (its static asserts included) does not disturb these small TUs.
- The generated rows have no `// Leaf:` line; their objects also define the pick-any inline `??0SCountBlock@@QAE@PAX@Z`, so "largest
  function" fallbacks are fragile there. Pin the Leaf from the HEAD object's DEF list.
- cmpobj.py compares relocation symbol NAMES, so after a respell every function shows DIFF; use the rename map (tools/cmpobjm.py) and
  read objsec's ALL_EQ as the proof.
- An `@Scope` in a template argument mangles the same for a namespace and an enclosing class: decide which it is from rebuild/include
  (`namespace X` v `class X {`) before writing a forward declaration; never invent `namespace Outer` for a nested class.
- A tracker "self_leaf = PDB" can be a plain or prefix Leaf whose object defines another symbol (`?DeleteData@@YIXPAUAutoTinyVirtualObject@@@Z`);
  a link of binders + owners shows which targets really resolve (53 of 60 here, 0 at HEAD).
- An owner row can compile under VC7.1 and still fail the clang precheck (0x004ADC40: incomplete element type in an STLport model); such a
  row cannot be compiled under the brief's rule, so release its lock and rely on its Leaf.
