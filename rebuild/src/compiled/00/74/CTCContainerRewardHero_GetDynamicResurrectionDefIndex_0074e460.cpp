// CTCContainerRewardHero::GetDynamicResurrectionDefIndex() -- the resurrection phial's def index when the live hero carries no phial yet (so the container should reward one), else 0
//
// Retail 0x0074E460, 151 bytes (plain `ret`). Callees: CTCBase::GetPlayerManager @ 0x00686D50
// (`[0x013B8A1C]+0x30`; manifest "CViewBase::GetDisplayEngine", propagated), CPlayerManager::
// GetMainPlayer @ 0x00449960, CPlayer::GetControlledCreature @ 0x00487DD0 (label propagated),
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0, CCharString::CCharString(const
// char*, long) @ 0x0099EBF0, CGameDefinitionManager::Get() @ 0x0044C6B0, CDefinitionManager::
// GetDefGlobalIndexFromName(const CCharString&) @ 0x009AD410, ~CCharString @ 0x0099EAE0,
// CTCInventoryBase::GetNumberOfItemsOfTypeInInventory(long) @ 0x005BDF08. String 0x0125A2C4
// "OBJECT_RESURRECTION_PHIAL".
// Shape: the inline IsThingAlive (null, dead bit +0x91) and the whole-word interface-0x11 test
// (`test dword [eax+20h],20000h`) in one `&&` chain; the inventory is a named local (edi); the name is
// a block-scoped CCharString destroyed right after the index lookup; `== 0` returns the index, every
// other path the shared `xor eax,eax`.
struct CTCBase;

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;                   // 0x004365B0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_stateFlags;                             // +0x91 (bit 0 = dead)
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetInterface(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};

inline bool IsThingAlive(const CThing* thing)
{
    return thing && !(thing->m_stateFlags & 1);
}

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};

struct CGameDefinitionManager : CDefinitionManager {
    static CGameDefinitionManager& Get();                   // 0x0044C6B0
};

struct CTCInventoryBase {
    long GetNumberOfItemsOfTypeInInventory(long defIndex);  // 0x005BDF08
};

struct CPlayer {
    CThing* GetControlledCreature();                        // 0x00487DD0
};

struct CPlayerManager {
    CPlayer* GetMainPlayer() const;                               // 0x00449960
};

struct CTCContainerRewardHero {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    CPlayerManager* GetPlayerManager();                     // 0x00686D50 (CTCBase)
    long GetDynamicResurrectionDefIndex();
};

long CTCContainerRewardHero::GetDynamicResurrectionDefIndex()
{
    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetControlledCreature();
    if (IsThingAlive(hero) && hero->HasInterface(0x11))
    {
        CTCInventoryBase* inventory = (CTCInventoryBase*)hero->GetInterface(0x11);
        long index;
        {
            CCharString name("OBJECT_RESURRECTION_PHIAL", -1);
            index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name);
        }
        if (inventory->GetNumberOfItemsOfTypeInInventory(index) == 0)
            return index;
    }
    return 0;
}