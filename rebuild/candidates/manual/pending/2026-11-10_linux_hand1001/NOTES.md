# agent1001 (chunk 1001) notes

Result: the 38 gap-scan rows are 172 functions (not 38): 29 rows x 5 PKCTRList forwarders (145), 8 rows x 3 item slots (24), 1 row x 3
file-handle forwarders (3). Every function was split (SPLITS.tsv, oracle via split_oracle.py), authored as its own src/ + test/
pair and graded MATCH on its first grade with its full leaf confirmed in the object (`objdump -t`). Grade with
`FABLE_HDR_OVERLAY=$A/hdr` (all 172 rows include one of the three new libpk headers). All names are descriptive (libpk: no PDB, no
RTTI) under the owner's vendor-name rule.

## Splits
- 29 x 72-byte rows (0x00C09C40 .. 0x00C477B0): five 8-byte functions at +0x00 / +0x10 / +0x20 / +0x30 / +0x40 (8 int3 between).
- 8 x 67 / 47-byte rows (0x00C1F6D0 / FAB0 / FB90 / FC70: 12 + 19 + 19 bytes at +0 / +0x10 / +0x30; 0x00C1F7B0 / F870 / F930 /
  F9F0: 12 + 15 + 15 bytes at +0 / +0x10 / +0x20).
- 0x00C33070 (40 bytes): 7 + 8 + 8 bytes at +0 / +0x10 / +0x20.
- Every length in SPLITS.tsv is the code only (last byte = the tail `jmp`'s), int3 padding excluded (chunk 899's 0x00C3EC10 rule).
  Each function's start is proven by its own `.rdata` vtable cell(s) (the evidence column).

## Identities proven
### PKCTRList<T> forwarders (libpk/PKCTRList.h), 145 rows
- Each 72-byte row is slots 11..15 of one list instance's ABSTRACT vtable (16 slots: 0..3 _purecall, 4 its deleting destructor,
  5..15 forwarders to the +4 container implementation). 29 abstract vtables, one per row (0x012B1BC0, 0x012B1C00, 0x012B2248, ...,
  0x012B7EA8); each is inherited unchanged by 1..8 concrete 17/18-slot list vtables (same cells at slots 11..15), which add their
  own comparators (slots 0..3), TransferAllFrom (16) and FindByKey (17). The concrete lists of one abstract differ in their
  comparators, so the abstract class is templated on the item type alone and the concrete ones on (item, key getter): one
  `PKCTRList<T>` per item type, hence the per-instance copies of byte-identical bodies.
- Slot -> container slot (read from the bytes): 11 GetFirst -> 11 (+0x2C), 12 GetLast -> 12 (+0x30), 13 GetItemByIndex(long) -> 9
  (+0x24), 14 Reserve(long) -> 4 (+0x10), 15 GetCount -> 10 (+0x28). The container slot names are the landed PKCContainer_00c17b70
  ones (PKCTRRedblackTree 0x012B2F0C: 9 GetItemByIndex 0x00C190F0, 10 GetCount 0x00C19130, 11 GetFirst 0x00C19150, 12 GetLast
  0x00C19180; Reserve is the landed MoveAllItems 0x00C17B70 / TransferAllFrom slot-4 call). The landed TransferAllFrom rows (slot 16
  of the concrete lists, e.g. 0x00C09C90) call exactly these list slots (11 GetFirst, 15 GetCount) on another list, which fixes the
  list-side names.
- Class name `PKCTRList` (descriptive, from the trace file): the family's unkeyed concrete lists report
  "d:\dev\audio\ver_fable_1.5_b\libpk\PKCTRList.h(39) : Error: Cannot resolve access to a key object in a list!" from their key
  comparator slots (0x00C0A250 / 0x00C0A300 in vtable 0x012B1C98, an instance of the 0x012B1C00 abstract); libpk names classes
  after their files (PKCTRArray, PKCTRMorphable, PKCTRRedblackTree). Root `CTypedListBase` = the landed name of 0x012B1AB0.
- Item type T: `CListItem` (struct) for the abstract 0x012B1BC0 -- its concrete CItemList 0x012B1B78 is the list the landed
  CItemDispatcher::RunAllItems 0x00C10400 / CCollectAllItems::Visit(CListItem*) 0x00C10720 fill, and ??0CItemAction@@QAE@PAUCItemDispatcher@@PAUCListItem@@@Z
  binds the same item. The other 28 item types are unproven: `CUnprovenListItem_<abstract vtable>` (one per instance).
### Item slots (libpk/CItem.h), 24 rows
- 0x00C1F6D0 / FAB0 / FB90 / FC70 and 0x00C1F7B0 / F870 / F930 / F9F0: slots 3 / 6 / 7 of the eight key-1 and key-2 item vtables
  (each function has its own vtable's cell; slots 6 / 7 of the key-2 classes are also inherited by their key-3 subclasses CNamedItem
  0x012B3600 / CIntelligentPointer_00c1fcc0 / _00c1fd60 / _00c1fe00 0x012B3594 / 35B8 / 35DC).
