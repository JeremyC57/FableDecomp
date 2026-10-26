// CTCShop::IsPermanentlyStocked() const -- true when the shop thing's CShopDef sub-def says the stock display is permanent or the shop is always open
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007721A0, 170 bytes (plain `ret`).
// Callees: CTCBase::GetDefinitionManager @ 0x006869D0, CDefinitionManager::GetDef_0042af3c(long, CDefPointer&) @ 0x0042AF3C
// (the thing-def instantiation; result ignored), CDefClassBase::GetSubDef_005130a0(CDefPointer&) @ 0x005130A0 (the 'CShopDef'
// instantiation); the defs' virtual slot 1 (destroy on the last release). Names follow the landed bodies of both callees.
// Layout: CThing def index (unsigned short) +0x8c; retail CShopDef sits 0x10 below Ego_r.pdb (two vectors and two maps of 12
// bytes before): IsStockDisplayPermanent +0x98, AlwaysOpen +0x9c.
// Shape: two CDefPointer locals whose destructors release them (the out slots are zeroed after the argument pushes); GetDef
// and GetSubDef are same-TU noinline stand-in bodies that write only the out slot, so each def is read once into a register;
// the bool rides in bl across both releases; on the found path the shop def is already dereferenced, so its release loses
// the null test.
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> class CDefPointer { public:                 // a counted def reference (released at scope end)
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
class CShopDef : public CDefBase { public:
    unsigned char _pad08[0x98 - 0x08];
    bool IsStockDisplayPermanent;                       // +0x98
    bool IsBarberShop;                                  // +0x99
    bool IsTattooShop;                                  // +0x9a
    bool IsTitleShop;                                   // +0x9b
    bool AlwaysOpen;                                    // +0x9c
};
class CDefClassBase : public CDefBase { public:
    bool GetSubDef_005130a0(CDefPointer<CShopDef>& out);    // 0x005130A0
};
void* Find_CDefClassBase_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef_005130a0(CDefPointer<CShopDef>& out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.Object = (CShopDef*)def;
    return true;
}
#pragma auto_inline(on)
class CDefinitionManager { public:
    bool GetDef_0042af3c(long index, CDefPointer<CDefClassBase>& out);   // 0x0042AF3C
};
void* Lookup_CDefinitionManager_GetDef(const void* manager, long index);   // stand-in callee
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef_0042af3c(long index, CDefPointer<CDefClassBase>& out)
{
    void* def = Lookup_CDefinitionManager_GetDef(this, index);
    if (!def)
        return false;
    out.Object = (CDefClassBase*)def;
    return true;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char _pad00[0x8c];
    unsigned short DefIndex;                            // +0x8c
};
class CTCShop { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    CDefinitionManager* GetDefinitionManager() const;   // 0x006869D0 (CTCBase)
    bool IsPermanentlyStocked() const;
};

bool CTCShop::IsPermanentlyStocked() const
{
    CDefPointer<CDefClassBase> thingDef;
    GetDefinitionManager()->GetDef_0042af3c(ParentThing->DefIndex, thingDef);
    CDefPointer<CShopDef> shopDef;
    if (thingDef->GetSubDef_005130a0(shopDef))
        return shopDef->IsStockDisplayPermanent || shopDef->AlwaysOpen;
    return false;
}