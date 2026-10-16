# Lessons from chunk 188
- An intelligent pointer passed through an inline `operator T*() const { return GetPItem(); }` as a call argument is evaluated BEFORE the plain arguments (GetPItem runs first, then the member loads/pushes); a direct `.GetPItem()` argument is evaluated at its right-to-left position. Fixes "owner fetched first" new-expression shapes and spills an out-parameter mode manager to the frame (0x008C1E80, 0x008F4320, 0x008F41D0, 0x008F39A0).
- A struct whose inline ctor stores constants in a non-declaration order (the -1 before a later 1) is a ctor BODY with assignments in that order, not an init list (0x00855430).
- `x = T(arg)` assignment through an inline operator= taking T BY VALUE puts the parameter copy in the dead string slot (copy ctor + SetPItem(GetPItem) + dtor) (0x008CD730 family).
- A polymorphic class whose dtor never re-stores its vtable: `__declspec(novtable)` on it, and `__forceinline` an inline member dtor that VC would otherwise call out of line (0x00AF10E0).
- if/else of two identical calls differing in one constant argument tail-merges into `push 1 ; jmp ; push 0 ; call`; a ternary argument gives `mov eax,1 / xor eax,eax` (0x008BF0F0).
- A 3-float C3DVector ctor routes copied X/Y through the FPU; member-by-member assignment of a default-constructed vector copies them as dwords (0x008F3FD0).
- `cond ? "A" : "B"` literal choice compiles `mov eax,B ; je ; mov eax,A` from `p == 0 ? B : A` (null-first) (0x008BBCC0).
- Lane gap (new): the inline CCharString::operator==("") null-block strcmp intrinsic materialises the bool with `sete` where retail branches on the `repe cmpsb` flags (3 rows).
