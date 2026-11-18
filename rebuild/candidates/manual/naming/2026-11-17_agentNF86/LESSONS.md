# NF86 lessons (one line each)
- A `??_G` or inline ctor that must be pick-any (selection 2) can be emitted by a hashed anonymous-namespace function that references it; this is not retail, so flag it for a coordinator ruling.
- A `??_G` whose constructor is a separately landed strong row cannot be emitted alone (a second ctor definition gives LNK2005); block it with that ctor row.
- VC7.1 emits `??_GClass@@QAEPAXI@Z` (QAE) for a non-virtual-dtor class when a template deletes it (CCountedPointer<T>::DeleteData): the same TU explains the address.
- Check a stand-in's caller first: 0x0053CEF6 already bound the STLport `__uninitialized_fill_n` spelling, so the self rename was one explicit instantiation.
- Probe the old row (probe.sh) BEFORE grading the new form: both write the same pS object.
- vl.sh `cd`s into the repo, so give its log as an absolute path; end every Bash call with `; true`.
- Token-identity samples can miss nested opt-ins: also preprocess includers that define the enclosing branch macro (WIFEUPDATE here).
- An iterator typedef that is protected in STLport (C2247) can be spelt as `std::_Deque_iterator<T, _Const_traits<T> >` directly in an explicit instantiation.
