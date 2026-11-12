# Chunk 2040 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x005F6E40) A CCharString returned by an out-of-line call (CDefString::GetString 0x00415DD0) and pushed by reference: retail's
  `lea eax,<tmp> ; push eax` after the call, with the GetString result pointer pushed BEFORE the virtual GetInstantiationName call, is a
  NAMED local `CCharString name = def->GetInstantiationName().GetString(); v.push_back(name);` in its own block (destructor before the
  next statement). A temporary argument pushes eax; the implicit CDefString -> CCharString conversion re-addresses but evaluates the
  virtual call first. GetString's visibility (body / noinline / declaration) changes neither.
- (0x005F6E40) A CDefPointer filled through the hidden result of a SAME-TU callee (GetMiscThingsDef 0x005EE3B3, landed) releases in place
  without a null test (`mov ecx,[ebp-0Ch] ; dec dword ptr [ecx+4] ; jne`) only when the callee's body is visible in the TU; declared only,
  the local escapes and gets the long `add eax,4 ; dec [eax] ; cmp [eax],0` form plus the null test. A CDefPointer from an OTHER-TU
  callee (GetPDefFromInstantiationName 0x009ADA40) keeps the long form and the `and [ebp-0Ch],0` store in retail too.
- (0x005F6E40) In an optimize("s") TU c2 expands plain inlines only up to callee size 15 (smallest refused 22, refused without a score):
  CCharString::operator==(const char*) (sz 39) and CVectorMap::Find (sz 37) need the existing FORCEINLINE opt-ins
  (FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ, FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND) wherever retail expands them in such a TU.
- (0x005F6E40) `count = a > b ? b : a` merges both arms into one store; retail's two stores (`ja L2 ; mov [x],size ; jmp L3 ; ... L2: mov
  [x],eax`) are an if / else of assignments, and the arm after the `jmp` is where c2 parks earlier cold blocks (here the received-order
  `== ""` compare blocks): without that `jmp` they go after the epilogue.
- (0x005F6E40) `v.resize(misc->Count, NULL)` versus `unsigned long total = misc->Count; v.resize(total, NULL)`: identical instructions,
  but with the named count the list scheduler picks `lea eax,<tmp>` before the zero store (ties are broken by height + out-degree, then
  IL index; the store had an extra WAR edge to the misc reload into the zero register). Same lever as InitialiseAbilities 0x005EEDE2.
- (0x005F6E40) `Abilities[ReceivedAbilityOrder[i]]` read through a `std::pair<...>&` reference loads the def into a register before the
  null test; a named `EHeroAbility ability = ReceivedAbilityOrder[i];` indexed twice gives retail's `cmp [eax+4],esi ; je ; mov eax,[eax+4]`;
  indexing ReceivedAbilityOrder[i] at each use re-reads it.
- (0x005F6E40) A loop element used both by an inline `== ""` and by a later call argument is a named `const CCharString& name = v[i];`
  (retail homes the element address at [ebp-14h] and pushes it from there).
- (0x005F6E40) GetNumAssignableSpecialMoveButtons 0x005EDCF0 is called out of line in the save loop but its chain
  (PeekPlayerManager().GetMainPlayer().PlayerInterface->GetNumberOfAssignableSpecialMoveButtons()) sits in place in the load path:
  written out in the source: with its body visible in the TU, c2 refuses the call at every site (sz 20 > the size-TU limit 15;
  trace bin/inl/nbtn.txt), so the expanded copy cannot come from the member call.

Review at landing:
- NAME_FIXES.tsv: six callee spellings (0x005F7F4C, 0x005F8F89, 0x005F9BCD, 0x005F9DF8, 0x0043336A, 0x009AD390) replace invented
  stand-in classes / a pair outside std in reloc_bindings.tsv; the landed binders (e.g. 0x005F694D, 0x005EEDE2, 0x005EE3B3, 0x00CB5B90,
  0x0049B860, 0x004B2B70, 0x009D3FB0) need respelling with them.
- The row's TU emits the GetMiscThingsDef body (`?GetMiscThingsDef@CTCInventoryAbilities@@ABE?BV?$CDefPointer@$$CBVCUIMiscThingsDef@NUISystem@@@@XZ`,
  RELOCATION_MATCH against the landed 0x005EE3B3) and a GetDef<CUIMiscThingsDef> COMDAT (GETDEF_GLOBAL_BODY); the leaf is the
  OnSerialise symbol (Leaf line, checked with objdump through q.py / vland).
- Two new overlay opt-ins (hdr/README.md) to merge into rebuild/include/engine/CTCInventoryAbilities.h and CWorld.h.
- Two inline-budget devices (existing shared opt-ins) with the `// Inline-budget:` line and trace numbers in the source.
- The ~vector<CCharString> calls go to 0x00414EA0 (another TU's COMDAT; bound as ??1?$vector@VCCharString@@...); fine for the linker.
