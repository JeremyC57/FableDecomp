# agent1001 header overlay (chunk 1001)

Three NEW shared headers under `libpk/` (no existing repo header is changed, so no `FABLE_RETAIL_*` opt-in is needed: nothing
includes them until a row does). Grade with `FABLE_HDR_OVERLAY=$A/hdr`. At landing they go to `rebuild/include/libpk/`.

## libpk/PKCTRList.h (145 rows)
- `PKCContainer_00c17b70` -- the container interface, in the landed spelling of PKCTRMorphable::MorphToArray 0x00C18870 /
  MoveAllItems 0x00C17B70 (same slot names: IsArray, IsTree, MorphToArray, MorphToTree, Reserve, Add, Remove, Find, FindByKey,
  GetItemByIndex, GetCount, GetFirst, GetLast, ForEach, virtual destructor).
- `CTypedListBase` -- the one root list vtable 0x012B1AB0 (four pure comparators + virtual destructor; landed name from
  CItemList::~CItemList 0x00C09B70).
- `template <class T> class PKCTRList : public CTypedListBase` -- the 16-slot abstract list front of every list instance (29
  vtables, 0x012B1BC0 .. 0x012B7EA8), `m_pImpl` at +4, slots 5..15 declared (the five forwarders 11..15 are this chunk's rows;
  each row defines its member template body and explicitly instantiates it for its own item type).
- Evidence for the class name and the slot map: the comment block at the top of the header.

## libpk/CItem.h (24 rows)
- `CItemTable` (chunk 899's table interface, vtable 0x012B3444), `CItem` (the 9-slot item root), the dispatcher model
  `CItemDispatcher : PKCTRList<CItem>` (its own id-keyed list at +0, `CItemNameIndex m_names` at +0x10) and the eight key-1 / key-2
  item classes whose slots 3 / 6 / 7 are this chunk's rows (CKeyedItem, CVtable_012b34bc, CVtable_012b34e0, CVtable_012b3504,
  CHashedItem, CPointerBase_00c1fcc0, CPointerBase_00c1fd60, CPointerBase_00c1fe00), plus the key-3 classes chunk 899 landed
  (CNamedItem, CIntelligentPointer_00c1fcc0 / _00c1fd60 / _00c1fe00).
- Includes libpk/PKCTRList.h. Chunk 899's landed Validate rows (0x00C1FD50 / FDF0 / FE90 / 0x00C202C0) carry local copies of the
  same model (struct keys and slot names identical) and can be rebased onto this header without a byte change.

## libpk/CVtable_012b4f60.h (3 rows)
- `CVtable_012b4f60` -- the 12-byte file handle over an outside file manager (vtable 0x012B4F60; landed ctor 0x00C330A0, its
  slot 3 0x00C33050 Close), `CUnprovenFileObject_012b4f60` (the wrapped file object's four slots) and a forward declaration of the
  adapter `CVtable_012b3224` (landed Create 0x00C1B8B0). Slot names from the sibling default-file handle CVtable_012b4f4c
  (0x00C32F40 GetSize, 0x00C32F60 Read, 0x00C32F30 GetUnitSize).
