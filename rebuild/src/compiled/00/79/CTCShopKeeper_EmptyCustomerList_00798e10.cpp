// CTCShopKeeper::EmptyCustomerList() -- clears the three recent-customer slots (vector of CIntelligentPointer at +0x1c) by copying a null temporary into each
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00798E10, length 89 bytes (+7 int3 padding).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, GetPItem @ 0x00A01B50, SetPItem @ 0x00A01B90,
// ~CBaseIntelligentPointer @ 0x00A01C10; the temporary's vtable is the CIntelligentPointer copy 0x0123E7D8.
// Shape: one temporary per iteration (constructed, SetPItem(0), copied into the slot, destroyed); the slot index is
// strength-reduced to a byte offset. The slots are a retail std::vector (first at +0x1c); `slot.SetPItem(tmp.GetPItem())`
// keeps the vector's first pointer loaded before the GetPItem call and forms the element address after it. VC7.1 aligns
// this frame (`and esp,-8`) by itself.
struct CBaseObject;
struct CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                  // +4
    CBaseIntelligentPointer();             // 0x00A01B10
    ~CBaseIntelligentPointer();            // 0x00A01C10
    CBaseObject* GetPItem() const;         // 0x00A01B50
    void SetPItem(CBaseObject* item);      // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

// Retail std::vector (12 bytes: first/last/end) with an iterator OBJECT: `*(begin() + n)` is not folded into a plain
// `first[n]`, which gives retail's `mov eax,[first] ; lea ecx,[off+eax]` element address.
template<class T> struct CRetailVector {
    T* first; T* last; T* end_of_storage;
    struct iterator {
        T* p;
        explicit iterator(T* q) : p(q) {}
        iterator operator+(long n) const { return iterator(p + n); }
        T& operator*() const { return *p; }
    };
    iterator begin() { return iterator(first); }
    T& operator[](unsigned long n) { return *(begin() + n); }
};

struct CTCShopKeeper {
    unsigned char        _pad[0x1c];
    CRetailVector<CIntelligentPointer> m_recentCustomers;   // +0x1c: vector of three slots (first/last/end)
    void EmptyCustomerList();
};

void CTCShopKeeper::EmptyCustomerList()
{
    for (long i = 0; i < 3; ++i) {
        CIntelligentPointer empty;
        empty.SetPItem(0);
        m_recentCustomers[i].SetPItem(empty.GetPItem());
    }
}