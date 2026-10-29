// CTCInventoryClothing::GetCurrentTotalArmorValue() -- sum the armour of every worn clothing modifier plus the "no clothing" defaults of the uncovered suit parts
#pragma optimize("y", off)
#pragma optimize("s", on)
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);           // 0x0099EBF0
    ~CCharString();                                            // 0x0099EAE0
};
struct SAllocator { SAllocator() { } };
struct SLongListNode { SLongListNode* _M_next; SLongListNode* _M_prev; long _M_data; };
struct CLongList {
    SLongListNode* _M_node;
    explicit CLongList(const SAllocator& allocator = SAllocator());   // 0x0042AB58
    ~CLongList();                                              // 0x0042AB73
};
struct SNode { long _M_color; SNode* _M_parent; SNode* _M_left; SNode* _M_right; long first; float second; };
SNode* __cdecl Successor(SNode* node);                         // 0x004292C0
struct SSetInsertResult { SNode* first; bool second; };
struct CSuitPartSet {                                          // set<EClothingSuitPart> (12 bytes)
    SNode* _M_header; unsigned long _M_node_count; long _M_key_compare;
    CSuitPartSet();                                            // 0x005BB3B3
    ~CSuitPartSet();                                           // 0x005BB04F
    SSetInsertResult insert(const long& value);                // 0x005BAF94
    unsigned long erase(const long& value);                    // 0x005BBB34
};
struct CDamageFactorMap {                                      // map<long, float> (12 bytes)
    SNode* _M_header; unsigned long _M_node_count; long _M_key_compare;
    SNode* find(const long& key) const;                        // 0x005BA9BE
};

struct CDefBase {
    virtual void v00();
    virtual void Destroy() const;                              // slot 1
    mutable long RefCount;                                     // +0x04
    void ReleaseTemporary() const { --RefCount; if (RefCount == 0) Destroy(); }
};
struct CThingDefPointer;
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    __forceinline CDefPointer(const CThingDefPointer& o);
    __forceinline ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
// Layout: retail CAppearanceModifierDef / armour def (Ego_r.pdb - 8, the 16-byte containers are 12 in retail).
class CAppearanceModifierDef : public CDefBase { public:
    unsigned char _pad08[0x4c - 0x08];
    long  SuitPart;                                            // +0x4c
    unsigned char _pad50[0x5c - 0x50];
    long  ArmourDefIndex;                                      // +0x5c
    float OveralProtectionWeighting;                           // +0x60
};
class CArmourDef : public CDefBase { public:
    unsigned char _pad08[0x48 - 0x08];
    CDamageFactorMap DamageFactors;                            // +0x48
};
struct CThingDef : CDefBase {
    bool GetSubDef(CDefPointer<const CAppearanceModifierDef>* out) const;  // 0x0057FF92
};
void* FindDef_4a95_0();
#pragma auto_inline(off)
__declspec(noinline) bool CThingDef::GetSubDef(CDefPointer<const CAppearanceModifierDef>* out) const
{
    void* def = FindDef_4a95_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CThingDefPointer {                                      // CDefPointer<const CThingDef> as returned untyped
    const CThingDef* Object;
    __forceinline ~CThingDefPointer() { if (Object) { Object->ReleaseTemporary(); Object = 0; } }
    const CThingDef* operator->() const { return Object; }
};
template<class T> __forceinline CDefPointer<T>::CDefPointer(const CThingDefPointer& o) : Object((T*)o.Object) { if (Object) ++Object->RefCount; }
class CDefinitionManager { public:
    CThingDefPointer GetPDefFromGlobalIndex(long index) const;   // 0x009AD9E0
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager
{
};

class CTCHeroAttachableAppearanceModifiers { public:                  // interface 0x5E
    void GetAppearanceModifierList(CLongList& out, long type) const;   // 0x00706800
};
struct CPresenceBits { bool test(long bit) const; };           // 0x00410DE0
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;          // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char  _pad00[0x20];
    CPresenceBits  m_present;                                  // +0x20
    unsigned char  _pad21[0x23];
    CInterfaceMap  m_interfaces;                               // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted) const
    {
        if (m_present.test(wanted)) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            *out = (T*)((void*)((it == map->last || wanted < it->first) ? map->last : it)->second);
            return true;
        }
        return false;
    }
};

class CTCBase
{
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
};
namespace NInventory {
class CTCInventoryBase : public CTCBase
{
public:
};
}

class CTCInventoryClothing : public NInventory::CTCInventoryBase { public:
    void*   __vftable;
    CThing* ParentThing;                                       // +0x04
    long GetCurrentTotalArmorValue() const;
};
long CTCInventoryClothing::GetCurrentTotalArmorValue() const
{
    CSuitPartSet uncovered;
    uncovered.insert(1);
    uncovered.insert(2);
    uncovered.insert(3);
    uncovered.insert(4);
    uncovered.insert(5);
    long total = 0;
    CTCHeroAttachableAppearanceModifiers* modifiers;
    if (ParentThing->GetInterface(&modifiers, 0x5e)) {
        CLongList worn;
        modifiers->GetAppearanceModifierList(worn, 2);
        for (SLongListNode* node = worn._M_node->_M_next, *end = worn._M_node; node != end; node = node->_M_next) {
            CThingDefPointer def = GetDefinitionManager().GetPDefFromGlobalIndex(node->_M_data);
            CDefPointer<const CAppearanceModifierDef> modifier;
            if (def->GetSubDef(&modifier)) {
                uncovered.erase(modifier->SuitPart);
                float factor = 1.0f;
                CDefPointer<const CArmourDef> armour = GetDefinitionManager().GetPDefFromGlobalIndex(modifier->ArmourDefIndex);
                SNode* found = armour->DamageFactors.find(0);
                if (found != armour->DamageFactors._M_header)
                    factor = found->second;
                total += (long)(modifier->OveralProtectionWeighting / factor * 500.0f);
            }
        }
    }
    for (SNode* part = uncovered._M_header->_M_left, *end = uncovered._M_header; part != end; part = Successor(part)) {
        long index = 0;
        switch (part->first) {
        case 1: index = GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_HAT"); break;
        case 2: index = GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_SHIRT"); break;
        case 3: index = GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_GLOVES"); break;
        case 4: index = GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_TROUSERS"); break;
        case 5: index = GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_BOOTS"); break;
        }
        CThingDefPointer def = GetDefinitionManager().GetPDefFromGlobalIndex(index);
        CDefPointer<const CAppearanceModifierDef> modifier;
        if (def->GetSubDef(&modifier)) {
            float factor = 1.0f;
            CDefPointer<const CArmourDef> armour = GetDefinitionManager().GetPDefFromGlobalIndex(modifier->ArmourDefIndex);
            SNode* found = armour->DamageFactors.find(0);
            if (found != armour->DamageFactors._M_header)
                factor = found->second;
            total += (long)(modifier->OveralProtectionWeighting / factor * 500.0f);
        }
    }
    return total;
}