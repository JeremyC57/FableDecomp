// CTCInventoryTrade::CloseInventory() -- closes the console trade screen: restores the hero-info screen type, empties the special categories, closes the base view, deletes every UI component, clears the buy/sell maps, notifies the front end, re-enables the HUD, stamps the world's close frame, restores the sound categories, resets the player's GUI state, re-checks sold wielded weapons and closes the menu count
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059F304, 538 bytes, plain `ret`. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: 0x0059D9AD (RemoveAllItemsFromCategory 0x0059CF49 for categories 12 and 11; name descriptive; manifest
// "CMemoryAllocator::ClearPerformanceStats" is propagated), CTCInventoryBase::CloseInventory @ 0x005BD597 (manifest
// "CCategory::SetQuestPersistentFlagForItems" is propagated), CPlayerGui::GetPHeroInfoScreensManager @ 0x00437CD0 (g_pPlayerGui 0x013B8790)
// and NHeroInformationScreens::CManager::SetCurrentScreenType @ 0x0066E799 given PreviousScreenType (+0x174), the
// Components erase(first, last) @ 0x00485AA7, the CComponent-pair map clear @ 0x005B1C18 (ItemsToBuy/ItemsToSell; manifest
// "CTCCarryable::OnKill" propagated), CFrontEndManager::GetInstance @ 0x0041E5F2 and its slots 6/29/37/40/42 (names unknown),
// the global front-end node list @ 0x013B8A98 clear @ 0x004855A3, the main game component (0x013B86A0) chain +0x28 -> +0x2C
// slots 76 (+0x130, SetEnabled) and 56 (+0xE0), CTCBase::GetWorld @ 0x00686A70, GetWorldFrame @ 0x0049D870,
// CTBaseSingleton<CUserProfileManager>::Get @ 0x0040D2A0, the sound system (0x013B8394) slot 46 RefreshCategory,
// std::bitset::test @ 0x00410DE0 + CVectorMap::LowerBound @ 0x0040F020 (inline GetInterface, type 0x13),
// CTCInventoryWeapons::CheckForSoldWieldedWeapons @ 0x005CB780, NUISystem::CManager::OnCloseMenus @ 0x0041D165.
// The PC twin CloseInventoryPC @ 0x0059E2C7 has the same body with DeleteComponents in place of the screen-type restore and
// the two open-flag clears in place of OnCloseMenus.
// Shape: the component loop and clear() are the landed ~CTCInventoryTrade (0x0059EC48) form; the volume argument is an inline
// profile accessor (`flag ? custom : default`, re-fetching Get() per call); the world is a named local so GetWorld runs
// before GetWorldFrame; the front-end slot 42 returns a counted pointer by value, released through an inline
// `--RefCount == 0 -> DeleteThis()` member on the data (`mov ecx,eax ; add eax,4 ; dec [eax]`), the release forceinlined.
// Layout: retail CTCInventoryTrade (see the landed dtor): Components +0x0C, ItemsToBuy/ItemsToSell +0x144/+0x150,
// PreviousScreenType +0x174; engine/CTCInventoryTrade.h is the PDB-flattened layout, so
// the class is modelled locally.
#pragma optimize("y", off)
#pragma optimize("s", on)
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
    NUISystem::CComponent** begin() { return _M_start; }
    NUISystem::CComponent** end() { return _M_finish; }
    void erase(NUISystem::CComponent** first, NUISystem::CComponent** last);   // 0x00485AA7
    void clear() { erase(begin(), end()); }
};
struct CComponentPairMap {
    void* _M_header; unsigned long _M_node_count; long _M_key_compare;
    void clear();                                    // 0x005B1C18
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
struct CNodeList {
    void* m_pHead;
    void clear();                                    // 0x004855A3
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
class CTCInventoryWeapons { public:
    void CheckForSoldWieldedWeapons();               // 0x005CB780
};
struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type);            // 0x0040F020
};
class CThing {
public:
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

namespace NHeroInformationScreens {
class CManager { public:
    void SetCurrentScreenType(long type);            // 0x0066E799 (stores +0x28)
};
}
class CPlayerGui { public:
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() const;   // 0x00437CD0
};
extern CPlayerGui* g_pPlayerGui;                     // 0x013B8790
void __fastcall OnCloseMenus();                      // 0x0041D165 (NUISystem::CManager, static)

class CTCBase
{
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};
namespace NInventory {
class CTCInventoryBase : public CTCBase
{
public:
};
}

class CTCInventoryTrade : public NInventory::CTCInventoryBase { public:
    void*            __vftable;                      // +0x00
    CThing*          ParentThing;                    // +0x04
    unsigned char    _pad08[0x0c - 0x08];
    CComponentVector Components;                     // +0x0c (CTCInventoryBase)
    unsigned char    _pad18[0x144 - 0x18];
    CComponentPairMap ItemsToBuy;                    // +0x144
    CComponentPairMap ItemsToSell;                   // +0x150
    unsigned char    _pad15c[0x174 - 0x15c];
    long             PreviousScreenType;             // +0x174
    void RemoveTattooAndHairCategories();            // 0x0059D9AD
    void BaseCloseInventory();                       // 0x005BD597 (CTCInventoryBase::CloseInventory)
    void CloseInventory();
};

void CTCInventoryTrade::CloseInventory()
{
    g_pPlayerGui->GetPHeroInfoScreensManager()->SetCurrentScreenType(PreviousScreenType);
    RemoveTattooAndHairCategories();
    BaseCloseInventory();
    for (unsigned long i = 0; i < Components.size(); ++i)
        delete Components[i];
    Components.clear();
    ItemsToBuy.clear();
    ItemsToSell.clear();
    CFrontEndManager::GetInstance()->Slot6();
    CFrontEndManager::GetInstance()->Slot29();
    g_frontEndNodeList.clear();
    CFrontEndManager::GetInstance()->Slot37();
    g_pMainGameComponent->World->Hud->SetEnabled(true);
    CWorld* world = (&GetWorld());
    world->InventoryClosedFrame = GetWorldFrame();
    if (g_pSoundSystem) {
        g_pSoundSystem->RefreshCategory(0x10, CUserProfileManager::Get().GetSFXVolume(), 0.0f);
        g_pSoundSystem->RefreshCategory(0x40, CUserProfileManager::Get().GetMusicVolume(), 0.0f);
        g_pSoundSystem->RefreshCategory(0x80, CUserProfileManager::Get().GetSFXVolume(), 0.0f);
    }
    CFrontEndManager* a = CFrontEndManager::GetInstance();
    CFrontEndManager* b = CFrontEndManager::GetInstance();
    b->Slot40(&a->Slot42().Data->SomeState, true);
    g_pMainGameComponent->World->Hud->Slot56(0, 0);
    CTCInventoryWeapons* weapons;
    if (ParentThing->GetInterface(&weapons, 0x13))
        weapons->CheckForSoldWieldedWeapons();
    OnCloseMenus();
}