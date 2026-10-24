# Chunk 409 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Inline counted release `mov edx,[eax+0xc] ; dec edx ; mov ecx,edx ; test ecx,ecx ; mov [eax+0xc],edx ; jg` comes from `CWideStringData* data = PStringData; if (data) { if (--data->NoRefs <= 0) delete PStringData; PStringData = 0; }`. The pointer is a LOCAL copy and `delete` re-reads the member. Decrementing through the member gives `dec dword ptr [eax+0xc]` (0x99B770, same as landed 0x99B4D0, inlined again in 0x99BB80).
- Win32 lookup chains such as `FindResourceEx -> LoadResource -> LockResource -> use` with the failure block on the fall-through need one early `return -1;` per step. Nested ifs put the shared failure exit after the success call (0x99D4D0).
- FableWin.pdb `@Name@N` symbols are extern "C" `__fastcall` and work as exact leafs (0x99D470, 0x99D520, 0x99DBE0, 0x99DC50). Two same-TU calls to one helper: VC7.1 inlines only the first by itself, so write the plain form (0x99D520).
- STLport classes must be `class` in the model: `class char_traits`, `class allocator`, `class CFileScanInfo`. A `struct` mangles as `U` instead of `V`, so a mangled `// Leaf:` silently names no symbol and the grader falls back to the largest function (0x99C990, 0x999760, 0x999C50, 0x999CB0). Check every leaf with `objdump -t`.
- `basic_string<unsigned short>::push_back` = inline `append(1, c)` matches only with the landed 0x99B9A0 inline spelling `fill = c; count = n;`, which puts c in a frame word and 1 in c's dead slot. The out-of-line append 0x99CB00 needs the verbatim STLport body; with the copies it is 154v145.
- STLport fill constructor `basic_string(n, c, a)`: VC7.1 turns `fill_n` over the by-value c into `rep stosd ; adc ecx,ecx ; rep stosw` by itself. Write the plain loop (0x99CA70).
- `vector<T> v; size = f(); v.resize(size);` folds `size() == 0` (no erase, `_M_fill_insert(0, n, &zero)`) only when the vector is declared before the call and `_M_fill_insert` has a same-TU noinline stand-in that never leaks `this`. The stand-in also keeps begin() in a register across the later calls (0x99BB80).
- Any header comment line starting `// Flags: /GS (cookie ...` is read by gall.sh and finish.py as extra compiler flags. Keep `// Flags:` only on the one real flags line.
- int3-after-noreturn: the STLport wide-string members ending in `_M_throw_length_error` (0x99C670, 0x99CA70, 0x99CB00, 0x99CC80) are one byte short in the chunk. Grade them against the N+1 row (probe/n1.py) and land the same way.
- Shapes from the CAFile rows:
  - A named local for a `ToWideString()` result makes VC re-take its address with `lea` instead of caching eax (0x9990B0).
  - A positive `if (h != -1) { ...; return true; }` puts the failure arm last (0x999760).
  - `/GS` split buffers in an inner block share the dead temporary's slot (0x999180, 0x999970).
  - `(u64)high * 0x100000000ui64 + low` keeps `__allmul` (0x999320).
  - An STLport `vector<unsigned char>` buffer explains CopyFileW's 0x18 frame (0x998E70).
- CCharString `operator==(const char*)` and `operator==(char)` need `__forceinline` at both the CCharString and the CBasicString level, or they stay calls (0x998170, 0x997E90).
- GF parsers match with the fetch-ahead loop `char c = GetChar(i); while (i < len) { ...; c = GetChar(++i); }` (0x99E620, 0x99E7F0, 0x99E870). A `for` loop moves the load to the top.

Review at landing:
- 0x0099BFF0 replaces the landed asm-bake CWideString_AddLiteralAndWideString_0099bff0.cpp with compiler-emitted C++.
- 0x00998210 uses a local retail CStringParser model (0x28 bytes: CursorPos +0x1C, StringPos +0x20). The generated engine header has the PDB 0x30 layout.
- Landed sources name the two CWideString text accessors inconsistently. 0x0099AF00 and 0x0099AF10 appear both as `operator const wchar_t*` and as `GetAsCharArray`. Check them against the PDB before retyping.
- Landed misnames:
  - 0x0099F930 "CCharString::FromUInt64" is FableWin.pdb `?GFUHUGEToCharString@@YI?AVCCharString@@_K@Z` (a free `__fastcall`).
  - 0x0099F1F0 "CCharString::Format_vsnprintf" is `?Sprintf@CCharString@@QAAXPBDZZ`.
- The four int3-after-noreturn rows (0x99C670, 0x99CA70, 0x99CB00, 0x99CC80) need N+1 oracle rows. Their sources and tests are in differ/.
- Rows parked at the 4-grade budget: 0x997F80 (scheduling only), 0x99E690 (x87 allocation), 0x99BB80 (one `lea` vs `mov`+`add`).
- 0x99DBE0 carries the PDB name SetDirtyExitFlag, but the retail body only opens the key, reads ExitFlag and returns 0. The write side may be compiled out.
