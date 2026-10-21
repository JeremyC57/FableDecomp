// Grade: DIFFER(396v396) -- same length; retail materialises a zero register (ebx) right after GetWorldFrame for every later 0 (profile flag byte, sound-system test, def release, pushes), spills the second UI manager to [ebp-4] and builds the zero vector before the GetWorld call; ours zeroes esi late, keeps the manager in ebx and sinks the vector stores into the AddMessage pushes
// CTCInventoryExperience::CloseInventory() -- closes the experience screen: HUD back on, stamp the world's close frame, restore the two sound categories to the profile's level and stop the tutorial sounds, re-apply the UI manager's setting, deactivate the screen, release the HUD, post message 0x4A and close the menu count
// Leaf: CloseInventory
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0056F30B, 396 bytes, plain `ret`. Size TU with a frame pointer:
//   `#pragma optimize("y", off)` + `("s", on)` (as the landed TU-mate OpenInventory 0x0056FAD7).
// Callees: the main game component (0x013B86A0) chain +0x28 -> +0x2C and that object's slots 76 (+0x130, SetEnabled)
//   and 56 (+0xE0), CTCBase::GetWorld @ 0x00686A70 (twice), CWorld::GetWorldFrame @ 0x0049D870 (static),
//   CTBaseSingleton<CUserProfileManager>::Get @ 0x0040D2A0, the sound system (0x013B8394) slot 46 (+0xB8,
//   RefreshCategory(long, float, float)) and slot 9 (+0x24, its channel manager, whose slot 9 stops a handle),
//   _Rb_global<bool>::_M_increment @ 0x004292C0 (cdecl), NUISystem::CManager::GetInstance @ 0x0041E5F2 (twice) and
//   its slots 42 (+0xA8, a counted def by hidden pointer) and 40 (+0xA0), this's slot 89 (+0x164),
//   CMessageEventManager::AddMessage @ 0x00687540 (world +0x60), the open-menus counter's close @ 0x0041D165.
// Data: g_FramesPerSecond @ 0x01375550.
// Layout (retail, 12-byte containers): TutorialSoundHandles map<CCharString, unsigned long> +0x134 (header first; node
//   value +0x14); CWorld +0x8C the last inventory-close frame; CUserProfileManager +0x0C flag, +0x24 / +0xE4 levels.
#pragma optimize("y", off)
#pragma optimize("s", on)

void __fastcall OnCloseMenus();                      // 0x0041D165
extern long g_FramesPerSecond;                       // 0x01375550

struct C3DVector {
    float x, y, z;
    C3DVector() : x(0), y(0), z(0) { }
};
class CThing;
struct CMessageEventManager {
    void* AddMessage(long type, unsigned long distance, C3DVector* pos, CThing* source, CThing* extra, float a, float b, long c);   // 0x00687540
};
struct CWorld {
    unsigned char _pad00[0x60];
    CMessageEventManager* m_pMessages;               // +0x60
    unsigned char _pad64[0x8c - 0x64];
    long LastInventoryCloseFrame;                    // +0x8C
    static long GetWorldFrame();                     // 0x0049D870
};

struct CHudOwner {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55();
    virtual void Release_slot56(long a, long b);     // slot 56 (+0xE0)
    virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61();
    virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66();
    virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75();
    virtual void SetEnabled(bool enabled);           // slot 76 (+0x130)
};
struct CWorldPart {
    unsigned char _pad_0x00[0x2c];
    CHudOwner* Hud;                                  // +0x2c
};
struct CMainGameComponent {
    unsigned char _pad_0x00[0x28];
    CWorldPart* World;                               // +0x28
};
extern CMainGameComponent* g_pMainGameComponent;     // 0x013B86A0

struct CUserProfileManager {
    unsigned char _pad00[0x0c];
    bool  UseAlternateLevel;                         // +0x0C
    unsigned char _pad0d[0x24 - 0x0d];
    float Level;                                     // +0x24
    unsigned char _pad28[0xe4 - 0x28];
    float AlternateLevel;                            // +0xE4
    float GetLevel() const { return UseAlternateLevel ? AlternateLevel : Level; }
    static CUserProfileManager& __fastcall Get();    // 0x0040D2A0
};

struct CSoundChannels {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void StopSound(unsigned long handle);    // slot 9 (+0x24)
};
struct CSoundSystem {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual CSoundChannels* GetChannels();           // slot 9 (+0x24)
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45();
    virtual void RefreshCategory(long category, float a, float b);   // slot 46 (+0xB8)
};
extern CSoundSystem* g_pSoundSystem;                 // 0x013B8394

