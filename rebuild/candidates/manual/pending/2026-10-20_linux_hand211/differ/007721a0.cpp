// CTCShop::IsPermanentlyStocked() const -- true when the shop thing's CShopDef sub-def says the stock display is permanent or the shop is always open
// Grade: DIFFER(194v170) -- retail pushes ebx inside the found branch (bool in bl); the lane never sinks the push, keeps the bool in a frame byte and the shop def in eax (long release form)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007721A0, 170 bytes (plain `ret`).
// Callees: CTCBase::GetDefinitionManager @ 0x006869D0, CDefinitionManager::GetDef_0042af3c(long, CDefPointer&) @ 0x0042AF3C
// (the thing-def instantiation; result ignored), CDefClassBase::GetSubDef_005130a0(CDefPointer&) @ 0x005130A0 (the 'CShopDef'
// instantiation); the defs' virtual slot 1 (destroy on the last release). Names follow the landed bodies of both callees.
// Layout: CThing def index (unsigned short) +0x8c; retail CShopDef sits 0x10 below Ego_r.pdb (two vectors and two maps of 12
// bytes before): IsStockDisplayPermanent +0x98, AlwaysOpen +0x9c.
// Shape: each def is read ONCE from its out slot into a holder whose destructor releases it (the out slots are zeroed after
// the argument pushes); the bool rides in bl across both releases; on the found path the shop def is already dereferenced,
// so its release loses the null test.
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefPointer {                 // the out slot (released by the holder below)
    T* Object;
    CDefPointer() : Object(0) { }
};
template <class T> struct CDefHolder {                  // the def read once from the out slot, released at scope end
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CShopDef : CDefBase {
    unsigned char _pad08[0x98 - 0x08];
    bool IsStockDisplayPermanent;                       // +0x98
    bool IsBarberShop;                                  // +0x99
    bool IsTattooShop;                                  // +0x9a
    bool IsTitleShop;                                   // +0x9b
    bool AlwaysOpen;                                    // +0x9c
};
struct CDefClassBase : CDefBase {
    bool GetSubDef_005130a0(CDefPointer<CShopDef>& out);    // 0x005130A0
};
struct CDefinitionManager {
    bool GetDef_0042af3c(long index, CDefPointer<CDefClassBase>& out);   // 0x0042AF3C
};
struct CThing {
    unsigned char _pad00[0x8c];
    unsigned short DefIndex;                            // +0x8c
};
struct CTCShop {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    CDefinitionManager* GetDefinitionManager() const;   // 0x006869D0 (CTCBase)
    bool IsPermanentlyStocked() const;
};

bool CTCShop::IsPermanentlyStocked() const
{
    CDefPointer<CDefClassBase> thingDefOut;
    GetDefinitionManager()->GetDef_0042af3c(ParentThing->DefIndex, thingDefOut);
    CDefHolder<CDefClassBase> thingDef(thingDefOut.Object);
    CDefPointer<CShopDef> shopDefOut;
    bool found = thingDef->GetSubDef_005130a0(shopDefOut);
    CDefHolder<CShopDef> shopDef(shopDefOut.Object);
    if (found)
        return shopDef->IsStockDisplayPermanent || shopDef->AlwaysOpen;
    return false;
}
