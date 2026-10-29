// CWorld::UpdateSaveGameLoad() -- step the queued save-game load: fade the screen out and pause, wait for the fade, load the save (and its hero state), then show the region name, restore the display, sounds and pause state
// Leaf: ?UpdateSaveGameLoad@CWorld@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004A2F10, 736 bytes + the 4-entry jump table 0x004A31F0 (catalog row 752); Ego_r.pdb
//   `?UpdateSaveGameLoad@CWorld@@IAEXXZ` (protected). States (SaveGameLoadStatus, set to 1 by LoadGameState 0x004A3200):
//   1 start the fade-out and pause, 2 wait for the fade, 3 load, 4 finish; anything else returns.
// Callees: CDisplayEngine::StartScreenFadeOut(float, float, CRGBColour, float) 0x00434C00, IsScreenFullyFadedOut() const
//   0x004A9210, EndScreenFadeOut() 0x00434C90; CWideString() 0x0099AED0 / ~CWideString 0x0099B510; CAFile::SplitPath
//   0x00997620 (__fastcall static); operator+(const CWideString&, const CWideString&) 0x0099BE70 (__fastcall);
//   CWorld::IsHeroStateFileValid 0x0049D4E0, LoadGameStateInternal 0x004A21F0, LoadHeroStateInternal 0x004A2D70 (all
//   protected); CIEngine::StartNewScene(unsigned long, float) virtual slot 55 (+0xDC; RTTI vtables CIEngine 0x012A0D24 and
//   CEngine 0x012A0F3C hold ?StartNewScene@ at slot 55); CWorld::PeekWorldMap() const virtual slot 13 (+0x34);
//   CWorldMap::GetLoadedRegion() const 0x004FB150, GetRegion(long) const 0x004FC180; CRegion::GetDisplayName() const
//   0x006BBF20; CPlayerManager::GetMainPlayer() 0x00449970; CPlayer::GetPControlledCreature() const 0x00487DC0;
//   CVectorMap<ETCInterfaceType, CTCBase*, ...>::LowerBound 0x0040F020; CTCRegionDisplay::SetCurrentAreaName 0x00639BA0;
//   NProgressDisplay::StopProgress(bool, bool) 0x009E9F20 (__fastcall); CUserProfileManager::Get 0x0040D2A0;
//   the sound manager's SetGroupVolume (slot 46, +0xB8) on the object at 0x013B8394.
// Inlined: CWorld::SetAsPaused(bool) (Ego_r.pdb `?SetAsPaused@CWorld@@IAEX_N@Z`: store only on change),
//   CMainGameComponent::GetDisplayEngine() (scoped_ptr PDisplayEngine +0x28), CThing::GetTC<CTCRegionDisplay> (its
//   out-of-line copy is 0x004ACAB0; interface 0x8E, bit 14 of the bitset word +0x30; the result is used unchecked, so a
//   missing component leaves the out pointer uninitialised as in retail), CUserProfileManager::GetSoundVolume() const /
//   GetDialogueVolume() const (Ego_r.pdb QBEMXZ; UseNewProfileValues +0x0C picks the new-profile copy).
// Globals: 0x013B85F0 is the bool CProgressDisplay::StartProgress 0x00499AA0 copies into DebugDisplay (retail +0x78);
//   0x013B89B8 is the copy this function keeps across the load. Neither has a PDB data symbol: descriptive names.
// Shape: the outgoing colour is built in its argument slot through CRGBColour's user copy constructor (as the landed
//   CTCWife::HaveSex 0x006CF920 model; without it the colour goes through a frame temporary). VC7.1 declines to inline
//   GetTC here unless it is __forceinline (as the landed CThingSwitch::TriggerSetPlayerAreaName 0x0083C990 model); not
//   inlined, the frame loses ebp and the register schedule shifts.
// Layout: retail CWorld (engine/CWorld.h offsets): Component +0x08, PlayerManager +0x0C, Paused +0xA4, SaveGameLoadStatus
//   +0xF8, SaveGamePathName +0xFC, SerialisingAboutToLoadHeroState +0x101, DisplayMiniMapOnRegionLoad +0x12F. The header
//   flattens the vptr into data, and this body makes a virtual call (PeekWorldMap), so the class is modelled here (header debt).
//   engine/CDisplayEngine.h (Engine3D +0x2C), engine/CRegion.h, engine/CWorldMap.h, engine/CPlayer.h and
//   engine/CPlayerManager.h are used through the rename trick (callee classes: methods only). engine/CIEngine.h also
//   flattens its vptr, so the interface with its slot-55 virtual is declared here (header debt).
#define CDisplayEngine CDisplayEngineLayout
#include "engine/CDisplayEngine.h"
#undef CDisplayEngine
#define CRegion CRegionLayout
#include "engine/CRegion.h"
#undef CRegion
#define CWorldMap CWorldMapLayout
#include "engine/CWorldMap.h"
#undef CWorldMap
#define CPlayer CPlayerLayout
#include "engine/CPlayer.h"
#undef CPlayer
#define CPlayerManager CPlayerManagerLayout
#include "engine/CPlayerManager.h"
#undef CPlayerManager

