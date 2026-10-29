// GiveHeroMeleeAbility(long, bool) -- (anonymous namespace) add or remove a melee flourish ability on the live hero's combat component
// Leaf: ?GiveHeroMeleeAbility@?A0x
// Retail: Fable.exe @ 0x00736910, 120 bytes, __fastcall (ecx = ability def index, dl = give).
// Identity (proven): Ego_r.pdb `?GiveHeroMeleeAbility@?A0x7e2a648c@@YIXJ_N@Z` (fastcall, long + bool), sitting before
// GiveHeroMeleeAbilityFlourishLevel1..4 exactly as the retail callers 0x00736990.. do (each builds a def index from a
// CCharString and calls this with the vector's bool argument); the callees are CTCCombat::AddMeleeFlourishAbility(long)
// 0x00736640 and RemoveMeleeFlourishAbility(long) 0x007366C0 (Ego_r neighbours of this function's PDB block).
// Callees: CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0,
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020.
// Globals: g_pMainGameComponent 0x013B86A0 (CMainGameComponent::PPlayerManager +0x1c, Ego_r).
// Layout: CThing interface bitset +0x20 (interface 0x30 = CTCCombat: dword +0x24 bit 16), interface map +0x44,
// dead flag +0x91 bit 0 (the landed CThingAICreature::OnHeroPush 0x008328D0 model).
// Shape: the inlined CThing::GetInterface template with an UNINITIALISED component local whose miss path reads the
// key temporary's frame slot (landed CThingNoise::PassToNoiseComponent 0x008386E0); the result is not checked.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                          // CVectorMap<ETCInterfaceType, CTCBase*>: {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                     // +0x91 bit 0
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
class CThingPlayerCreature : public CThing { public:
};
class CTCCombat { public:
    void AddMeleeFlourishAbility(long defIndex);       // 0x00736640
    void RemoveMeleeFlourishAbility(long defIndex);    // 0x007366C0
};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer& GetMainPlayer();                   // 0x00449970
};
class CMainGameComponent { public:
    unsigned char _pad00[0x1c];
    CPlayerManager* PPlayerManager;             // +0x1c scoped_ptr<CPlayerManager>
};
extern CMainGameComponent* g_pMainGameComponent;    // 0x013B86A0

#pragma auto_inline(off)
namespace {
void __fastcall GiveHeroMeleeAbility(long defIndex, bool give)
{
    CThingPlayerCreature* hero = g_pMainGameComponent->PPlayerManager->GetMainPlayer().GetPControlledCreature();
    if (hero && !hero->Dead) {
        CTCCombat* combat;
        hero->GetInterface(&combat, 0x30);
        if (give)
            combat->AddMeleeFlourishAbility(defIndex);
        else
            combat->RemoveMeleeFlourishAbility(defIndex);
    }
}
}
#pragma auto_inline(on)

void __fastcall CallGiveHeroMeleeAbility(long defIndex, bool give)    // keeps the internal-linkage leaf emitted
{
    GiveHeroMeleeAbility(defIndex, give);
}