// CTCShopKeeper::AddNewCustomer(CIntelligentPointer) -- remembers a customer in the three-slot ring of recent customers (vector at +0x1c, next slot +0x28, wrapping to 0 after slot 2)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00798D90, length 63 bytes (+1 int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, SetPItem @ 0x00A01B90, ~CBaseIntelligentPointer @ 0x00A01C10
// (the by-value parameter is destroyed by the callee, `lea ecx,[esp+10h]` = its argument slot).
// Shape: `ret 8` = one 8-byte CIntelligentPointer by value; the slots are a retail std::vector (first/last/end at +0x1c,
// ring index at +0x28). The vector's first pointer is read first (ebx), the index second (edi), the incremented index
// stored back before the copy; the element address (`lea ecx,[ebx+edi*8]`) is formed AFTER the GetPItem call only when
// the copy is written `slot.SetPItem(customer.GetPItem())` (the inline operator= binds `this` first and computes it
// before the call); the wrap test re-reads the member.
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
    long                 m_nextCustomerSlot;     // +0x28
    void AddNewCustomer(CIntelligentPointer customer);
};

void CTCShopKeeper::AddNewCustomer(CIntelligentPointer customer)
{
    m_recentCustomers[m_nextCustomerSlot++].SetPItem(customer.GetPItem());
    if (m_nextCustomerSlot == 3)
        m_nextCustomerSlot = 0;
}
