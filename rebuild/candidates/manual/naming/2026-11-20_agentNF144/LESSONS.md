# NF144 lessons (one line each)
- A mode-pointer binder moves onto CTCCreatureModeManager.h in one form when its mode class has no header: re-base the local mode on the nested CModeBase, declare GetType() (pure in the base) and resize the pad; the mode-ctor spelling stays, so no new spelling appears anywhere.
- Before re-basing, grep `(class|struct) <Mode>\b` across rebuild/include: a forward declaration (CCreatureAction_JackScream.h) is not a header; a definition (NCreatureMode_CModeBriarRoseConcentrate.h) means a full port.
- The shared CTCCreatureModeManager.h pulls CTCBase.h / CCountedPointer.h: rows with an empty local `enum ETCInterfaceType { };` need engine/ETCInterfaceType.h, and rows with a local CCountedPointer (template or not) need the shared one first (C2953 / C2990).
- Script the transform (xmode2.py) and grade the batch with a per-row HEAD-vs-new defined-symbol diff: 11 of 12 matched first time; the miss was a row that models CCountedPointer<T> itself.
- An inline-budget residue whose callee is a header `inline` (not noinline) is a /d2inl sweep: S60 changed nothing, S150 / T200 over-expanded (874v619); bisect between the two.
