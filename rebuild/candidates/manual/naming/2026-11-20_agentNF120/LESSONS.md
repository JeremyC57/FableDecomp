# NF120 lessons (one line each)
- Probe every REFS row once up front (NF113 probe.py, ~300 rows in ~10 minutes): two WORKLIST binder claims were stale and the census decides which entries are binder-only.
- A vtable slot forwarder stand-in (`Inner` vtable view) ports in one grade onto its shared header with the `PImp.Get(); if (imp) imp->F(args);` recipe (0x007E7450).
- A `mov al,[m] ; test al,al ; setne al` getter of a bool member is `return b ? true : false;` (plain `return b;` is 4 bytes, `b != false` 11).
- A container stand-in (local tree / node / map structs) inside a ctor swapped for the shared STLport std::map graded RELOCATION_MATCH first time (0x007744A0); try it before assuming EH-state trouble.
- Calling a now-virtual member by name on a local object stays a direct call: making the local model's declaration `virtual` is enough to bind UAE (keep the layout: the vptr takes the first pad word).
- A self rename can drag a callee's spelling (header-declared base call): check the callee's own self leaf and REFS, and respell the callee's self row in place when the caller is its only REFS row (0x004C7CF0).
- Byte-identical, unreferenced library destructors (0x00742260 / 0x00743220) block a family: no caller means no second source for which STLport name each carries.
