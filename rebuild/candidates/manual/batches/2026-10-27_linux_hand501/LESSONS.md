# Chunk 501 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Gap-scan script rows are named by their vtable slot. Run xref.py on the row address to get the .rdata slot, then
  agent499's own.py on the slot to get the vtable start and the Alloc or constructor that stores it. Level scripts with
  no landed constructor are named by a unique Ego_r layout match plus their Main's strings. 37 of 40 rows matched on the
  first grade with agent499's models.
- Example 0x00EAE5A0: CScriptThing::operator== must be `PImp->IsEqualTo(&*other.PImp)`, using CCountedPointer's inline
  operator-> and operator*. Reading `.Data` swaps eax/edx in the slot-78 call and keeps &Me in ebp. This is the landed
  0x00D9EC50 lesson.
- Example 0x00E38D80: an OnPersist that calls out-of-line CPersistContext::GetDefaultVal<CCharString> 0x0040E1E0 but
  stores an enum default inline uses the two-argument Transfer(name, value).
  - Declare GetDefaultVal as a protected static __fastcall template, declaration only, above Transfer.
  - Add an inline explicit specialization for the enum.
  - A generic body plus a CCharString specialization makes VC7.1 fail with C1001.
- Example 0x00E7DDE0: a Clone that loads a member before the vtable store is `new T(*Member)`, through the member's
  constructor. `new T(*this)`, the implicit copy, stores the vtable first.
- Example 0x00EAE820: STLport push_back of a polymorphic element: placement new over the implicit copy stores only the
  most-derived vtable. A by-value interface argument with a user copy constructor stores both vtables (0x00E04B50).
  `__false_type` lives in namespace std (`ABU__false_type@2@`).
- pGlobalScriptDef: the retail offset is the Ego_r offset minus 4 per Dinkumware vector before the member. Examples:
  TollBanditFee 0x464 -> 0x430; BooksNumberWanted 0x524 -> 0x4e0; HeroKillsSheriffMorality 0x764 -> 0x708.
- To get an exact Leaf before spending a grade, compile only (cc.py: V.cl plus objdump) and put several variants in one
  probe. Gall/q.py report MATCH even for a wrong Leaf, so check each Leaf against `objdump -t` `(ty 20)` names.

Review at landing:
- Apply the label corrections in NOTES.md. In particular, the landed CVtable_012EC138 and CVtable_012e5ff0
  constructors and the free GetDefaultVal<CCharString> 0x0040E1E0 have wrong names.
- The binding `?erase@CScriptThingVector@@...` (0x00CD3152) and the landed CConversation copy constructor
  (CRetailVector_CCharString, CDefClassBase_Retail) use invented stand-ins.
- SHARED_HEADER_AVAILABLE warnings remain: CGameScriptInterface on 17 rows, CScriptThing on 3. The local polymorphic
  models stay until the headers model the vptr.
- No row needed more than 2 grades. Four rows used 2: 0x00EAE5A0, 0x00E38D80 (the first was a C1001 SRC_FAIL),
  0x00E7DDE0 and 0x00EAE820 (a symbol-only change).
