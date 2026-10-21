// CGameScriptInterface::GetDeathRecoveryMarkerName() const -- the hero's death-recovery marker name (hero-stats component), or "" without a live hero holding it
// Leaf: ?GetDeathRecoveryMarkerName@CGameScriptInterface@@UBE?AVCCharString@@XZ
// Retail: Fable.exe @ 0x00896EE0, 125 bytes, `ret 4` (hidden result); Ego_r.pdb same symbol.
// Body: hero = PlayerManager (+0x14)->GetMainPlayer() (0x00449970)->GetPControlledCreature() (0x00487DD0: `add ecx,2Ch;
// jmp` into CPlayer::ControlledCreature +0x2c, Ego_r.pdb CIntelligentPointer<CThingPlayerCreature> at 44); if hero, not dead
// and holding interface 4 (bit 4 of UsingComponents +0x20): PeekTC (const LowerBound 0x004365B0) and return a copy
// (0x0099EC30) of CTCHeroStats::DeathRecoveryMarkerName (+0x170); else return CCharString("", -1) (0x0099EBF0, literal
// 0x0122D70E).
// Layout: Ego_r.pdb CTCHeroStats DeathRecoveryMarkerName +0x188; retail has it at +0x170 (-0x18, the only struct with that
// member name). Interface 4 = CTCHeroStats rests on that member; the CCharString(const char*, long) length argument
// meaning (-1 = strlen) is as the landed C2DVector::ToString row.
// Shape: /O2 (16-aligned entry); the PeekTC out-pointer form.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* s, long length);             // 0x0099EBF0
    CCharString(const CCharString& other);               // 0x0099EC30
};
struct CTCBase;
class CInterfaceMap {                                    // CThing::Components, CVectorMap<ETCInterfaceType, CTCBase*> (retail 8 bytes)
public:
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry*       LowerBound(const long* type);          // 0x0040F020
    const SEntry* LowerBound(const long* type) const;    // 0x004365B0
};
class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char Type;                                  // +0x10 (1 = a creature)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long UsingComponents[9];                    // +0x20 bitset<274>
    CInterfaceMap Components;                            // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                              // +0x91 bit 0
    bool IsDead() const { return Dead; }
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetTC(T** out)               // ??$GetTC@V...@@@CThing@@QAE_NPAPAV...@@@Z
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        long type = T::INTERFACE_TYPE;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || T::INTERFACE_TYPE < it->type)
            it = map->last;
        *out = (T*)it->component;
        return true;
    }
    template <class T> __forceinline bool PeekTC(const T** out) const  // ??$PeekTC@$$CBV...@@@CThing@@QBE_NPAPBV...@@@Z
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        long type = T::INTERFACE_TYPE;
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || T::INTERFACE_TYPE < it->type)
            it = map->last;
        *out = (const T*)it->component;
        return true;
    }
};
class CTCHeroStats {                                    // interface 4
public:
    enum { INTERFACE_TYPE = 4 };
    unsigned char _pad000[0x170];
    CCharString   DeathRecoveryMarkerName;               // +0x170 (Ego_r.pdb +0x188)
};
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DD0
};
class CPlayerManager {
public:
    CPlayer& GetMainPlayer();                            // 0x00449970
};
class CGameScriptInterface {
public:
    virtual CCharString GetDeathRecoveryMarkerName() const;
    void*           World;                               // +0x04
    void*           Component;                           // +0x08
    void*           DisplayEngine;                       // +0x0c
    void*           DefinitionManager;                   // +0x10
    CPlayerManager* PlayerManager;                       // +0x14
};
CCharString CGameScriptInterface::GetDeathRecoveryMarkerName() const
{
    const CThing* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (hero && !hero->IsDead())
    {
        const CTCHeroStats* stats;
        if (hero->PeekTC(&stats))
            return stats->DeathRecoveryMarkerName;
    }
    return CCharString("", -1);
}