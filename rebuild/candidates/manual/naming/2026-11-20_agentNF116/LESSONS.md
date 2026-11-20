# NF116 lessons (one line each)
- Probe every REFS row once (tools/probe.py) and also the callers of every row whose OWN leaf carries the string type (refs.py on the port rows): here the callers already bound the template spellings, so the ports closed splits instead of growing the family.
- A row-local STLport template model needs only `private:` around the 3-argument member to reach the PDB spelling (13 rows, one grade each); a free function calling it must go through STLport's public 2-argument wrapper.
- `q.py` strips retail's trailing int3, so self rows ending in a noreturn call show DIFFER(281v280) even at HEAD; grade them with vland / unstripped parity_of.
- Same-TU bodies decide register reuse: operator+ on the header differed (218v210, free-path reload) until the private append body was in the TU under auto_inline(off); fable_string_bodies.h cannot be included beside FABLE_RETAIL_STLP_STRING_THROW_INLINE, so the row carries the STLport append body itself.
- With the declaration alone, `_M_append_dispatch` became a call, not the retail tail jump; with fable_string_bodies.h it matched.
- substr needed fable_string_bodies.h: with `_M_throw_out_of_range` only declared, VC7.1 kept `this` live across the call (94 v 86 bytes).
- An overlay header whose quoted sibling include (`"LHNetworkTypes.h"`) lives in engine/ does not resolve from hdr/engine; grade through a symlink dir (overlay files + the sibling), hand back only hdr/.
- Before handing back a port, list every binding it changes outside the family: a vector<string> overflow port also renamed its uninitialized_fill_n / _M_clear bindings to spellings nobody defines, which blocks it.
