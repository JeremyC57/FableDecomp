// CTCInventory::CloseInventory() -- shut the inventory: clear the quest-persistent flags and edit boxes, release the primitive handles, delete the PC screens, release the UI manager's modal state, reset the inventory camera, deactivate the hero's active mode components, restore audio volumes and the front end's style, and restart game rendering
// Leaf: ?CloseInventory@CTCInventory@@QAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave (stand-in map): the local stand-in map class (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad21[0x44 - 0x21] removed. Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Naming wave 18: the global stand-in g_pEngine (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
class CTCBase;
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CTCActiveMode {                                  // hero mode components with an active flag
    virtual void a000(); virtual void a001(); virtual void a002(); virtual void a003(); virtual void a004(); virtual void a005();
    virtual void a006(); virtual void a007(); virtual void a008(); virtual void a009(); virtual void a010(); virtual void a011();
    virtual void a012(); virtual void a013(); virtual void a014(); virtual void a015(); virtual void a016(); virtual void a017();
    virtual void a018(); virtual void a019(); virtual void a020(); virtual void a021(); virtual void a022(); virtual void a023();
    virtual void a024(); virtual void a025(); virtual void a026(); virtual void a027(); virtual void a028(); virtual void a029();
    virtual void a030();
    virtual void Deactivate();                                 // slot 31 (+0x7C)
    unsigned char _pad04[0x58 - 0x04];
    bool Active;                                        // +0x58
};
class CEnginePrimitiveHandle { public:
    void Clear();                                       // 0x0041BE70
    unsigned long m_handle;
    unsigned long m_extra;
};
struct CMemFunRefClear {
    void (CEnginePrimitiveHandle::*m_pmf)();
    explicit CMemFunRefClear(void (CEnginePrimitiveHandle::*pmf)()) : m_pmf(pmf) { }
};
CMemFunRefClear __fastcall for_each(CEnginePrimitiveHandle* first, CEnginePrimitiveHandle* last, CMemFunRefClear f);   // 0x0043F360
struct CHandleVector {
    CEnginePrimitiveHandle* _M_start;
    CEnginePrimitiveHandle* _M_finish;
    CEnginePrimitiveHandle* _M_end_of_storage;
};
struct CStyleDefPointer;
struct CDefObject {
    virtual void d00();
    virtual void Destroy();                             // slot 1
};
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CStyleDef : CDefObject, CRefCounted {            // ref count at +0x04
    FABLE_FORCEINLINE void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
    unsigned char _pad08[0x1a8 - 0x08];
    unsigned char Style[4];                             // +0x1a8
};
struct CStyleDefPointer {
    CStyleDef* Object;
    FABLE_FORCEINLINE ~CStyleDefPointer()
    {
        if (Object)
            Object->ReleaseTemporary();
    }
    CStyleDef* operator->() const { return Object; }
};
namespace NUISystem {
class CComponent { public:
    virtual ~CComponent();
};
class CManager { public:
    virtual void m000(); virtual void m001(); virtual void m002(); virtual void m003(); virtual void m004(); virtual void m005();
    virtual void ReleaseFocus();                               // slot 6 (+0x18)
    virtual void m007(); virtual void m008(); virtual void m009(); virtual void m010(); virtual void m011(); virtual void m012();
    virtual void m013(); virtual void m014(); virtual void m015(); virtual void m016(); virtual void m017(); virtual void m018();
    virtual void m019(); virtual void m020(); virtual void m021(); virtual void m022(); virtual void m023(); virtual void m024();
    virtual void m025(); virtual void m026(); virtual void m027(); virtual void m028();
    virtual void ShowMousePointer();                           // slot 29 (+0x74)
    virtual void m030(); virtual void m031(); virtual void m032(); virtual void m033(); virtual void m034(); virtual void m035();
    virtual void m036();
    virtual void PopModal();                                   // slot 37 (+0x94)
    virtual void m038(); virtual void m039();
    virtual void SetCurrentStyle(void* style, bool b);         // slot 40 (+0xA0)
    virtual void m041();
    virtual CStyleDefPointer GetStyleDef();                    // slot 42 (+0xA8)
    static CManager* __fastcall GetInstance();          // 0x0041E5F2
};
}
struct CComponentVector {
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
    bool          UseAlternateVolumes;                  // +0x0C
    unsigned char _pad0d[0x24 - 0x0D];
    float         MusicVolume;                          // +0x24
    unsigned char _pad28[0x2C - 0x28];
    float         SoundVolume;                          // +0x2C
    unsigned char _pad30[0xE4 - 0x30];
    float         AlternateMusicVolume;                 // +0xE4
    unsigned char _padE8[0xEC - 0xE8];
    float         AlternateSoundVolume;                 // +0xEC
    float GetMusicVolume() const { return UseAlternateVolumes ? AlternateMusicVolume : MusicVolume; }
    float GetSoundVolume() const { return UseAlternateVolumes ? AlternateSoundVolume : SoundVolume; }
    static CUserProfileManager& __fastcall Get();       // 0x0040D2A0
};
struct CSoundSystem {
    virtual void s000(); virtual void s001(); virtual void s002(); virtual void s003(); virtual void s004(); virtual void s005();
    virtual void s006(); virtual void s007(); virtual void s008(); virtual void s009(); virtual void s010(); virtual void s011();
    virtual void s012(); virtual void s013(); virtual void s014(); virtual void s015(); virtual void s016(); virtual void s017();
    virtual void s018(); virtual void s019(); virtual void s020(); virtual void s021(); virtual void s022(); virtual void s023();
    virtual void s024(); virtual void s025(); virtual void s026(); virtual void s027(); virtual void s028(); virtual void s029();
    virtual void s030(); virtual void s031(); virtual void s032(); virtual void s033(); virtual void s034(); virtual void s035();
    virtual void s036(); virtual void s037(); virtual void s038(); virtual void s039(); virtual void s040(); virtual void s041();
    virtual void s042(); virtual void s043(); virtual void s044(); virtual void s045();
    virtual void SetGainFader(long groupMask, float volume, float fadeTime); // slot 46 (+0xB8)
};
class CASoundManager;
extern CASoundManager* PSoundManager;                    // 0x013B8394
struct CRenderer {
    virtual void r000(); virtual void r001(); virtual void r002(); virtual void r003(); virtual void r004(); virtual void r005();
    virtual void r006(); virtual void r007(); virtual void r008(); virtual void r009(); virtual void r010(); virtual void r011();
    virtual void r012(); virtual void r013(); virtual void r014(); virtual void r015(); virtual void r016(); virtual void r017();
    virtual void r018(); virtual void r019(); virtual void r020(); virtual void r021(); virtual void r022(); virtual void r023();
    virtual void r024(); virtual void r025(); virtual void r026(); virtual void r027(); virtual void r028(); virtual void r029();
    virtual void r030(); virtual void r031(); virtual void r032(); virtual void r033(); virtual void r034(); virtual void r035();
    virtual void r036(); virtual void r037(); virtual void r038(); virtual void r039(); virtual void r040(); virtual void r041();
    virtual void r042(); virtual void r043(); virtual void r044(); virtual void r045(); virtual void r046(); virtual void r047();
    virtual void r048(); virtual void r049(); virtual void r050(); virtual void r051(); virtual void r052(); virtual void r053();
    virtual void r054(); virtual void r055();
    virtual void SetPaused(bool a, bool b);                    // slot 56 (+0xE0)
    virtual void r057(); virtual void r058(); virtual void r059(); virtual void r060(); virtual void r061(); virtual void r062();
    virtual void r063(); virtual void r064(); virtual void r065(); virtual void r066(); virtual void r067(); virtual void r068();
    virtual void r069(); virtual void r070(); virtual void r071(); virtual void r072(); virtual void r073(); virtual void r074();
    virtual void r075();
    virtual void SetEnabled(bool enabled);                     // slot 76 (+0x130)
};
class CDisplayEngine { public:
    unsigned char _pad00[0x2C];
    CRenderer*    PRenderer;                            // +0x2C
};
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char   _pad00[0x28];
    CDisplayEngine* PDisplayEngine;                     // +0x28
};
struct CInventoryCamera {
    void Reset();                                       // 0x004855A3
};
extern CInventoryCamera g_InventoryCamera;              // 0x013B8A98
class CWorld { public:
    unsigned char _pad00[0x8c];
    long LastInventoryCloseFrame;                       // +0x8c
public:
    static long __fastcall GetFrame();   // 0x0049D870
};
void __fastcall RestoreCursor();                        // 0x0041D165
struct CTextInput {
    void Clear();                                       // 0x00593235
    unsigned char _pad[0x0c];
};
// Layout: retail CTCInventory Components +0x0C, handle vectors +0x178 / +0x184, flag +0x1CD, text inputs +0x1FC / +0x208.
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

