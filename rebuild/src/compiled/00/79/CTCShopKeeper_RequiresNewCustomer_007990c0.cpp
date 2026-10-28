// CTCShopKeeper::RequiresNewCustomer() -- true when there is no current customer (+0xc), when the current customer is the shopkeeper's own owner (+4), or when the current customer no longer passes the potential-customer filter
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007990C0, length 85 bytes (+11 int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThingFilter_IsPotentialCustomer::CThingFilter_IsPotentialCustomer
// (CTCShopKeeper*, bool) @ 0x00799D80 and its operator()(CThing*) @ 0x00799E30 (bool).
// Shape: the 0x18-byte filter is a stack object with an out-of-line constructor and no destructor (its constructor
// stores a plain word at +0, no vtable); `return !filter(customer)` gives `neg al ; sbb eax,eax ; inc eax`; the owner
// is read into a local before the second GetPItem call (ebx, pushed inside the branch); the two early-true tests are
// written as nested `!=` ifs with ONE `return true` at the end (two `if (..) return true;` place the `mov al,1` exit in
// the middle of the body instead of after it).
class CBaseObject;
class CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                  // +4
    CBaseIntelligentPointer();             // 0x00A01B10
    virtual ~CBaseIntelligentPointer();            // 0x00A01C10
    CBaseObject* GetPItem() const;         // 0x00A01B50
    void SetPItem(CBaseObject* item);      // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

class CTCShopKeeper;

class CThingFilter_IsPotentialCustomer { public:
    unsigned char m_state[0x18];
    CThingFilter_IsPotentialCustomer(CTCShopKeeper* shopKeeper, bool checkShop);   // 0x00799D80
    bool operator()(CThing* thing);                                                  // 0x00799E30
};

class CTCShopKeeper { public:
    void*               m_vtable;            // +0
    CThing*             m_pOwner;            // +4
    unsigned char       _pad8[4];
    CIntelligentPointer m_currentCustomer;   // +0xc
    bool RequiresNewCustomer();
};

bool CTCShopKeeper::RequiresNewCustomer()
{
    if (m_currentCustomer.GetPItem() != 0) {
        CThing* owner = m_pOwner;
        if ((CThing*)m_currentCustomer.GetPItem() != owner) {
            CThingFilter_IsPotentialCustomer filter(this, true);
            return !filter((CThing*)m_currentCustomer.GetPItem());
        }
    }
    return true;
}