# Lessons (wave 2, section 3)

- **Literal to a `const CCharString&` parameter.**
  - Pass the bare literal: `f("NAME", &x)`. The implicit conversion builds the temporary BEFORE the other
    argument pushes. An explicit `f(CCharString("NAME"), &x)` builds it after the pushes.
  - This matched retail in 10 rows, including:
    - a switch whose four arms each build their own slot and tail-merge one dtor (0078cd80);
    - two rows that landed on retail's exact length (007ce7a0, 00865f40).
  - Exception: a one-argument call whose object expression is itself a call keeps the explicit temp (007c99f0).
- **A user-declared empty inline destructor `~T() { }` changes VC7.1's per-site inlining** compared with the
  implicit one.
  - 0088b0c0: the implicit ~CStringParser inlined at all five returns; retail inlines only at the first return.
    The user dtor gives retail's split.
  - Cost: the user dtor adds a vptr reset, so the vtable constant gets cached in a register.
- **By-value C3DVector helper parameters, together with a `__forceinline` memberwise copy ctor, copy the vector
  into x87 registers**, with one component going through an int stack slot. Use this when retail loads all three
  components before the arithmetic (00875b10 Normalise).
- **Unit axes as literals.** `C3DVector(1,0,0) * sin + C3DVector(0,1,0) * cos` reproduces retail's `fmul [0.0f
  pool]` products. VC does not fold `0.0f * x`.
  - The cos term evaluates first because it is the right operand of operator+ (0080d300).
  - `up = C3DVector(0,0,1) * h` follows the same pattern (008651b0).
- **Inline `cos(angle)` / `sin(angle)` inside the vector expression** give retail's `fsin`, YAxis products, `fcos`,
  XAxis products schedule, with the angle kept on the x87 stack (00872af0). Named `float s, c` locals do not.
- **Def-pointer copy inline.** Write a by-value def getter as `return CDefPointer<T>(raw)` with a `__forceinline`
  raw-pointer AddRef ctor. That gives retail's inline `cmp ; je ; inc [def+4]` as the && temporary (0087ee70). A
  local copy plus NRVO adds an extra inc/dec pair.
- **Reference locals.** A reference local hoisted out of an inner loop (`const T& src = a[i].b[t]`) changes the
  base register. Retail re-indexes `a[i].b[t]` at each use (008873c0, 401 -> 228 lines).
- **Check every member offset against Ego_r.pdb**, applying the STLport 12-byte vector shift. 007a8700 had Range
  at +0x3B where the PDB DashDistance is at +0x3C, after a pad byte that follows the packed DirectionOverride.
- **Split arms.** When retail duplicates a call tail for two probe pairs, write them as separate if / else-if arms
  (0084d2f0).
- **finish.py: oracle_extra must win over oracle.tsv** for BOUNDS_FIX rows. Otherwise vland grades the
  catalog length and reports DIFFER (007a8be0 576v617).
- **Tests compile under VC7.1 /Od.** Its std::basic_string has no push_back (use `+=`), and `<cctype>` should be
  `<ctype.h>`. Check a test with verify_and_land's cl() before the dry vland.
- **Grades that do not move.** These stayed byte-identical and are not worth repeating:
  - 008591e0: AddRef as an `inc [mem]` member call or as a RAII copy. VC splits it into load/inc/store around the
    next load in every form.
  - 007bc6fa: the "physics reload before the pops" scheduling.
  - 00807c30: inline budget. Forceinline, implicit copy and an out-of-class definition all left it unchanged.
- **Bounds.** A row whose ours-length equals a `ret` before int3 padding usually has an over-long catalog extent.
  Check the bytes after the padding before reading the residue as code (007a8be0, 00871a80).
- **Skeleton-draft levers (wave 2 sec3, the 5 skeleton parks).**
  - Interface tests read the presence bit in three forms. Match the one retail shows:
    - `m_presentBits[t>>5] & mask`, an `unsigned long`, gives `test reg, mask`, with a 0x80000000 mask kept in a register.
    - `(bits >> n) & 1`, a value bool, gives `shr; and cl,1` with a byte store (0087d590, 0082afa0).
    - `!= 0` gives `shr; test cl,1`.
  - When retail's frame is smaller than ours, pass a literal or a temporary to a `const T&` parameter instead of a
    named local. 0082afa0 went from 0x58 to retail's 0x50 (410 -> 366); 0087cfe0's map key went 362 -> 344.
  - Write `if (!(x < 0.0f))` to get `test ah,5; jnp`. `x >= 0.0f` gives `test ah,1; jne`.
  - A three-term dot that retail evaluates y, z, then x is written `X*RX + (Y*RY + Z*RZ)` (0087d590).
  - Bind a list member to a reference local (`CCardList& cards = g->Cards`). Retail keeps `&g->Cards` in a register and
    re-reads the end node each iteration (0082afa0).
  - An STLport `bitset::test` range check inlines as `push "bitset"; call puts; jmp abort`. Declare abort
    `__declspec(noreturn)` and shift the index unsigned (00848020).
  - A helper that retail inlines everywhere but our build calls out of line (UpdateDestination in 0079f970) needs
    `__forceinline`. A user-declared C3DVector operator= makes VC store the floats with fstp. The implicit operator= gives
    retail's integer moves.
