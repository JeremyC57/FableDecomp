// CTCBoss::CTCBoss(CThing* owner) -- CTCBase(owner), own vtable, empty def pointer and vectors, counters cleared and both flags set, the boss sub-def fetched from the owner's base def, the phase start at the owner's position, then any effects cleared
// Leaf: ??0CTCBoss
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007AEFD0, length 114 bytes (+14 int3 padding). `ret 4`. Vtable 0x0126D494.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CDefClassBase::GetSubDef(CDefPointer<const CBossDef>*) @ 0x007AFA90 (catalog:
// the <CAICreatureWillPowerIndicatorDef> byte-twin), CThing::GetPosition @ 0x004C73D0, CTCBoss::ClearEffects @ 0x007AEF60.
// Shape: the member ctors (def pointer, two vectors modelled as STLport _Vector_base + _STLP_alloc_proxy, which places the
// `lea ecx,[esi+0Ch]` before the last zero store) zero through one eax after the vtable store; the scalars are body
// assignments (currentBossPhase, StrikesSinceHit, BCountStrikes, GenericCounter, EnablePhaseChanges; `true` through dl),
// interleaved by the scheduler with the GetSubDef argument set-up; PhaseStartPos is a three-dword copy through edx.
// Layout: PDB CTCBoss (0x50) with retail 12-byte vectors (0x48): PBossDef +0xC, GenericCounter +0x10, StrikesSinceHit +0x14,
// currentBossPhase +0x18, NextWeapon +0x1C, BCountStrikes +0x20, EnablePhaseChanges +0x21, PhaseStartPos +0x22 (unaligned, so
// C3DVector is declared packed), HelperCreatures +0x30, Effects +0x3C; retail CThing: bitset +0x20, map +0x44, base def +0x70,
// Dead +0x91.
#pragma pack(push, 1)
struct C3DVector {                              // packed: CTCBoss keeps one at the unaligned +0x22
    float X, Y, Z;
};
#pragma pack(pop)
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020 (the FrameUpdate copy is 0x004365B0)
};
struct CDefClassBase;
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
    const C3DVector& GetPosition() const;       // 0x004C73D0
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
    virtual void v00();
    void* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
inline bool IsThingAlive(const CBaseIntelligentPointer& p) { return p.GetPItem() && !p.GetPItem()->IsDead(); }
template <class T> struct SRetailAllocProxy {   // STLport _STLP_alloc_proxy: the end-of-storage pointer
    T* _M_data;
    SRetailAllocProxy(T* data) : _M_data(data) {}
};
template <class T> struct CRetailVector {       // STLport vector (12 bytes): _Vector_base start / finish + the alloc proxy
    T* _M_start;
    T* _M_finish;
    SRetailAllocProxy<T> _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    T* begin() const { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) const { return *(begin() + n); }
};
struct CBossDef;
struct CPlayer {
    CThing* GetHeroThing();                     // 0x00487DC0 (CPlayer::GetHeroThing)
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                   // 0x00449970
};
template <class T> struct CDefPointer {
    T* Data;
    CDefPointer() : Data(0) {}
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<const CBossDef>* out) const;    // 0x007AFA90 (catalog: a byte-twin instantiation)
};

struct CTCEntityEvents;
class CTCBase {
public:
    CTCBase(CThing* owner);                     // 0x00686800
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    CPlayerManager* GetPlayerManager();         // 0x00686D40 (catalog "CViewBase::GetDisplayEngine")
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

CTCBoss::CTCBoss(CThing* owner)
    : CTCBase(owner)
{
    currentBossPhase = 0;
    StrikesSinceHit = 0;
    BCountStrikes = true;
    GenericCounter = 0;
    EnablePhaseChanges = true;
    ParentThing->PBaseDef->GetSubDef(&PBossDef);
    PhaseStartPos = ParentThing->GetPosition();
    ClearEffects();
}
