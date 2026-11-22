# NF233 lessons (one line each)
- Check every PDB for the vtable names before reasoning about VC's naming: all three PDBs carry ??_7<Class>@@6B<Base>@@@ publics verbatim.
- A header class without a user ctor declaration is silently POD: CStopWatch's out-of-line ctor never got called until the header declared it.
- `m_member()` value-initialisation of a float POD gives `stosd`; retail's `fldz ; fstp` triple means an inline user ctor (Ego_d / FableWin record ??0 for it, often under a nested scope).
- ??_G scalar deleting destructors take the optimize mode of the point where the vtable is emitted (the ctor / dtor), not the end of the TU; other COMDATs take the end-of-TU mode.
- An inline-expanded CCountedPointer release whose Info store precedes Data is Reset(0) over the Data-first ReleaseData (dead-store elimination), not ReleaseData itself.
- Unwind funclet rows include `../src/compiled/...` relative to the include tree: a copied tree for equiv_check needs a `src` link beside it.
- clean.py refuses a dir directly under the agent folder (wd1): remove it with a literal guarded path.