typedef unsigned short wchar16;
class CCharString;
class CWideStringData;
class CWideString { public:
    CWideStringData* PStringData;
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
};
CWideString __fastcall operator+(const CWideString& left, const CWideString& right);   // 0x0099BE70
class CAFile { public:
    static void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* directory,
                                     CWideString* name, CWideString* extension);   // 0x00997620
};

class CRGBColour { public:                                  // B,G,R,A bytes over a packed 32-bit colour
    union {
        struct { unsigned char B, G, R, A; };
        unsigned long Colour;
    };
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        R = r;
        G = g;
        B = b;
        A = a;
    }
    CRGBColour(const CRGBColour& other) : Colour(other.Colour) { }
};

class CIEngine { public:
    virtual void CUnprovenSlot_00(); virtual void CUnprovenSlot_01(); virtual void CUnprovenSlot_02(); virtual void CUnprovenSlot_03();
    virtual void CUnprovenSlot_04(); virtual void CUnprovenSlot_05(); virtual void CUnprovenSlot_06(); virtual void CUnprovenSlot_07();
    virtual void CUnprovenSlot_08(); virtual void CUnprovenSlot_09(); virtual void CUnprovenSlot_10(); virtual void CUnprovenSlot_11();
    virtual void CUnprovenSlot_12(); virtual void CUnprovenSlot_13(); virtual void CUnprovenSlot_14(); virtual void CUnprovenSlot_15();
    virtual void CUnprovenSlot_16(); virtual void CUnprovenSlot_17(); virtual void CUnprovenSlot_18(); virtual void CUnprovenSlot_19();
    virtual void CUnprovenSlot_20(); virtual void CUnprovenSlot_21(); virtual void CUnprovenSlot_22(); virtual void CUnprovenSlot_23();
    virtual void CUnprovenSlot_24(); virtual void CUnprovenSlot_25(); virtual void CUnprovenSlot_26(); virtual void CUnprovenSlot_27();
    virtual void CUnprovenSlot_28(); virtual void CUnprovenSlot_29(); virtual void CUnprovenSlot_30(); virtual void CUnprovenSlot_31();
    virtual void CUnprovenSlot_32(); virtual void CUnprovenSlot_33(); virtual void CUnprovenSlot_34(); virtual void CUnprovenSlot_35();
    virtual void CUnprovenSlot_36(); virtual void CUnprovenSlot_37(); virtual void CUnprovenSlot_38(); virtual void CUnprovenSlot_39();
    virtual void CUnprovenSlot_40(); virtual void CUnprovenSlot_41(); virtual void CUnprovenSlot_42(); virtual void CUnprovenSlot_43();
    virtual void CUnprovenSlot_44(); virtual void CUnprovenSlot_45(); virtual void CUnprovenSlot_46(); virtual void CUnprovenSlot_47();
    virtual void CUnprovenSlot_48(); virtual void CUnprovenSlot_49(); virtual void CUnprovenSlot_50(); virtual void CUnprovenSlot_51();
    virtual void CUnprovenSlot_52(); virtual void CUnprovenSlot_53(); virtual void CUnprovenSlot_54();
    virtual void StartNewScene(unsigned long scene, float time);   // slot 55 (+0xDC)
};

class CDisplayEngine : public CDisplayEngineLayout { public:
    void StartScreenFadeOut(float fadeOutTime, float fadeInTime, CRGBColour colour, float holdTime);   // 0x00434C00
    void EndScreenFadeOut();                                // 0x00434C90
    bool IsScreenFullyFadedOut() const;                     // 0x004A9210
};

template <class T> class scoped_ptr { public:
    T* ptr;
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
};
class CMainGameComponent { public:
    unsigned char              _pad00[0x28];
    scoped_ptr<CDisplayEngine> PDisplayEngine;              // +0x28
    CDisplayEngine& GetDisplayEngine() { return *PDisplayEngine; }
};

