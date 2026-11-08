# chunk 867 — lessons (cl 13.10.3077 RTM, /Os, Wine lane); one line each

## Lane / tooling
- cl 13.10 under Wine spins at 100% CPU (never exits) on a `#error` directive and on a missing `#include`; bisect with a preprocess-only run (`/E` under `timeout`) before grading, and wrap every grade in `timeout 240`.
- A trimmed overlay must carry every sibling header its new headers include by bare name (MSVC searches the includer's directory first); a python writer that aborts mid-way leaves a silent spin, so count the files it wrote.
- Insert an opt-in block at the LAST occurrence of a marker string when the header repeats the class name in a forward-declaration block at the top; the first occurrence puts the members outside the class.
- A `/FA` probe listing with the frame-slot lines (`$T`) is the fastest way to ask c2 how it groups temps; a full grade is only needed once the listing shows the right slots.
- VC7.1 refuses `offsetof` on a private member (C2248); assert sizeof only, and document the offset arithmetic in a comment.

## c2 shapes proven this chunk
- Operand order of `==` sets evaluation order: retail evaluating `rhs()` first means the C++ had the call on the right (`a.Get() == b->GetPTarget().Get()`).
- `if (A) c = 2; else if (B && C) c = 2;` head/tail-merges into one store; `if (A || (B && C))` with a CDefPointer temp in the RHS instead makes a lifetime-flag dword (`test byte [slot],1`) and a flag byte for the condition.
- Two sibling `else if (n1) c = 1; else if (n2) c = 1;` statements with one-store bodies merge into one `c = 1 ; jmp` block placed before the else-block (first test jumps on true, second falls through); `if (n1 || n2)` with the same body gets the block placed last.
- `if (GetTC(...))` (the bool tested, handle unused) flips the LowerBound end-or-greater polarity to `jg` with the end kept in ebx; `GetTC(...) && h->X()` keeps `jge`.
- For `a * b` with two call operands the evaluation order follows the operand order only when neither operand is a hidden-return temp already live; swapping fixed the height product but not the width product.
- A discarded hidden-return call (`dims();` as a statement) costs the same temp as a named local but a named local changes the frame by 8 bytes and is read via `[ebp+x]`; retail's `fild [eax]` on a struct member proves a temp, not a local.
- Temps are freed at the end of the full-expression; hidden-return temps and inline-constructed value temps (C2DVector) share one free list; named locals (even dead ones) never share with temps; sibling/nested scopes, destructors, const-reference binding, inlined helpers and the QFE-4035 toolset do not change the grouping. Slot order among temps follows descending reference count.
- A `continue`-style cleanup is specialised (null test dropped, release laid inline) when a same-TU inline GetSubDef keeps the "handle != 0" fact alive; retail's shared release chain means the fact died at a call boundary.
- c2 orders a commutative add chain by the operands' first-appearance rank, not the source order of the final sum; a bool that reaches its test through a join or a frame slot spells `sete al ; test al,al ; jne` instead of a fused `je`.
- A text-bank reference that retail homes in memory (stored once, reloaded) is a non-candidate local; making its address escape reproduces retail's colouring, so look for a natural spelling that does the same before touching register priorities.

## Shapes carried over from the matched rows
- CreateComponent's argument list converts its CCharString/CWideString temporaries in the implicit-conversion order (right-to-left pushes, temps constructed in argument order) — spell the literals as the parameter types take them.
- `cond ? CCharString(a) : CCharString(b)` fuses the two constructions into one temp with a flag; two `if/else` assignments do not.
- CRGBFloatColour → CRGBColour is a `__forceinline` conversion operator (R,G,B,A `* 255` truncations in member order); a plain inline is called out of line under /Os.
- The morality fill `?:` fuses the icon index selection; a separate `if` splits the stores.
- STLport `map::operator[]` is visible inline only when the retail opt-in declares it out of line (COMDAT 0x005B322B etc.); otherwise c2 inlines the tree walk and the size changes by hundreds of bytes.
- CCountedPointer<T>(T*) ctor form: the called ctor (FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR) for UI components, the direct dtor (CIVCOUNTEDPOINTER_DTOR_DIRECT) for text-entry handles.
- Float threshold tests compile to the `fcomp ; fnstsw ax ; test ah,41h` idiom from `if (x >= limit)`; `>` gives `test ah,1`.
- `#pragma optimize("s", on)` alone gives the biased `lea ebp,[esp-74h]` frame; adding `optimize("y", off)` gives the plain `push ebp ; mov ebp,esp` frame (CTCInventory* / CManager rows).
