// CTCInventory::ClosePCInventory(bool closeInGameMenu) -- leave the PC inventory screens: cancel the pending UI action,
// Leaf: ?ClosePCInventory@CTCInventory@@QAEX_N@Z
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Naming wave 18: the global stand-in g_pEngine (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// close the edit boxes and the menu, close the in-game menu, stop the inventory effect, destroy the PC screens, reset the
// state, release the UI manager's modal state, restore the three audio group volumes from the user profile, clear the
// quest-persistent flags and restart game rendering
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00587847, 506 bytes, `ret 4` (frame pointer, `leave`).
// Sound manager (agentSND2, 2026-11-10): PSoundManager is the shared engine/CASoundManager.h one; the local CSoundSystem model is gone and its slots carry the header names (SetGainFader slot 46).
// Shape: the two discarded `HasTC(t) ? FindTCInline(t) : 0` component fetches (0x13, 0x12) keep the parent thing loaded in
// eax for `lea ecx,[eax+20h]` (a bare HasTC would fold to `mov ecx; add ecx,20h`); GetInterface one-ternary for 0xc6.
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)

class CTCBase;
namespace std {                                                       // STLport 4.6 <bitset>: _Base_bitset<9> words + bitset<274>
template <unsigned int _Nb> class bitset { public:
    // the row lays the 9 words (36 bytes) out itself: the stand-in was an empty class
    bool test(unsigned int __pos) const;                            // 0x00410DE0 ?test@?$bitset@$0BBC@@std@@QBE_NI@Z
};
}
// agentNF15: the stand-in CInterfaceMap struct is the explicit specialisation of CVectorMap<ETCInterfaceType, CTCBase*> (the landed
//   0x0074EE90 idiom); 0x0040F020 is its LowerBound (NF14 / established spelling).
enum ETCInterfaceType { };
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class C> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> value_type;
    value_type* _M_start;                                   // +0x00 (the STLport vector)
    value_type* _M_finish;                                  // +0x04
    value_type* LowerBound(const ETCInterfaceType& key);    // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::value_type* CInterfaceMap::LowerBound(const ETCInterfaceType& key)
{
    value_type* it = _M_start;
    long count = _M_finish - _M_start;
    while (count > 0) {
        long half = count / 2;
        if (it[half].first < key) {
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
    std::bitset<274> present;                                  // +0x20 std::bitset of interface types
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap m_interfaces;                             // +0x44
    FABLE_FORCEINLINE bool HasTC(long type) { return present.test(type); }
    // Inline component find (lower bound + not-found -> end), the shape of a fully-inlined GetTC: with its result unused
    // VC7.1 deletes the search but keeps the thing pointer it loaded for it (`mov eax,[edi+4]` / `lea ecx,[eax+20h]`).
    CTCBase* FindTCInline(long type)
    {
        CInterfaceMap::value_type* it = m_interfaces._M_start;
        long count = m_interfaces._M_finish - m_interfaces._M_start;
        while (count > 0) {
            long half = count / 2;
            if (it[half].first < type) { it += half + 1; count -= half + 1; } else count = half;
        }
        if (it == m_interfaces._M_finish || type < it->first) it = m_interfaces._M_finish;
        return it->second;
    }
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (present.test(interfaceType)) {
            ETCInterfaceType type = (ETCInterfaceType)interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            *ppOut = (T*)((it == map->_M_finish || interfaceType < it->first) ? map->_M_finish : it)->second;
            return true;
        }
        return false;
    }
    template<class T> FABLE_FORCEINLINE T* FindTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        if (it == map->_M_finish || interfaceType < it->first)
            it = map->_M_finish;
        return (T*)it->second;
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
#include "engine/CASoundManager.h"
struct CRenderer {
    virtual void r00(); virtual void r01(); virtual void r02(); virtual void r03(); virtual void r04(); virtual void r05(); virtual void r06(); virtual void r07(); virtual void r08(); virtual void r09(); virtual void r10(); virtual void r11(); virtual void r12(); virtual void r13(); virtual void r14(); virtual void r15(); virtual void r16(); virtual void r17(); virtual void r18(); virtual void r19(); virtual void r20(); virtual void r21(); virtual void r22(); virtual void r23(); virtual void r24(); virtual void r25(); virtual void r26(); virtual void r27(); virtual void r28(); virtual void r29(); virtual void r30(); virtual void r31(); virtual void r32(); virtual void r33(); virtual void r34(); virtual void r35(); virtual void r36(); virtual void r37(); virtual void r38(); virtual void r39(); virtual void r40(); virtual void r41(); virtual void r42(); virtual void r43(); virtual void r44(); virtual void r45(); virtual void r46(); virtual void r47(); virtual void r48(); virtual void r49(); virtual void r50(); virtual void r51(); virtual void r52(); virtual void r53(); virtual void r54(); virtual void r55(); 
    virtual void SetPaused_slot56(bool a, bool b);          // slot 56 (+0xE0)
};
class CDisplayEngine { public:
    unsigned char _pad00[0x2C];
    CRenderer*    PRenderer;                                // +0x2C
};
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char   _pad00[0x28];
    CDisplayEngine* PDisplayEngine;                         // +0x28
};
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
    if (PSoundManager) {
        PSoundManager->SetGainFader(0x10, CUserProfileManager::Get().GetMusicVolume(), 0.0f);
        PSoundManager->SetGainFader(0x40, CUserProfileManager::Get().GetSoundVolume(), 0.0f);
        PSoundManager->SetGainFader(0x80, CUserProfileManager::Get().GetMusicVolume(), 0.0f);
    }
    ClearQuestPersistentFlags();
    StartGameRendering();
    CMainGameComponent::Get()->PDisplayEngine->PRenderer->SetPaused_slot56(false, false);
}