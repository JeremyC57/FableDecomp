// CCreatureAction_TrollGroundPound::Initialise() -- base Initialise; with a troll-battle def, find or add the creature's CTCHaste and register this action's speed multiplier (lerp of the def's range by 1 - normalised value), then recompute
// Leaf: ?Initialise@CCreatureAction_TrollGroundPound
// Grade: DIFFER(268v268) -- only the named `range` float's frame word: ours frame+0 (the key/name word), retail frame+4 (the inserted pair's first word); tried unnamed and inline-helper forms
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C7280, 268 bytes (272 with padding), virtual Initialise.
// Callees: CCombatActionBase::Initialise 0x00857520 (direct, first), CBaseIntelligentPointer::GetPItem 0x00A01B50,
//   CDefClassBase::GetSubDef<CTrollBattleDef> 0x008C78F0, CVectorMap::LowerBound 0x0040F020 (inlined GetInterface 0x8C =
//   CTCHaste, bit 12 of +0x30), CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0 ("CTCHaste" 0x0123C2A0), CThing::AddTC
//   0x004C9D60, the creature's normalised value 0x006A4E40 (float; manifest label propagated), map<void*, float>::insert
//   0x00580D21, CTCHaste::RecalculateMultiplier 0x00800020. Float 1.0f at 0x0122DED8.
// Layout: CTrollBattleDef +0x3c / +0x40 the multiplier range; CTCHaste multiplier map at +0x18 (landed RemoveMultiplier).
// Shape: the creature's def is a named local (GetPItem runs before the out argument is pushed: `mov eax,[eax+70h] ;
//   lea ; push ; mov ecx,eax`); GetInterface-or-AddTC (the else block builds the name as a named local), the range a named float local that
//   shares its frame word with the inserted pair, `Min + range * (1 - value)`; the sub-def is held in ebp through the
//   GetSubDef same-TU stand-in (retail's LTCG knowledge).
struct C3DVector { float x, y, z; };
struct CTCBase;
struct CTCParamsBase;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CTrollBattleDef {
    virtual void v00();
    virtual void Destroy();                           // slot 1
    long RefCount;                                    // +0x04
    unsigned char _pad08[0x3c - 0x08];
    float MinMultiplier;                              // +0x3c
    float MaxMultiplier;                              // +0x40
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(T* object) : Object(object) { if (Object) ++Object->RefCount; }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer() { if (Object) { if (DecRefToZero(Object->RefCount)) Object->Destroy(); } }
    T* operator->() const { return Object; }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->RefCount == 0)
                    Object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};
struct CDefinitionManager {
    CDefPointer<CTrollBattleDef> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (hidden result pointer)
};
struct CDefClassBase {
    virtual void v00(); virtual void Destroy(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34();
    virtual long* GetSubDefIndex(const CCharString& name);   // slot 14 (+0x38)
    long RefCount;                                    // +0x04
    unsigned char _pad08[0x1c - 8];
    CDefinitionManager* m_pManager;                   // +0x1c
    bool GetSubDef(CDefPointer<CTrollBattleDef>* out);   // 0x008C78F0
};
#pragma auto_inline(off)
// Same-TU stand-in for GetSubDef<CTrollBattleDef> 0x00669580 (its landed body): VC7.1 then knows the sub-def pointer's
// address does not escape, so the def is held in esi and the name string reuses its frame word, as in retail.
bool CDefClassBase::GetSubDef(CDefPointer<CTrollBattleDef>* out)
{
    long* pIndex;
    {
        CCharString name("CTrollBattleDef");
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CDefPointer<CTrollBattleDef> result = m_pManager->GetPDefFromGlobalIndex(*pIndex);
        *out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* m_pDef;                            // +0x70
    float GetNormalisedValue();                       // 0x006A4E40 (name unconfirmed)
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
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
    CDefPointer<CDefClassBase> GetDef() const { return CDefPointer<CDefClassBase>(m_pDef); }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                  // +4
    CThing* GetPItem() const;                         // 0x00A01B50
};
struct CMultiplierEntry {                             // map<void*, float>::value_type
    void* first;
    float second;
    CMultiplierEntry(void* key, float value) : first(key), second(value) { }
};
struct CMultiplierMap {
    struct node;
    struct iterator { node* current; };
    struct insert_result { iterator first; bool second; };
    void* m_head;
    insert_result insert(const CMultiplierEntry& value);   // 0x00580D21
};
struct CTCHaste {                                     // interface type 0x8C
    unsigned char _pad00[0x18];
    CMultiplierMap Multipliers;                       // +0x18
    void RecalculateMultiplier();                     // 0x00800020
};
struct CTCParamsBase;

struct CCombatActionBase {                            // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0x114 - 0x10];
    void Initialise();                                // 0x00857520
};
struct CCreatureAction_TrollGroundPound : CCombatActionBase {
    void Initialise();
};

void CCreatureAction_TrollGroundPound::Initialise()
{
    CCombatActionBase::Initialise();
    CDefPointer<CTrollBattleDef> battle;
    CDefClassBase* creatureDef = PCreature.GetPItem()->m_pDef;
    if (creatureDef->GetSubDef(&battle)) {
        CTCHaste* haste;
        if (!PCreature.GetPItem()->GetInterface(&haste, 0x8C)) {
            CCharString name("CTCHaste");
            haste = (CTCHaste*)PCreature.GetPItem()->AddTC(name, false, 0);
        }
        if (haste) {
            float range = battle->MaxMultiplier - battle->MinMultiplier;
            haste->Multipliers.insert(CMultiplierEntry(this, battle->MinMultiplier + range * (1.0f - PCreature.GetPItem()->GetNormalisedValue())));
            haste->RecalculateMultiplier();
        }
    }
}
