// CTCShopKeeper::HasBeenCustomer(CThing*) -- true when the thing is one of the three recent customers (array of CIntelligentPointer at +0x1c)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00798DD0, length 55 bytes (+9 int3 padding).
// Callee: CBaseIntelligentPointer::GetPItem @ 0x00A01B50.
// Shape: a counted loop over the three 8-byte slots, strength-reduced to a byte offset (`add esi,8 ; cmp esi,18h`); the
// vector's first pointer is re-read every iteration. The slots are a retail std::vector (first/last/end at +0x1c..+0x24,
// the ring index follows at +0x28); the element bound to a named REFERENCE local gives retail's
// `mov eax,[edi+1Ch] ; lea ecx,[esi+eax]` (calling through `m_recentCustomers[i].` directly gives `mov ecx ; add ecx,esi`).
// The found exit is `mov al,1`, the fall-through `xor al,al`.
class CBaseObject;
class CThing;

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

class CTCShopKeeper { public:
    unsigned char        _pad[0x1c];
    CRetailVector<CIntelligentPointer> m_recentCustomers;   // +0x1c: vector of three slots (first/last/end)
    bool HasBeenCustomer(CThing* thing);
};

bool CTCShopKeeper::HasBeenCustomer(CThing* thing)
{
    for (long i = 0; i < 3; ++i)
    {
        CIntelligentPointer& customer = m_recentCustomers[i];
        if (customer.GetPItem() == (CBaseObject*)thing)
            return true;
    }
    return false;
}