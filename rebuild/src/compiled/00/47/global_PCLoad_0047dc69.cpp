// PCLoad() -- the PC "load this save?" Yes callback: release the yes/no dialog, the mouse pointer and both buttons from the UI manager, give input back to the pointer and arm the hero's in-game menu to load the chosen save
// Leaf: ?PCLoad@@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047DC69, 308 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`, as the landed CTCInGameMenu::CloseInGameMenuPC 0x0047DBB8), `ret`.
// Callees: NUISystem::CManager::GetInstance() @ 0x0041E5F2 (x5), CCharString::CCharString(const char*, long) @ 0x0099EBF0
//          (built in the by-value argument slot), CPlayerManager::GetMainPlayer() @ 0x00449970, CPlayer::GetPControlledCreature()
//          @ 0x00487DC0 (manifest labels on both are propagated junk), std::bitset::test @ 0x00410DE0, CVectorMap<ETCInterfaceType,
//          CTCBase*>::LowerBound @ 0x0040F020; the dialog's CComponent slot 3 (+0x0C, find child by name), the manager's
//          CObservable slots 10 (+0x28) and 2 (+0x08), the pointer's CObserver slot 3 (+0x0C, event 0x19).
// Globals: the yes/no dialog 0x013B87A8 and the mouse pointer 0x013B87A4 (file statics of the in-game-menu TU, shared
//          with ConfirmedVideoOptionsChanged 0x0047D012 and PCSetToLoad 0x0047DD9D), g_pMainGameComponent 0x013B86A0.
// Layout: retail CTCInGameMenu (12-byte containers; PDB -0x20 past the eight vectors): LoadGameIndex +0x64 (PDB +0x78),
//          SaveGameToLoad +0xB8 (PDB +0xD8). TC interface 0xC6 is the in-game menu (as SavedConfirmed 0x0047DB73).
// Shape: each global goes to the manager as its CObserver sub-object (`neg/sbb/and` converting null test); the dialog's
//          vtable is read before the by-value name is built; the menu pointer is zeroed BEFORE the hero fetch (`xor ebx,ebx`
//          ahead of GetMainPlayer; declared after it, the zero lands just before the bitset test) and is dereferenced
//          without a null test, as retail does.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length);      // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();
};

namespace NUISystem {
class CObserver { public:                                   // the observer interface at +4 of every component
    virtual void o00(); virtual void o01(); virtual void o02();
    virtual void ObserveEvent(long eventType);       // slot 3 (+0x0C)
};
struct CComponentBase {                              // the primary vtable
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual class CComponent* FindChild(CCharString name);   // slot 3 (+0x0C)
};
class CComponent : public CComponentBase, public CObserver { public:
};
class CYesNo : public CComponent { public:
};
class CManager { public:                                    // NUISystem::CManager (CObservable vtable part)
    virtual void m00(); virtual void m01();
    virtual void AddObserver(CObserver* observer);   // slot 2 (+0x08)
    virtual void m03(); virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07(); virtual void m08();
    virtual void m09();
    virtual void ReleaseObserver_0055ca00(CObserver* observer);   // slot 10 (+0x28)
    static CManager* __fastcall GetInstance();       // 0x0041E5F2
};
}

class CTCBase;

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);            // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad1[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    bool HasInterface(long type) { return m_present.test(type); }
    template <class T> __forceinline T* GetInterfaceUnchecked(long type)
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(key);
        if (it == map->last || type < it->first)
            it = map->last;
        return (T*)it->second;
    }
};
class CThingPlayerCreature : public CThing { };

class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CMainGameComponent { public:
    unsigned char   _pad00[0x1c];
    CPlayerManager* PlayerManager;                   // +0x1c
};
extern CMainGameComponent* g_pMainGameComponent;     // 0x013B86A0

class CTCInGameMenu { public:
    void*         __vftable;                         // +0x00
    unsigned char _pad_0x04[0x60];
    long          LoadGameIndex;                     // +0x64 (PDB +0x78)
    unsigned char _pad_0x68[0x50];
    long          SaveGameToLoad;                    // +0xB8 (PDB +0xD8)
};

enum { TC_IN_GAME_MENU = 0xC6 };

extern NUISystem::CYesNo*     g_pPCYesNoDialog;      // 0x013B87A8
extern NUISystem::CComponent* g_pPCMousePointer;     // 0x013B87A4

void PCLoad()
{
    NUISystem::CManager::GetInstance()->ReleaseObserver_0055ca00(g_pPCYesNoDialog);
    NUISystem::CManager::GetInstance()->ReleaseObserver_0055ca00(g_pPCMousePointer);
    NUISystem::CManager::GetInstance()->AddObserver(g_pPCMousePointer);
    g_pPCMousePointer->ObserveEvent(0x19);
    NUISystem::CComponent* yesButton = g_pPCYesNoDialog->FindChild(CCharString("PC_YESNO_GAME_YESBUTTON", -1));
    if (yesButton)
        NUISystem::CManager::GetInstance()->ReleaseObserver_0055ca00(yesButton);
    NUISystem::CComponent* noButton = g_pPCYesNoDialog->FindChild(CCharString("PC_YESNO_GAME_NOBUTTON", -1));
    if (noButton)
        NUISystem::CManager::GetInstance()->ReleaseObserver_0055ca00(noButton);
    CTCInGameMenu* menu = 0;
    CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (hero->HasInterface(TC_IN_GAME_MENU))
        menu = hero->GetInterfaceUnchecked<CTCInGameMenu>(TC_IN_GAME_MENU);
    menu->LoadGameIndex = menu->SaveGameToLoad;
}