// CTCBoss::FrameUpdate() -- while strikes are being counted, reset the count once the hero has been hit in melee by this boss (a matching CEventHitBy in the hero's entity events)
// Leaf: ?FrameUpdate@CTCBoss@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007AF050, length 153 bytes (+7 int3 padding).
// Callees: the CTCBase player-manager accessor @ 0x00686D40, CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetPControlledCreature
// @ 0x00487DC0, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0 (inlined GetInterface for CTCEntityEvents, type
// 0x42 = bit 2 of +0x28), CTCEntityEvents::GetEvent<const CEventHitBy, CFilter_Bind2<CIsHitEventHitByMelee, CIsHitEventHitBy>>
// @ 0x007AFB90.
// Shape: both predicates are argument temporaries of the Bind2 filter (named locals swap their frame slots): the empty melee
// predicate at esp+0xB, the hit-by predicate holding the owner thing at esp+0xC (sharing the interface-type key slot), the
// Bind2 filter is two pointers to them (esp+0x14), the event out pointer sits at esp+0x10; filter by reference pushed first.
// Layout: PDB CTCBoss (0x50) with retail 12-byte vectors (0x48): PBossDef +0xC, GenericCounter +0x10, StrikesSinceHit +0x14,
// currentBossPhase +0x18, NextWeapon +0x1C, BCountStrikes +0x20, EnablePhaseChanges +0x21, PhaseStartPos +0x22 (unaligned, so
// C3DVector is declared packed), HelperCreatures +0x30, Effects +0x3C; retail CThing: bitset +0x20, map +0x44, base def +0x70,
// Dead +0x91.
#pragma pack(push, 1)
class C3DVector { public:                              // packed: CTCBoss keeps one at the unaligned +0x22
    float X, Y, Z;
};
#pragma pack(pop)
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;       // 0x0040F020 (the FrameUpdate copy is 0x004365B0)
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CDefClassBase;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* PBaseDef;                    // +0x70 CDefPointer<CThingBaseDef const> object word
    unsigned char _pad74[0x91 - 0x74];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    void Kill(bool immediately);                // 0x004C9B80
    const C3DVector& GetPos() const;       // 0x004C73D0
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
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
inline bool IsThingAlive(const CBaseIntelligentPointer& p) { return p.GetPItem() && !p.GetPItem()->IsDead(); }
template <class T> struct CRetailVector {       // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    T* begin() const { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) const { return *(begin() + n); }
};
class CBossDef;
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0 (CPlayer::GetPControlledCreature)
};
class CPlayerManager { public:
public:
    CPlayer& GetMainPlayer();   // 0x00449970
};
template <class T> class CDefPointer { public:
    T* Data;
    CDefPointer() : Data(0) {}
};
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<const CBossDef>* out) const;    // 0x007AFA90 (catalog: a byte-twin instantiation)
};

namespace NEntityEvents {
class CEventHitBy;
class CIsHitEventHitByMelee { public: CIsHitEventHitByMelee() {} };
class CIsHitEventHitBy { public:
    CThing* Attacker;
    CIsHitEventHitBy(CThing* attacker) : Attacker(attacker) {}
};
}
template <class A, class B> class CFilter_Bind2 { public:
    const A* PA;
    const B* PB;
    CFilter_Bind2(const A& a, const B& b) : PA(&a), PB(&b) {}
};
class CTCEntityEvents { public:
    bool GetEvent(const NEntityEvents::CEventHitBy** out,
                  const CFilter_Bind2<NEntityEvents::CIsHitEventHitByMelee, NEntityEvents::CIsHitEventHitBy>& filter);   // 0x007AFB90
};
class CTCBase {
public:
    CTCBase(CThing* owner);                     // 0x00686800
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
protected:
    CPlayerManager& GetPlayerManager();   // 0x00686D40
};
class CTCBoss : public CTCBase {                // vtable 0x0126D494
public:
    CDefPointer<const CBossDef> PBossDef;       // +0x0c
    long GenericCounter;                        // +0x10
    long StrikesSinceHit;                       // +0x14
    long currentBossPhase;                      // +0x18
    long NextWeapon;                            // +0x1c
    bool BCountStrikes;                         // +0x20
    bool EnablePhaseChanges;                    // +0x21
    C3DVector PhaseStartPos;                    // +0x22
    CRetailVector<CBaseIntelligentPointer> HelperCreatures;   // +0x30 vector<CIntelligentPointer<CThingAICreature>>
    CRetailVector<CBaseIntelligentPointer> Effects;           // +0x3c vector<CIntelligentPointer<CThing>>
    CTCBoss(CThing* owner);
    virtual ~CTCBoss();
    void ClearEffects();
    virtual void FrameUpdate();
};

void CTCBoss::FrameUpdate()
{
    if (StrikesSinceHit > 0) {
        CThing* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCEntityEvents* events;
        if (hero && !hero->IsDead() && hero->GetInterface(&events, 0x42)) {
            const NEntityEvents::CEventHitBy* hit;
            if (events->GetEvent(&hit, CFilter_Bind2<NEntityEvents::CIsHitEventHitByMelee, NEntityEvents::CIsHitEventHitBy>(
                    NEntityEvents::CIsHitEventHitByMelee(), NEntityEvents::CIsHitEventHitBy(ParentThing))))
                StrikesSinceHit = 0;
        }
    }
}