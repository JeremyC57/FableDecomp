// CTCInventoryMap::vfunc_31() -- closes the PC map screen: clears the hover/selection state; unless the hero-information screen is still active, restores the sound categories, resets the player's GUI state, re-enables the HUD, stamps the world's close frame, returns the hero-info manager to the previous screen, closes the base view, deletes every UI component, notifies the front end, clears the front-end node list and closes the menus; then marks the map dirty, clears its region lists and drops the two pending flags
// Retail: Fable.exe (TLC, VC7.1) @ 0x005FCE60, 584 bytes, plain `ret`. Speed TU (no pragmas, no frame pointer).
// Callees: as CTCInventoryQuests::CloseInventoryPC (0x00621720) plus CTCInventoryMap's list clear 0x005FC510.
// Layout: local view -- Components +0x0C, hero-info CBase +0x130, +0x188 / +0x214 cleared longs, +0x240 / +0x248 flags,
//   previous hero-info screen +0x244, +0x58 flag.
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int n);   // 0x00BFEAE6
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
namespace NUISystem {
class CComponent { public:
    virtual ~CComponent();
};
}
struct CComponentVector {
    NUISystem::CComponent** _M_start;
    NUISystem::CComponent** _M_finish;
    NUISystem::CComponent** _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    NUISystem::CComponent*& operator[](unsigned long i) { return _M_start[i]; }
    static NUISystem::CComponent** copy_trivial(NUISystem::CComponent** first, NUISystem::CComponent** last, NUISystem::CComponent** result)
    {
        if (first == last)
            return result;
        unsigned int n = (char*)last - (char*)first;
        return (NUISystem::CComponent**)((char*)memmove(result, first, n) + n);
    }
    void erase(NUISystem::CComponent** first, NUISystem::CComponent** last)
    {
        _M_finish = copy_trivial(last, _M_finish, first);
    }
    void clear() { erase(_M_start, _M_finish); }
};
struct CRefCounted {
    virtual void v0();
    virtual void DeleteThis();                       // slot 1 (+0x04)
    long RefCount;                                   // +0x04
    void Release() { if (--RefCount == 0) DeleteThis(); }
};
struct CPlayerData : CRefCounted {
    unsigned char _pad08[0x1a8 - 0x08];
    long SomeState;                                  // +0x1a8
};
struct CPlayerPtr {
    CPlayerData* Data;
    __forceinline ~CPlayerPtr() { if (Data) Data->Release(); }
};
struct CFrontEndManager {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void Slot6();                            // +0x18
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual void Slot29();                           // +0x74
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36();
    virtual void Slot37();                           // +0x94
    virtual void v38(); virtual void v39();
    virtual void Slot40(long* state, bool b);        // +0xA0
    virtual void v41();
    virtual CPlayerPtr Slot42();                     // +0xA8
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
};
struct CNodeListNode { CNodeListNode* _M_next; CNodeListNode* _M_prev; };
struct CNodeList {
    CNodeListNode* _M_node;
    void clear()
    {
        CNodeListNode* cur = _M_node->_M_next;
        while (cur != _M_node) {
            CNodeListNode* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
};
extern CNodeList g_frontEndNodeList;                 // 0x013B8A98
struct CHudOwner {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55();
    virtual void Slot56(long a, long b);             // +0xE0
    virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75();
    virtual void SetEnabled(bool enabled);           // slot 76 (+0x130)
};
struct CWorldPart {
    unsigned char _pad_0x00[0x2c];
    CHudOwner* Hud;                                  // +0x2c
};
class CMainGameComponent { public:
    unsigned char _pad_0x00[0x28];
    CWorldPart* World;                               // +0x28
};
extern CMainGameComponent* g_pMainGameComponent;     // 0x013B86A0
struct CSoundSystem {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45();
    virtual void RefreshCategory(long category, float a, float b);   // slot 46 (+0xB8)
};
extern CSoundSystem* g_pSoundSystem;                 // 0x013B8394
class CUserProfileManager { public:
    unsigned char _pad00[0x0c];
    bool  UseCustomVolumes;                          // +0x0c
    unsigned char _pad0d[0x24 - 0x0d];
    float SFXVolume;                                 // +0x24
    unsigned char _pad28[0x2c - 0x28];
    float MusicVolume;                               // +0x2c
    unsigned char _pad30[0xe4 - 0x30];
    float CustomSFXVolume;                           // +0xe4
    unsigned char _pade8[0xec - 0xe8];
    float CustomMusicVolume;                         // +0xec
    float GetSFXVolume() const { return UseCustomVolumes ? CustomSFXVolume : SFXVolume; }
    float GetMusicVolume() const { return UseCustomVolumes ? CustomMusicVolume : MusicVolume; }
    static CUserProfileManager& __fastcall Get();    // 0x0040D2A0
};
class CWorld { public:
    unsigned char _pad00[0x8c];
    long InventoryClosedFrame;                       // +0x8c
};
long __fastcall GetWorldFrame();                     // 0x0049D870
void __fastcall CloseMenus();                        // 0x0041D165
namespace NHeroInformationScreens {
class CManager { public: void SetScreen(long screen);        // 0x0066E799 (name unproven)
};
class CBase { public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual bool IsActive();                         // slot 3 (+0x0C)
};
}
class CPlayerGui { public:
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() const;   // 0x00437CD0
};
extern CPlayerGui* g_pPlayerGui;                     // 0x013B8790
class CTCInventoryMap { public:
    void*            __vftable;                      // +0x00
    void*            ParentThing;                    // +0x04
    unsigned char    _pad08[0x0c - 0x08];
    CComponentVector Components;                     // +0x0c
    unsigned char    _pad18[0x58 - 0x18];
    bool             Flag58;                         // +0x58
    unsigned char    _pad59[0x130 - 0x59];
    NHeroInformationScreens::CBase Screen;           // +0x130
    unsigned char    _pad134[0x188 - 0x134];
    long             Value188;                       // +0x188
    unsigned char    _pad18c[0x214 - 0x18c];
    long             Value214;                       // +0x214
    unsigned char    _pad218[0x240 - 0x218];
    bool             Dirty;                          // +0x240
    unsigned char    _pad241[0x244 - 0x241];
    long             PreviousScreen;                 // +0x244
    bool             Flag248;                        // +0x248
    void BaseCloseInventory();                       // 0x005BD597 (CTCInventoryBase::CloseInventory)
    CWorld* GetWorld();                              // 0x00686A70 (CTCBase)
    void ClearAllLists();                            // 0x005FC510
    void vfunc_31();
};
void CTCInventoryMap::vfunc_31()
{
    Value214 = 0;
    Value188 = 0;
    if (!Screen.IsActive()) {
        if (g_pSoundSystem) {
            g_pSoundSystem->RefreshCategory(0x10, CUserProfileManager::Get().GetSFXVolume(), 0.0f);
            g_pSoundSystem->RefreshCategory(0x40, CUserProfileManager::Get().GetMusicVolume(), 0.0f);
            g_pSoundSystem->RefreshCategory(0x80, CUserProfileManager::Get().GetSFXVolume(), 0.0f);
        }
        CFrontEndManager* a = CFrontEndManager::GetInstance();
        CFrontEndManager* b = CFrontEndManager::GetInstance();
        b->Slot40(&a->Slot42().Data->SomeState, true);
        g_pMainGameComponent->World->Hud->SetEnabled(true);
        CWorld* world = GetWorld();
        world->InventoryClosedFrame = GetWorldFrame();
        g_pPlayerGui->GetPHeroInfoScreensManager()->SetScreen(PreviousScreen);
        BaseCloseInventory();
        for (unsigned long i = 0; i < Components.size(); ++i)
            delete Components[i];
        Components.clear();
        CFrontEndManager::GetInstance()->Slot6();
        CFrontEndManager::GetInstance()->Slot29();
        g_frontEndNodeList.clear();
        CFrontEndManager::GetInstance()->Slot37();
        g_pMainGameComponent->World->Hud->Slot56(0, 0);
        CloseMenus();
    }
    Dirty = true;
    ClearAllLists();
    Flag58 = false;
    Flag248 = false;
}