class CRegion : public CRegionLayout { public:
    const CCharString& GetDisplayName() const;              // 0x006BBF20
};
class CWorldMap : public CWorldMapLayout { public:
    long GetLoadedRegion() const;                           // 0x004FB150
    const CRegion& GetRegion(long region) const;            // 0x004FC180
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy {
    _Value _M_data;
};
template <class _Tp, class _Alloc> class vector {           // retail STLport vector (12 bytes)
public:
    typedef _Tp* iterator;
    _Tp* _M_start;                                            // +0x00
    _Tp* _M_finish;                                           // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    iterator end() { return this->_M_finish; }
};
template <unsigned int _Nw> struct _Base_bitset {
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    static unsigned int _S_whichword(unsigned int __pos) { return __pos / 32; }
    static _WordT _S_maskbit(unsigned int __pos) { return 1UL << (__pos % 32); }
};
template <unsigned int _Nb> class bitset : public _Base_bitset<(_Nb + 31) / 32> {
public:
    struct reference {
        typedef _Base_bitset<(_Nb + 31) / 32> _Bitset_base;
        unsigned long* _M_wp;
        unsigned int _M_bpos;
        reference(bitset<_Nb>& __b, unsigned int __pos)
        {
            _M_wp = &__b._M_w[_Bitset_base::_S_whichword(__pos)];
            _M_bpos = __pos % 32;
        }
        ~reference() { }
        operator bool() const { return (*(_M_wp) & _Bitset_base::_S_maskbit(_M_bpos)) != 0; }
    };
    reference operator[](unsigned int __pos) { return reference(*this, __pos); }
};
}   // namespace std

template <class T> class CArray : public std::vector<T, std::allocator<T> > { };
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    CArray<value_type> Data;                                  // +0x00
    TCompare           Compare;                               // +0x0C
    bool               Dirty;                                 // +0x0D
    value_type* LowerBound(const K& key);                     // 0x0040F020 for the TC map
};

enum ETCInterfaceType { ETCInterfaceType_Force32 = 0x7fffffff };   // enumerators not needed
class CTCBase;
class CTCRegionDisplay { public:
    enum { INTERFACE_TYPE = 0x8E };                         // GetInterfaceType 0x0063A1F0
    void SetCurrentAreaName(const CCharString& name, float time, bool a, bool b, bool c);   // 0x00639BA0
};

