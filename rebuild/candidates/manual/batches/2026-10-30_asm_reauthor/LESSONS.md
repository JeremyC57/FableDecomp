# Lessons (GOTCHAS one-liners)
- vland skips already-landed rows: re-authoring asm_bake rows needs `--recheck-landed` for the dry run.
- A retail_asm block that must live in an ebp-frame function goes in the function body; VC7.1 inlines a __forceinline helper
  holding the asm without giving the caller a frame (0x0042DF9E: 357 bytes without, 478 with the body asm + optimize y-off).
- `fnstcw word ptr cw` on an `unsigned long cw` lets VC7.1 share the word with a later 4-byte temporary; an `unsigned short`
  is placed at [ebp-2]. `reinterpret_cast<unsigned char*>(&cw)[1] &= 0xF3` gives retail's `and byte ptr [ebp-3],0F3h`;
  `cw &= 0xF3FF` gives `and dword`.
- x87 `1.0f - a*b` into a by-value float argument: name the whole difference (`float alpha = 1.0f - f() * h; Set(alpha);`)
  for retail's `fsubr [1.0f]`; naming only the product gives `fld1 ; fsub st(1)` + a dead pop; the expression as the
  argument CSEs the vtable across the two virtual calls.
- Inline bookmark-mask update: `state.BookmarkMask |= mgr.CurrentBookmarkMask` (no local copy) is retail's `or mask,old`.
- A call taking `const CWideString&` with an implicit `L"..."` conversion re-materialises the temp with `lea`; an explicit
  `CWideString(L"...")` argument uses the constructor's eax return (8 bytes off in 0x00413C50).
- VC7.1 inlines a static singleton Get() until the caller's inline budget is spent and calls it out of line after that
  (0x00413C50: 26 inline expansions, the last two calls out of line) -- write every call the same way and let it happen.
- Conditional temporaries in `new T(CCharString("..."))` arguments are tracked in a flag register (ebx bits) without /EHsc:
  no source trick needed.
- VC7.1 evaluates the right operand of `a == b` first when both are calls (0x00597BF2: `map[k] == stack.top()` calls top()
  first).
- A class key matters for the leaf: `struct C2DBoxI` mangles U, the PDB's class mangles V -- the grader silently grades
  the largest function when the leaf binds nothing.
- reloc_bindings spellings are not evidence: 0x009EC890 is CConsole::RunScript in both PDBs, bound as ExecuteConfigFile.
- Tests for virtual-heavy engine classes: a fake class with the same virtual slot layout, reinterpret_cast to the
  interface, stands in for the object; a stub returning a by-value class with a destructor should copy from a raw pointer
  lvalue (VC7.1 /Od runs the named local's destructor after the copy -- no NRVO).

# Review at landing
- Retire the asm twin of 009c85a0 (C++ already landed).
- Rename the landed 00c8db10 file/row to libogg ogg_stream_packetout.
- 00413c50: three bound global names disagree with their console names (NOTES.md).
