# NF88 lessons (one line each)
- A scaffolding reference to an inline body must be an ordinary function: an in-class (implicitly inline) static that nothing calls is never compiled, so no COMDAT is emitted (check the object size / symsel before trusting the grade).
- Take the address of the inline body rather than calling it from the scaffolding: a call can be expanded in the helper and emit nothing.
- A protected member can be referenced from an anonymous derived class (`&Derived::Member`), never instantiated; a free helper cannot name it.
- Run the control link first: the landed `_DEFINE` object plus an includer must show LNK2005 with `LINK=` cleared, or "no LNK2005" proves nothing.
- A "broken" PASS22 header chain may need only the missing prerequisite macro (here FABLE_RETAIL_DEF_CLASS_BASE); compile a stub first before writing new opt-ins.
- A binder recorded "binds no entry address" in an earlier slice must be re-probed when a later entry adds its callee (0x005BF2ED bound 0x005C08C0 under a stand-in).
- Two loads swapped by the scheduler (key vs end pointer) came right with the predicate as a named object built before the end pointer.
- Copy the Leaf from the object (or the PDB name set), never retype it: a `@3@` / `@4@` back-reference slip cost one grade.
