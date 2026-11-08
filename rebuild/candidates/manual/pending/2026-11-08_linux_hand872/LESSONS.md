# Chunk 872 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A "does any element qualify" loop that retail keeps as an up-counter beside a pointer is an EARLY-EXIT loop:
  `for (i = 0; i < 8 && !any; ++i) any = x[i] != 0.0f;` (or `if (any) break;`). Without the second exit VC7.1 fully unrolls a
  constant-trip for/while loop, and a do-while becomes a `dec / jne` down-counter (0x00C45990).
- A clamp whose result is stored in two places with an int-bit copy on the in-range path and x87 select + constant stores with a
  re-test on the out-of-range path is an if/else storing BOTH copies per arm; `a = clamp; b = a;` or `a = b = clamp` go through
  `fst / fstp` (0x00C4EA30). The clamp test order `fcomp hi; test ah,41h; jp` then `fcomp lo; test ah,1; jne` = `v <= hi && v >= lo`.
- `if (id && !data) return result;` with result still 0 folds into an earlier `return 0` exit; retail's jump to the shared
  `mov eax, edi` tail is `if (!id || data) { ... }` (0x00C4EA30).
- Integer lea operand order follows register allocation, not the source: `lea eax,[ecx+eax-1]` vs `[eax+ecx-1]` for the same
  `(size + bs - 1) / bs` flipped once the surrounding candidates got retail's registers (0x00C4A3B0); fix registers first.
- A value that must survive a `div` in a callee-saved register (retail edi) while the divisor takes ecx: compute the other operand
  (the claim count) before the division and name the recomputed total (`unsigned long total = ...; unsigned long block = ...;`)
  -- c2prio showed the blockCount/blockSize priorities 44 v 36 flipping (0x00C4A3B0).
- `bytes = GetAvailableReadSize(offset, bytes)` (writing the clamped size back) explains a retail `mov [esp+N], eax` between
  the argument pushes of the next call (0x00C4A3B0).
- An inline wrapper's callee (CAudioContext::GetProcAddress, landed body in the TU) may be expanded only inside a loop and called
  out of line elsewhere: VC7.1 does this by itself; give it the body and no flag (0x00C880A0).
- A float local updated after a call (`error = GetError(); pitch *= 0.8f;`) can be scheduled ahead of the call when it is in the
  same block; the source order that matches has the multiply AFTER the call (0x00C880A0).
- `x*x + (y*y + z*z)` gives retail's `y*y, z*z, add, x*x, add` order over three loaded coordinates; the flat sum gives x*x first.

Review at landing:
- Rows needing more than 4 grades: 0x00C4A3B0 (23), 0x00C45990 (15), 0x00C880A0 (12), 0x00C4EA30 (11).
- Catalog rows holding several functions: 0x00C51A20 (extra 0x00C51EB0), 0x00C53340 (extra 0x00C53400/10/20/30) -- land at the
  true lengths 1164 / 180; finish.py's oracle.tsv carries the catalog bytes for these two.
- Local models next to engine headers: CAudioContext (no shared header; landed rows each carry a local model), PKCBufferedReader
  (local, as the landed rows), CEAXVoice (local, extends the 0x228-byte block of CAudioObject::Open).
- Landed spellings this chunk proves wrong: NAME_FIXES.tsv (incl. the fastcall free-function form of 0x00C415A0 and
  g_eaxSourcePropertySetGuid_012beea0's comment calling the EAX 3.0 buffer GUID "EAX20").
- Two names for 0x0143BB2C / 0x0139C900 in landed libpk rows (g_pfnLogSink_0143bb2c / g_pchNil_0139c900 vs engine/CString.h's
  g_pfnAssertHook / _afxPchNil); this chunk uses the header's.
