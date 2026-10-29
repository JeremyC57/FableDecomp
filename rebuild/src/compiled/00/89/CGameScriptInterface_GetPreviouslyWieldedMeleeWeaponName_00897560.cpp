// CGameScriptInterface::GetPreviouslyWieldedMeleeWeaponName(CCharString& name) const -- the definition name of the
// melee weapon the hero wielded before (hero TC 0x13), true when there is one with a non-empty name
// Leaf: ?GetPreviouslyWieldedMeleeWeaponName@CGameScriptInterface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00897560, length 208 bytes, `ret 4`.
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetPControlledCreature @ 0x00487DC0, the TC map lower bound
// copy @ 0x004365B0, CDefinitionManager::GetDefNameFromGlobalIndex(long) @ 0x009ACCC0 (CDefString by hidden
// pointer), CDefStringTable::GetString(long) @ 0x009D49B0 on the global table 0x013CA828 (CCharString by hidden
// pointer), CCharString::operator= @ 0x0099EFB0, ~CCharString @ 0x0099EAE0, CCharString::operator!=(const char*)
// @ 0x0099E960 ("" @ 0x0122D70E).
// Layout: CGameScriptInterface DefinitionManager +0x10, PlayerManager +0x14; CThing TC bitset +0x20 (TC 0x13 = bit
// 19), TC map +0x44, dead bit +0x91; TC 0x13's previously wielded melee weapon (global def index) +0x168.
// Shape: the out-parameter GetInterface template keeps the component in eax (`mov eax,[eax+4] ; mov eax,[eax+N]`); the CDefString and CCharString temporaries take the two
// `sub esp,8` frame words (the string returned through an inline CDefString::GetString()); the success test is an
// `if` so both failures share the `xor al,al` tail.

class CCharString {
public:
    void* PStringData;
    ~CCharString();                                 // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    bool operator!=(const char* text) const;        // 0x0099E960
};

struct CDefStringTable {
public:
    CCharString GetString(long) const;   // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;            // 0x013CA828

class CDefString { public:
    long Index;
    CCharString GetString() const { return g_DefStringTable.GetString(Index); }
};

class CDefinitionManager { public:
public:
    CDefString GetDefNameFromGlobalIndex(long) const;   // 0x009ACCC0
};

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                    // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;           // 0x004365B0 (this TU's copy of the 0x0040F020 body)
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    virtual void v000();
    unsigned char _pad04[0x1c];
    unsigned long m_presentBits[9];       // +0x20 std::bitset of the interface types present
    CInterfaceMap m_interfaces;           // +0x44
    unsigned char _pad4c[0x45];
    unsigned char m_dead : 1;             // +0x91 bit 0
    unsigned char m_91_1 : 7;
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };

struct CTCHeroWeaponHistory {           // TC 0x13
    unsigned char _pad[0x168];
    long PreviouslyWieldedMeleeWeapon;    // +0x168 (global def index)
};

class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;               // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer& GetMainPlayer();             // 0x00449970 (the non-const overload; 0x00449960 is the const one)
};

class CGameScriptInterface { public:
    virtual void v000();
    void* World;                          // +0x04
    void* Component;                      // +0x08
    void* DisplayEngine;                  // +0x0c
    CDefinitionManager* DefinitionManager;    // +0x10
    CPlayerManager* PlayerManager;        // +0x14
    virtual bool GetPreviouslyWieldedMeleeWeaponName(CCharString& name) const;
};

bool CGameScriptInterface::GetPreviouslyWieldedMeleeWeaponName(CCharString& name) const
{
    CThing* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    CTCHeroWeaponHistory* history;
    if (hero && !hero->m_dead && hero->GetInterface(&history, 0x13)) {
        long weapon = history->PreviouslyWieldedMeleeWeapon;
        if (weapon > 0) {
            name = DefinitionManager->GetDefNameFromGlobalIndex(weapon).GetString();
            if (name != "")
                return true;
        }
    }
    return false;
}