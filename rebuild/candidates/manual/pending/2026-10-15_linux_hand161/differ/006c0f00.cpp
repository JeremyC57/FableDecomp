// CTCBuyableHouse::CreateMainSignPost() -- create the main sign post (for sale / owned / rented def name) and keep it in MainSignPost
// Grade: DIFFER(109v109) -- scheduling only: retail forms `lea ecx,[esp+8]` before `push edi` for the temporary SetPItem and `lea ecx,[esi+24h]` before `push eax` for the member SetPItem; ours pushes first (the landed twin 0x006C0F70 has our order)
//
// Retail 0x006C0F00, 109 bytes. Callees: CTCBuyableHouse::CreateSignPost(const CCharString&, bool)
// @ 0x006C0970, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, SetPItem @ 0x00A01B90
// (the temporary, then the member), GetPItem @ 0x00A01B50, ~CBaseIntelligentPointer @ 0x00A01C10.
// Globals 0x013BAAF8 / 0x013BAB00 / 0x013BAB04 are static CCharString def names (for sale, owned,
// owned-and-rented). Vtable 0x01238C6C = CIntelligentPointer<CThing>.
// Shape: the landed CreateDressingSignPost (0x006C0F70) with `mainSign = true` and the def name
// chosen by a nested conditional on OwnedByPlayer (+0x11) / Rented (+0x12): the `push 1` is
// emitted before the selection and the three `push offset` arms meet at the call.

struct CThing;
struct CBaseObject;
struct CCharString { void* PStringData; };

struct CBaseIntelligentPointer
{
    virtual void v00();
    CBaseObject* m_pItem;                           // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                  // 0x00A01B50
    void SetPItem(CBaseObject* item);               // 0x00A01B90
};

template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer   // vtable 0x01238C6C for <CThing>
{
    virtual void v00();
    CIntelligentPointer() {}
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

extern const CCharString g_ForSaleSignPostDefName;    // 0x013BAAF8
extern const CCharString g_OwnedSignPostDefName;      // 0x013BAB00
extern const CCharString g_RentedSignPostDefName;     // 0x013BAB04

#pragma pack(push, 1)
struct CTCBuyableHouse
{
    void*         __vftable;                        // +0x00 (CTCBase)
    void*         ParentThing;                      // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x4];                   // +0x08 (CTCBase flags)
    void*         PDef_Object;                      // +0x0c CDefPointer<const CBuyableHouseDef>
    bool          IsScripted;                       // +0x10
    bool          OwnedByPlayer;                    // +0x11
    bool          Rented;                           // +0x12
    unsigned char _pad_0x13[0x11];                  // +0x13 days / dress levels
    CIntelligentPointer<CThing> MainSignPost;       // +0x24
    CIntelligentPointer<CThing> DressingSignPost;   // +0x2c

    CThing* CreateSignPost(const CCharString& defName, bool mainSign);   // 0x006C0970
    void CreateMainSignPost();
};
#pragma pack(pop)

void CTCBuyableHouse::CreateMainSignPost()
{
    MainSignPost = CIntelligentPointer<CThing>(OwnedByPlayer
        ? (Rented ? CreateSignPost(g_RentedSignPostDefName, true) : CreateSignPost(g_OwnedSignPostDefName, true))
        : CreateSignPost(g_ForSaleSignPostDefName, true));
}
