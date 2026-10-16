# Chunk 177 lessons
- A float constant written as a literal keeps retail's `fld [mem] ; fadd [const]` order; an `extern const float` operand is reordered by address.
- x87 sum-of-squares order: when a single `a*a + b*b + c*c` gives the wrong term order, a `s = z*z; s += x*x; s += y*y;` chain in retail's order matches (0x00860A50).
- Def temporaries destroyed inline: derived def classes with implicit destructors drop their dead vtable stores; a user `virtual ~X() {}` keeps them alive across free calls (0x0067D610).
- A component/appearance pointer that retail loads into ecx right after a CCharString ctor, before the argument pushes, is a named local read after the string (0x00661AA0).
- `bool b = !flag && f().g() > 0;` gives retail's frame-byte bool plus the conditional temporary flag in bl (0x00851920); the "bool kept in a frame byte" gap is reachable this way.
- A top-tested retry loop with the increment before the call is `while (n < N && (++n, call() != 0));`; for/while with `if (...) break` is rotated to a bottom test (0x00A3B130).
- VC7.1 has no NRVO: a by-value POD result that retail writes straight into the caller's object only on success is an out reference parameter (0x00A67530 family).
- strmbase-style dtors: a CBaseObject DIRECT base gives the 8-byte EH frame (`sub esp,8`) seen in 0x00CA9D00; retail null-checks base conversions one level more than our CUnknown model (0x00CAEFB0/0x00CAF480 still parked).
- Unoptimised bodies (frame pointer, all values through memory, `rep movsd` result copy) build under `#pragma optimize("", off)`; the remaining diff is local allocation order.
