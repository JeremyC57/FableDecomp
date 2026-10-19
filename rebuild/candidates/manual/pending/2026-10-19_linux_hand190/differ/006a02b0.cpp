// CCreatureAction_UnsheatheItemFromInventoryWithHasteAction::FinishAction() -- the unsheathe FinishAction, then (owner has a physics component) register the owner's air resistance under HasteKey in the owner's CTCHaste component (added on demand) and refresh it
// Grade: DIFFER(240v240) -- frame only: retail sub esp,14h keeps the key/CTCHaste-string word apart from the {key,float} value and the insert result (offsets +4/+0Ch), ours sub esp,10h shares the string word with the value
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A02B0, length 240 bytes, plain `ret` (virtual).
// Callees: CCreatureAction_UnsheatheItemFromInventory::FinishAction @ 0x0069FC40, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound(const ETCInterfaceType*) @ 0x004365B0 (the enum-keyed GetInterface 0x92,
// bit 18 of +0x30), CVectorMap::LowerBound(const long*) @ 0x0040F020 (the long-keyed lookup of 0x8C, bit 12 of +0x30),
// CCharString::CCharString(const char*, long) @ 0x0099EBF0, CThing::AddTC(const CCharString&, bool, CTCParamsBase*) @ 0x004C9D60,
// CCharString::~CCharString @ 0x0099EAE0, CTCPhysicsBase::GetAirResistance @ 0x0079C740 (float), the haste map's
// insert(const value_type&) @ 0x00580D21 (pair<iterator, bool> through the hidden pointer), the haste component's refresh
// @ 0x00800020 (manifest label is noise).
// Shape: the physics lookup is the CHECKED enum-keyed out-parameter form; the haste component is either looked up (bit set) or
// added through a block-scoped "CTCHaste" string that shares the lookup key's frame word (VC forms `lea ecx,[esp+10h]` once for
// both arms); the {key, float} value is a temporary with a two-argument constructor (float stored first, key after the first
// push).
// Layout: owner CIntelligentPointer +8; Ego_r.pdb HasteKey +0xc4, retail +0xb8 (CCreatureActionBase is 0xa8 in retail).
enum ETCInterfaceType { TC_PHYSICS = 0x92, TC_HASTE = 0x8c };
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
    SEntry* LowerBound(const ETCInterfaceType* type);  // 0x004365B0
};
struct CTCPhysicsBase {
    float GetAirResistance();                          // 0x0079C740
};
struct SHasteValue {                                   // map<void*, float>::value_type
    void* first;
    float second;
    SHasteValue(void* key, float value) : first(key), second(value) { }
};
struct SHasteInsertResult {                            // pair<iterator, bool>
    void* node;
    bool  inserted;
    SHasteInsertResult() { }
};
struct CHasteMap {
    SHasteInsertResult insert(const SHasteValue& value);   // 0x00580D21
};
struct CTCHaste {
    unsigned char _pad[0x18];
    CHasteMap     m_modifiers;                         // +0x18
    void RecalculateHaste();                           // 0x00800020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                    // +0x20
    CInterfaceMap m_interfaces;                        // +0x44
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
    template<class T> __forceinline bool GetInterface(T** ppOut, ETCInterfaceType interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
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
};

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                          // 0x00A01B50
};

class CCreatureAction_UnsheatheItemFromInventory {
public:
    virtual void FinishAction();                       // 0x0069FC40
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;                 // +0x08
    unsigned char           _pad10[0xb8 - 0x10];
};

class CCreatureAction_UnsheatheItemFromInventoryWithHasteAction : public CCreatureAction_UnsheatheItemFromInventory {
public:
    void* HasteKey;                                    // +0xb8 (PDB +0xc4)
    virtual void FinishAction();
};

void CCreatureAction_UnsheatheItemFromInventoryWithHasteAction::FinishAction()
{
    CCreatureAction_UnsheatheItemFromInventory::FinishAction();
    CTCPhysicsBase* physics;
    if (PCreature.GetPItem()->GetInterface(&physics, TC_PHYSICS)) {
        CTCHaste* haste;
        if (!PCreature.GetPItem()->GetInterface(&haste, 0x8c)) {
            CCharString name("CTCHaste", -1);
            haste = (CTCHaste*)PCreature.GetPItem()->AddTC(name, true, 0);
        }
        haste->m_modifiers.insert(SHasteValue(HasteKey, physics->GetAirResistance()));
        haste->RecalculateHaste();
    }
}
