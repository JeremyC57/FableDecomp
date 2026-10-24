# Chunk 376 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Two locals sharing one frame slot, with both start pointers held in registers, mean VC7.1 proved that neither
  local's address escapes. Example: a `vector<char>` buffer and a later named `std::string` (0x0055F090 / 0x0055F140,
  frame 0x10).
  - Give every out-of-line callee that receives `this` a same-TU noinline stand-in. Here that was the vector fill
    ctor, `basic_string::_M_range_initialize`, and also the noreturn `_M_throw_length_error() const` it calls.
  - One unknown callee taking `this` (even the length-error thrower) is enough to make the local escape: 174v170 with
    a 0x1c frame.
  - Check the stand-ins against the retail callees: the fill-ctor stand-in grades RELOCATION_MATCH at 0x00567560.
- STLport list copy (`list<T*> copy(Observers)`) followed by a loop (0x0055CBE0): the GOTCHAS recipe of a same-TU
  noinline `_M_insert_dispatch(pos, first, last, const __false_type&)` that never writes the head word matches on the
  first grade. ecx and the tag both land on the dead `event` argument slot.
- A function in an anonymous namespace (`?MD5@?A0xf010c0d0@@...`) gets a different namespace hash in our object.
  Use a `// Leaf: ?MD5@` prefix.
  - finish.py's line-1 parser needs line 1 to start with the bare name: `// MD5(...) -- (anonymous namespace) ...`.
    `// (anonymous namespace)::MD5(` falls through to a later comment line.
- A static `__fastcall` that writes a file through a local CDiskFileWin32 (0x0055CFB0) matches the landed
  WriteRecoveryFile runtime model with these details:
  - the slot-9 `GetLength()` defined inline (`mov eax,[file.Length]`)
  - the text pointer taken into a local before the length call
  - an explicit `Close()` before the inline destructor's own Close
  - all virtual calls on the local are direct
- The Deutsch md5.c entry points are extern "C" `__fastcall` (`@md5_append@12`): ecx = state, edx = data, length on
  the stack (0x0055E5F0). A `char hex[3]` plus `_snprintf(hex, 3, "%02x", b); hex[2] = 0;` needs `/GS`.
- `x / g1 * g2` over two float globals is reassociated to `(g2 / g1) * x`. Write `r = x / g1; r *= g2;` as two
  statements (0x0052E580). /Op changes the size instead.
- `while (range > 1 && result == previous)` keeps the result in ebx (0x005472A0). The
  `if (range > 1) { while (...) }` form gives 145v134.
- A temporary built from a string literal: the implicit `const char*` -> CCharString conversion gives `lea`; an
  explicit `CCharString(...)` temporary pushes eax instead.
- A member read through a POINTER accessor (`GetFontManager() { return P.Data; }`) puts the singleton getter call
  between the temporary's construction and the pushes (0x0054F4B0). The `operator->` form adds `add eax,84h`; a direct
  member read sinks the getter after the pushes.
- `a() >= b()` calls b first and compares `cmp a,b`. To get retail's `cmp eax,esi`, put the operand retail calls
  first on the right (0x0055BB40).
- An explicit instantiation (`template void vector<T>::push_back(const T&);`) emits only that member. In an
  `optimize("s", on)` TU it lands on an unaligned address, as retail's 0x0053CFC2 does.
- STLport `clear()` = `erase(begin(), end())` = `__copy_ptrs(last, _M_finish, first, __false_type())`, which calls
  the out-of-line `__copy`; `_Destroy` folds for trivial elements. `for_each(..., mem_fun_ref(&T::F))` returns the
  functor through a hidden pointer (0x00542730).
- A bitfield cleared in a constructor BODY (`OverrideAutomaticLocking = false;`) is scheduled before the derived
  vtable store (0x0052AC10).
- Static `_M_increment(node)` (STLport `_Rb_global<bool>`) rather than a member `_M_increment()` for tree iterators:
  the member swaps esi/edx (0x0052DEC0, 0x0055E7F0).

Review at landing:
- More than 4 grades:
  - 0x0054F4B0 GetCharWidthW took about 10 grades (temporary / accessor / `operator->` / direct member forms) before
    the pointer accessor matched. That broke the 4-attempt rule.
  - 0x0055F090 took 6 grades: 4 forms plus 2 `/Oa` probes. The final source has no flag.
- Local models next to existing engine headers. The headers carry PDB sizes that differ from retail, so the local
  views were kept:
  - CThingGameObject.h: PDB 0xE8, retail 0xD0
  - CCompressorZlib.h: PDB 0x5C, retail 0x58
  - CHoverable.h: PDB 0x19C, retail 0x16C (00559040)
  - CDiskFileWin32.h: flat donor storage, no base or vtable; 0055CFB0 uses the landed runtime model
- Same-TU stand-in bodies (accepted per GOTCHAS wave 25):
  - 0x0055CBE0: `_M_insert_dispatch`
  - 0x0055F090 / 0x0055F140: the vector fill ctor, `_M_range_initialize`, `_M_throw_length_error`
  - 0x0052DEC0: `__find` instantiated by its landed caller IgnoreEvent
- Already-landed source errors:
  - 0x0053BEAC declares 0x0053CFC2 as `CScrollLayout::AddChild(long)` at +0x17C. It is
    `vector<map<unsigned long, CUIState> >::push_back` on CList::ScrollingChildrenStates.
  - The landed CHttpUrl GetHost 0x0083F5C0 header calls 0x00567D00 `__fastcall`. Its code (and the landed
    RangeInitialize) is a thiscall member with `ret 0Ch`.
- Catalogue gaps: 0x0055E4F0 DecryptBuffer, 0x0055E510 EncryptBuffer, 0x0055F1F0 DecryptString and 0x0055F2D0
  EncryptString are missing from rebuild/manifest/functions.tsv.
- Propagated labels misleading for callers: 0x0099AF10 / 0x0099B220 are labelled CCharString::* in the manifest but
  are called on a CWideString (the landed rows say CWideString).
- No catalogue row held two functions. Every row starts at a real function entry.
