// CTCCarriedActionUsePotion::OnActionUse(CThing* user) -- drink the carried potion: run the base use hook, apply the potion's bonus-item def to the user's hero stats (when it has them), then kill the potion thing
// Leaf: vfunc_28
// (catalogued slot +0x28; it is OnActionUse: the qualified base call is CTCActionUseBase::OnActionUse 0x007EAB90,
// as in the landed CTCCarriedActionUseAbility::OnActionUse 0x007F4D50)
//
// Retail 0x007EDD60, 133 bytes (`ret 4`). Vtable 0x0123FDE4 (CTCCarriedActionUsePotion) slot 28.
// Callees: the base hook @ 0x007EAB90 (empty `ret 4` in another TU), CDefClassBase::GetSubDef
// <CBonusItemDef>(CDefPointer<const CBonusItemDef>*) @ 0x007E1D30 (landed), CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound @ 0x0040F020, CTCHeroStats::ApplyBonusItemEffects(const CBonusItemDef*)
// @ 0x0057C4F1, CThing::Kill(bool) @ 0x004C9B80, the def's virtual slot 1 (release at zero).
// Layout: CThing PDef_Object +0x70 (the parent thing's def), the interface bitset +0x20 and map +0x44.
// Shape: GetSubDef has a same-TU out-slot-only stand-in body, so the CDefPointer local (starting null) is homed in the
// dead `user` argument slot (user rides in esi) and that slot is reused for the interface key; the def class is read into
// a local first (`mov eax,[edi+4] ; mov ecx,[eax+70h]`); the hero stats come from the out-parameter GetInterface
// (bitset test, LowerBound, end/hit choice, `*out = component`: the component goes through eax into ecx); the pointer's
// release has a null test and no null store (the object is in ebx).
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);                   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CDefPointeeBase { public:
    virtual void Destroy();                     // slot 0
    virtual void Delete();                      // slot 1: `call [eax+4]`
    long m_refCount;                            // +0x04
};

template <class T>
class CDefPointer { public:                            // GetSubDef's out pointer (released by hand below)
    CDefPointeeBase* Object;                    // +0x00
};

class CBonusItemDef;

class CDefClassBase : public CDefPointeeBase { public:
    bool GetSubDef(CDefPointer<const CBonusItemDef>* out);  // 0x007E1D30 GetSubDef<CBonusItemDef>
};
void* Find_CDefClassBase_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const CBonusItemDef>* out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out->Object = (CDefPointeeBase*)def;
    return true;
}
#pragma auto_inline(on)

class CTCHeroStats { public:
    void ApplyBonusItemEffects(const CBonusItemDef* def);   // 0x0057C4F1
};

class CThing { public:
    unsigned char    _pad0[0x20];
    unsigned long    m_presentBits[9];                      // +0x20 interface-present bitset
    CInterfaceMap    m_interfaces;                          // +0x44
    unsigned char    _pad4c[0x70 - 0x4c];
    CDefPointeeBase* PDef_Object;                           // +0x70 CDefPointer<const CThingDef>::Object
    template <class T> __forceinline bool GetInterface(T** out, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
    void Kill(bool removeNow);                              // 0x004C9B80
};

class CTCCarriedActionUseBase { public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    void OnUseBase(CThing* user);                           // 0x007EAB90 (empty base implementation)
};

struct CTCCarriedActionUsePotion : CTCCarriedActionUseBase {
    void vfunc_28(CThing* user);
};

void CTCCarriedActionUsePotion::vfunc_28(CThing* user)
{
    CTCCarriedActionUseBase::OnUseBase(user);
    CDefPointer<const CBonusItemDef> out;
    out.Object = 0;
    CDefClassBase* defClass = (CDefClassBase*)ParentThing->PDef_Object;
    bool found = defClass->GetSubDef(&out);
    CDefPointeeBase* bonus = out.Object;
    if (found) {
        CTCHeroStats* stats;
        if (user->GetInterface(&stats, 4))
            stats->ApplyBonusItemEffects((const CBonusItemDef*)bonus);
    }
    ParentThing->Kill(true);
    if (bonus && --bonus->m_refCount == 0)
        bonus->Delete();
}