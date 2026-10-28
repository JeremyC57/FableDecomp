// CTCInGameMenu::CloseInGameMenu() -- restores the sound volumes, books the paused time, tears down the menu UI, plays the close sound and, when the inventory was asked for, queues the open-inventory game event
// Leaf: CloseInGameMenu
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047D78B, 600 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`, as the landed CloseInGameMenuPC 0x0047DBB8) with /GS (the game
// event's data array gets the cookie 0x0139C8A8, __security_check_cookie @ 0x00BFE9F9).
// Callees: CTBaseSingleton<CUserProfileManager>::Get() @ 0x0040D2A0 with the same-TU inline GetSoundVolume 0x0047CFC2 /
//          GetDialogueVolume 0x0047CFD3, the game state (0x013B8394) slot 46 (+0xB8, category volume), the 3D engine's
//          slot 76 (+0x130) through g_pMainGameComponent (0x013B86A0) ->PDisplayEngine (+0x28) ->Engine3D (+0x2C) and slot
//          56 (+0xE0), CTCBase::GetWorld() @ 0x00686A70, GFGetCurrentFrame() @ 0x0049D870, GFGetTime() @ 0x009E1BC0,
//          the components' deleting destructors (slot 0), vector<CComponent*>::erase @ 0x00485AA7,
//          NUISystem::CManager::GetInstance() @ 0x0041E5F2 (slots 6, 29, 37, 40 and 42), the UI list clear @ 0x004855A3 on
//          0x013B8A98, CPlayerGui::Close() @ 0x004374B0 (g_pPlayerGui 0x013B8790), CTCBase::GetPlayerManager() @ 0x00686D40,
//          CPlayerManager::GetMainPlayer() @ 0x00449970, CPlayer::GetPControlledCreature() @ 0x00487DC0,
//          IS_THING_ALIVE<CThing*> @ 0x0043DBD0, CMemoryBuffer::Copy @ 0x009F1760, CMainGameComponent::AddGameEvent
//          @ 0x0041649C, the static menu reset @ 0x0041D165 (descriptive name).
// Shape: the volume getters are the same-TU inline profile accessors (called after Get(), before the game state load);
// the world pointer is a named local taken before GFGetCurrentFrame(); the hero is a const reference to the call result
// (keeps the dead `Player = -1` store, as in the landed CloseInGameMenuViaGameEvent); the event payload is AddLong(4).
// Layout: retail CTCInGameMenu (12-byte containers; engine/CTCInGameMenu.h carries PDB offsets): InGameMenuOpen +0x0C,
// Components +0x58, CurStartPauseTime +0x68, AccumulativePausedTime +0x70, OpenInventory +0xA8 (PDB +0xC8),
// PVideoOptionsNotAcceptedDialog +0xB0, PVideoOptionsChangedDialog +0xB4; CWorld::FrameStarted3DRendering +0x8C.
#pragma optimize("y", off)
#pragma optimize("s", on)

double __fastcall GFGetTime();                       // 0x009E1BC0
long __fastcall GFGetCurrentFrame();                 // 0x0049D870

class CUserProfileManager { public:
    unsigned char _pad00[0xC];
    bool  UseNewProfileValues;                       // +0x0C
    unsigned char _pad0d[0x24 - 0xD];
    float SoundVolume;                               // +0x24
    unsigned char _pad28[0x4];
    float DialogueVolume;                            // +0x2C
    unsigned char _pad30[0xE4 - 0x30];
    float SoundVolumeNewProfile;                     // +0xE4
    unsigned char _pade8[0x4];
    float DialogueVolumeNewProfile;                  // +0xEC
    static CUserProfileManager* __fastcall Get();    // 0x0040D2A0 CTBaseSingleton<CUserProfileManager>::Get
    float GetSoundVolume() const { return UseNewProfileValues ? SoundVolumeNewProfile : SoundVolume; }        // 0x0047CFC2
    float GetDialogueVolume() const { return UseNewProfileValues ? DialogueVolumeNewProfile : DialogueVolume; }  // 0x0047CFD3
};

#define V10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
    virtual void v##n##8(); virtual void v##n##9();
struct CGameState {
    V10(0) V10(1) V10(2) V10(3)
    virtual void v400(); virtual void v401(); virtual void v402(); virtual void v403(); virtual void v404(); virtual void v405();
    virtual void SetCategoryVolume(long category, float volume, float fadeTime);   // slot 46 (+0xB8)
};
extern CGameState* g_pGameState;                     // 0x013B8394

class CIEngine { public:
    V10(0) V10(1) V10(2) V10(3) V10(4)
    virtual void v500(); virtual void v501(); virtual void v502(); virtual void v503(); virtual void v504(); virtual void v505();
    virtual void SetCameraMoved(bool fastMode, bool moved);   // slot 56 (+0xE0)
    virtual void v570(); virtual void v571(); virtual void v572(); virtual void v573(); virtual void v574(); virtual void v575();
    virtual void v576(); virtual void v577(); virtual void v578(); virtual void v579();
    virtual void v670(); virtual void v671(); virtual void v672(); virtual void v673(); virtual void v674(); virtual void v675();
    virtual void v676(); virtual void v677(); virtual void v678();
    virtual void SetEngineFlag76(bool on);           // slot 76 (+0x130)
};
#undef V10

struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);   // 0x009F1760
};
class CGameEvent : public CMemoryBuffer { public:
    long          Type;                              // +0x00
    signed char   Player;                            // +0x04
    unsigned char Data[32];                          // +0x05
    unsigned char EndPos;                            // +0x25
    bool          Valid;                             // +0x26
    bool          Replacement;                       // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddLong(long value) { Copy(sizeof(value), &value); }
};
class CDisplayEngine { public:
    unsigned char _pad00[0x2C];
    CIEngine*     Engine3D;                          // +0x2C
};
class CMainGameComponent { public:
    unsigned char   _pad00[0x28];
    CDisplayEngine* PDisplayEngine;                  // +0x28 scoped_ptr
    void AddGameEvent(const CGameEvent& event);      // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;     // 0x013B86A0

class CThing { public:
};
template <class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing)   // 0x0043DBD0 (T = CThingPlayerCreature*: fed by GetPControlledCreature 0x00487DC0 at 25 of 32 retail call sites)
{
    return thing != 0 && !(reinterpret_cast<const unsigned char*>(thing)[0x91] & 1);   // CThing dead bit +0x91
}
class CThingCreatureBase : public CThing { public:
    unsigned char _pad_0x00[0x90];
    signed char   PlayerNumber;                      // +0x90
};
class CThingPlayerCreature : public CThingCreatureBase { };
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;         // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer* GetMainPlayer();                        // 0x00449970
};
class CWorld { public:
    unsigned char _pad00[0x8C];
    long          FrameStarted3DRendering;           // +0x8C
};

struct CRefCounted {
    long m_count;
    __forceinline void DecRef() { --m_count; }
    __forceinline long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                          // slot 1
};
struct CDefBase : CDefVirtual, CRefCounted {
    __forceinline void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
struct CUISound;
struct CUIIconsDef : CDefBase {
    unsigned char _pad_0x08[0x1A0];
    CUISound*     CloseMenuSound_1a8;                // +0x1A8 (not identified)
};
struct CIconsDefPointer {                            // CDefPointer<const NUISystem::CUIIconsDef>
    CUIIconsDef* Object;
    CIconsDefPointer() {}
    __forceinline ~CIconsDefPointer() { if (Object) Object->ReleaseTemporary(); }
    CUIIconsDef* operator->() const { return Object; }
};
namespace NUISystem {
class CComponent { public:
    virtual ~CComponent();                           // slot 0 (scalar deleting destructor)
};
class CManager { public:
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04(); virtual void m05();
    virtual void ResetA();                           // slot 6 (+0x18)
    virtual void m07(); virtual void m08(); virtual void m09(); virtual void m10(); virtual void m11(); virtual void m12();
    virtual void m13(); virtual void m14(); virtual void m15(); virtual void m16(); virtual void m17(); virtual void m18();
    virtual void m19(); virtual void m20(); virtual void m21(); virtual void m22(); virtual void m23(); virtual void m24();
    virtual void m25(); virtual void m26(); virtual void m27(); virtual void m28();
    virtual void ResetB();                           // slot 29 (+0x74)
    virtual void m30(); virtual void m31(); virtual void m32(); virtual void m33(); virtual void m34(); virtual void m35();
    virtual void m36();
    virtual void ResetC();                           // slot 37 (+0x94)
    virtual void m38(); virtual void m39();
    virtual void PlayUISound_0041d0a8(CUISound* const& sound, bool flag);   // slot 40 (+0xA0)
    virtual void m41();
    virtual CIconsDefPointer GetIconsDef();          // slot 42 (+0xA8)
    static CManager* __fastcall GetInstance();       // 0x0041E5F2
    static void __fastcall ResetMenus();             // 0x0041D165 (descriptive)
};
}
struct CGlobalList { void clear(); };                // 0x004855A3
extern CGlobalList g_UIList;                         // 0x013B8A98
class CPlayerGui { public: void Close(); };                 // 0x004374B0
extern CPlayerGui* g_pPlayerGui;                     // 0x013B8790

struct CComponentVector {                            // retail 12-byte STLport vector<NUISystem::CComponent*>
    NUISystem::CComponent** _M_start;
    NUISystem::CComponent** _M_finish;
    NUISystem::CComponent** _M_end_of_storage;
    NUISystem::CComponent** begin() { return _M_start; }
    NUISystem::CComponent** end() { return _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
    NUISystem::CComponent*& operator[](unsigned long n) { return *(begin() + n); }
    NUISystem::CComponent** erase(NUISystem::CComponent** first, NUISystem::CComponent** last);   // 0x00485AA7
    void clear() { erase(begin(), end()); }
};
struct CYesNo;
enum { GAME_EVENT_OPEN_INVENTORY = 0x1A };
class CTCBase
{
protected:
    CPlayerManager& GetPlayerManager();   // 0x00686D40
    CWorld& GetWorld() const;   // 0x00686A70
};

class CTCInGameMenu : public CTCBase { public:
    void*            __vftable;                      // +0x00
    void*            ParentThing;                    // +0x04 CTCBase
    unsigned char    _pad_0x08[0x04];
    bool             InGameMenuOpen;                 // +0x0C
    unsigned char    _pad_0x0d[0x4B];
    CComponentVector Components;                     // +0x58 (PDB +0x68)
    long             LoadGameIndex;                  // +0x64
    double           CurStartPauseTime;              // +0x68
    double           AccumulativePausedTime;         // +0x70
    unsigned char    _pad_0x78[0x30];
    bool             OpenInventory;                  // +0xA8 (PDB +0xC8)
    unsigned char    _pad_0xa9[0x07];
    CYesNo*          PVideoOptionsNotAcceptedDialog; // +0xB0
    CYesNo*          PVideoOptionsChangedDialog;     // +0xB4
    void CloseInGameMenu();
};

void CTCInGameMenu::CloseInGameMenu()
{
    if (g_pGameState) {
        g_pGameState->SetCategoryVolume(0x10, CUserProfileManager::Get()->GetSoundVolume(), 0.0f);
        g_pGameState->SetCategoryVolume(0x40, CUserProfileManager::Get()->GetDialogueVolume(), 0.0f);
        g_pGameState->SetCategoryVolume(0x80, CUserProfileManager::Get()->GetSoundVolume(), 0.0f);
    }
    g_pMainGameComponent->PDisplayEngine->Engine3D->SetEngineFlag76(true);
    CWorld* world = (&GetWorld());
    world->FrameStarted3DRendering = GFGetCurrentFrame();
    AccumulativePausedTime += GFGetTime() - CurStartPauseTime;
    for (unsigned long i = 0; i < Components.size(); ++i)
        delete Components[i];
    Components.clear();
    PVideoOptionsNotAcceptedDialog = 0;
    PVideoOptionsChangedDialog = 0;
    NUISystem::CManager::GetInstance()->ResetA();
    NUISystem::CManager::GetInstance()->ResetB();
    g_UIList.clear();
    NUISystem::CManager::GetInstance()->ResetC();
    g_pPlayerGui->Close();
    InGameMenuOpen = false;
    NUISystem::CManager* owner = NUISystem::CManager::GetInstance();
    NUISystem::CManager::GetInstance()->PlayUISound_0041d0a8(owner->GetIconsDef()->CloseMenuSound_1a8, true);
    g_pMainGameComponent->PDisplayEngine->Engine3D->SetCameraMoved(false, false);
    if (OpenInventory) {
        CThingPlayerCreature* const& hero = GetPlayerManager().GetMainPlayer()->GetPControlledCreature();
        if (IS_THING_ALIVE(hero)) {
            CGameEvent event(GAME_EVENT_OPEN_INVENTORY);
            event.Player = hero->PlayerNumber;
            event.AddLong(4);
            g_pMainGameComponent->AddGameEvent(event);
        }
        OpenInventory = false;
    }
    NUISystem::CManager::ResetMenus();
}