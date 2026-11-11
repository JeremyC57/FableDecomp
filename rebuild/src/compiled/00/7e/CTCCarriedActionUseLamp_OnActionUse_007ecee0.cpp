// CTCCarriedActionUseLamp::OnActionUse(CThing& user) [Ego_r.pdb; was vfunc_28(CThing*)] -- toggle the user's hand-lamp light (interface 0x27) from the lamp's light def and notify the lamp's own interface-0x1a component
// Leaf: OnActionUse
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
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
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                             // slot 1
};
class CLightDef : public CDefVirtual, public CRefCounted { public:
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
// agentNF15: the pointee is held non-const whatever T is (engine/CDefPointer.h stores CDefPointeeBase*), so the
//   CDefPointer<const T> out slots of the const def lookups keep the landed member code.
template <class T> struct CDefNonConst { typedef T Type; };
template <class T> struct CDefNonConst<const T> { typedef T Type; };
template <class T> class CDefPointer { public:
    typename CDefNonConst<T>::Type* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object)
            Object->ReleaseTemporary();
    }
};
class CDefClassBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x0059346A GetSubDef<CLightDef> (Ego_r.pdb member template)
};
class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                    // +0x20 interface-present bitset
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad54[0x1c];
    CDefClassBase* PDef;                                // +0x70
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        if (it == map->end() || interfaceType < it->first)
            it = map->end();
        return it->second;
    }
};
class CTCLight { public:
    unsigned char _pad00[0x1c];
    bool Active;                                        // +0x1c
    void SetLightDef(const CDefPointer<const CLightDef>& def);   // 0x00640E60
    void SetActive(bool active);                        // 0x006412B0
};
struct CTCShowableItem {
    void OnShowEnded();                                 // 0x005D8E50
};
class CThing;
class CTCActionUseBase { public:                 // Ego_r.pdb base of CTCCarriedActionUseBase (row-local view: the one member called)
    virtual void OnActionUse(CThing& user);   // 0x007EAB90 Ego_r.pdb ?OnActionUse@CTCActionUseBase@@UAEXAAVCThing@@@Z (empty `ret 4`, RTTI slot 28; was OnUseBase(CThing*))
};
class CTCCarriedActionUseBase : public CTCActionUseBase { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
};
struct CTCCarriedActionUseLamp : CTCCarriedActionUseBase {
    virtual void OnActionUse(CThing& user);   // Ego_r.pdb (UAE; RTTI slot 28, was vfunc_28(CThing*))
};

void CTCCarriedActionUseLamp::OnActionUse(CThing& thing)
{
    CThing* user = (CThing*)&thing;
    CTCCarriedActionUseBase::OnActionUse(thing);
    CTCLight* light = 0;
    if (user->HasTC(0x27))
        light = (CTCLight*)user->GetTC(0x27);
    CDefPointer<const CLightDef> def;
    ParentThing->PDef->GetSubDef(&def);
    light->SetLightDef(def);
    light->SetActive(!light->Active);
    if (ParentThing->HasTC(0x1a))
        ((CTCShowableItem*)ParentThing->GetTC(0x1a))->OnShowEnded();
}