class CTCInventory : public NInventory::CTCInventoryBase { public:
    void*            __vftable;                         // +0x000
    CThing*          ParentThing;                       // +0x004
    unsigned char    _pad08[0x0c - 0x08];
    CComponentVector Components;                        // +0x00c
    unsigned char    _pad18[0x178 - 0x18];
    CHandleVector    HandlesA;                          // +0x178
    CHandleVector    HandlesB;                          // +0x184
    unsigned char    _pad190[0x1cd - 0x190];
    bool             InventoryOpen;                     // +0x1cd
    unsigned char    _pad1ce[0x1fc - 0x1ce];
    CTextInput       InputA;                            // +0x1fc
    CTextInput       InputB;                            // +0x208
    void ClearQuestPersistentFlags();                   // 0x005BD597
    void SetEditBoxesActive(bool active);               // 0x00581A14
    void CloseMenus();                                  // 0x0058771C
    void CloseInventory();
};

void CTCInventory::CloseInventory()
{
    ClearQuestPersistentFlags();
    SetEditBoxesActive(false);
    InventoryOpen = false;
    CloseMenus();
    for_each(HandlesA._M_start, HandlesA._M_finish, CMemFunRefClear(&CEnginePrimitiveHandle::Clear));
    for_each(HandlesB._M_start, HandlesB._M_finish, CMemFunRefClear(&CEnginePrimitiveHandle::Clear));
    for (unsigned long i = 0; i < Components.size(); ++i)
        delete Components[i];
    Components.clear();
    NUISystem::CManager::GetInstance()->PopModal();
    NUISystem::CManager::GetInstance()->ReleaseFocus();
    NUISystem::CManager::GetInstance()->ShowMousePointer();
    g_InventoryCamera.Reset();
    {
    CTCActiveMode* a = 0;
    CTCActiveMode* b = 0;
    CTCActiveMode* c = 0;
    CTCActiveMode* d = 0;
    CTCActiveMode* e = 0;
    ParentThing->GetTC(TCI_INVENTORY_WEAPONS, &a);
    CTCActiveMode* unused;
    ParentThing->GetTC(TCI_INVENTORY, &unused);
    ParentThing->GetTC(TCI_INVENTORY_MAP, &b);
    ParentThing->GetTC(TCI_HERO_ABILITIES, &c);
    ParentThing->GetTC(TCI_INVENTORY_STATS, &d);
    ParentThing->GetTC(TCI_INVENTORY_CLOTHING, &e);
    if (a && a->Active)
        a->Deactivate();
    if (b && b->Active)
        b->Deactivate();
    if (c && c->Active)
        c->Deactivate();
    if (d && d->Active)
        d->Deactivate();
    if (e && e->Active)
        e->Deactivate();
    }
    InputA.Clear();
    InputB.Clear();
    CMainGameComponent::Get()->PDisplayEngine->PRenderer->SetEnabled(true);
    CWorld* world = (&GetWorld());
    world->LastInventoryCloseFrame = CWorld::GetFrame();
    if (((CSoundSystem*)PSoundManager)) {
        ((CSoundSystem*)PSoundManager)->SetGainFader(0x10, CUserProfileManager::Get().GetMusicVolume(), 0.0f);
        ((CSoundSystem*)PSoundManager)->SetGainFader(0x40, CUserProfileManager::Get().GetSoundVolume(), 0.0f);
        ((CSoundSystem*)PSoundManager)->SetGainFader(0x80, CUserProfileManager::Get().GetMusicVolume(), 0.0f);
    }
    NUISystem::CManager::GetInstance()->SetCurrentStyle(NUISystem::CManager::GetInstance()->GetStyleDef()->Style, true);
    CMainGameComponent::Get()->PDisplayEngine->PRenderer->SetPaused(false, false);
    RestoreCursor();
}