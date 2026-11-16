# NF37 lessons (one line each)
- Most "port candidate" self rows are 2-10 line stand-ins whose header already ships the opt-in naming that address: they move in 1-2 grades.
- Copy the include chain from the smallest landed row that defines the same opt-in (bin/pre.sh); guessing the prerequisites costs a grade each.
- A generated (flattened) header layout often lacks a method the retail class has under an opt-in; add the same declaration under the same macro in the overlay.
- A derived-class stand-in method (`bool IsEventGameAction(long, ...)` on CInputProcessX) binds the wrong class: drop it and let the call resolve to the base declaration.
- Per-T deleters stored by DIR32 (CCountedPointer<T>::DeleteData) are respelled by naming `CCountedPointer<T>::DeleteData` from engine/CCountedPointer.h; a forward declaration of T is enough when the row models T locally.
- A "global" that is really a float literal (`__real@41000000`) is written as the literal; the compiler emits the pool symbol itself.
- Rows that model CCharString / CCountedPointer / FileAccessType locally cannot include the header that declares the claim; check for local models before trying a header include.
- Insert new #include lines before the first non-comment line, not at the first "#include" substring (it can sit in a comment or after the use).
- Pass absolute paths to helpers that cd into the repo (a relative body file silently became empty).
