# NF182 lessons (one line each)
- A ??_G is emitted only by a TU that compiles a constructor or destructor of its class: when the class's constructor is out of line with its own row (most CTC / thing classes), the ??_G row cannot be ported as a single row; respell its destructor call IN PLACE (`virtual ~X();` called as `this->X::~X()`, a direct call) instead.
- VC7.1 never expanded CTCFlashOnHit's small inline constructor (stack object or `new`, with or without the size pragma): a scaffolding use emits it as a pick-any COMDAT, so grade that copy against the constructor's own row (q.py <owner addr> <row> '<ctor leaf>').
- objdump -t on a COFF object with COMDATs mis-numbers sections (defined COMDAT symbols show as sec 0): use `nm -g --defined-only` and `objdump -h` (COMDAT names) for the defined-symbol diff.
- A row with no `// Leaf:` line is graded on its LARGEST function: after a port adds header inlines (??_G, inline dtors) add the Leaf line or vland grades the wrong function (0x00BF3290 showed DIFFER(30v25) until it had one).
- reloc_bindings witnesses go stale (0x00BC7E90, 0x00C23BA0, 0x005297D0, 0x00CBF6DC all bound something else at HEAD): compile the binder before porting it.
- Swapping a 1-aligned stand-in member (`unsigned char _body[N]`) for a real class changes the containing struct's alignment and size: re-check the offsets of the members after it (0x004473A0: +2 shift, fixed by shrinking the next pad).
- Keep a row's FABLE_STATIC_ASSERT lines on their line numbers when adding a #define (drop a blank line to compensate).
- End every Bash call with `; true`: two calls here ended non-zero on a missing-file cat.
