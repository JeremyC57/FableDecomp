// CTCInventoryMap::OpenInventory() -- opens the world-map screen: remembers the previous hero-info screen, opens the base inventory, fades the sound channels, builds the map menu and the mouse pointer and keeps both components
// Leaf: OpenInventory@CTCInventoryMap
// Retail: Fable.exe (TLC, VC7.1) @ 0x006116E0, 535 bytes (+ int3 padding), speed TU (no frame pointer, `sub esp,10h`).
// Callees: NHeroInformationScreens::CBase slots 2 (+0x08) / 3 (+0x0C) through the +0x130 secondary base,
//   CTCInventoryBase 0x005BC6ED (manifest "_Dest_val<...>" is propagated; named descriptively) and OpenInventory 0x005BD4EA,
//   CFrontEndManager::GetInstance 0x0041E5F2 + manager slots 16 (+0x40) / 26 (+0x68) + CreateComponent 0x0041DB1D,
//   CPlayerGui::GetPHeroInfoScreensManager 0x00437CD0 (g_pPlayerGui 0x013B8790), CManager::GetCurrentScreen 0x0066E7BD /
//   SetCurrentScreenType 0x0066E799, the sound system's slot 46 (+0xB8) (g 0x013B8394), OnOpenMenus 0x0041D122, the input
//   controller's slot 76 (+0x130), FindTeleports 0x005FC620, the map-menu builder 0x0060EEB0 (manifest
//   "CTCInventoryMap_BuildMapMenu"), component slots 43 (+0xAC) / 48 (+0xC0), this's slots 32 (+0x80) / 46 (+0xB8),
//   vector _M_insert_overflow 0x00485DBC (same-TU stand-in body, as the landed CTCCreditsUI::Activate 0x007E4B60).
// Shape: the player-GUI global is read into a named local before the screen-type call; the failed-build arm (CloseInventory)
//   is the else of `if (screen)` so VC7.1 places it after the push_back tail as retail does.
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
namespace NUISystem {
class CComponent { public:
#define V(n) virtual void v##n();
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
    V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30) V(31) V(32) V(33) V(34) V(35) V(36) V(37) V(38) V(39)
    V(40) V(41) V(42)
    virtual void Reset();                               // slot 43 (+0xac)
    V(44) V(45) V(46) V(47)
#undef V
    virtual void ChangeState(long state);               // slot 48 (+0xc0)
};
class CManager { public:
#define V(n) virtual void v##n();
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15)
    virtual void AddActiveTC(void* tc);                 // slot 16 (+0x40)
    V(17) V(18) V(19) V(20) V(21) V(22) V(23) V(24) V(25)
    virtual void SetActiveThing(void* thing);           // slot 26 (+0x68)
#undef V
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
};
class CFrontEndManager : public CManager { public:
    static CFrontEndManager* __fastcall GetInstance();  // 0x0041E5F2
};
}
struct __true_type { __true_type() { } };
template <class T> struct CRetailVector {               // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fill_len, bool atend);   // 0x00485DBC
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                *_M_finish = x;
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
#pragma auto_inline(off)
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
template <class T>
void CRetailVector<T>::_M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fill_len, bool atend)
{
    unsigned long oldSize = _M_finish - _M_start;
    unsigned long len = oldSize + (oldSize < fill_len ? fill_len : oldSize);
    T* newStart = len ? (T*)malloc(len * sizeof(T)) : 0;
    T* newFinish = newStart;
    for (T* p = _M_start; p != position; ++p, ++newFinish)
        *newFinish = *p;
    for (unsigned long i = 0; i < fill_len; ++i, ++newFinish)
        *newFinish = x;
    if (!atend)
        for (T* p = position; p != _M_finish; ++p, ++newFinish)
            *newFinish = *p;
    if (_M_start)
        free(_M_start);
    _M_start = newStart;
    _M_finish = newFinish;
    _M_end_of_storage = newStart + len;
}
template struct CRetailVector<NUISystem::CComponent*>;
#pragma auto_inline(on)
namespace NHeroInformationScreens {
class CBase { public:
    virtual void v00(); virtual void v01();
    virtual long GetScreenType();                       // slot 2 (+0x08)
    virtual bool IsActive();                            // slot 3 (+0x0C)
};
class CManager { public:
    CBase* GetCurrentScreen();                          // 0x0066E7BD
    void SetCurrentScreenType(long type);               // 0x0066E799
};
}
class CPlayerGui { public:
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() const;   // 0x00437CD0
};
extern CPlayerGui* g_pPlayerGui;                        // 0x013B8790
struct CSoundSystem {
#define V(n) virtual void v##n();
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
    V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30) V(31) V(32) V(33) V(34) V(35) V(36) V(37) V(38) V(39)
    V(40) V(41) V(42) V(43) V(44) V(45)
