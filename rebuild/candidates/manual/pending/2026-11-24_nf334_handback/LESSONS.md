# NF334 lessons

- An anonymous-namespace symbol's `?A0x........` hash depends on the compile path. The table spelling (?A0x116aaba5) is not what a HEAD
  object compiled here defines (?A0x7d949760). Map the HEAD object's own symbol in the proof, not the table spelling.
- Re-scoping a type that is a template argument renames every non-inlined instance of that template in the TU, e.g. the COMDAT for
  CCountedPointer<T>::operator->. The grade still matches, but the COMDAT set changes. Read objsec before calling a re-scope T1.
- T3_PORT_QUEUE lines name binders in their prose column, not only in the target column. Grep the queue for the binder address before
  you edit (00757760 sits under nf308 for target 0x007573C0).
- lock.sh takes one address per call; its third argument is the mode. `lock.sh NF334 a b` locks only a.
- Zero line delta for a nested type: put `struct Outer::Inner; ` inside the class on an existing member line, define it as
  `}; struct Outer::Inner { ... };` on the class's closing line, and comment out the old global lines. A member function body finds the
  nested name before the global one.
- Before you re-scope a local model, list every symbol in the TU that names the type: own definitions, made-up globals, other callees'
  parameter types and template COMDATs. One of those moving onto another wrong form makes the change a port (43, 59, 72, 93, 108).
- Check the two PDB sources for the exact symbol, not only for the class. 0x00ABEF60's ctor is in FableWin only; Ego_r has the nested
  type through its `??_E` symbol.
- Before you write `replace old -> new`, test whether `new<TAB>target` is already in reloc_bindings.tsv. If it is, write `delete old`.
  apply_table.py --dry does not catch the duplicate.
- The table keeps one witness per (symbol, target) pair, so a binder in the same wrong form can be invisible there (007266c0 for 0x00723FD0).
  Compile every reachable exe-census binder at HEAD and read it with bindaddr before calling an entry done.
- The NF299 Q4 header clause can be scripted: grep each binder's engine/*.h includes for the PDB member. One hit in three was fixable in place.
