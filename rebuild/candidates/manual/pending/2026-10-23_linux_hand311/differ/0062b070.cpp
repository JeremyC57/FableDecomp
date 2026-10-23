// Grade: DIFFER(898v891) -- block order now retail's; residue: retail keeps the first three interface keys in the dead event-argument slot (frame 0x34, ours 0x38 with a fresh key slot) and loads each flag byte into cl with the pointer in eax (ours al/edx)
// CWorld::EAOpenHeroInformationScreen(const CGameEvent* event) -- game-event action: open the requested hero information screen unless the player is busy (mode 0x2F, a pending screen, dead hero, screen fade, blocking interfaces); a pending tutorial for the inventory/quests screen defers the open
// Leaf: ?EAOpenHeroInformationScreen@CWorld@
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x0062B070, 889 bytes, `ret 4`, /GS (cookie 0x0139C8A8).
//   PDB ?EAOpenHeroInformationScreen@CWorld@@QAEXPBVCGameEvent@@@Z.
// Callees: CWorld::GetPlayer 0x0049D850 (x3), CPlayer::HasMode 0x00634160, CPlayer::GetHeroCreature 0x00487DC0,
//   CDisplayEngine::IsScreenFadeInactive 0x00434D60 (g_pMainGameComponent 0x013B86A0 +0x28), CPlayerGui::ToggleLiveGui
//   0x00438310 / GetPHeroInfoScreensManager 0x00437CD0 (g_pPlayerGui 0x013B8790), interface-map LowerBound const 0x004365B0
//   (0xF6, 0xE8, 0xFC, 0xC6, 0x11) and non-const 0x0040F020 (0x29), CTCInventoryBase::GetCanItemBeQuickAssigned-labelled
//   0x00625930 on interface 0xF6, CGameEvent::GetData 0x009F17A0, CMemoryBuffer::Read 0x009F18F0, CThing::
//   GetJoystickDeviceNumber 0x004C7CA0, g_pInputManager 0x013B8388 (+0x2c device table, slot 18 +0x48),
//   CTCHero::GiveTutorialForCategory 0x006C9FA0, CWorld::OpenHeroInformationScreen 0x0049F3F0.
// Layout: local models at retail offsets as the landed sibling EAUseQuickAccessItem 0x0062ABA0 (interface bitset +0x20,
//   map +0x44, Dead +0x91 bit 0); creature HitPoints +0xB4; CWorld PendingHeroInfoScreen +0xE2 / PendingHeroInfoScreenMode +0xE4.
struct CMemoryBuffer {
    unsigned char Data[0x20];                      // +0x00
    long          Length;                          // +0x20
    long          ReadPos;                         // +0x24
    long          WritePos;                        // +0x28
    CMemoryBuffer() : Length(0), ReadPos(0), WritePos(0) { }
    void Read(unsigned long length, void* data);   // 0x009F18F0
};
struct CGameEvent {
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    long GetData(CMemoryBuffer& buffer) const;     // 0x009F17A0
};
struct CTCBase;
struct CInterfaceMap {                             // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                 // 0x0040F020
    const SEntry* LowerBound(const long* type) const;     // 0x004365B0
};
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
__declspec(noinline) const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CTCInterfaceF6 { bool IsBusy(); };          // 0x00625930 (catalog: CTCInventoryBase::GetCanItemBeQuickAssigned)
struct CTCInterfaceE8 { unsigned char _pad[0x2a]; bool Busy; };   // +0x2A
struct CTCFlagC { unsigned char _pad[0xc]; bool Active; };      // interfaces 0xFC / 0xC6: +0xC
struct CTCInventoryState { unsigned char _pad[0x140]; long State; };   // interface 0x11: +0x140
enum ETutorialCategory { TUTORIAL_QUESTS = 0x16, TUTORIAL_INVENTORY = 0x26 };
struct CTCHero { bool GiveTutorialForCategory(ETutorialCategory category, bool force); };   // 0x006C9FA0
struct CThingCreatureBase {
    void* m_pVtable;
    unsigned char _pad04[0x20 - 4];
    unsigned long UsingComponents[9];              // +0x20 bitset<274>
    CInterfaceMap Components;                      // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                        // +0x91 bit 0
    unsigned char _pad92[0xb4 - 0x92];
    float HitPoints;                               // +0xB4
    long GetJoystickDeviceNumber() const;          // 0x004C7CA0
    __forceinline bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
    template<class T> __forceinline T* GetTCConst(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
};
enum EPlayerMode { PLAYER_MODE_0x2F = 0x2f };
struct CPlayer {
    bool HasMode(EPlayerMode mode);                // 0x00634160
    CThingCreatureBase* GetHeroCreature();         // 0x00487DC0
};
struct CDisplayEngine { bool IsScreenFadeInactive() const; };   // 0x00434D60
struct CMainGameComponent {
    unsigned char _pad0[0x28];
    CDisplayEngine* PDisplayEngine;                // +0x28
};
struct CHeroInfoScreensManager { unsigned char _pad[0x34]; bool Open; };   // +0x34
struct CPlayerGui {
    unsigned char _pad[0xfd];
    bool Busy;                                     // +0xFD
    unsigned char _padfe[0x2be - 0xfe];
    bool LiveGuiToggle;                            // +0x2BE
    CHeroInfoScreensManager* GetPHeroInfoScreensManager() const;   // 0x00437CD0
    void ToggleLiveGui();                          // 0x00438310
};
struct CInputDevice {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void Flush();                          // slot 18 (+0x48)
};
struct CInputDeviceEntry { CInputDevice* Device; void* Info; };
struct CInputManager {
    unsigned char _pad[0x2c];
    CInputDeviceEntry* Devices;                    // +0x2C
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
extern CPlayerGui* g_pPlayerGui;                   // 0x013B8790
extern CInputManager* g_pInputManager;             // 0x013B8388
struct CWorld {
    unsigned char _pad00[0xe2];
    bool PendingHeroInfoScreen;                    // +0xE2
    long PendingHeroInfoScreenMode;                // +0xE4
    CPlayer* GetPlayer(long number);               // 0x0049D850
    void OpenHeroInformationScreen(EPlayerMode mode);   // 0x0049F3F0
    void EAOpenHeroInformationScreen(const CGameEvent* event);
};

void CWorld::EAOpenHeroInformationScreen(const CGameEvent* event)
{
    if (GetPlayer(event->Player)->HasMode(PLAYER_MODE_0x2F))
        return;
    if (PendingHeroInfoScreen)
        return;
    CThingCreatureBase* hero = GetPlayer(event->Player)->GetHeroCreature();
    if (!hero || hero->Dead)
        return;
    if (!(hero->HitPoints > 0.0f))
        return;
    if (g_pMainGameComponent->PDisplayEngine->IsScreenFadeInactive()) {
        if (!hero->HasTC(0xF6))
            return;
        if (hero->GetTCConst<CTCInterfaceF6>(0xF6)->IsBusy())
            return;
        if (!hero->HasTC(0xE8) || hero->GetTCConst<CTCInterfaceE8>(0xE8)->Busy)
            return;
        if (!hero->HasTC(0xFC) || hero->GetTCConst<CTCFlagC>(0xFC)->Active)
            return;
        if (g_pPlayerGui->Busy)
            return;
        CMemoryBuffer buffer;
        event->GetData(buffer);
        GetPlayer(event->Player);
        long mode;
        buffer.Read(4, &mode);
        if (mode == 0x17 && g_pPlayerGui->GetPHeroInfoScreensManager()->Open)
            return;
        if (hero->HasTC(0xC6) && hero->GetTCConst<CTCFlagC>(0xC6)->Active)
            return;
        CTCInventoryState* inventory;
        if (mode == 0x17) {
            g_pInputManager->Devices[hero->GetJoystickDeviceNumber()].Device->Flush();
        } else if (hero->HasTC(0x11) && (inventory = hero->GetTCConst<CTCInventoryState>(0x11)) != 0 && inventory->State == 4) {
            if (hero->HasTC(0x29)) {
                CTCHero* tc = hero->GetTC<CTCHero>(0x29);
                if (tc && tc->GiveTutorialForCategory(TUTORIAL_INVENTORY, true)) {
                    PendingHeroInfoScreen = true;
                    PendingHeroInfoScreenMode = mode;
                    return;
                }
            }
        } else if (hero->HasTC(0x29)) {
            CTCHero* tc = hero->GetTC<CTCHero>(0x29);
            if (tc && tc->GiveTutorialForCategory(TUTORIAL_QUESTS, true)) {
                PendingHeroInfoScreen = true;
                PendingHeroInfoScreenMode = mode;
                return;
            }
        }
        OpenHeroInformationScreen((EPlayerMode)mode);
    } else if (g_pPlayerGui->LiveGuiToggle)
        g_pPlayerGui->ToggleLiveGui();
}
