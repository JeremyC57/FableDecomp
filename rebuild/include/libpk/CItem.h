// CItem family -- libpk's keyed items (the trigger / event items an item dispatcher indexes) and the dispatcher they attach to.
// Identity (descriptive; libpk has no PDB or RTTI): the factory CFactory::Create 0x00C1EFC0 builds ten item classes by type code
//   0xXYZZ.. (bound ctor names ??0CFactoryProduct_<code>): ZZ is the key kind each class's slot 4 returns (1 = long id, 2 = name
//   hash, 3 = named: hash + kept CString), and (X, Y) picks the 8- or 16-byte table member at +8 (ctors 0x00C35170 / 0x00C34AA0 /
//   0x00C34B30 / 0x00C34DD0). Every class is its own 9-slot vtable with its own copy of slots 3, 6 and 7 (one template instance
//   per class in the original), so each copy keeps its class name:
//     key 1 (long id): CKeyedItem 0x012B3498 (ctor 0x00C1F720, bound ??0CKeyedItem@@QAE@J@Z by CItemAction 0x00C1EE60),
//       CVtable_012b34bc 0x012B34BC (0x00C1F7E0), CVtable_012b34e0 0x012B34E0 (0x00C1F8A0), CVtable_012b3504 0x012B3504 (0x00C1F960);
//     key 2 (hash):    CHashedItem 0x012B3474 (0x00C1F640), CPointerBase_00c1fcc0 0x012B3528 (0x00C1FA20), CPointerBase_00c1fd60
//       0x012B354C (0x00C1FB00), CPointerBase_00c1fe00 0x012B3570 (0x00C1FBE0) -- the chunk 899 spellings;
//     key 3 (named):   CNamedItem 0x012B3600, CIntelligentPointer_00c1fcc0 / _00c1fd60 / _00c1fe00 0x012B3594 / 35B8 / 35DC (derived
//       from the key-2 classes; slots 6 / 7 inherited).
//   Slot map: 0 / 1 `lea eax,[ecx+4]` (the key), 2 the key accessor, 3 Validate (GetTable()->Validate()), 4 the key kind, 5 GetTable
//   (`lea eax,[ecx+8]`), 6 Attach(dispatcher), 7 Detach(dispatcher), 8 the deleting destructor. Slot 6 is the landed "Attach"
//   (CItemAction 0x00C1EE60 / 0x00C1EEE0 call it on every new item with the dispatcher); slot 7 is its inverse (the same list's
//   Remove).
// CItemDispatcher (struct, the bound spelling: ?FindItem@CItemDispatcher@@ 0x00C10140, ?Find@CItemDispatcher@@ 0x00C101D0) is itself
//   the id-keyed item list (its own slot 17 FindByKey answers FindItem) and keeps the name-hash index at +0x10 (Find by name and
//   RunAllItems 0x00C10400 use it): long-id items attach to the dispatcher's own list, hashed / named items to the +0x10 index;
//   both reach the PKCTRList slots 8 (Remove) and 9 (Add).
// CItemTable is chunk 899's table interface (vtable 0x012B3444; slot 9 = CItemTable::Validate 0x00C348F0).
#ifndef FABLE_LIBPK_CITEM_H
#define FABLE_LIBPK_CITEM_H
#include "libpk/PKCTRList.h"
#ifdef FABLE_RETAIL_CNAMEDITEM_CTOR
#include "engine/CString.h"
#endif

class CItemTable {                                  // the table interface (vtable 0x012B3444)
public:
    virtual unsigned long GetType() = 0;            // slot 0
    virtual long v01();
    virtual long v02();
#ifdef FABLE_RETAIL_CITEMTABLE_INSERT
    // NF65 overlay opt-in (row 0x00C1F4E0 CEventTriggerRef::InsertDriver): slot 3 (+0x0c) takes (index, pointer) and returns a long (negative
    // on failure); the trigger's insertion dispatcher forwards both arguments to it (0x00C1F4F7 `push edi ; push ebx ; mov ecx,eax ; call [edx+0Ch]`).
    // DESCRIPTIVE name (libpk has no PDB). Same slot position.
    virtual long Insert(unsigned long index, void* value);   // slot 3 (+0x0c)
#else
    virtual void v03();
#endif
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Validate();                        // slot 9 (+0x24): 0x00C348F0
};

struct CItemDispatcher;

struct CItem {                                      // the item root (empty inline destructor; its vtable store is dead)
    virtual void* v00();                            // slot 0
    virtual void* v01();                            // slot 1
    virtual const void* GetKey();                   // slot 2
    virtual void Validate();                        // slot 3 (+0x0c)
    virtual unsigned long GetKeyType();             // slot 4
    virtual CItemTable* GetTable();                 // slot 5 (+0x14)
    virtual void Attach(CItemDispatcher* dispatcher);   // slot 6 (+0x18)
    virtual void Detach(CItemDispatcher* dispatcher);   // slot 7 (+0x1c)
    virtual ~CItem() { }                            // slot 8
};

class CItemNameIndex : public PKCTRList<CItem> {   // the dispatcher's name-hash index (+0x10), a 16-byte concrete list
public:
    virtual int IsEqual(const void* a, const void* b) const;
    virtual int IsLess(const void* a, const void* b) const;
    virtual int IsKeyEqual(const void* key, const void* item) const;
    virtual int IsKeyLess(const void* key, const void* item) const;
    unsigned char m_impl[8];                        // +0x08 the embedded morphable container
};