struct SHandleNode {                                 // _Rb_tree_node<pair<const CCharString, unsigned long> >
    long color;
    SHandleNode* parent;
    SHandleNode* left;
    SHandleNode* right;
    void* first;                                     // +0x10 CCharString
    unsigned long second;                            // +0x14
};
SHandleNode* __cdecl RbIncrement(SHandleNode* node); // 0x004292C0 (_Rb_global<bool>::_M_increment)
struct CHandleIterator {
    SHandleNode* _M_node;
    CHandleIterator(SHandleNode* node) : _M_node(node) { }
    void operator++() { _M_node = RbIncrement(_M_node); }
    bool operator!=(const CHandleIterator& other) const { return _M_node != other._M_node; }
    SHandleNode* operator->() const { return _M_node; }
};
struct CTutorialSoundHandleMap {                     // map<CCharString, unsigned long> (retail 12 bytes)
    SHandleNode* _M_header;
    long _M_node_count;
    long _M_compare;
    CHandleIterator begin() { return CHandleIterator(_M_header->left); }
    CHandleIterator end() { return CHandleIterator(_M_header); }
};

struct CDestroyable {
    virtual void v00();
    virtual void Destroy();                          // slot 1
};
struct CRefCount {
    long RefCount;
    void DecRef() { --RefCount; }
    long GetRefCount() const { return RefCount; }
};
struct CDefClassBase : CDestroyable, CRefCount {     // ref count at +0x04
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CUISettingsDef : CDefClassBase {
    unsigned char _pad08[0x1a8 - 0x08];
    long Setting;                                    // +0x1A8
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer(const CDefPointer& other);
    __forceinline ~CDefPointer()
    {
        if (Object) {
            Object->ReleaseTemporary();
            Object = 0;
        }
    }
};

namespace NUISystem {
struct CManager {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04();
    virtual void m05(); virtual void m06(); virtual void m07(); virtual void m08(); virtual void m09();
    virtual void m10(); virtual void m11(); virtual void m12(); virtual void m13(); virtual void m14();
    virtual void m15(); virtual void m16(); virtual void m17(); virtual void m18(); virtual void m19();
    virtual void m20(); virtual void m21(); virtual void m22(); virtual void m23(); virtual void m24();
    virtual void m25(); virtual void m26(); virtual void m27(); virtual void m28(); virtual void m29();
    virtual void m30(); virtual void m31(); virtual void m32(); virtual void m33(); virtual void m34();
    virtual void m35(); virtual void m36(); virtual void m37(); virtual void m38(); virtual void m39();
    virtual void ApplySetting_slot40(const long* setting, bool b);       // slot 40 (+0xA0)
    virtual void m41();
    virtual CDefPointer<CUISettingsDef> GetSettings_slot42();            // slot 42 (+0xA8)
    static CManager* __fastcall GetInstance();       // 0x0041E5F2
};
}
using NUISystem::CManager;

struct CTCInventoryExperience {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64();
    virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
    virtual void v75(); virtual void v76(); virtual void v77(); virtual void v78(); virtual void v79();
    virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83(); virtual void v84();
    virtual void v85(); virtual void v86(); virtual void v87(); virtual void v88();
    virtual void SetScreenInactive(long value);      // slot 89 (+0x164)
    CThing* ParentThing;                             // +0x04
    unsigned char _pad_0x08[0x134 - 0x08];
    CTutorialSoundHandleMap TutorialSoundHandles;    // +0x134

    CWorld* GetWorld() const;                        // 0x00686A70 (CTCBase)
    void CloseInventory();
};

void CTCInventoryExperience::CloseInventory()
{
    g_pMainGameComponent->World->Hud->SetEnabled(true);
    CWorld* world = GetWorld();
    world->LastInventoryCloseFrame = CWorld::GetWorldFrame();
    if (g_pSoundSystem) {
        g_pSoundSystem->RefreshCategory(0x10, CUserProfileManager::Get().GetLevel(), 0.0f);
        g_pSoundSystem->RefreshCategory(0x80, CUserProfileManager::Get().GetLevel(), 0.0f);
        for (CHandleIterator it = TutorialSoundHandles.begin(); it != TutorialSoundHandles.end(); ++it)
            g_pSoundSystem->GetChannels()->StopSound(it->second);
    }
    CManager::GetInstance()->ApplySetting_slot40(&CManager::GetInstance()->GetSettings_slot42().Object->Setting, true);
    SetScreenInactive(1);
    g_pMainGameComponent->World->Hud->Release_slot56(0, 0);
    C3DVector origin;
    GetWorld()->m_pMessages->AddMessage(0x4a, g_FramesPerSecond * 10, &origin, 0, 0, 0.0f, 0.0f, 0);
    OnCloseMenus();
}
