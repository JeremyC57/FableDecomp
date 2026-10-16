// CTCShop::GetName() const -- fetch the parent thing's definition by its index, take its shop sub-definition and return the shop name word at +0x28 (0 when the thing has no shop def)
// Leaf: GetName
// Grade: DIFFER(166v151) -- retail zeroes the thing-def out pointer after the GetDef pushes and the shop-def one after `mov ecx,esi`, loads the def straight into esi, and releases the sub-def on the hit arm with `mov eax,[ecx+4] ; dec eax ; mov [ecx+4],eax ; jne` (no null test) where ours emits `lea eax,[ecx+4] ; dec ; test ; mov [eax]` on both arms; lane gap: the mov/dec/mov release form (as the CTCCreatureGenerator TU); next: a by-value inline GetDef helper for the late zero store
// Retail: Fable.exe 0x00772610, 151 bytes (plain `ret`).
// Callees: CTCBase::GetDefinitions() const @ 0x006869D0 (manifest "_Cons_val<...>" is propagated),
// CDefinitionManager::GetDef(long index, CDefPointer*) @ 0x0042AF3C (manifest "GetDef<CSpecialAbilitiesDrunkennessDef>",
// a byte-twin template instance), CDefClassBase::GetSubDef(CDefPointer*) @ 0x005130A0 (manifest
// "GetSubDef<CAICreatureWillPowerIndicatorDef>", byte twin), the defs' virtual slot 1 (+0x04, release at zero).
// Layout: CThing def index word +0x8C; ref count +0x04 on the def base; the shop def's name word +0x28.
// Shape: both CDefPointer out locals start null; the defs are released by hand (the hit arm drops the sub-def without
// a null test, the miss arm with one; the thing def with a null test on both arms).
struct CDefPointeeBase {
    virtual void Destroy();                         // slot 0
    virtual void Delete();                          // slot 1 (+0x04)
    long m_refCount;                                // +0x04
};
struct CShopDef : CDefPointeeBase {
    unsigned char _pad08[0x28 - 0x08];
    long Name;                                      // +0x28
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CShopDef>* out);     // 0x005130A0 (CDefPointer<const CShopDef>*)
};
struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<CDefClassBase>* out);   // 0x0042AF3C (CDefPointer<const CThingDef>*)
};
struct CThing {
    unsigned char  _pad00[0x8c];
    unsigned short DefIndex;                        // +0x8c
};
struct CTCShop {
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
    CDefClassBase* def = thingDef.Object;
    CDefPointer<CShopDef> shopDef;
    bool found = def->GetSubDef(&shopDef);
    CShopDef* sub = shopDef.Object;
    if (found) {
        long name = sub->Name;
        if (--sub->m_refCount == 0)
            sub->Delete();
        if (def && --def->m_refCount == 0)
            def->Delete();
        return name;
    }
    if (sub && --sub->m_refCount == 0)
        sub->Delete();
    if (def && --def->m_refCount == 0)
        def->Delete();
    return 0;
}
