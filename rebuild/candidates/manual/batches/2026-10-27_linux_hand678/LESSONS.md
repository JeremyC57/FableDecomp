# Chunk 678 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A same-TU `CDefClassBase::GetSubDef<T>` template body has three effects in the caller (0x0077A180, 0x0074EE90):
  - the out def pointer's value stays in a register across later calls;
  - its release becomes `test esi,esi ; dec [esi+4] ; jne`;
  - a later interface key can share the def's dead argument slot.
  Without the body the def is re-read from the frame and the release is the long `mov/dec/mov` form.
- When retail keeps a zero in ebx across an out-of-line `CVectorMap::LowerBound` call (`xor ebx,ebx ; cmp edi,ebx` on the
  null test, then the zero stores of a vector and a counter), give LowerBound a same-TU out-of-line body under
  `#pragma auto_inline(off)` (the landed 0x0040F020 text). Its register facts turn the zero web on and spill the lookup
  result to the frame (0x0074EE90: nonreloc 83 -> 29).
- A same-TU `auto_inline(off)` stand-in for the out-param filler (`GetRewardItems(items, gold)`) decides where the later
  block's temporaries live. Keep it to the real body's opening steps (`gold = 0; items.erase(begin, end)`). A stand-in
  that never touches the vector lets VC fold every `items.empty()` test away (0x0074EE90).
- `if (gold <= 0 && items.empty()) { ...; return false; } if (gold > 0) {...} if (!items.empty()) {...; return true;}`
  gives retail's jump-threaded layout: the nothing block falls through, the gold arm sits at the end, and the
  non-empty test jumps straight to the give block (0x0074EE90).
- A GetSubDef result kept in a named `bool found = ...;` (the by-value GetPBaseDef temporary is released before the test)
  removes a stray entry-block zero store that the in-condition form hoists (0x0074EE90).
- `*component.PCountedPtr` as an argument leaves a dead `add eax,<offset>`. Retail went through the Ego_r inline accessor
  (`PeekCutSceneMainFont()`, `GetTextBank()` on CMainGameComponent). `*ptr.Data` instead sinks the getter call below the
  pushes (0x0074EE90).
- A literal passed to a `const CCharString&` parameter: implicit conversion (needs the `long length = -1` default) re-takes
  `lea` for the argument after the ctor. The explicit `CCharString("X", -1)` temporary reuses the ctor's eax
  (0x0077A180).
- `std::map<long,long>` pickup loops: re-read `GetDef().Map.begin()/end()` instead of binding `const map& m`. The
  reference keeps a dead `add eax,38h` and loses the `jmp` over the loop-top padding (0x0077A180).
- The interface-map lookup with the map address in esi (`lea esi,[ebx+44h] ; mov ecx,esi ; ... mov esi,[esi+4]`) is
  `const CInterfaceMap* map = &Components; it = map->LowerBound(key); ... map->_M_finish` (0x0077A180).
- A member stored as `unsigned char Data[N]` gives alignment 1 and shifts the next member (CSoundMap at +0x25 instead of
  +0x28, CChecksumCacheFile at +0xF5). Model opaque blobs as `unsigned long Body[N/4]` (0x0077A180, 0x009CE180).
- Block placement of two identical `return false` exits (0x009D06F0) did not move under 16 source forms (early returns,
  gotos, nested ifs). VC merges into the later exit. This is a candidate lane gap.

Review at landing:
- 0x0074EE90 defines a stand-in body for ?GetRewardItems@CTCContainerRewardHero (0x0074E860). It holds only the real
  opening steps (gold = 0, items.erase), under auto_inline(off). It must not be bound as the real 0x0074E860 when the TU
  is linked; the real row is still unlanded. The object also references ?erase@?$vector<pair<long,CCharString> > (0x0074FB20).
- 0x0074EE90 and 0x0077A180 define a CVectorMap::LowerBound body (non-const, 0x0040F020 text) and a
  CDefClassBase::GetSubDef<T> body as COMDATs in their objects. These are the same bodies as the landed rows; check the
  verifier's duplicate-COMDAT handling.
- Rows that needed more than 4 grades:
  - 0x0074EE90 (23 probes);
  - 0x0077A180 (8);
  - 0x009D06F0 (16, parked);
  - 0x0075AEC0 (4);
  - 0x009CE180 (2).
- Local base models next to existing engine headers: see NOTES.md "Header debt". CBankFile.h, CTCPhysicsRigidBody.h,
  CTCBase.h, CPlayer.h, CPlayerGui.h, CSubtitleRenderer.h and CTCHeroStats.h are flattened and carry no methods.
- fable_string.h: CCharString(const char*, long) has no `= -1` default. The PDB ctor is used with implicit
  conversions in retail.
- Already-landed sources with bare or descriptive leaf names for PDB-named functions:
  - 0x0057B327 AddMoney;
  - 0x007CA9E0 GiveItems;
  - 0x009C95E0 GetTextBySymbol;
  - 0x004E34B2 CActiveFile::OnReadFinished (it is the vector dtor);
  - 0x00A61B10 CDirectInputDeviceLists stand-in.
  Respell them via NAME_FIXES.tsv.
