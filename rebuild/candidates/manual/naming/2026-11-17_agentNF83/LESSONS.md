# NF83 lessons

- gx.sh / g.sh change directory into the repo, so the row file must be an absolute path. A relative path fails as CPP_FAIL, which looks like a code error.
- Check the leaf string for a template COMDAT twice (`@2@` vs `@3@` back-references). One misspelt leaf graded the wrong function and cost a grade.
- The explicit `_Destroy` / `~_Rb_tree` instantiations are the clean way to make VC7.1 emit pair `??_G` and tree destructors as standalone COMDATs.
- CIntelligentPointer<T>'s T* constructor needs T complete and derived from CBaseObject. Rows that model T as a forward declaration cannot use it until T's header is included, so check that before choosing the shared template.
- The implicit `CIntelligentPointer::operator=` does not match retail; FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN (and _COPY) does.
- CCountedPointer::ReleaseData: the default form matches the rows here, and the RELEASEDATA opt-in changes the Data/Info store order.
- An ebx/ebp register swap at the same instruction count did not move across 4 source forms (0x009FE380). Stop at 3 forms.
- Run fcheck.sh before porting: it finds `_match.cpp` companions, such as 0x0059A238's.
- Respelling a local model's parameters in place (for example to `CArray<T*>&`) is often the whole fix for a ctor-chain binder, because the derived constructor's own leaf follows the parameter type.
