# NF243 lessons (one line each)
- Do the hard rows of a family first: three rows with extra local std models (vector / map) decided that the 0x004046B0 unit cannot close, before any easy row was graded.
- A row-local list stand-in can only be ported onto stlport/fable_containers.h if it has no other local std model: the shared header defines vector / allocator / pair, so the whole namespace std block must go at once.
- An inline list ctor (the parser built in place) needs the element type complete for allocate(1); a forward-declared element works only with the declaration-only _BASE_OUTOFLINE.
- `#define protected public` in a test changes the mangling (IAE -> QAE): such a test cannot link the row object and must carry the body.
- A body with no 5-byte jmp thunk anywhere has only one source for ~list v ~_List_base (STLport structure): leave it undecided.
- Never write `rm -rf` with a variable path, even for a fresh scratch dir: use the guarded clean.py (the auto-mode classifier refuses it too).
