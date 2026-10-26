#pragma optimize("s", on)
// CTCVillage::OnKill() -- restart game time if the village had stopped it, then notify the live hero's interface-243 component
// Leaf: ?OnKill@CTCVillage
// Retail: Fable.exe @ 0x0065B0B4, 142 bytes; size TU (unaligned entry): `optimize("s",on)`.
// Callees: CTCBase::GetWorld() 0x00686A70; CGameTimeManager::SetTimeAsStopped(bool, long*) 0x006BBE30 (the world's +0x1c);
//          CTCBase::GetPlayerManager() 0x00686D40; CPlayerManager::GetCurrentPlayer() 0x00449970; CPlayer::GetCharacterThing()
//          0x00487DC0; IS_THING_ALIVE<CThing*>(CThing* const&) 0x0043DBD0 (__fastcall); std::bitset<>::test(size_t) const
//          0x00410DE0 (interface 243); CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inlined GetTC); the
//          component's method 0x007E2780 (no arguments; manifest "CXMVPlaybackInfo" ctor label is a propagation, real name
//          unknown).
// Layout: retail CTCVillage = PDB minus 0x8c from Initialised: TimeIsPaused +0x90 (PDB +0x11c), TimeStopIndex +0x9c (PDB
//         +0x128).
// Shape: the world is a named local (GetWorld precedes the argument set-up); the flag is reached through a reference local (`lea eax,[esi+90h]` once); the hero (CThingCreatureBase*) binds
//        IS_THING_ALIVE's const reference through a converted temporary at [ebp-4], later the GetTC key (edi = 243).
class CTCBase;
class CThing;

struct CBitset {
    bool test(unsigned long position) const;    // 0x00410DE0
};
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    CBitset       m_present;                    // +0x20
    unsigned char _pad24[0x20];
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long type) const { return m_present.test(type); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
class CThingCreatureBase : public CThing {};
bool __fastcall IS_THING_ALIVE_0043dbd0(CThing* const& thing);   // 0x0043DBD0

struct CTCInterface243 {                        // the hero's interface-243 component (class name unknown)
    void OnVillageKilled();                     // 0x007E2780
};

class CPlayer { public:
    CThingCreatureBase* GetCharacterThing();    // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer* GetCurrentPlayer();                // 0x00449970
};
class CGameTimeManager { public:
    void SetTimeAsStopped(bool stopped, long* stopIndex);   // 0x006BBE30
};
class CWorld { public:
    unsigned char     _pad[0x1c];
    CGameTimeManager* TimeManager;              // +0x1c
};

class CTCVillage { public:
    virtual void v00();
    unsigned char _pad_0x04[0x8c];
    bool          TimeIsPaused;                 // +0x90
    unsigned char _pad_0x91[0xb];
    long          TimeStopIndex;                // +0x9c
    CWorld* GetWorld();                         // 0x00686A70 (CTCBase)
    CPlayerManager* GetPlayerManager();         // 0x00686D40 (CTCBase)
    void OnKill();
};

void CTCVillage::OnKill()
{
    bool& paused = TimeIsPaused;
    if (paused) {
        paused = false;
        CWorld* world = GetWorld();
        world->TimeManager->SetTimeAsStopped(false, &TimeStopIndex);
    }
    CThingCreatureBase* hero = GetPlayerManager()->GetCurrentPlayer()->GetCharacterThing();
    if (IS_THING_ALIVE_0043dbd0(hero) && hero->HasTC(243))
        ((CTCInterface243*)hero->GetTC(243))->OnVillageKilled();
}