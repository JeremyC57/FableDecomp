// CTCBuyableHouse::IsForSale() const -- a house is for sale when the player does not own it and its main sign post still exists and is alive
//
// Retail 0x006BF960, 49 bytes. Callee: CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50 (twice
// on MainSignPost). Layout (Ego_r.pdb CTCBuyableHouse): OwnedByPlayer +0x11, MainSignPost +0x24
// (CIntelligentPointer<CThing>); CThing::Dead is +0x91 bit 0 in retail (PDB +0xa1).
// Shape: the owned test returns early with its own `xor al,al ; ret` before esi is pushed; the
// sign-post test is `if (p && !p->IsDead()) return true; return false;` (two exits after the push)
// with the Dead bit read through an inline `bool IsDead()` accessor: testing the raw bit-field in a
// bool-returning function makes VC7.1 materialise the result (`mov dl,[ecx+91h] ; mov al,1 ;
// test al,dl`), the bool accessor keeps retail's `test byte ptr [eax+91h],1 ; jne`.

struct CThing
{
    unsigned char _pad_0x00[0x91];
    unsigned char Dead : 1;                         // +0x91 bit 0
    bool IsDead() const { return Dead; }
};

struct CBaseObject;

struct CBaseIntelligentPointer
{
    virtual void v00();
    void* m_pItem;                                  // +0x04
    CBaseObject* GetPItem() const;                  // 0x00A01B50
};

template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer
{
    T* Get() const { return (T*)GetPItem(); }
};

#pragma pack(push, 1)
struct CTCBuyableHouse
{
    void*         __vftable;                        // +0x00 (CTCBase)
    void*         ParentThing;                      // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x4];                   // +0x08 (CTCBase flags)
    void*         PDef_Object;                      // +0x0c CDefPointer<const CBuyableHouseDef>::Object
    bool          IsScripted;                       // +0x10
    bool          OwnedByPlayer;                    // +0x11
    bool          Rented;                           // +0x12
    unsigned char _pad_0x13[0x1];                   // +0x13
    long          DaySomeoneWillMoveIn;             // +0x14
    long          DayNextRentIsDue;                 // +0x18
    long          CurrentDressLevel;                // +0x1c
    long          MaxDressLevel;                    // +0x20
    CIntelligentPointer<CThing> MainSignPost;       // +0x24

    bool IsForSale() const;
};
#pragma pack(pop)

bool CTCBuyableHouse::IsForSale() const
{
    if (OwnedByPlayer)
        return false;
    if (MainSignPost.Get() && !MainSignPost.Get()->IsDead())
        return true;
    return false;
}
