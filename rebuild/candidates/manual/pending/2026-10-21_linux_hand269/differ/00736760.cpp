// Grade: DIFFER(427v427) -- register allocation only: retail shrink-wraps the ebx push into the particle branch and swaps the first loop's index/element registers; same instructions otherwise
// CTCCombat::UpdateFlourishParticles() -- keep the melee-flourish particles in step: kill them all when no flourish is valid; otherwise wait for live ones and the cooldown, then start a new particle on a melee weapon held in the primary slot
// Retail 0x00736760, 432 bytes (plain `ret`). Callees: CTCCombat::IsMeleeFlourishValid @ 0x00733EE0,
// CIntelligentPointer<CThing>::GetPItem @ 0x00A01B50, CThing::Kill(bool) @ 0x004C9B80,
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const @ 0x004365B0 (inlined GetInterface: 0x46 carrying,
// 0x48 weapon), CTCCarrying::GetThingInPrimarySlot @ 0x006A1440, CTCWeapon::IsMeleeWeapon @ 0x005DAA00,
// CTCCombat::AttemptToStartParticle(CThing*) @ 0x00735FD0.
// Layout: engine/CTCCombat.h is the PDB view; retail CurFlourishTimer is +0x84 and FlourishParticles (a 12-byte
// STLport vector of 8-byte CIntelligentPointer<CThing>) +0x88, 0x10 below the PDB (+0x94/+0x98). CThing Dead
// is bit 0 of +0x91, the interface bitset +0x20 (0x46 = +0x28 bit 6, 0x48 = +0x28 bit 8), the map +0x44.
struct CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                         // +0x91 bit 0
    void Kill(bool b);                              // 0x004C9B80
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};

template<class T> struct CIntelligentPointer {
    void* m_pHandle;
    void* m_pExtra;
    T* GetPItem() const;                            // 0x00A01B50
};

__forceinline bool IsThingAlive(const CIntelligentPointer<CThing>& p)
{
    return p.GetPItem() && !p.GetPItem()->Dead;
}

struct CTCCarrying {
    CThing* GetThingInPrimarySlot();                // 0x006A1440
};
struct CTCWeapon {
    bool IsMeleeWeapon();                           // 0x005DAA00
};

template<class T> struct CVector {                  // retail STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    int size() const { return int(_M_finish - _M_start); }
    T& operator[](int i) { return _M_start[i]; }
    T* begin() { return _M_start; }
};

struct CTCCombat {
    void*         __vftable;                        // +0x00
    CThing*       Thing;                            // +0x04 CTCBase parent thing
    unsigned char _pad08[0x84 - 0x08];
    long          CurFlourishTimer;                 // +0x84
    CVector<CIntelligentPointer<CThing> > FlourishParticles;   // +0x88
    bool IsMeleeFlourishValid();                    // 0x00733EE0
    void AttemptToStartParticle(CThing* weapon);    // 0x00735FD0
    void UpdateFlourishParticles();
    __forceinline void KillFlourishParticles()
    {
        CurFlourishTimer = 0;
        for (int i = 0; i < FlourishParticles.size(); ++i) {
            if (IsThingAlive(*(FlourishParticles.begin() + i)))
                (*(FlourishParticles.begin() + i)).GetPItem()->Kill(true);
        }
    }
};

void CTCCombat::UpdateFlourishParticles()
{
    if (IsMeleeFlourishValid()) {
        for (int i = 0; i < FlourishParticles.size(); ++i) {
            if (IsThingAlive(FlourishParticles[i]))
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
        if (!weapon || weapon->Dead)
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