struct CItemDispatcher : public PKCTRList<CItem> { // the id-keyed item list (+0) ...
#ifdef FABLE_RETAIL_CITEMDISPATCHER_FINDITEM
    // NF65 overlay opt-in (row 0x00C2E4D0): the id lookup 0x00C10140 (slot 17 FindByKey by key address), the spelling the landed
    // 0x00C1EE60 binds (?FindItem@CItemDispatcher@@QAEPAUCItem@@J@Z; descriptive, libpk has no PDB). Declaration only.
    CItem* FindItem(long key);                      // 0x00C10140
#endif
    unsigned char m_impl[8];                        // +0x08 its embedded morphable container
    CItemNameIndex m_names;                         // +0x10 ... and the name-hash index
};

struct CKeyedItem : public CItem {            // vtable 0x012B3498 (key 1)
    virtual void Validate();                        // slot 3
    virtual void Attach(CItemDispatcher* dispatcher);   // slot 6
    virtual void Detach(CItemDispatcher* dispatcher);   // slot 7
};

struct CVtable_012b34bc : public CItem {      // vtable 0x012B34BC (key 1)
    virtual void Validate();                        // slot 3
    virtual void Attach(CItemDispatcher* dispatcher);   // slot 6
    virtual void Detach(CItemDispatcher* dispatcher);   // slot 7
};

struct CVtable_012b34e0 : public CItem {      // vtable 0x012B34E0 (key 1)
    virtual void Validate();                        // slot 3
    virtual void Attach(CItemDispatcher* dispatcher);   // slot 6
    virtual void Detach(CItemDispatcher* dispatcher);   // slot 7
};

struct CVtable_012b3504 : public CItem {      // vtable 0x012B3504 (key 1)
    virtual void Validate();                        // slot 3
    virtual void Attach(CItemDispatcher* dispatcher);   // slot 6
    virtual void Detach(CItemDispatcher* dispatcher);   // slot 7
};

struct CHashedItem : public CItem {           // vtable 0x012B3474 (key 2)
#if defined(FABLE_RETAIL_CHASHEDITEM_CTOR) || defined(FABLE_RETAIL_CNAMEDITEM_CTOR)
    // NF65 overlay opt-in (row 0x00C1EEE0 CEventTriggerRef::CEventTriggerRef): the hash-keyed item's constructor (the landed self row's
    // spelling ??0CHashedItem@@QAE@K@Z, 0x00C1F640) and its 0x10-byte layout (0x00C1EEE0 `push 10h ; call operator new`): the key hash
    // +4 and the 8-byte table member +8 (slot 5 GetTable answers `lea eax,[ecx+8]`). Declaration only.
    CHashedItem(unsigned long hash);                // 0x00C1F640
    unsigned long m_hash;                           // +0x04
    unsigned char m_table[8];                       // +0x08
#endif
    virtual void Validate();                        // slot 3
    virtual void Attach(CItemDispatcher* dispatcher);   // slot 6
    virtual void Detach(CItemDispatcher* dispatcher);   // slot 7
};

struct CPointerBase_00c1fcc0 : public CItem { // vtable 0x012B3528 (key 2)
    virtual void Validate();                        // slot 3
    virtual void Attach(CItemDispatcher* dispatcher);   // slot 6
    virtual void Detach(CItemDispatcher* dispatcher);   // slot 7
};

struct CPointerBase_00c1fd60 : public CItem { // vtable 0x012B354C (key 2)
    virtual void Validate();                        // slot 3
    virtual void Attach(CItemDispatcher* dispatcher);   // slot 6
    virtual void Detach(CItemDispatcher* dispatcher);   // slot 7
};

struct CPointerBase_00c1fe00 : public CItem { // vtable 0x012B3570 (key 2)
    virtual void Validate();                        // slot 3
    virtual void Attach(CItemDispatcher* dispatcher);   // slot 6
    virtual void Detach(CItemDispatcher* dispatcher);   // slot 7
};

// key 3 (named) classes: derived from the key-2 ones, with their own slots 2 / 3 / 4 (chunk 899 landed their Validate).
struct CNamedItem : public CHashedItem {                      // vtable 0x012B3600
#ifdef FABLE_RETAIL_CNAMEDITEM_CTOR
    // NF65 overlay opt-in (row 0x00C1EEE0): the named item's constructor (the landed self row's spelling ??0CNamedItem@@QAE@ABVCString@@K@Z,
    // 0x00C20230) and the kept name +0x10 (0x14 bytes: 0x00C1EEE0 `push 14h ; call operator new`). Declaration only.
    CNamedItem(const CString& name, unsigned long hash);   // 0x00C20230
    CString m_name;                                 // +0x10
#endif
    virtual void Validate();                        // slot 3: 0x00C202C0
};

struct CIntelligentPointer_00c1fcc0 : public CPointerBase_00c1fcc0 {  // vtable 0x012B3594
    virtual void Validate();                        // slot 3: 0x00C1FD50
};

struct CIntelligentPointer_00c1fd60 : public CPointerBase_00c1fd60 {  // vtable 0x012B35B8
    virtual void Validate();                        // slot 3: 0x00C1FDF0
};

struct CIntelligentPointer_00c1fe00 : public CPointerBase_00c1fe00 {  // vtable 0x012B35DC
    virtual void Validate();                        // slot 3: 0x00C1FE90
};

#endif
