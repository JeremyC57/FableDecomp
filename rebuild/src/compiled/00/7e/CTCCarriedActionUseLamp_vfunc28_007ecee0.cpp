// CTCCarriedActionUseLamp::vfunc_28(CThing* user) -- toggle the user's hand-lamp light (interface 0x27) from the lamp's light def and notify the lamp's own interface-0x1a component
// Leaf: vfunc_28
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007ECEE0, 211 bytes (`ret 4`). OnActionUse(CThing&) slot 28 (GOTCHAS: "vfunc_28"
// rows are OnActionUse).
// Callees: the empty base hook CTCCarriedActionUseBase @ 0x007EAB90 (another TU), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x0040F020 (two inlined GetInterface lookups), CDefClassBase::GetSubDef<CLightDef> @ 0x0059346A, CTCLight::SetLightDef
// @ 0x00640E60, CTCLight::SetActive @ 0x006412B0, the interface-0x1a component's OnShowEnded @ 0x005D8E50 (descriptive name
// shared with the landed CEventShownItem dtor 0x008116D0), the light def's virtual slot 1 on the last release.
// Layout: CThing interface bitset +0x20 (0x27 = bit 7 of the +0x24 word, tested as `mov al ; test al,al ; jns`), component map
// +0x44, def +0x70; CTCLight (Ego_r.pdb) Active +0x1c.
// Shape: the light pointer is zero-initialised before the presence test (`xor esi,esi` preloaded, GOTCHAS chunk 144), the def
// out pointer is homed in the dead `user` argument slot and released by a null-tested CDefPointer dtor calling the def's inline
// ReleaseTemporary() over its ref-count base at +4 (`mov esi,[eax+4] ; mov ecx,eax ; add eax,4 ; dec esi ; ... test`).
struct CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                             // slot 1
};
struct CLightDef : CDefVirtual, CRefCounted {
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object)
            Object->ReleaseTemporary();
    }
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CLightDef>* out) const;   // 0x0059346A
};
struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                    // +0x20 interface-present bitset
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad4c[0x70 - 0x4c];
    CDefClassBase* PDef;                                // +0x70
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
struct CTCLight {
    unsigned char _pad00[0x1c];
    bool Active;                                        // +0x1c
    void SetLightDef(const CDefPointer<CLightDef>& def);   // 0x00640E60
    void SetActive(bool active);                        // 0x006412B0
};
struct CTCShowableItem {
    void OnShowEnded();                                 // 0x005D8E50
};
struct CTCCarriedActionUseBase {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    void OnUseBase(CThing* user);                       // 0x007EAB90 (empty base implementation)
};
struct CTCCarriedActionUseLamp : CTCCarriedActionUseBase {
    void vfunc_28(CThing* user);
};

void CTCCarriedActionUseLamp::vfunc_28(CThing* user)
{
    CTCCarriedActionUseBase::OnUseBase(user);
    CTCLight* light = 0;
    if (user->HasTC(0x27))
        light = (CTCLight*)user->GetTC(0x27);
    CDefPointer<CLightDef> def;
    ParentThing->PDef->GetSubDef(&def);
    light->SetLightDef(def);
    light->SetActive(!light->Active);
    if (ParentThing->HasTC(0x1a))
        ((CTCShowableItem*)ParentThing->GetTC(0x1a))->OnShowEnded();
}