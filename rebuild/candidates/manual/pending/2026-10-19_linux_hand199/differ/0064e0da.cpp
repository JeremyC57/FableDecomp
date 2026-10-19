#pragma optimize("y", off)
#pragma optimize("s", on)
// NPlayerGui::CDrawGuildSeal::AddMessage(const CCharString& message, long duration, bool force) -- queue a guild-seal message (when forced or nothing is queued) for a live adult hero who carries at least one OBJECT_GUILD_SEAL_1
// Grade: DIFFER(228v228) -- register swap only: retail keeps `this` in ebx and the 0x11 constant in edi, ours the reverse; the inline-accessor form gives the same swap, the plain member chain keeps the registers but loads the manager after pushing the name
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064E0DA, length 228 bytes (ret 0Ch; ebp frame closed with `leave`).
// Pragmas: `#pragma optimize("y", off)` + `#pragma optimize("s", on)` (the HUD TU).
// Callees: CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetControlledCreature 0x00487DD0, IS_THING_ALIVE<CThingPhysical*>
// 0x0043DBF0 (as in 0x0064B5C1), CThingCreatureBase::IsChild 0x00661F70, std::bitset::test 0x00410DE0, CVectorMap::LowerBound
// const 0x004365B0 (interface 0x11 = the inventory), CCharString(const char*, long) 0x0099EBF0, the narrow copy ctor 0x0099EC30,
// ~CCharString 0x0099EAE0, CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) 0x009AD410,
// CTCInventoryBase::GetNumberOfItemsOfTypeInInventory(long) 0x005BDF08, deque<pair<CCharString, long> >::push_back 0x00656E0C
// (manifest "ComboMultiplierData_ConstructOrAppend" is a propagation).
// Layout: CDrawBase PlayerManager +8, World +0xC (its definition manager at +0x10); CDrawGuildSeal Messages +0x68 (the queue's
// STLport deque: start cursor +0x68, finish cursor +0x78; PDB +0x70).
// Shape: the GetActivationTimeForButton hero/alive/presence shape with the unchecked returning lookup; the def name is a
// block-scoped string (its frame word is the lookup key's) and the definition manager a named pointer loaded after it
// (so it is in ecx before the name's address is pushed); the queued pair (a
// `__forceinline` constructor in this size TU) copies the message then stores the duration.
struct CTCBase;

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
};

struct CPresenceBits {
    bool test(long bit);                           // 0x00410DE0 (std::bitset::test)
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;    // 0x004365B0 (const copy)
};

struct CThing {
    unsigned char _pad0[0x20];
    CPresenceBits m_present;                       // +0x20
    unsigned char _pad1[0x23];
    CInterfaceMap m_interfaces;                    // +0x44
    bool HasInterface(long type) { return m_present.test(type); }
    template <class T> __forceinline T* GetInterfaceUnchecked(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        if (it == map->last || type < it->type)
            it = map->last;
        return (T*)it->component;
    }
};
struct CThingPhysical : CThing { };
struct CThingCreatureBase : CThingPhysical {
    bool IsChild() const;                          // 0x00661F70
};

template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0043DBF0 (CThingPhysical*)

struct CPlayer {
    CThing* GetControlledCreature();               // 0x00487DD0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                      // 0x00449970
};
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CWorld {
    unsigned char       _pad0[0x10];
    CDefinitionManager* DefinitionManager;         // +0x10
};
struct CTCInventoryBase {                          // the thing's interface 0x11
    long GetNumberOfItemsOfTypeInInventory(long defIndex);   // 0x005BDF08
};

struct SGuildSealMessage {                         // pair<CCharString, long>
    CCharString first;
    long        second;
    __forceinline SGuildSealMessage(const CCharString& text, const long& duration) : first(text), second(duration) { }
};
struct CMessageQueue {                             // queue<pair<CCharString, long> > over an STLport deque
    SGuildSealMessage* m_startCur;                 // +0x00
    void*              m_start[3];
    SGuildSealMessage* m_finishCur;                // +0x10
    void*              m_finish[3];
    void*              m_map[2];
    bool empty() const { return m_finishCur == m_startCur; }
    void push(const SGuildSealMessage& value);     // 0x00656E0C (deque::push_back)
};

enum { TC_INVENTORY = 0x11 };

namespace NPlayerGui {

struct CDrawGuildSeal {
    void*           _vtable;                       // +0x00
    void*           PlayerGui;                     // +0x04
    CPlayerManager* PlayerManager;                 // +0x08
    CWorld*         World;                         // +0x0c
    unsigned char   _pad10[0x68 - 0x10];
    CMessageQueue   Messages;                      // +0x68
    void AddMessage(const CCharString& message, long duration, bool force);
};

void CDrawGuildSeal::AddMessage(const CCharString& message, long duration, bool force)
{
    CThingCreatureBase* hero = static_cast<CThingCreatureBase*>(PlayerManager->GetMainPlayer()->GetControlledCreature());
    if (IS_THING_ALIVE<CThingPhysical*>(hero) && !hero->IsChild() && hero->HasInterface(TC_INVENTORY)) {
        CTCInventoryBase* inventory = hero->GetInterfaceUnchecked<CTCInventoryBase>(TC_INVENTORY);
        long seal;
        {
            CCharString name("OBJECT_GUILD_SEAL_1", -1);
            CDefinitionManager* definitions = World->DefinitionManager;
            seal = definitions->GetDefGlobalIndexFromName(name);
        }
        if (seal > 0 && inventory->GetNumberOfItemsOfTypeInInventory(seal) > 0 && (force || Messages.empty()))
            Messages.push(SGuildSealMessage(message, duration));
    }
}

}
