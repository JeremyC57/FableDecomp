# Chunk 1010b lessons (GOTCHAS-ready)

- (chunk 1010b, 0x0089B5B0) A one-register residue in an inline release (`delete Info` reloads into ecx/edx, retail eax) is
  c2's expression-temporary rotation, not the allocator: `c2prio --rotation` shows the pointer per line; reading a member
  through its PDB inline accessor (FableWin `GetCurrentScriptID()`) instead of directly removed one rotation step earlier
  and matched. An argument written `*p.Get()` (inline wrapper) is evaluated before a later member load; `*static_cast<T*>(p.GetPItem())` after it.
- (chunk 1010b, 0x00896390) The OpenChest 711v711 block-order park was the goto form: the natural nested positive form
  (`if (alive) { if (tc) { if (useHero) {..return true;} else { Open(); return true; } } } return false;`) gives retail's
  true tail / direct arm / false tail order on the first probe.
- (chunk 1010b, 0x008A8C60) Frame-slot packing of a counted pointer whose ctor and dtor retail CALLS out of line
  (0x008AB380 / 0x008AB3D0): give both visible noinline bodies (`FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR` + new
  `_VISIBLE_CALLED_DTOR`); declaration-only bodies keep the pointer address-exposed: own slot (frame +4) and a re-read of
  Info in the by-value copy.
- (chunk 1010b, 0x008A8C60) An uninitialised out-pointer of an expanded GetTC/PeekTC shares the key slot (`mov ecx,[esp+10h]`
  on the absent path); `T* p = 0;` gives it its own slot (frame +4) and `xor ecx,ecx`.
- (chunk 1010b, 0x008A8C60) `new X(..., defString)` (implicit CDefString -> CCharString) takes the temporary's address after the
  call and sets the conditional-destroy flag after it; `defString.GetString()` pushes eax and sets the flag first. Retail calls
  GetString 0x00415DD0 there: force the forwarder inline and keep GetString a visible noinline body
  (`FABLE_RETAIL_CDEFSTRING_FORCEINLINE_CONVERSION`); declaration-only GetString shifts the frame by 4.
- (chunk 1010b, 0x008A93A0) c2prio --blocks makes register ties concrete: thing -11 v this -20, of which +24 is the found
  block's two references of thing (ctor argument + SetCurrentAction this); taking that block's creature from elsewhere flips
  ebx to this. Copies (`creature = static_cast<..>(thing)`, GFDebugPDynamicCast, references) are forwarded and change nothing.
