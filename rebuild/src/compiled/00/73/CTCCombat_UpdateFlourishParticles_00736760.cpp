// CTCCombat::UpdateFlourishParticles() -- keep the melee-flourish particles in step: kill them all when no flourish is valid; otherwise wait for live ones and the cooldown, then start a new particle on a melee weapon held in the primary slot
// Grade: RELOCATION_MATCH (427v427) -- retry wave 2 sec2b: CThing::IsDead() const (Ego_r ?IsDead@CThing@@QBE_NXZ) for the
//   dead tests gives retail's shrink-wrapped ebx and loop registers. Was DIFFER(427v427) -- register allocation only: retail shrink-wraps the ebx push into the particle branch and swaps the first loop's index/element registers; same instructions otherwise
// Retail 0x00736760, 432 bytes (plain `ret`). Callees: CTCCombat::IsMeleeFlourishValid @ 0x00733EE0,
// CIntelligentPointer<CThing>::GetPItem @ 0x00A01B50, CThing::Kill(bool) @ 0x004C9B80,
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const @ 0x004365B0 (inlined GetInterface: 0x46 carrying,
// 0x48 weapon), CTCCarrying::GetThingInPrimarySlot @ 0x006A1440, CTCWeapon::IsMeleeWeapon @ 0x005DAA00,
// CTCCombat::AttemptToStartParticle(CThing*) @ 0x00735FD0.
// Layout: engine/CTCCombat.h is the PDB view; retail CurFlourishTimer is +0x84 and FlourishParticles (a 12-byte
// STLport vector of 8-byte CIntelligentPointer<CThing>) +0x88, 0x10 below the PDB (+0x94/+0x98). CThing Dead
// is bit 0 of +0x91, the interface bitset +0x20 (0x46 = +0x28 bit 6, 0x48 = +0x28 bit 8), the map +0x44.
class CTCBase;

enum ETCInterfaceType { };
namespace std { template <class _T1, class _T2> struct pair { _T1 first; _T2 second; }; }
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C = CKeyPairCompareLess<K, V> >
class CVectorMap {                                  // retail STLport vector<pair<K, V> > + comparator (only the used members)
public:
    typedef std::pair<K, V> value_type;
    value_type* _M_start;                           // +0x00
    value_type* _M_finish;                          // +0x04
    const value_type* LowerBound(const K& key) const;     // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*> CInterfaceMap;

class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                         // +0x91 bit 0
    bool IsDead() const { return Dead; }                // Ego_r.pdb ?IsDead@CThing@@QBE_NXZ
    void Kill(bool b);                              // 0x004C9B80
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(type);
            *ppOut = (T*)(it == map->_M_finish || interfaceType < it->first ? map->_M_finish : it)->second;
            return true;
        }
        return false;
    }
};

class CBaseObject;
class CBaseIntelligentPointer {
public:
    void* m_pHandle;
    CBaseObject* m_pItem;
    CBaseObject* GetPItem() const;                  // 0x00A01B50 Ego_r ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

template <class T> __forceinline bool __fastcall IS_THING_ALIVE(const T& p)   // Ego_r ??$IS_THING_ALIVE@V?$CIntelligentPointer@...@@@@YI_NABV...@Z
{
    return p.GetPItem() && !p.GetPItem()->IsDead();
}

class CTCCarrying {
public:
    CThing* GetThingInPrimarySlot() const;          // 0x006A1440 Ego_r ?GetThingInPrimarySlot@CTCCarrying@@QBEPAVCThing@@XZ
};
class CTCWeapon {
public:
    bool IsMeleeWeapon() const;                     // 0x005DAA00 Ego_r ?IsMeleeWeapon@CTCWeapon@@QBE_NXZ (def Class +0x2c in {1,2})
};

template<class T> struct CVector {                  // retail STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    int size() const { return int(_M_finish - _M_start); }
    T& operator[](int i) { return _M_start[i]; }
    T* begin() { return _M_start; }
};

class CTCCombat {
public:
    void*         __vftable;                        // +0x00
    CThing*       Thing;                            // +0x04 CTCBase parent thing
    unsigned char _pad08[0x84 - 0x08];
    long          CurFlourishTimer;                 // +0x84
    CVector<CIntelligentPointer<CThing> > FlourishParticles;   // +0x88
    bool IsMeleeFlourishValid() const;              // 0x00733EE0
private:
    void AttemptToStartParticle(CThing* weapon);    // 0x00735FD0
    void UpdateFlourishParticles();
public:
    __forceinline void KillFlourishParticles()
    {
        CurFlourishTimer = 0;
        for (int i = 0; i < FlourishParticles.size(); ++i) {
            if (IS_THING_ALIVE(*(FlourishParticles.begin() + i)))
                (*(FlourishParticles.begin() + i)).GetPItem()->Kill(true);
        }
    }
};

void CTCCombat::UpdateFlourishParticles()
{
    if (IsMeleeFlourishValid()) {
        for (int i = 0; i < FlourishParticles.size(); ++i) {
            if (IS_THING_ALIVE(FlourishParticles[i]))
                return;
        }
        if (CurFlourishTimer > 0) {
            --CurFlourishTimer;
            return;
        }
        CTCCarrying* carrying;
        if (!Thing->GetInterface(&carrying, 0x46))
            return;
        CThing* weapon = carrying->GetThingInPrimarySlot();
        if (!weapon || weapon->IsDead())
            return;
        CTCWeapon* weaponTC;
        if (!weapon->GetInterface(&weaponTC, 0x48))
            return;
        if (!weaponTC->IsMeleeWeapon())
            return;
        AttemptToStartParticle(weapon);
        return;
    }
    KillFlourishParticles();
}