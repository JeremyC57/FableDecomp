// CTCTavernTable::GetFood(unsigned long index) -- returns the CThingObject held by the index-th intelligent pointer of Food (vector at +0x28), or 0 past the end
// Leaf: GetFood
// Retail: Fable.exe @ 0x007B85C0, 38 bytes (`ret 4`).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (thiscall on the 8-byte element).
// Layout: Ego_r.pdb CTCTavernTable with retail 12-byte vectors: Occupants +0x10, Tankards +0x1C, Food
// +0x28 (PDB +0x10/+0x20/+0x30) -- no engine header. The manifest labels both this body and its
// twin 0x007B8590 "GetFood"; the +0x1C copy reads Tankards, the +0x28 copy Food.
// Shape: `size() > index` in that operand order (`cmp edx,eax ; jbe`, unsigned count from `sar edx,3`);
//        the element is reached through the retail vector's iterator object, so `first` is
//        re-loaded after the check (the landed CRetailVector model of 0x004AF3C0).
class CThingObject;

struct CIntelligentPointer {            // 8 bytes: vptr + item
    void* __vftable;
    CThingObject* m_pItem;
    CThingObject* GetPItem() const;     // 0x00A01B50 (CBaseIntelligentPointer)
};

template<class T> struct CRetailVector {
    T* first; T* last; T* end_of_storage;
    struct iterator {
        T* p;
        explicit iterator(T* q) : p(q) {}
        iterator operator+(long n) const { return iterator(p + n); }
        T& operator*() const { return *p; }
    };
    iterator begin() { return iterator(first); }
    unsigned long size() const { return (unsigned long)(last - first); }
    T& operator[](unsigned long n) { return *(begin() + n); }
};

struct CTCTavernTable {
    void* __vftable;                                    // +0x00
    unsigned char _pad_0x04[0x8];                       // +0x04 CTCBase
    bool BeingServed;                                   // +0x0c
    CRetailVector<void*> Occupants;                     // +0x10 vector<CThingAICreature*>
    CRetailVector<CIntelligentPointer> Tankards;        // +0x1c
    CRetailVector<CIntelligentPointer> Food;            // +0x28
    CThingObject* GetFood(unsigned long index);
};

CThingObject* CTCTavernTable::GetFood(unsigned long index)
{
    if (Food.size() > index)
        return Food[index].GetPItem();
    return 0;
}