# Chunk 904 lessons
- A tail-called __stdcall method on a pointer argument re-stores that pointer into its own argument slot in the lane (`mov [esp+4],eax`); retail jumps with the slot untouched -- no calling-convention form reproduces it (00caef70).
- A six-argument record constructor emitted out of line keeps its byte arguments in al/dl across the `push esi` and stores members in declaration order with the string copies as calls to the landed CCharString copy constructor (005271c0).