#undef V
    virtual void FadeChannel(long channel, long target, float time);   // slot 46 (+0xb8)
};
extern CSoundSystem* g_pSoundSystem;                    // 0x013B8394
void __fastcall OnOpenMenus();                          // 0x0041D122
struct CInputController {
#define V(n) virtual void v##n();
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
    V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30) V(31) V(32) V(33) V(34) V(35) V(36) V(37) V(38) V(39)
    V(40) V(41) V(42) V(43) V(44) V(45) V(46) V(47) V(48) V(49) V(50) V(51) V(52) V(53) V(54) V(55) V(56) V(57) V(58) V(59)
    V(60) V(61) V(62) V(63) V(64) V(65) V(66) V(67) V(68) V(69) V(70) V(71) V(72) V(73) V(74) V(75)
#undef V
    virtual void SetEnabled(bool enabled);              // slot 76 (+0x130)
};
class CDisplayEngine { public: unsigned char _pad[0x2c]; CInputController* Input; };   // +0x2c
class CMainGameComponent { public: unsigned char _pad[0x28]; CDisplayEngine* Display; };   // +0x28
extern CMainGameComponent* g_pMainGameComponent;      // 0x013B86A0
struct CTCInventoryBase {
#define V(n) virtual void v##n();
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
    V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30) V(31)
    virtual void CloseInventory();                      // slot 32 (+0x80)
    V(33) V(34) V(35) V(36) V(37) V(38) V(39) V(40) V(41) V(42) V(43) V(44) V(45)
    virtual void RefreshScreen(long mode);              // slot 46 (+0xb8)
#undef V
    void* ParentThing;                                  // +0x04
    unsigned char _pad08[0x0c - 0x08];
    CRetailVector<NUISystem::CComponent*> Components;   // +0x0c
    unsigned char _pad18[0x130 - 0x18];
    void ResetOpenState();                              // 0x005BC6ED
    void OpenInventory();                               // 0x005BD4EA
};
// Layout: retail CTCInventoryMap (landed OnCreate 0x00602E30 / DrawPreparePrimitivesForRendering 0x005FAF50): the
//   NHeroInformationScreens::CBase secondary base +0x130, AccumFadeTime +0x140, the teleports-found flag +0x234,
//   PreviousScreenType +0x244.
class CTCInventoryMap : public CTCInventoryBase, public NHeroInformationScreens::CBase { public:
    unsigned char _pad134[0x140 - 0x134];
    float         AccumFadeTime;                        // +0x140
    unsigned char _pad144[0x234 - 0x144];
    bool          TeleportsFound;                       // +0x234
    unsigned char _pad235[0x244 - 0x235];
    long          PreviousScreenType;                   // +0x244
    void FindTeleports();                               // 0x005FC620
    NUISystem::CComponent* BuildMapMenu(long a, long b);   // 0x0060EEB0
    void OpenInventory();
};
void CTCInventoryMap::OpenInventory()
{
    PreviousScreenType = -1;
    if (!IsActive()) {
        ResetOpenState();
        AccumFadeTime = 0.0f;
        NUISystem::CFrontEndManager::GetInstance()->SetActiveThing(ParentThing);
        PreviousScreenType = g_pPlayerGui->GetPHeroInfoScreensManager()->GetCurrentScreen()->GetScreenType();
        RefreshScreen(0);
        TeleportsFound = true;
        FindTeleports();
    }
    if (!IsActive()) {
        CPlayerGui* gui = g_pPlayerGui;
        gui->GetPHeroInfoScreensManager()->SetCurrentScreenType(GetScreenType());
    }
    CTCInventoryBase::OpenInventory();
    NUISystem::CFrontEndManager::GetInstance()->AddActiveTC(this);
    if (IsActive())
        return;
    if (g_pSoundSystem) {
        g_pSoundSystem->FadeChannel(0x10, 0, 500.0f);
        g_pSoundSystem->FadeChannel(0x40, 0, 500.0f);
        g_pSoundSystem->FadeChannel(0x80, 0, 500.0f);
    }
    OnOpenMenus();
    g_pMainGameComponent->Display->Input->SetEnabled(false);
    NUISystem::CComponent* screen = BuildMapMenu(0, 0);
    if (screen) {
        NUISystem::CComponent* pointer = NUISystem::CFrontEndManager::GetInstance()->CreateComponent("UI_MOUSE_POINTER", false);
        pointer->Reset();
        Components.push_back(pointer);
        screen->Reset();
        screen->ChangeState(5);
        Components.push_back(screen);
    } else {
        CloseInventory();
    }
}