- Slot 3 Validate (`GetTable()->Validate()`, chunk 899's name and shape). Slot 6 Attach(CItemDispatcher*) -- the landed name: both
  CItemAction constructors (0x00C1EE60 / 0x00C1EEE0) call slot 6 with the dispatcher on every item they create. Slot 7 Detach: the
  same list's Remove (list slot 8). Key-1 (long id) items use the dispatcher's own list (`mov edx,[ecx]` on the argument); key-2
  (hash) items the list at +0x10 (`mov edx,[eax+10h] ; add eax,10h`), which is where the landed CItemDispatcher::Find (by name,
  0x00C101D0) and FindItem (by id, slot 17 on +0, 0x00C10140) look.
- Factory type codes (CFactory::Create 0x00C1EFC0 bindings ??0CFactoryProduct_<code>): key 1 = 0x10101 CKeyedItem, 0x20101
  CVtable_012b34bc, 0x10201 CVtable_012b34e0, 0x20201 CVtable_012b3504; key 2 = 0x10102 CHashedItem, 0x20102 CPointerBase_00c1fcc0,
  0x10202 CPointerBase_00c1fd60, 0x20202 CPointerBase_00c1fe00 (the low byte = slot 4's key kind, the other digits = the table member
  type built at +8: 0x00C35170 / 0x00C34AA0 / 0x00C34B30 / 0x00C34DD0).
- Class names: CHashedItem and CPointerBase_00c1fcc0 / _00c1fd60 / _00c1fe00 are chunk 899's spellings (as the brief asks).
  CKeyedItem is the landed caller binding of ctor 0x00C1F720 (NAME_FIXES: its own row still says CVtable_012b3498). The other three
  key-1 classes keep their only landed non-factory names, CVtable_012b34bc / _34e0 / _3504 (their ctors 0x00C1F7E0 / F8A0 / F960).
### File handle (libpk/CVtable_012b4f60.h), 3 rows
- 0x00C33070 / 80 / 90 = slots 0 / 1 / 2 of 0x012B4F60 (landed ctor 0x00C330A0, only caller CVtable_012b3224::Create 0x00C1B8B0,
  the outside-file-manager adapter CSoundSystem builds): GetSize(unsigned long*), Read(count, unused, a, b, c, d), GetUnitSize(),
  forwarding to the wrapped file object (+4). Slot names and signatures are the sibling default-file handle CVtable_012b4f4c's
  (0x00C32F40 / 0x00C32F60 / 0x00C32F30, landed); slot 3 is Close (0x00C33050, NAME_FIXES).

## Callee names (bound spellings used)
- None: every row is a tail-jumped virtual call (no relocations). The virtual targets are named in each row's comments.

## Name corrections (NAME_FIXES.tsv)
- 0x00C33050 CEmitterOwner_00c33050::Validate -> CVtable_012b4f60::Close; 0x00C1B930 CTCDParticleEmitter::EditFrameUpdate /
  CEmitter_00c33050::EditFrameUpdate -> CVtable_012b3224::DestroyHandle; 0x00C1F720 CVtable_012b3498 -> CKeyedItem (and its vtable
  symbol); vtable symbols 0x012B3474 -> ??_7CHashedItem@@6B@, 0x012B3528 -> ??_7CPointerBase_00c1fcc0@@6B@.
- Recommended rename wave (not in NAME_FIXES.tsv because the exact parameter / return types of those slots are not proven by a
  graded body): sibling_names.tsv lists slots 5..10 of all 29 abstract lists with the proposed `PKCTRList<T>::MorphToArray /
  MorphToTree / Find / Remove / Add / ForEach` names. 87 of them are landed as `CTCCreatureNavigation::IsNewDestinationGoingToReset
  Navigation` (a game class propagated onto libpk forwarders) and the rest as gapscan / `CCaller_<addr>::Call_<addr>` placeholders.
  The ctors 0x00C1FA20 / FB00 / FBE0 (landed CVtable_012b3528 / 354C / 3570) build chunk 899's CPointerBase_00c1fcc0 / fd60 / fe00.

## Header debt
- New: libpk/PKCTRList.h, libpk/CItem.h, libpk/CVtable_012b4f60.h (hdr/README.md). Chunk 899's landed Validate rows, CItemList's
  ctor/dtor, the TransferAllFrom rows, RunAllItems / CItemAction and the CVtable_012b4f4c rows all carry local copies of these
  models and can be rebased on the headers.
- The 28 unproven list item types need the owning classes of each list instance (the concrete lists' storers: CSoundSystem,
  CSoundEventSet, PKCDeferredBank, CEventScript, ...); naming them is a separate pass.

## Parked
- None.
