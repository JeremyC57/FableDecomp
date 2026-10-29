// CCreatureAction_HeroLoadRangedWeapon::FinishAction() -- load the weapon if the action finished, base FinishAction, then drop this action's entry from the creature's type-0x8C component map and let it recalculate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0078B640, length 111 bytes (+ int3 padding). Vtable 0x0126B9D4 slot 4.
// Callees: CCreatureAction_HeroLoadRangedWeapon::SetWeaponLoaded @ 0x0078B6B0 (thiscall, `this` still in ecx),
// CCreatureActionBase::FinishAction @ 0x00694430 (direct base call), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8),
// CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface for type 0x8C = bit 12 of the presence word at +0x30, `test ch,10h`),
// the component's keyed-map erase @ 0x0058119B (a frame-pointer tree erase(const key&); the manifest label
// "CBankFile::CompareDataOffset::operator()" is propagated), the component's recalculation @ 0x00800020 (manifest
// "CAnimation_RecalculateScale": it resets +0xc to 1.0f and multiplies in every map value).
// Shape: `Finished` is CCreatureActionBase +0x61 (PDB header, which agrees with retail below +0x84); the erase key is `this`
// bound to a `const&` parameter -- VC builds the temporary in the lookup-key frame word (`mov [esp+0Ch],edi` after the
// push), the component stays in esi for both calls. Retail CCreatureActionBase is 0xa8 bytes (the PDB vectors are 16 bytes,
// retail 12), so the class is modelled locally.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                      // a thing's sorted {type, component} vector map at +0x44
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CCreatureActionBase;
struct CActionKeyedMap {                    // std::map keyed by the action pointer
    unsigned long Erase(CCreatureActionBase* const& key);   // 0x0058119B
};
struct CTCActionScaled {                    // interface type 0x8C component
    unsigned char _pad00[0x18];
    CActionKeyedMap m_actions;              // +0x18
    void Recalculate();                     // 0x00800020
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();            // slot 4: 0x00694430
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x61 - 0x10];      // +0x10
    bool Finished;                          // +0x61
    unsigned char _pad62[0xa8 - 0x62];      // +0x62
};

class CCreatureAction_HeroLoadRangedWeapon : public CCreatureActionBase { public:
    virtual void FinishAction();
    void SetWeaponLoaded();                 // 0x0078B6B0
};

void CCreatureAction_HeroLoadRangedWeapon::FinishAction()
{
    if (Finished)
        SetWeaponLoaded();
    CCreatureActionBase::FinishAction();
    CTCActionScaled* scaled;
    if (PCreature.GetPItem()->GetInterface(&scaled, 0x8C)) {
        scaled->m_actions.Erase(this);
        scaled->Recalculate();
    }
}