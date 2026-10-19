// CCreatureAction_UnsheatheItemFromInventoryWithHasteAction::FinishAction() -- the plain unsheathe FinishAction, then (when the creature has the interface-0x92 component) register that component's speed factor as a haste multiplier under this action's HasteKey on the creature's CTCHaste (added when missing) and recalculate the haste
// Grade: DIFFER(240v240) -- frame only: retail keeps a 0x14 frame with the multiplier pair at [esp+14h] beside the key/string word, ours shares a 0x10 frame (temporary, named local and make-pair forms alike)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A02B0, length 240 bytes (plain `ret`, virtual).
// Callees: CCreatureAction_UnsheatheItemFromInventory::FinishAction 0x0069FC40 (direct base call), CBaseIntelligentPointer::GetPItem
// 0x00A01B50, CVectorMap::LowerBound const 0x004365B0 (interface 0x92) and non-const 0x0040F020 (interface 0x8C = CTCHaste),
// CThing::AddTC(const CCharString&, bool, CTCParamsBase*) 0x004C9D60 ("CTCHaste", active), CCharString 0x0099EBF0 / 0x0099EAE0,
// the 0x92 component's float getter 0x0079C740 (manifest "CTCPhysicsBase::GetAirResistance"; class not proven),
// map<void*, float>::insert 0x00580D21 on CTCHaste::Multipliers (+0x18), CTCHaste::RecalculateSpeed 0x00800020.
// Layout: retail HasteKey +0xB8 (PDB +0xC4: the debug CCreatureActionBase is 0xC larger).
// Shape: the 0x92 lookup is the out-parameter GetInterface over the const map copy; CTCHaste comes from GetInterface or
// AddTC, whose string is the implicit const char* conversion (built before the argument pushes); the pair is built
// value-first (the getter runs before HasteKey is stored).
struct CTCBase;
struct CTCParamsBase;

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);              // 0x0040F020
    const SEntry* LowerBound(const long* type) const;  // 0x004365B0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
    template<class T> __forceinline bool GetInterface(const T** ppOut, long interfaceType) const
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

struct CTCSpeedSource {                         // interface 0x92
    float GetSpeedFactor() const;               // 0x0079C740
};

struct SMultiplier {                            // pair<void* const, float>
    void* first;
    float second;
    SMultiplier(void* key, float value) : first(key), second(value) { }
};
struct SInsertResult { void* node; bool inserted; };
struct CMultiplierMap {                         // map<void*, float>
    unsigned char _body[0xc];
    SInsertResult insert(const SMultiplier& value);   // 0x00580D21
};
struct CTCHaste {                               // interface 0x8c
    unsigned char  _pad0[0x18];
    CMultiplierMap Multipliers;                 // +0x18
    void RecalculateSpeed();                    // 0x00800020
};

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

struct CCreatureAction_UnsheatheItemFromInventory {
    virtual void v00();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0xb8 - 0x10];
    void FinishAction();                        // 0x0069FC40
};

struct CCreatureAction_UnsheatheItemFromInventoryWithHasteAction : CCreatureAction_UnsheatheItemFromInventory {
    void* HasteKey;                             // +0xb8
    void FinishAction();
};

void CCreatureAction_UnsheatheItemFromInventoryWithHasteAction::FinishAction()
{
    CCreatureAction_UnsheatheItemFromInventory::FinishAction();
    const CTCSpeedSource* source;
    if (((const CThing*)PCreature.GetPItem())->GetInterface(&source, 0x92)) {
        CTCHaste* haste;
        if (!PCreature.GetPItem()->GetInterface(&haste, 0x8c))
            haste = (CTCHaste*)PCreature.GetPItem()->AddTC("CTCHaste", true, 0);
        haste->Multipliers.insert(SMultiplier(HasteKey, source->GetSpeedFactor()));
        haste->RecalculateSpeed();
    }
}
