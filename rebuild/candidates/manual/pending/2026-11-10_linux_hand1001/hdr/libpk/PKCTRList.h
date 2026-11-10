// PKCTRList<T> -- libpk's typed list front (PKCTRList.h): an abstract class template over the item type T that keeps a
// pointer (+4) to its container implementation (the morphable PKCTRMorphable an owner embeds, which switches between a
// PKCTRRedblackTree and a PKCTRArray) and forwards every list operation to it, passing itself as the comparator where the
// container needs one. Concrete lists derive from it (keyed / unkeyed comparators, TransferAllFrom slot 16, FindByKey 17).
// Identity (descriptive; libpk has no PDB or RTTI):
//   - CTypedListBase is the one root vtable 0x012B1AB0 every list shares (four pure comparator slots + the virtual
//     destructor; landed name from CItemList::~CItemList 0x00C09B70);
//   - PKCTRList<T> is the 16-slot abstract vtable each list instance gets (27 copies in 0x012B1BC0 .. 0x012B7EA8, slots
//     0..3 _purecall, slot 4 its deleting destructor, slots 5..15 these forwarders; one copy per item type T, so the
//     forwarders are per-instance COMDATs at different addresses). Its constructor is the 10-byte `vtable ; m_pImpl = impl`
//     (0x00C09DE0 / 0x00C09E20). The class name comes from the trace file of the family's unkeyed comparators: the concrete
//     lists' key slots 2 / 3 report "d:\dev\audio\ver_fable_1.5_b\libpk\PKCTRList.h(39) : Error: Cannot resolve access to a
//     key object in a list!" (0x00C0A250 / 0x00C0A300, vtable 0x012B1C98); libpk names its classes after their files
//     (PKCTRArray.cpp, PKCTRMorphable.cpp, PKCTRRedblackTree.cpp).
//   - PKCContainer_00c17b70 is the landed spelling of the container interface (PKCTRMorphable::MorphToArray 0x00C18870,
//     MoveAllItems 0x00C17B70): PKCTRArray 0x012B4E80, PKCTRRedblackTree 0x012B2F0C and the morphable 0x012B2DEC share it.
// Slot map (list -> container): 5 MorphToArray(this) -> 2, 6 MorphToTree(this) -> 3, 7 Find -> 7, 8 Remove -> 6,
//   9 Add -> 5, 10 ForEach -> 13 (through a stack visitor adapter), 11 GetFirst -> 11, 12 GetLast -> 12,
//   13 GetItemByIndex -> 9, 14 Reserve -> 4, 15 GetCount -> 10 (the landed TransferAllFrom rows read slots 11 / 15 / 8 / 9
//   of a list and 4 / 10 of its container exactly so).
#ifndef FABLE_LIBPK_PKCTRLIST_H
#define FABLE_LIBPK_PKCTRLIST_H

class PKCContainer_00c17b70 {                   // libpk container interface (morphable 0x012B2DEC, tree 0x012B2F0C, array 0x012B4E80)
public:
    virtual int IsArray();                                      // slot 0
    virtual int IsTree();                                       // slot 1
    virtual void MorphToArray(void* comparator);                // slot 2
    virtual void MorphToTree(void* comparator);                 // slot 3
    virtual void Reserve(long count);                           // slot 4 (+0x10)
    virtual void Add(void* owner, void* item);                  // slot 5 (+0x14)
    virtual void Remove(void* owner, void* item);               // slot 6 (+0x18)
    virtual void* Find(void* comparator, const void* item);     // slot 7 (+0x1c)
    virtual void* FindByKey(void* comparator, const void* key); // slot 8 (+0x20)
    virtual void* GetItemByIndex(long index);                   // slot 9 (+0x24)
    virtual long GetCount();                                    // slot 10 (+0x28)
    virtual void* GetFirst();                                   // slot 11 (+0x2c)
    virtual void* GetLast();                                    // slot 12 (+0x30)
    virtual void ForEach(void* visitor);                        // slot 13 (+0x34)
    virtual ~PKCContainer_00c17b70();                           // slot 14
};

class CTypedListBase {                          // vtable 0x012B1AB0: the item comparators every list supplies
public:
    virtual int IsEqual(const void* a, const void* b) const = 0;        // slot 0
    virtual int IsLess(const void* a, const void* b) const = 0;         // slot 1
    virtual int IsKeyEqual(const void* key, const void* item) const = 0;// slot 2
    virtual int IsKeyLess(const void* key, const void* item) const = 0; // slot 3
    virtual ~CTypedListBase() { }                                       // slot 4
};

template <class T> class PKCTRList : public CTypedListBase {
public:
    explicit PKCTRList(PKCContainer_00c17b70* impl) : m_pImpl(impl) { }
    virtual void MorphToArray();                // slot 5
    virtual void MorphToTree();                 // slot 6
    virtual T* Find(const T* item);             // slot 7
    virtual void Remove(T* item);               // slot 8 (+0x20)
    virtual void Add(T* item);                  // slot 9 (+0x24)
    virtual void ForEach(void* visitor);        // slot 10
    virtual T* GetFirst();                      // slot 11 (+0x2c)
    virtual T* GetLast();                       // slot 12 (+0x30)
    virtual T* GetItemByIndex(long index);      // slot 13 (+0x34)
    virtual void Reserve(long count);           // slot 14 (+0x38)
    virtual long GetCount();                    // slot 15 (+0x3c)
protected:
    PKCContainer_00c17b70* m_pImpl;             // +0x04 the container implementation (usually the owner's embedded morphable)
};

#endif
