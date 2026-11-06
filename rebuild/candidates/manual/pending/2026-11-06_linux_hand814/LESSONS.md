# Chunk 814 lessons (GOTCHAS one-liners)
- NScript entity Mains with a landed sibling (MurderTwist BrotherYounger, Bordello Guard) match first try from the sibling's defines/includes/declarations verbatim (0x00EA8F40 RELOCATION_MATCH a1; 0x00E44EA0 a2): start there.
- Implicit MovieBase dtor expansion has a plain C++ spelling: implicit dtor + declared-only `template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();` gives retail's expanded/called mix from the inline budget alone (0x00EA8F40 and 0x00E44EA0 RELOCATION_MATCH); rows with every site expanded (0x00CF68C0) are gated by frame-slot sharing of the movies / T-exit pad cross-jumping, not by the dtor spelling.
- With FORCEINLINE_CTOR on the movie / widescreen header, declare the ~CCountedPointer<MovieBase> specialisation right after CCountedPointer.h (forward-declare the class) or VC7.1 errors C2908.
- One `timer=120` site that retail expands while ctor/dtor/GetTime follow the budget: `__forceinline CTimer::operator=` only (0x00E44EA0 a1 35 lines -> a2 RELOCATION_MATCH).
- Retail calling 0x00704430 inside an otherwise expanded by-value CScriptThing copy = declared-only `template<> CCountedPointer<CScriptThing>::CCountedPointer(const CCountedPointer<CScriptThing>&);` with an inline CScriptThing copy (0x00CF68C0 a2).
- CSpawnedFunc1 takes Par1 by value and MSVC is callee-destroys: the param copy/dtor live inside the forced ctor, so retail's "called in ctor, expanded at depth 1" split is not reachable by copy/dtor spelling alone (0x00E2FF00, 10 attempts).
- relpair string decoding must handle `??_C@` mangled `?0`-`?9` and `?$XX` escapes, else false string mismatches on punctuation-heavy lines.
- p.sh-style grade wrappers that cd into the repo need absolute source paths.
