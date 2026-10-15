// CTCBoss::ClearEffects() -- kill every live effect thing in the Effects vector
// Leaf: ClearEffects
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007AEF60, length 110 bytes (+2 int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3), CThing::Kill(bool) @ 0x004C9B80.
// Shape: `for (unsigned i = 0; i < Effects.size(); ++i) if (IsThingAlive(Effects[i])) Effects[i].GetPItem()->Kill(true);`
// with operator[] as `*(begin() + i)`: the element address (esi) feeds the inline alive test, the kill re-reads begin
// (`lea ecx,[eax+ebp]`, i*8 kept in ebp); size() recomputed at the foot.
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
template <class T> struct CRetailVector {       // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
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

void CTCBoss::ClearEffects()
{
    for (unsigned int i = 0; i < Effects.size(); ++i) {
        if (IsThingAlive(Effects[i]))
            Effects[i].GetPItem()->Kill(true);
    }
}
