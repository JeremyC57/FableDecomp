// CCreatureAction_UnsheatheItemFromInventoryWithHasteAction::FinishAction() -- the plain unsheathe FinishAction, then (when the creature has the interface-0x92 component) register that component's speed factor as a haste multiplier under this action's HasteKey on the creature's CTCHaste (added when missing) and recalculate the haste
// Grade: DIFFER(240v240) -- frame only: retail keeps a 0x14 frame (key/string word at +0, the multiplier pair at +4, the insert result at +0Ch), ours a 0x10 frame with the pair sharing the key/string word (chunk 609: inline SetActionSpeedMultiplier, std::make_pair, named pair/result, conditional get-or-add, key as inline parameter, get-or-add helper, StatGetClassTypeName string -- none moves it)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A02B0, length 240 bytes (plain `ret`, virtual).
// Callees: CCreatureAction_UnsheatheItemFromInventory::FinishAction 0x0069FC40 (direct base call), CBaseIntelligentPointer::GetPItem
// 0x00A01B50, CVectorMap::LowerBound const 0x004365B0 (interface 0x92) and non-const 0x0040F020 (interface 0x8C = CTCHaste),
// CThing::AddTC(const CCharString&, bool, const CTCParamsBase*) 0x004C9D60 (PDB QAEPAVCTCBase@@ABVCCharString@@_NPBVCTCParamsBase@@@Z) ("CTCHaste", active), CCharString 0x0099EBF0 / 0x0099EAE0,
// the 0x92 component's float getter 0x0079C740 (manifest "CTCPhysicsBase::GetAirResistance"; class not proven),
// map<void*, float>::insert 0x00580D21 on CTCHaste::Multipliers (+0x18) and CTCHaste::UpdateActionSpeedMultiplier 0x00800020
// (private; Multiplier +0x0C = 1.0f times every map value), both through the inline CTCHaste::SetActionSpeedMultiplier(void*, float)
// whose out-of-line copy is 0x005812AE (`ret 8`: insert(pair(key, m)) then 0x00800020; catalog label GetBlockData is wrong).
// Layout: retail HasteKey +0xB8 (PDB +0xC4: the debug CCreatureActionBase is 0xC larger).
// Shape: the 0x92 lookup is the out-parameter GetInterface over the const map copy; CTCHaste comes from GetInterface or
// AddTC, whose string is the implicit const char* conversion (built before the argument pushes); the pair is built
// value-first (the getter runs before HasteKey is stored).
class CTCBase;
class CTCParamsBase;
class CBaseObject;

class CCharString {
public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);              // 0x0040F020
    const SEntry* LowerBound(const long* type) const;  // 0x004365B0
};

class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    CTCBase* AddTC(const CCharString& name, bool activate, const CTCParamsBase* params);   // 0x004C9D60
    template<class T> __forceinline bool GetTC(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool PeekTC(const T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (const T*)it->component;
            return true;
        }
        return false;
    }
};

class CUnprovenTC_0079c740 {                    // interface 0x92 (catalog CTCPhysicsBase, not proven)
public:
    float GetSpeedFactor() const;               // 0x0079C740
};

namespace std {
template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
}
struct SMultiplierInsertResult { void* node; bool inserted; };   // pair<iterator, bool>
struct CMultiplierMap {                         // std::map<void*, float> (12 bytes)
    unsigned char _body[0xc];
    SMultiplierInsertResult insert(const std::pair<void* const, float>& value);   // 0x00580D21
};
class CTCHaste {                                // interface 0x8c
public:
    unsigned char  _pad0[0xc];
    float          Multiplier;                  // +0x0c
    unsigned char  _pad10[0x18 - 0x10];
    CMultiplierMap Multipliers;                 // +0x18
    void SetActionSpeedMultiplier(void* key, float multiplier)   // out of line at 0x005812AE
    {
        Multipliers.insert(std::pair<void* const, float>(key, multiplier));
        UpdateActionSpeedMultiplier();
    }
private:
    void UpdateActionSpeedMultiplier();         // 0x00800020
};

class CBaseIntelligentPointer {
public:
    void*        __vftable;
    CBaseObject* m_pItem;                       // +0x04
    CBaseObject* GetPItem() const;              // 0x00A01B50
};

class CCreatureAction_UnsheatheItemFromInventory {
public:
    virtual void FinishAction();                // 0x0069FC40
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0xb8 - 0x10];
};

class CCreatureAction_UnsheatheItemFromInventoryWithHasteAction : public CCreatureAction_UnsheatheItemFromInventory {
public:
    void* HasteKey;                             // +0xb8
    virtual void FinishAction();
};

void CCreatureAction_UnsheatheItemFromInventoryWithHasteAction::FinishAction()
{
    CCreatureAction_UnsheatheItemFromInventory::FinishAction();
    const CUnprovenTC_0079c740* source;
    if (((const CThing*)PCreature.GetPItem())->PeekTC(&source, 0x92)) {
        CTCHaste* haste;
        if (!((CThing*)PCreature.GetPItem())->GetTC(&haste, 0x8c))
            haste = (CTCHaste*)((CThing*)PCreature.GetPItem())->AddTC("CTCHaste", true, 0);
        haste->SetActionSpeedMultiplier(HasteKey, source->GetSpeedFactor());
    }
}
