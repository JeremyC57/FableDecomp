// CTCBuyableHouse::CreateDressingSignPost() -- create the dressing sign post thing (by its global def name, not as the main sign) and keep it in DressingSignPost
//
// Retail 0x006C0F70, 81 bytes. Callees: CTCBuyableHouse::CreateSignPost(const CCharString&, bool)
// @ 0x006C0970, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, SetPItem @ 0x00A01B90
// (twice: the temporary, then the member), GetPItem @ 0x00A01B50, ~CBaseIntelligentPointer
// @ 0x00A01C10. Global 0x013BAAFC is a static CCharString (the sign-post def name; its storage is
// in the image's zero-fill area, built by a dynamic initialiser). Vtable 0x01238C6C =
// CIntelligentPointer<CThing>. DressingSignPost is +0x2c (Ego_r.pdb CTCBuyableHouse).
// Shape: `DressingSignPost = CIntelligentPointer<CThing>(created);` -- the call result is kept in edi,
// the temporary lives in the 8-byte frame (base ctor, its vtable, SetPItem), the inline assignment
// is SetPItem(other.GetPItem()) on the member, and the temporary's destructor is the base one (the
// derived pointer template declares no destructor, so no vtable store on the way out).

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

extern const CCharString g_DressingSignPostDefName;   // 0x013BAAFC

#pragma pack(push, 1)
struct CTCBuyableHouse
{
    void*         __vftable;                        // +0x00 (CTCBase)
    void*         ParentThing;                      // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x4];                   // +0x08 (CTCBase flags)
    void*         PDef_Object;                      // +0x0c CDefPointer<const CBuyableHouseDef>
    unsigned char _pad_0x10[0x14];                  // +0x10 flags / days / dress levels
    CIntelligentPointer<CThing> MainSignPost;       // +0x24
    CIntelligentPointer<CThing> DressingSignPost;   // +0x2c

    CThing* CreateSignPost(const CCharString& defName, bool mainSign);   // 0x006C0970
    void CreateDressingSignPost();
};
#pragma pack(pop)

void CTCBuyableHouse::CreateDressingSignPost()
{
    DressingSignPost = CIntelligentPointer<CThing>(CreateSignPost(g_DressingSignPostDefName, false));
}
