// CTCTavernGame::GetHeroStats(CTCHeroStats** out) -- hands out the living hero creature's hero-stats component (interface 4), leaving *out untouched otherwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EB4B0, length 92 bytes (ret 4). Name and signature: Ego_r.pdb
// `?GetHeroStats@CTCTavernGame@@QAEXPAPAVCTCHeroStats@@@Z`.
// Callees: CPlayerManager::GetMainPlayer() @ 0x00449960 and CPlayer::GetPlayerCreature() @ 0x00487DC0 (landed naming,
// 0x007B1EB0), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (another out-of-line copy of the interface
// map search). The player manager is the member at +0x1c of the global world object @ 0x013B86A0.
// Layout: the hero's dead bit is bit 0 of +0x91 (landed 0x00836BE0 / 0x007B1EB0); the interface-present bitset is at +0x20
// (interface 4 = `test byte ptr [eax+20h],10h`), the interface map at +0x44.
// Shape: the inline out-parameter GetInterface(T** ppOut, long type) template of the landed 0x007504E0 / 0x007B2CF0 family
// writes straight through the caller's pointer; the key 4 lives in the `push ecx` frame slot. Here the template must be
// `__forceinline`: with its bool result unused VC7.1 keeps it out of line (50v92).
struct CTCBase;
struct CTCHeroStats;

struct CInterfaceMap {                       // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);    // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];          // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;              // +0x44
    unsigned char _pad_0x4c[0x45];
    unsigned char m_flags;                   // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags & 1) != 0; }
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

struct CPlayer {
    CThing* GetPlayerCreature();             // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                // 0x00449960
};
struct CWorld {
    unsigned char   _pad_0x00[0x1c];
    CPlayerManager* PPlayerManager;          // +0x1c
};
extern CWorld* g_pWorld;                     // 0x013B86A0

struct CTCTavernGame {
    void GetHeroStats(CTCHeroStats** out);
};

void CTCTavernGame::GetHeroStats(CTCHeroStats** out)
{
    CThing* hero = g_pWorld->PPlayerManager->GetMainPlayer()->GetPlayerCreature();
    if (hero && !hero->IsDead())
        hero->GetInterface(out, 4);
}