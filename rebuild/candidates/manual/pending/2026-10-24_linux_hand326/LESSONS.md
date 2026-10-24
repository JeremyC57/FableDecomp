# Chunk 326 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A 1 KB body made of repeated inline `if (HasTC(t)) GetTC(t)->F();` blocks exceeds VC7.1's auto-inline budget: HasTC,
  GetInterface and GetTC stay out of line (776 v 1100) until each is `__forceinline` (0x0072B420 matched on that alone).
- A local STLport vector whose finish pointer retail keeps in a register across unknown calls, or whose first push_back
  on the freshly zeroed vector goes straight to the grow call, means every callee that receives the vector (out-of-line
  push_back, `_M_insert_overflow`, a by-reference consumer) was visible earlier in the TU: same-TU `auto_inline(off)`
  stand-ins that READ their inputs and write only the vector's own words (never through the position pointer) reproduce
  it (0x007FC210: grow call and esi-held finish; 0x006276F0: the edi-held finish, the late `push ebx` and the frame).
  A stand-in that stores through `position` is inert (the store may alias anything).
- When retail expands an inline member at one site and calls its out-of-line copy (another catalog address) at the
  others, define the member above under `auto_inline(off)` and give the expanded site a `__forceinline` copy
  (0x006276F0: push_back expanded only in the switch default).
- IS_THING_ALIVE 0x0040DCE0 takes `CBaseObject* const&`: a `CThing*` local binds through a converted temporary (the thing
  stays in esi, the temporary lands in a dead argument slot) while a `CBaseObject*` local -- GetPItem's PDB return type --
  binds directly and lives in its frame slot (0x005CB2B0).
- `push_back(list)` with a `CList*` local into a `vector<CComponent*>` binds a converted temporary, so the list stays in a
  callee-saved register; a `CComponent*` local is address-taken and spilled (0x00628000).
- Any model class that declares virtuals over a non-polymorphic base shifts the base's members by 4 (the derived vptr
  goes first): declare the virtual slots in the base at +0 (0x007FC210 CThingAICreature over CThing, 0x00628000
  CFrontEndManager over CManager). A flat CTCBase model needs its 2 pad bytes (+0x0A) before a bool at +0x0C.
- `MIN(v.size() - 1, x)` as a macro over an inline `size()` loads finish first and leaves the re-evaluated arm to reuse
  only the start register (0x007FC210); the open-coded `finish - start` form loads start first and CSEs both arms.
- An STLport map walk (`for (n = header->left; n != header; n = _M_increment(n))` with a static inline increment and
  end() re-read from the member) matches as written (0x00766DD0 both sound maps); `T* tc = 0;` declared before the thing
  local it is looked up on moves tc from ecx to ebx.
- A named `CWideString winPhrase = GetWinPhrase(); winPhrase != L"";` (not the bare temporary) gives retail's `lea`
  re-addressing and its own frame word (0x005ECA50).

Review at landing:
- 0x0072B420 binds `?CancelMove@CTCWillEnflame@@` to 0x007A44C0 while landed 0x007C6A40 binds the same symbol to itself;
  0x007C6A40 should become CTCTurncoatSpell::CancelMove (see NOTES Label corrections). It also binds
  CTCBattleCharge::CancelMove (PDB) to 0x007A7D10 (landed "CancelCharge") and CTCForcePushPower::CancelMove to 0x007A5420
  (landed "NotifyOwnerProperty150"): expect cross-row RELOC_CONFLICT warnings until the landed names follow.
- 0x0072B420 declares callee-only stubs `struct CTCBulletTime / CTCTurncoatSpell / CTCWillLightning { void CancelMove(); }`
  next to existing engine headers that carry no methods (SHARED_HEADER_AVAILABLE warnings, same as the landed
  0x0072BA40 / 0x0072BC60); the stubs never model `this`.
- 0x006276F0 is one source change from a likely match (fetch the text bank through an inline accessor); it was parked only
  because of the four-grade cap.
- Three parked rows carry same-TU stand-ins in differ/ (0x006276F0, 0x007FC210, 0x005ECA50): check them against
  the landed callees before reuse.
- No row holds a second function; 33 rows are decode-only parks (no graded source), 11 of them x87-heavy.
