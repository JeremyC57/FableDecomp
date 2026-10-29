#pragma optimize("y", off)
#pragma optimize("s", on)
// NPlayerGui::CDrawGuildSeal::AddMessage(const CCharString& text, long time, bool force) -- queue a guild-seal message when the hero is alive, not a child and carries a guild seal (OBJECT_GUILD_SEAL_1) in the inventory; unless forced, only when no message is queued
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- the interface lookup written as the ternary entry pick (`((it == last || type < it->type) ? last : it)->component`, the landed 0x00584157 idiom): the component loads straight into ecx
// Leaf: AddMessage
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064E0DA, length 226 bytes, `ret 0Ch`.
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::PeekPControlledCreature @ 0x00487DD0,
// IS_THING_ALIVE<T*>(const T*&) @ 0x0043DBF0 (`__fastcall`, by reference), CThingCreatureBase::IsChild @ 0x00661F70,
// std::bitset::test @ 0x00410DE0 (the thing's interface presence bits), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x004365B0, CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("OBJECT_GUILD_SEAL_1" @ 0x01251B4C),
// CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) @ 0x009AD410, ~CCharString @ 0x0099EAE0,
// CTCInventoryBase::GetNumberOfItemsOfTypeInInventory(long) @ 0x005BDF08, CCharString copy ctor @ 0x0099EC30, the
// message deque's push_back @ 0x00656E0C.
// Layout: CDrawGuildSeal: player manager +8, the def-manager owner +0xc (definition manager at its +0x10), the message
// deque<SMessage> +0x68 (STLport: start iterator +0x68, finish +0x78). CThing interface bits +0x20, interface map +0x44
// (landed 0x007BDCFC model); interface 0x11 = the inventory.
// Shape (best of 4): a frame-pointer size TU; IS_THING_ALIVE<CThing*> binds the derived hero through a converted
// temporary (the hero stays in esi), the definition manager is a named local read after the seal name, the message
// ctor is __forceinline; the alive test's thing copy, the interface key, the seal name and the queued
// message's time all share the frame word [ebp-4] (each in its own block); the interface lookup is the landed
// `__forceinline` GetInterfaceUnchecked template after an inline HasInterface.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);        // 0x0099EC30
    ~CCharString();                               // 0x0099EAE0
};

class CTCBase;
struct CPresenceBits {
    bool test(long bit) const;                           // 0x00410DE0 (std::bitset::test)
};
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
    unsigned char _pad0[0x20];
    CPresenceBits m_present;                       // +0x20
    unsigned char _pad1[0x23];
    CInterfaceMap m_interfaces;                    // +0x44
    bool HasInterface(long type) const { return m_present.test(type); }
    template <class T> __forceinline T* GetInterfaceUnchecked(long type) const
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(key);
        return (T*)((it == map->last || type < it->first) ? map->last : it)->second;
    }
};
template <class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing)   // 0x0043DBF0
{
    return thing != 0 && !(reinterpret_cast<const unsigned char*>(thing)[0x91] & 1);   // CThing dead bit +0x91
}
class CThingCreatureBase : public CThing { public:
public:
    bool IsChild() const;   // 0x00661F70
};
class CThingPlayerCreature : public CThingCreatureBase { };
class CPlayer { public:
    const CThingPlayerCreature* PeekPControlledCreature() const;   // 0x00487DD0
};
class CPlayerManager { public:
public:
    CPlayer& GetMainPlayer();   // 0x00449970
};
class CDefinitionManager { public:
public:
    long GetDefGlobalIndexFromName(const CCharString&) const;   // 0x009AD410
};
struct CDefinitionOwner {
    unsigned char _pad[0x10];
    CDefinitionManager* DefinitionManager;         // +0x10
};
struct CTCInventoryBase {
    long GetNumberOfItemsOfTypeInInventory(long type);   // 0x005BDF08
};

namespace NPlayerGui {
struct SGuildSealMessage {
    CCharString Text;                              // +0
    long Time;                                     // +4
    __forceinline SGuildSealMessage(const CCharString& text, long time) : Text(text), Time(time) { }
};
struct CMessageDeque {                             // STLport deque<SGuildSealMessage>
    void* StartCur; void* StartFirst; void* StartLast; void* StartNode;   // +0x00 start iterator
    void* FinishCur; void* FinishFirst; void* FinishLast; void* FinishNode;   // +0x10 finish iterator
    bool empty() const { return FinishCur == StartCur; }
    void push_back(const SGuildSealMessage& message);   // 0x00656E0C
};
class CDrawGuildSeal { public:
    unsigned char _pad0[8];
    CPlayerManager* PlayerManager;                 // +0x08
    CDefinitionOwner* Definitions;                 // +0x0c
    unsigned char _pad1[0x58];
    CMessageDeque Messages;                        // +0x68
    void AddMessage(const CCharString& text, long time, bool force);
};

void CDrawGuildSeal::AddMessage(const CCharString& text, long time, bool force)
{
    const CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().PeekPControlledCreature();
    if (!IS_THING_ALIVE(hero) || hero->IsChild())
        return;
    if (!hero->HasInterface(0x11))
        return;
    CTCInventoryBase* inventory = hero->GetInterfaceUnchecked<CTCInventoryBase>(0x11);
    long seal;
    {
        CCharString name("OBJECT_GUILD_SEAL_1");
        CDefinitionManager* definitions = Definitions->DefinitionManager;
        seal = definitions->GetDefGlobalIndexFromName(name);
    }
    if (seal > 0 && inventory->GetNumberOfItemsOfTypeInInventory(seal) > 0 && (force || Messages.empty()))
        Messages.push_back(SGuildSealMessage(text, time));
}
}