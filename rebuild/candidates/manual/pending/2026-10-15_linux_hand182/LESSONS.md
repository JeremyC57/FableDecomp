# Chunk 182 lessons (for GOTCHAS)
- GetInterface with the end/hit choice written as ONE ternary (`*pp = ((it == last || key < it->type) ? last : it)->component`) loads the component straight into ecx when its first use is `this` of a call; the if-form gives `mov eax,[eax+4] ; mov ecx,eax` (0x00961E80, 0x00950950, 0x00954690).
- `mov eax,[eax+0x4c] ; push edi ; mov ecx,eax` is an inline `GetPCombatManager()` accessor; the plain member gives `mov ecx,[eax+0x4c] ; push edi` (0x0096EDB0).
- A frame 4 bytes larger than the locals explain, over a stored XY delta, is a 12-byte delta local whose z is never written (0x0093D180; same gap parked on 0x0094FFF0).
- A global read into a named local before a call stays in a callee-saved register (`mov ebp,[g] ; call`); the bare global becomes a memory operand after the call (0x009190F0).
- An unnamed temporary of an OUT-OF-LINE converting ctor reuses the ctor's eax `this` (`mov ecx,eax`); retail's `lea ecx,[esp+N]` is a NAMED local (0x00961E80).
- A class-scope inline ctor inside a new-expression can stay an out-of-line call; `__forceinline` inlines it (0x00915290).
- The landed GetSubDef<T> body compiled above the caller under auto_inline(off) keeps the filled CDefPointer register-held, and VC drops the dtor's null test on the path that already dereferenced it (0x00950A30).
- A by-value counted-pointer argument is built in its stack slot only when the type declares a USER copy ctor (0x00950C30).
- Parked lane gaps: Bind2 first-pointer store scheduled after an out-of-line filter ctor (0x00922600); `lea edi,[esi+0x14]` hoisted above a temporary's GetPItem (0x0094C850, 0x0095B5A0); two independent loads swapped (0x0095A7D0); a GetInterface key slot not shared with later locals (0x0091DB50, 0x0092D600).
