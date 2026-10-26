// CTCShop::GetCurrentBuyPrice(long itemDefIndex, long count) -- the whole-gold price the shop asks for count items: the basic price scaled by the shop def's buy multiplier and the attitude multiplier, truncated per item
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007742C0, 231 bytes (`ret 8`).
// Callees: CTCBase::GetDefinitionManager @ 0x006869D0, CDefinitionManager::GetDef_0042af3c @ 0x0042AF3C (result ignored),
// CDefClassBase::GetSubDef_005130a0 @ 0x005130A0 (the 'CShopDef' instantiation), CTCShop::GetAttitudeBuyPriceMultiplier const
// @ 0x007735D0, CTCShop::GetCurrentBasicPrice(long, long) @ 0x007727E0 (float), __ftol2 @ 0x00BFEA70; the defs' slot 1.
// Layout: CThing def index (unsigned short) +0x8c; retail CShopDef sits 8 below Ego_r.pdb here (two 12-byte vectors before):
// BuyPriceMultiplier +0x58.
// Shape: both defs are dtor-carrying CDefPointer locals; GetDef and GetSubDef are same-TU noinline stand-in bodies that write
// only the out slot, so the thing def is read once into ebp and the shop def is re-read once on the found path (its release
// there has no null test); the price total is declared just before the loop, so its zero is the out slots' zero register
// (edi) and `push ebx` sinks into the found branch; the multiplier is a named float local.
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
class CShopDef : public CDefBase { public:
    unsigned char _pad08[0x58 - 0x08];
    float BuyPriceMultiplier;                           // +0x58
    float SellPriceMultiplier;                          // +0x5c
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
    float GetCurrentBasicPrice(long itemDefIndex, long forSale);   // 0x007727E0
    float GetAttitudeBuyPriceMultiplier() const;        // 0x007735D0
    long GetCurrentBuyPrice(long itemDefIndex, long count);
};

long CTCShop::GetCurrentBuyPrice(long itemDefIndex, long count)
{
    CDefPointer<CDefClassBase> thingDef;
    GetDefinitionManager()->GetDef_0042af3c(ParentThing->DefIndex, thingDef);
    CDefPointer<CShopDef> shopDef;
    if (!thingDef->GetSubDef_005130a0(shopDef))
        return 0;
    float multiplier = shopDef->BuyPriceMultiplier;
    multiplier = multiplier * GetAttitudeBuyPriceMultiplier();
    long total = 0;
    for (long i = count; i > 0; --i)
        total += (long)(GetCurrentBasicPrice(itemDefIndex, 0) * multiplier);
    return total;
}