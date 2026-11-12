# Chunk 2039 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Early `return`s inside the scope of a destructible local add one destructor call each to that local's frame-packer weight
  (counted before cross-jumping): 0x006ACA30's CCharString `sound` weighed 12 with five returns and took the frame word before the
  container pointer (7); writing the exits as switch `break`s plus a `collected` flag that c2 threads away (no instruction left)
  gave weight 7 and retail's slot order. Check the packer weights (pk.sh) whenever two 4-byte slots are swapped.
- Retail keeps unreferenced out-of-line COMDAT copies of the inline helpers a TU instantiates even when every call site expands
  them (0x006AE5E0 / 0x006AE630 / 0x006AE680 / 0x006AE740 in the CThingPlayerCreature TU, all uncatalogued): an unreferenced copy
  does NOT prove a helper was plain inline; it proves the body (compile it out of line and grade with QLEN for free).
- CThing::HasTC<T>() (Ego_r/FableWin member template): PeekTC(T's interface), then `GetClassTypeName() == T::StatGetClassTypeName()`
  in a nested if (no `&&`: an `&&` adds conditional-destruction flags for the two string temporaries). Plain-inline cost 175 ->
  FABLE_FORCEINLINE like GetTC / PeekTC (0x006AE680, row 0x006ACA30).
- `if (!flag) { A } else { B }` versus `if (flag) { B } else { A }`: VC7.1 lays the then-arm A out after the enclosing loop when
  it is the cold side of a threaded flag (0x006ACDBA auto-pickup block); the other order keeps A inside the loop.
- A `break` out of a def handle's scope adds a second, un-null-tested release copy that never cross-jumps with the scope-end one;
  a result flag (`bool added`) keeps retail's single shared release (0x006AD003).
- `stats->AddRentMoney(x); ... F((float)stats->GetRentMoney())` lets c2 forward the stored sum to the fild temp (`mov [esp+2Ch],ecx`,
  0x006AD161); a named `long rent = stats->GetRentMoney();` adds an allocator candidate that flipped two callee-saved registers
  (stats / gold) and shifted the scratch rotation of every later block by one step.
- Out-of-line def lookups whose bodies are visible (GETDEF_GLOBAL_BODY / GETSUBDEF_BODY) remove the reload + null re-test before
  each CDefPointer release; with declarations only the release came out as `mov esi,[eax+4] ; ... dec esi ; mov [eax],esi`.
- GetTC with a variable interface type expands the STLport bitset range check (`cmp esi,112h ; jb ; push "bitset" ; call puts ;
  call abort`, 0x006ACF1D) and the key compare as `cmp esi,[eax] ; jge`.

Review at landing:
- The row needs the hdr/ overlay (10 opt-ins / new headers, hdr/README.md); merge into rebuild/include before landing.
- NAME_FIXES.tsv: 0x004C79A0 (landed 0x006A9590 / 0x00837050 / 0x00837060 bind wrong spellings), 0x0055F3B0 enum nesting (landed
  self row), 0x006AC200 binding, 0x00437980 catalog label.
- 0x006AB820 ResolveFacingDirection was not attempted (no source).