class CThing {                                              // partial view, retail offsets
public:
    typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > TCMap;
    unsigned char      Header[0x20];                        // CClass / CBaseObject / CBaseClass part
    std::bitset<274>   UsingComponents;                     // +0x20
    TCMap              Components;                          // +0x44
    template <class T> __forceinline bool GetTC(T** out)    // out of line for CTCRegionDisplay at 0x004ACAB0
    {
        if (UsingComponents[T::INTERFACE_TYPE]) {
            ETCInterfaceType type = (ETCInterfaceType)T::INTERFACE_TYPE;
            TCMap* map = &Components;
            TCMap::value_type* it = map->LowerBound(type);
            if (it == map->Data.end() || T::INTERFACE_TYPE < it->first)
                it = map->Data.end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
class CPlayer : public CPlayerLayout { public:
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0
};
class CPlayerManager : public CPlayerManagerLayout { public:
    CPlayer& GetMainPlayer();                               // 0x00449970
};

namespace NProgressDisplay {
void __fastcall StopProgress(bool a, bool b);               // 0x009E9F20
}

class CUserProfileManager { public:
    unsigned char _pad00[0x0C];
    bool          UseNewProfileValues;                      // +0x0C
    unsigned char _pad0D[0x24 - 0x0D];
    float         SoundVolume;                              // +0x24
    float         MusicVolume;                              // +0x28
    float         DialogueVolume;                           // +0x2C
    unsigned char _pad30[0xE4 - 0x30];
    float         SoundVolumeNewProfile;                    // +0xE4
    float         MusicVolumeNewProfile;                    // +0xE8
    float         DialogueVolumeNewProfile;                 // +0xEC
    static CUserProfileManager& __fastcall Get();           // 0x0040D2A0
    float GetSoundVolume() const { return UseNewProfileValues ? SoundVolumeNewProfile : SoundVolume; }
    float GetDialogueVolume() const { return UseNewProfileValues ? DialogueVolumeNewProfile : DialogueVolume; }
};

class CSoundManagerLH { public:                             // the object at 0x013B8394 (vtable 0x0129D2C4)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034();
    virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045();
    virtual void SetGroupVolume(unsigned long groupMask, float volume, float fadeTime);   // slot 46 (+0xB8) 0x00A3E9D0
};
extern CSoundManagerLH* g_pSoundSystem;                     // 0x013B8394

extern bool g_ProgressDebugDisplay;                         // 0x013B85F0
extern bool g_ProgressDebugDisplaySaved;                    // 0x013B89B8

class CWorld { public:
    virtual void CUnprovenSlot_00(); virtual void CUnprovenSlot_01(); virtual void CUnprovenSlot_02(); virtual void CUnprovenSlot_03();
    virtual void CUnprovenSlot_04(); virtual void CUnprovenSlot_05(); virtual void CUnprovenSlot_06(); virtual void CUnprovenSlot_07();
    virtual void CUnprovenSlot_08(); virtual void CUnprovenSlot_09(); virtual void CUnprovenSlot_10(); virtual void CUnprovenSlot_11();
    virtual void CUnprovenSlot_12();
    virtual const CWorldMap& PeekWorldMap() const;           // slot 13 (+0x34) 0x004AE8C0
    unsigned char       _base04[0x4];
    CMainGameComponent* Component;                          // +0x08
    CPlayerManager*     PlayerManager;                      // +0x0C
    unsigned char       _pad10[0xA4 - 0x10];
    bool                Paused;                             // +0xA4
    unsigned char       _padA5[0xF8 - 0xA5];
    long                SaveGameLoadStatus;                 // +0xF8 ESaveGameLoadStatus
    CWideString         SaveGamePathName;                   // +0xFC
    bool                AutoSaveLocked;                     // +0x100
    bool                SerialisingAboutToLoadHeroState;    // +0x101
    unsigned char       _pad102[0x12F - 0x102];
    bool                DisplayMiniMapOnRegionLoad;         // +0x12F
protected:
    bool IsHeroStateFileValid(const CWideString& name) const;          // 0x0049D4E0
    bool LoadGameStateInternal(const CWideString& path, bool b);       // 0x004A21F0
    bool LoadHeroStateInternal(const CWideString& name);               // 0x004A2D70
    void SetAsPaused(bool paused)
    {
        if (Paused != paused)
            Paused = paused;
    }
    void UpdateSaveGameLoad();
};

void CWorld::UpdateSaveGameLoad()
{
    switch (SaveGameLoadStatus) {
    case 1:
        DisplayMiniMapOnRegionLoad = false;
        Component->GetDisplayEngine().StartScreenFadeOut(1.0f, 1.0f, CRGBColour(0, 0, 0, 255), -1.0f);
        SetAsPaused(true);
        SaveGameLoadStatus = 2;
        break;
    case 2:
        if (Component->GetDisplayEngine().IsScreenFullyFadedOut()) {
            g_ProgressDebugDisplaySaved = g_ProgressDebugDisplay;
            g_ProgressDebugDisplay = false;
            SaveGameLoadStatus = 3;
        }
        break;
    case 3: {
        CWideString name;
        CWideString extension;
        CAFile::SplitPath(SaveGamePathName, 0, 0, &name, &extension);
        SerialisingAboutToLoadHeroState = IsHeroStateFileValid(name + extension);
        bool loaded = LoadGameStateInternal(SaveGamePathName, true);
        SerialisingAboutToLoadHeroState = false;
        if (loaded)
            LoadHeroStateInternal(name + extension);
        Component->GetDisplayEngine().Engine3D->StartNewScene(0, 20.0f);
        SaveGameLoadStatus = 4;
        break;
    }
    case 4: {
        const CRegion& region = PeekWorldMap().GetRegion(PeekWorldMap().GetLoadedRegion());
        CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
        CTCRegionDisplay* regionDisplay;
        hero->GetTC(&regionDisplay);
        regionDisplay->SetCurrentAreaName(region.GetDisplayName(), -1.0f, false, false, true);
        NProgressDisplay::StopProgress(true, false);
        g_ProgressDebugDisplay = g_ProgressDebugDisplaySaved;
        Component->GetDisplayEngine().EndScreenFadeOut();
        DisplayMiniMapOnRegionLoad = true;
        if (g_pSoundSystem) {
            g_pSoundSystem->SetGroupVolume(0x10, CUserProfileManager::Get().GetSoundVolume(), 0.0f);
            g_pSoundSystem->SetGroupVolume(0x40, CUserProfileManager::Get().GetDialogueVolume(), 0.0f);
            g_pSoundSystem->SetGroupVolume(0x80, CUserProfileManager::Get().GetSoundVolume(), 0.0f);
        }
        SetAsPaused(false);
        SaveGameLoadStatus = 0;
        break;
    }
    }
}