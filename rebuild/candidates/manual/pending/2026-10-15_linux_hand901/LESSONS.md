# Chunk 901 lessons
- A one-bit bool bitfield store from a bool argument needs `#pragma optimize("s", on)` to go straight through memory
  (`xor cl,[mem] ; and cl,4 ; xor [mem],cl`); the /Ot form spills through dl (00455fb4).
- A size-optimised TU shares one zeroed register across byte compares, a switch's `sub eax,edx` and a `push edx` of
  constant 0 (00581ab3) and forms a flag's address once with `lea` while dl holds the zero (00581ae1).
- CScriptThing::operator== / != allocate retail's registers only when both operands go through the counted pointer's
  inline accessor (`GetPImp()->IsSameThing(other.GetPImp())`); bare member reads swap eax/edx (008aae20, 008aae40).
- `return (float)::pow(x, y)` tail-jumps to `_CIpow` in the lane; retail's `call ; ret` needs the result in a named
  float local first (00c40e50). atan2 and log10 match directly through `#pragma intrinsic`.
- STLport list<T> is one sentinel pointer: `empty()` = `node->next == node`, `front()` = `node->next->data` (+8);
  re-reading `front()` after each virtual call reproduces retail's reloads (0073a840).
- The grader (q.py) takes the FIRST batch oracle row for an address: a stale multi-function row in an older
  chunk oracle shadows the split piece -- delete the old row before grading a ledger piece.
