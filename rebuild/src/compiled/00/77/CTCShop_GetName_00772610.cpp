// CTCShop::GetName() const -- fetch the parent thing's definition by its index, take its shop sub-definition and return the shop name word at +0x28 (0 when the thing has no shop def)
// Leaf: GetName
// Retail: Fable.exe 0x00772610, 151 bytes (plain `ret`).
// Callees: CTCBase::GetDefinitions() const @ 0x006869D0 (manifest "_Cons_val<...>" is propagated),
// CDefinitionManager::GetDef(long index, CDefPointer*) @ 0x0042AF3C (manifest "GetDef<CSpecialAbilitiesDrunkennessDef>",
// a byte-twin template instance), CDefClassBase::GetSubDef(CDefPointer*) @ 0x005130A0 (manifest
// "GetSubDef<CAICreatureWillPowerIndicatorDef>", byte twin), the defs' virtual slot 1 (+0x04, release at zero).
// Layout: CThing def index word +0x8C; ref count +0x04 on the def base; the shop def's name word +0x28.
// Shape: both defs live in null-tested CDefPointer locals whose destructors release them (`if (Object) Object->ReleaseReference()`); the found
// path returns `shopDef->Name` directly, so its null test on the sub-def is dropped after the dereference while the not-found path keeps it.
// a null test, the miss arm with one; the thing def with a null test on both arms).
class CDefPointeeBase { public:
    virtual void Destroy();                         // slot 0
    virtual void Delete();                          // slot 1 (+0x04)
    long m_refCount;                                // +0x04
    void ReleaseReference() { if (--m_refCount == 0) Delete(); }
};
class CShopDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x28 - 0x08];
    long Name;                                      // +0x28
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->ReleaseReference(); }
    T* operator->() const { return Object; }
};
class CDefClassBase : public CDefPointeeBase { public:
    bool GetSubDef(CDefPointer<CShopDef>* out);     // 0x005130A0 (CDefPointer<const CShopDef>*)
};
void* FindDef_CDefClassBase_GetSubDef_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CShopDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CDefinitionManager { public:
    bool GetDef(long index, CDefPointer<CDefClassBase>* out);   // 0x0042AF3C (CDefPointer<const CThingDef>*)
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CDefClassBase>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char  _pad00[0x8c];
    unsigned short DefIndex;                        // +0x8c
};
class CTCShop { public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    bool    ToBeRemoved;                            // +0x08
    bool    IsInGlobalUpdateTCs;                    // +0x09
    CDefinitionManager* GetDefinitions() const;     // 0x006869D0 (CTCBase)
    long GetName() const;
};

long CTCShop::GetName() const
{
    CDefPointer<CDefClassBase> thingDef;
    GetDefinitions()->GetDef(ParentThing->DefIndex, &thingDef);
    CDefPointer<CShopDef> shopDef;
    if (thingDef->GetSubDef(&shopDef))
        return shopDef->Name;
    return 0;
}