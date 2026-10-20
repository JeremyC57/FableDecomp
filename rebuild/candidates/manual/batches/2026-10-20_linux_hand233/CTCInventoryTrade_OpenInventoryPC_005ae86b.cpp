// CTCInventoryTrade::OpenInventoryPC() -- open the PC trade screen: base screen preparation, HUD off, fade reset, the three sound categories refreshed, front-end notifications, the top-level list built, the mouse pointer component created and registered, then the base inventory opened and the screen marked open
// Leaf: OpenInventoryPC
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005AE86B, length 263 bytes, plain `ret`. Frame-pointer size TU: optimize("y",off)
// + ("s",on). Vtable 0x0125318C slot (rtti abs reference); FableWin.pdb ?OpenInventoryPC@CTCInventoryTrade@@UAEXXZ.
// Callees: CTCInventoryBase::PrepareScreen @ 0x005BC6ED, the main game component (0x013B86A0) chain +0x28 -> +0x2C slot 76
// (+0x130, SetEnabled(false)), the sound system (0x013B8394) slot 46 (+0xB8, RefreshCategory(long, float, float)) for
// categories 0x10 / 0x40 / 0x80 with (0.0f, 500.0f @ 0x01237584), NUISystem::CFrontEndManager::GetInstance @ 0x0041E5F2 and
// its slots 23 (+0x5C, given this; name descriptive) and 26 (+0x68, OnThingInventoryOpened(ParentThing)), CTCInventoryTrade
// @ 0x005ACEDA (no arguments: builds the PC screen; name descriptive), CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0
// ("UI_MOUSE_POINTER" @ 0x0124DFB4), CManager::CreateComponent(const CCharString&, bool) @ 0x0041DB1D, the pointer's slot 43
// (+0xAC, a reset hook), the base Components vector's push_back(const T&) @ 0x00486024, CTCInventoryBase::OpenInventory @
// 0x005BD4EA.
// Shape: the landed CTCInventoryExperience::OpenInventory (0x0056FAD7) model: the front-end manager fetched once into a local
// (edi) after the sound block, the pointer's name a block-scoped CCharString (the member address kept in ebx for the store,
// the reset call and the push_back).
// Layout: retail CTCInventoryTrade (see ~CTCInventoryTrade 0x0059EC48): Components +0x0c (base), InventoryTradeOpen +0x218,
// PMouse_Pointer +0x21c, AccumFadeTime +0x228.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CThing;
namespace NUISystem {
struct CComponent {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42();
    virtual void Reset();                       // slot 43 (+0xAC) (name descriptive)
};
struct CManager {                               // the front-end manager's vptr is its first word
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual void OnInventoryScreenOpened(void* screen);   // slot 23 (+0x5C) (name descriptive)
    virtual void v24(); virtual void v25();
    virtual void OnThingInventoryOpened(CThing* thing);   // slot 26 (+0x68)
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
};
struct CFrontEndManager : CManager {
    static CFrontEndManager* __fastcall GetInstance();    // 0x0041E5F2
};
}
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
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75();
    virtual void SetEnabled(bool enabled);      // slot 76 (+0x130)
};
struct CWorldPart {
    unsigned char _pad_0x00[0x2c];
    CHudOwner* Hud;                             // +0x2c
};
struct CMainGameComponent {
    unsigned char _pad_0x00[0x28];
    CWorldPart* World;                          // +0x28
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
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
extern CSoundSystem* g_pSoundSystem;            // 0x013B8394
struct CComponentVector {                       // vector<NUISystem::CComponent*>
    NUISystem::CComponent** _M_start;
    NUISystem::CComponent** _M_finish;
    NUISystem::CComponent** _M_end_of_storage;
    void push_back(NUISystem::CComponent* const& value);   // 0x00486024
};

struct CTCInventoryTrade {
    void*            __vftable;                 // +0x00
    CThing*          ParentThing;               // +0x04
    unsigned char    _pad08[0x0c - 0x08];
    CComponentVector Components;                // +0x0c (CTCInventoryBase)
    unsigned char    _pad18[0x218 - 0x18];
    bool             InventoryTradeOpen;        // +0x218
    bool             InventoryTradeInitialised; // +0x219
    NUISystem::CComponent* PMouse_Pointer;      // +0x21c
    NUISystem::CComponent* PTop_Level_List;     // +0x220
    NUISystem::CComponent* PCurrentOpenTradeMenu;   // +0x224
    float            AccumFadeTime;             // +0x228
    void PrepareScreen();                       // 0x005BC6ED (CTCInventoryBase)
    void ConstructTradeScreenPC();              // 0x005ACEDA
    void OpenInventory();                       // 0x005BD4EA (CTCInventoryBase)
    void OpenInventoryPC();
};

void CTCInventoryTrade::OpenInventoryPC()
{
    PrepareScreen();
    g_pMainGameComponent->World->Hud->SetEnabled(false);
    AccumFadeTime = 0.0f;
    if (g_pSoundSystem) {
        g_pSoundSystem->RefreshCategory(0x10, 0.0f, 500.0f);
        g_pSoundSystem->RefreshCategory(0x40, 0.0f, 500.0f);
        g_pSoundSystem->RefreshCategory(0x80, 0.0f, 500.0f);
    }
    NUISystem::CFrontEndManager* frontEnd = NUISystem::CFrontEndManager::GetInstance();
    frontEnd->OnInventoryScreenOpened(this);
    frontEnd->OnThingInventoryOpened(ParentThing);
    ConstructTradeScreenPC();
    {
        CCharString name("UI_MOUSE_POINTER");
        PMouse_Pointer = frontEnd->CreateComponent(name, false);
    }
    PMouse_Pointer->Reset();
    Components.push_back(PMouse_Pointer);
    OpenInventory();
    InventoryTradeOpen = true;
}
