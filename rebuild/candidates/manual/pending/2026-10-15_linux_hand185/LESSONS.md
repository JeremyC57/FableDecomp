# Lessons (chunk 185)
- A C3DVector/box with a user copy ctor and NO operator= makes the implicit assignment a `lea` pointer-register block copy; adding a user operator= folds it into plain offsets.
- An inline lock-scoped helper that returns a bool from inside the lock's scope gives retail's duplicated lock-dtor tails with the caller's test threaded in; a bool local declared outside the block spills to bl instead.
- strcmp in this lane is an intrinsic that expands inline; `#pragma function(strcmp)` restores the real call (and the per-iteration reloads).
- CCharString::c_str() is null-first: `p == 0 ? "" : p->Text`; `p ? p->Text : ""` flips the branch layout.
- A CRITICAL_SECTION modelled as `unsigned char[0x18]` is byte aligned and shifts the members after it; model it with dwords.
- A local CDiskFileWin32 clears Opened in the ctor BODY (after Pathname); the landed copy ctor uses the init list.
- `x = T()` with a user-ctor T builds the temporary in a dead argument slot (third word copied from there).
- An implicit literal-to-`const CCharString&` conversion is built before the other arguments; an explicit CCharString(...) temporary is not.
