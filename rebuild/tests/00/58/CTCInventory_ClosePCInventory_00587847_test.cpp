#include <stdio.h>
#include <string.h>
// CTCInventory::ClosePCInventory(bool closeInGameMenu) -- leave the PC inventory screens: cancel the pending UI action,
// close the edit boxes and the menu, close the in-game menu, stop the inventory effect, destroy the PC screens, reset the
// state, release the UI manager's modal state, restore the three audio group volumes from the user profile, clear the
// quest-persistent flags and restart game rendering
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00587847, 506 bytes, `ret 4` (frame pointer, `leave`).
// Shape: the two discarded `HasTC(t) ? FindTCInline(t) : 0` component fetches (0x13, 0x12) keep the parent thing loaded in
// eax for `lea ecx,[eax+20h]` (a bare HasTC would fold to `mov ecx; add ecx,20h`); GetInterface one-ternary for 0xc6.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CTCBase;
struct CPresenceBits { bool test(long kind); };             // bitset<>::test 0x00410DE0
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
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
#pragma auto_inline(on)

class CThing {
public:
    unsigned char _pad00[0x20];
    CPresenceBits present;                                  // +0x20 std::bitset of interface types
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap m_interfaces;                             // +0x44
    __forceinline bool HasTC(long type) { return present.test(type); }
    // Inline component find (lower bound + not-found -> end), the shape of a fully-inlined GetTC: with its result unused
    // VC7.1 deletes the search but keeps the thing pointer it loaded for it (`mov eax,[edi+4]` / `lea ecx,[eax+20h]`).
    CTCBase* FindTCInline(long type)
    {
        CInterfaceMap::SEntry* it = m_interfaces.first;
        long count = m_interfaces.last - m_interfaces.first;
        while (count > 0) {
            long half = count / 2;
            if (it[half].type < type) { it += half + 1; count -= half + 1; } else count = half;
        }
        if (it == m_interfaces.last || type < it->type) it = m_interfaces.last;
        return it->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (present.test(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline T* FindTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
};
class CTCInGameMenu { public:                                      // interface type 0xC6
    void CloseInGameMenuPC(bool closeAll);                  // 0x0047DBB8
};
struct CTCInventoryEffect {                                 // interface type 0x10D
    void Stop();                                            // 0x00562E70
};
namespace NUISystem {
class CComponent { public:
    virtual ~CComponent();                                  // slot 0 (scalar deleting destructor)
};
class CManager { public:
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04(); virtual void m05(); 
    virtual void ReleaseFocus_slot6();                      // slot 6 (+0x18)
    virtual void m07(); virtual void m08(); virtual void m09(); virtual void m10(); virtual void m11(); virtual void m12(); virtual void m13(); virtual void m14(); virtual void m15(); virtual void m16(); virtual void m17(); virtual void m18(); virtual void m19(); virtual void m20(); virtual void m21(); virtual void m22(); virtual void m23(); virtual void m24(); virtual void m25(); virtual void m26(); virtual void m27(); virtual void m28(); 
    virtual void ShowMousePointer_slot29();                 // slot 29 (+0x74)
    virtual void m30(); virtual void m31(); virtual void m32(); virtual void m33(); virtual void m34(); virtual void m35(); virtual void m36(); 
    virtual void PopModal_slot37();                         // slot 37 (+0x94)
    static CManager* __fastcall GetInstance();              // 0x0041E5F2
};
}
struct CComponentVector {                                   // std::vector<NUISystem::CComponent*>
    NUISystem::CComponent** _M_start;
    NUISystem::CComponent** _M_finish;
    NUISystem::CComponent** _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    NUISystem::CComponent*& operator[](unsigned long n) { return _M_start[n]; }
    NUISystem::CComponent** erase(NUISystem::CComponent** first, NUISystem::CComponent** last);   // 0x00485AA7
    void clear() { erase(_M_start, _M_finish); }
};
class CUserProfileManager { public:
    unsigned char _pad00[0x0C];
    bool          UseAlternateVolumes;                      // +0x0C
    unsigned char _pad0d[0x24 - 0x0D];
    float         MusicVolume;                              // +0x24
    unsigned char _pad28[0x2C - 0x28];
    float         SoundVolume;                              // +0x2C
    unsigned char _pad30[0xE4 - 0x30];
    float         AlternateMusicVolume;                     // +0xE4
    unsigned char _padE8[0xEC - 0xE8];
    float         AlternateSoundVolume;                     // +0xEC
    float GetMusicVolume() const { return UseAlternateVolumes ? AlternateMusicVolume : MusicVolume; }
    float GetSoundVolume() const { return UseAlternateVolumes ? AlternateSoundVolume : SoundVolume; }
    static CUserProfileManager& __fastcall Get();           // 0x0040D2A0 CTBaseSingleton<CUserProfileManager>::Get
};
struct CSoundSystem {
    virtual void s00(); virtual void s01(); virtual void s02(); virtual void s03(); virtual void s04(); virtual void s05(); virtual void s06(); virtual void s07(); virtual void s08(); virtual void s09(); virtual void s10(); virtual void s11(); virtual void s12(); virtual void s13(); virtual void s14(); virtual void s15(); virtual void s16(); virtual void s17(); virtual void s18(); virtual void s19(); virtual void s20(); virtual void s21(); virtual void s22(); virtual void s23(); virtual void s24(); virtual void s25(); virtual void s26(); virtual void s27(); virtual void s28(); virtual void s29(); virtual void s30(); virtual void s31(); virtual void s32(); virtual void s33(); virtual void s34(); virtual void s35(); virtual void s36(); virtual void s37(); virtual void s38(); virtual void s39(); virtual void s40(); virtual void s41(); virtual void s42(); virtual void s43(); virtual void s44(); virtual void s45(); 
    virtual void SetGroupVolume(long groupMask, float volume, float fadeTime);   // slot 46 (+0xB8)
};
extern CSoundSystem* g_pSoundSystem;                        // 0x013B8394
struct CRenderer {
    virtual void r00(); virtual void r01(); virtual void r02(); virtual void r03(); virtual void r04(); virtual void r05(); virtual void r06(); virtual void r07(); virtual void r08(); virtual void r09(); virtual void r10(); virtual void r11(); virtual void r12(); virtual void r13(); virtual void r14(); virtual void r15(); virtual void r16(); virtual void r17(); virtual void r18(); virtual void r19(); virtual void r20(); virtual void r21(); virtual void r22(); virtual void r23(); virtual void r24(); virtual void r25(); virtual void r26(); virtual void r27(); virtual void r28(); virtual void r29(); virtual void r30(); virtual void r31(); virtual void r32(); virtual void r33(); virtual void r34(); virtual void r35(); virtual void r36(); virtual void r37(); virtual void r38(); virtual void r39(); virtual void r40(); virtual void r41(); virtual void r42(); virtual void r43(); virtual void r44(); virtual void r45(); virtual void r46(); virtual void r47(); virtual void r48(); virtual void r49(); virtual void r50(); virtual void r51(); virtual void r52(); virtual void r53(); virtual void r54(); virtual void r55(); 
    virtual void SetPaused_slot56(bool a, bool b);          // slot 56 (+0xE0)
};
class CDisplayEngine { public:
    unsigned char _pad00[0x2C];
    CRenderer*    PRenderer;                                // +0x2C
};
class CEngine { public:
    unsigned char   _pad00[0x28];
    CDisplayEngine* PDisplayEngine;                         // +0x28
};
extern CEngine* g_pEngine;                                  // 0x013B86A0
struct CInventoryCamera {
    void Reset();                                           // 0x004855A3
};
extern CInventoryCamera g_InventoryCamera;                  // 0x013B8A98
void __fastcall CancelPendingUIAction();                    // 0x00556470

struct CTreePane {
    void Reset();                                           // 0x00592BA7
};

class CTCInventory { public:
    void*            __vftable;                             // +0x00
    CThing*          ParentThing;                           // +0x04 CTCBase
    unsigned char    _pad_0x08[0x04];
    CComponentVector PCScreens;                             // +0x0C
    unsigned char    _pad_0x18[0x144 - 0x18];
    CTreePane        ItemTree;                              // +0x144
    unsigned char    _pad_0x145[0x21C - 0x145];
    long             PCInventoryState;                      // +0x21C
    void DeactivateAllEditBoxes(bool activate);             // 0x00582414
    void ClosePCInventoryMenu();                            // 0x00584B42
    void ClearQuestPersistentFlags();                       // 0x005BD597 (CTCInventoryBase)
    void StartGameRendering();                              // 0x00581C1E
    void ClosePCInventory(bool closeInGameMenu);
};

void CTCInventory::ClosePCInventory(bool closeInGameMenu)
{
    CancelPendingUIAction();
    DeactivateAllEditBoxes(false);
    ItemTree.Reset();
    ClosePCInventoryMenu();
    {
        CThing* parent = ParentThing;
        CTCBase* unused = parent->HasTC(0x13) ? parent->FindTCInline(0x13) : 0;
    }
    {
        CThing* parent = ParentThing;
        CTCBase* unused = parent->HasTC(0x12) ? parent->FindTCInline(0x12) : 0;
    }
    {
        CThing* parent = ParentThing;
        CTCInGameMenu* menu;
        if (parent->GetInterface(&menu, 0xc6))
            menu->CloseInGameMenuPC(closeInGameMenu);
    }
    {
        CThing* parent = ParentThing;
        if (parent->HasTC(0x10d))
            parent->FindTC<CTCInventoryEffect>(0x10d)->Stop();
    }
    for (unsigned long i = 0; i < PCScreens.size(); ++i)
        delete PCScreens[i];
    PCScreens.clear();
    PCInventoryState = 0;
    NUISystem::CManager::GetInstance()->PopModal_slot37();
    NUISystem::CManager::GetInstance()->ReleaseFocus_slot6();
    NUISystem::CManager::GetInstance()->ShowMousePointer_slot29();
    g_InventoryCamera.Reset();
    if (g_pSoundSystem) {
        g_pSoundSystem->SetGroupVolume(0x10, CUserProfileManager::Get().GetMusicVolume(), 0.0f);
        g_pSoundSystem->SetGroupVolume(0x40, CUserProfileManager::Get().GetSoundVolume(), 0.0f);
        g_pSoundSystem->SetGroupVolume(0x80, CUserProfileManager::Get().GetMusicVolume(), 0.0f);
    }
    ClearQuestPersistentFlags();
    StartGameRendering();
    g_pEngine->PDisplayEngine->PRenderer->SetPaused_slot56(false, false);
}

// ---- test stubs ----
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("CHECK failed line %d: %s\n", __LINE__, #c); fails++; } } while (0)
static char g_log[1024];
static void L(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }
static long g_tests[8]; static int g_ntests; static void* g_testThis; static bool g_hasC6, g_has10d; static int g_deleted;
static void* g_closeThis; static int g_closeArg; static void* g_stopThis;
static long g_volMask[4]; static float g_vol[4]; static float g_fade[4]; static int g_nvol; static int g_pauseA = -1, g_pauseB = -1;
bool CPresenceBits::test(long kind)
{
    if (g_ntests < 8) g_tests[g_ntests] = kind; g_ntests++; g_testThis = this;
    if (kind == 0xc6) return g_hasC6; if (kind == 0x10d) return g_has10d; return true;
}
void CTCInGameMenu::CloseInGameMenuPC(bool b) { g_closeThis = this; g_closeArg = b; L("closeIGM"); }
void CTCInventoryEffect::Stop() { g_stopThis = this; L("stop"); }
NUISystem::CComponent::~CComponent() { g_deleted++; }
static NUISystem::CManager g_mgr;
#define MS(n) void NUISystem::CManager::n() {}
MS(m00) MS(m01) MS(m02) MS(m03) MS(m04) MS(m05) MS(m07) MS(m08) MS(m09) MS(m10) MS(m11) MS(m12) MS(m13) MS(m14) MS(m15)
MS(m16) MS(m17) MS(m18) MS(m19) MS(m20) MS(m21) MS(m22) MS(m23) MS(m24) MS(m25) MS(m26) MS(m27) MS(m28) MS(m30) MS(m31)
MS(m32) MS(m33) MS(m34) MS(m35) MS(m36)
void NUISystem::CManager::ReleaseFocus_slot6() { L("focus6"); }
void NUISystem::CManager::ShowMousePointer_slot29() { L("mouse29"); }
void NUISystem::CManager::PopModal_slot37() { L("pop37"); }
NUISystem::CManager* __fastcall NUISystem::CManager::GetInstance() { return &g_mgr; }
NUISystem::CComponent** CComponentVector::erase(NUISystem::CComponent** first, NUISystem::CComponent** last)
{
    NUISystem::CComponent** out = first; for (NUISystem::CComponent** p = last; p != _M_finish; ++p) *out++ = *p; _M_finish = out; L("erase"); return first;
}
static CUserProfileManager* g_profile;
CUserProfileManager& __fastcall CUserProfileManager::Get() { return *g_profile; }
#define SS(n) void CSoundSystem::n() {}
SS(s00) SS(s01) SS(s02) SS(s03) SS(s04) SS(s05) SS(s06) SS(s07) SS(s08) SS(s09) SS(s10) SS(s11) SS(s12) SS(s13) SS(s14) SS(s15)
SS(s16) SS(s17) SS(s18) SS(s19) SS(s20) SS(s21) SS(s22) SS(s23) SS(s24) SS(s25) SS(s26) SS(s27) SS(s28) SS(s29) SS(s30) SS(s31)
SS(s32) SS(s33) SS(s34) SS(s35) SS(s36) SS(s37) SS(s38) SS(s39) SS(s40) SS(s41) SS(s42) SS(s43) SS(s44) SS(s45)
void CSoundSystem::SetGroupVolume(long mask, float v, float fade) { if (g_nvol < 4) { g_volMask[g_nvol] = mask; g_vol[g_nvol] = v; g_fade[g_nvol] = fade; } g_nvol++; L("vol"); }
CSoundSystem* g_pSoundSystem;
#define RS(n) void CRenderer::n() {}
RS(r00) RS(r01) RS(r02) RS(r03) RS(r04) RS(r05) RS(r06) RS(r07) RS(r08) RS(r09) RS(r10) RS(r11) RS(r12) RS(r13) RS(r14) RS(r15)
RS(r16) RS(r17) RS(r18) RS(r19) RS(r20) RS(r21) RS(r22) RS(r23) RS(r24) RS(r25) RS(r26) RS(r27) RS(r28) RS(r29) RS(r30) RS(r31)
RS(r32) RS(r33) RS(r34) RS(r35) RS(r36) RS(r37) RS(r38) RS(r39) RS(r40) RS(r41) RS(r42) RS(r43) RS(r44) RS(r45) RS(r46) RS(r47)
RS(r48) RS(r49) RS(r50) RS(r51) RS(r52) RS(r53) RS(r54) RS(r55)
void CRenderer::SetPaused_slot56(bool a, bool b) { g_pauseA = a; g_pauseB = b; L("paused"); }
CEngine* g_pEngine;
CInventoryCamera g_InventoryCamera;
void CInventoryCamera::Reset() { L("camreset"); }
void __fastcall CancelPendingUIAction() { L("cancel"); }
void CTreePane::Reset() { L("tree"); }
void CTCInventory::DeactivateAllEditBoxes(bool a) { L(a ? "edit1" : "edit0"); }
void CTCInventory::ClosePCInventoryMenu() { L("invmenu"); }
void CTCInventory::ClearQuestPersistentFlags() { L("questflags"); }
void CTCInventory::StartGameRendering() { L("render"); }
static void Reset() { g_log[0] = 0; g_ntests = 0; g_deleted = 0; g_closeThis = 0; g_closeArg = -1; g_testThis = 0; g_stopThis = 0; g_nvol = 0; g_pauseA = g_pauseB = -1; }
static unsigned char g_thingBuf[0x100], g_invBuf[sizeof(CTCInventory)], g_profBuf[0x100];
static NUISystem::CComponent* g_store[16];
int main()
{
    CThing* thing = (CThing*)g_thingBuf;
    static CTCInGameMenu igm; static CTCInventoryEffect eff;
    CInterfaceMap::SEntry e[] = { { 0x10, 0 }, { 0xc6, (CTCBase*)&igm }, { 0x10d, (CTCBase*)&eff } };
    thing->m_interfaces.first = e; thing->m_interfaces.last = e + 3;
    CTCInventory* inv = (CTCInventory*)g_invBuf; inv->ParentThing = thing;
    g_profile = (CUserProfileManager*)g_profBuf;
    g_profile->MusicVolume = 0.5f; g_profile->SoundVolume = 0.25f; g_profile->AlternateMusicVolume = 0.75f; g_profile->AlternateSoundVolume = 0.125f;
    static CSoundSystem snd; static CRenderer rend; static CDisplayEngine disp; static CEngine eng;
    disp.PRenderer = &rend; eng.PDisplayEngine = &disp; g_pEngine = &eng;
    inv->PCScreens._M_start = g_store; inv->PCScreens._M_end_of_storage = g_store + 16;
    // full path: both components present, sound system on, primary volumes
    g_store[0] = new NUISystem::CComponent; g_store[1] = 0; g_store[2] = new NUISystem::CComponent;
    inv->PCScreens._M_finish = g_store + 3; inv->PCInventoryState = 5;
    g_hasC6 = true; g_has10d = true; g_pSoundSystem = &snd; g_profile->UseAlternateVolumes = false; Reset();
    inv->ClosePCInventory(true);
    CHECK(strcmp(g_log, "cancel;edit0;tree;invmenu;closeIGM;stop;erase;pop37;focus6;mouse29;camreset;vol;vol;vol;questflags;render;paused;") == 0);
    CHECK(g_ntests == 4 && g_tests[0] == 0x13 && g_tests[1] == 0x12 && g_tests[2] == 0xc6 && g_tests[3] == 0x10d && g_testThis == (char*)thing + 0x20);
    CHECK(g_closeThis == &igm && g_closeArg == 1 && g_stopThis == &eff);
    CHECK(g_deleted == 2 && inv->PCScreens.size() == 0 && inv->PCInventoryState == 0);
    CHECK(g_nvol == 3 && g_volMask[0] == 0x10 && g_volMask[1] == 0x40 && g_volMask[2] == 0x80);
    CHECK(g_vol[0] == 0.5f && g_vol[1] == 0.25f && g_vol[2] == 0.5f && g_fade[0] == 0.0f && g_fade[2] == 0.0f);
    CHECK(g_pauseA == 0 && g_pauseB == 0);
    // alternate volumes, closeInGameMenu false, no effect component, no sound system later
    inv->PCScreens._M_finish = g_store; inv->PCInventoryState = 2;
    g_has10d = false; g_profile->UseAlternateVolumes = true; Reset();
    inv->ClosePCInventory(false);
    CHECK(g_closeArg == 0 && g_stopThis == 0 && g_deleted == 0);
    CHECK(g_nvol == 3 && g_vol[0] == 0.75f && g_vol[1] == 0.125f && g_vol[2] == 0.75f);
    // no menu component, no sound system
    g_hasC6 = false; g_pSoundSystem = 0; Reset();
    inv->ClosePCInventory(true);
    CHECK(strcmp(g_log, "cancel;edit0;tree;invmenu;erase;pop37;focus6;mouse29;camreset;questflags;render;paused;") == 0);
    CHECK(g_closeThis == 0 && g_nvol == 0 && g_ntests == 4);
    if (fails == 0) printf("00587847_TEST PASS\n"); else printf("00587847_TEST FAIL\n");
    return fails;
}