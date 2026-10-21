// Grade: DIFFER(506v506) -- the two result-less presence tests (types 0x13, 0x12): retail `mov eax,[edi+4] ; push N ; lea ecx,[eax+20h]`, ours `mov ecx,[edi+4] ; add ecx,20h`; tried a bare HasTC statement, `if (HasTC) { }`, a dead out-pointer inline and a bitset reference
// CTCInventory::ClosePCInventory(bool closeInGameMenu) -- leave the PC inventory screens: cancel the pending UI action,
// close the edit boxes and the menu, close the in-game menu, stop the inventory effect, destroy the PC screens, reset the
// state, release the UI manager's modal state, restore the three audio group volumes from the user profile, clear the
// quest-persistent flags and restart game rendering
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00587847, 506 bytes, `ret 4` (frame pointer, `leave`).
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
class CThing {
public:
    unsigned char _pad00[0x20];
    CPresenceBits present;                                  // +0x20 std::bitset of interface types
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap m_interfaces;                             // +0x44
    __forceinline bool HasTC(long type) { return present.test(type); }
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
struct CTCInGameMenu {                                      // interface type 0xC6
    void CloseInGameMenuPC(bool closeAll);                  // 0x0047DBB8
};
struct CTCInventoryEffect {                                 // interface type 0x10D
    void Stop();                                            // 0x00562E70
};
namespace NUISystem {
struct CComponent {
    virtual ~CComponent();                                  // slot 0 (scalar deleting destructor)
};
struct CManager {
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
struct CUserProfileManager {
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
struct CDisplayEngine {
    unsigned char _pad00[0x2C];
    CRenderer*    PRenderer;                                // +0x2C
};
struct CEngine {
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

struct CTCInventory {
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
        parent->HasTC(0x13);
    }
    {
        CThing* parent = ParentThing;
        parent->HasTC(0x12);
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
