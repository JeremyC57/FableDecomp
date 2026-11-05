// EnsureUndoStat() -- the "yes" answer of the experience screen's undo-stat prompt: if the main player's character is alive, undo the pending stat purchase and hand the mouse pointer back from the yes/no dialog, then drop the dialog's yes/no buttons from the UI manager and send the close event
// Naming wave 35c (queue land0411j 0x00487B70): calls 0x00487B70 as CPlayer::GetPPlayerCharacterThing() const (Ego_r.pdb ?GetPPlayerCharacterThing@CPlayer@@QBEPAVCThing@@XZ; was GetPlayerCharacter).
// Leaf: ?EnsureUndoStat@@YIXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave (stand-in map): the local stand-in map class (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad_0x21[0x23] removed. Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x005701DA, 344 bytes (0x158, no padding: /Os TU), `void __fastcall EnsureUndoStat()`
//   (Ego_r.pdb ?EnsureUndoStat@@YIXXZ, a free yes/no callback; the catalog's second "EnsureUndoAbility" is wrong).
//   CTCInventoryExperience::UndoBuyStat (0x00576ED1) installs it after parking the stat in 0x013B8AFC. Twin of
//   EnsureUndoAbility 0x00570009. Frame-pointer
//   size TU: `#pragma optimize("y", off)` + `("s", on)`, as the landed neighbour YesCloseInventory 0x0056FBED.
// Callees: CPlayerManager::GetMainPlayer() const 0x00449960 (the main game component 0x013B86A0 +0x1C),
//   CPlayer::GetPPlayerCharacterThing() const 0x00487B70, IS_THING_ALIVE<CThing*> 0x0040DCE0 (__fastcall, by reference),
//   std::bitset::test 0x00410DE0 + CVectorMap::LowerBound 0x0040F020 (the inline CThing::GetInterface for interface
//   0x16, the experience inventory), CTCInventoryExperience::UndoBuyStat(EHeroTrainableStatType, bool) 0x00576ED1,
//   NUISystem::CManager::GetInstance 0x0041E5F2 (six times) with its ReleaseObserver (slot 10), AddObserver (slot 2)
//   and SendEvent (slot 0), the observer interface's ObserveEvent (observer slot 3), the dialog's FindChild (slot 3);
//   CCharString(const char*, long) 0x0099EBF0 ("PC_YESNO_GAME_YESBUTTON" 0x01236734, "PC_YESNO_GAME_NOBUTTON"
//   0x0123671C, built in the by-value argument slot). Globals: the parked stat 0x013B8AFC, the yes/no dialog
//   0x013B8B04 and the mouse pointer 0x013B8B00 (UndoBuyStat stores the experience screen's +0x1C4 / +0x258 there).
// Shape: the YesCloseInventory prologue (the uninitialised out local, the hero and the interface key share [ebp-4]);
//   the CComponent* -> CObserver* (+4) upcasts of the globals are null-tested (`neg/sbb/and`), the found buttons are
//   not (already tested), and ObserveEvent through the component pointer adjusts `this` without a test.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CCharStringData;
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"

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
class CManager { public:                                    // NUISystem::CManager (CObservable vtable part)
    virtual void SendEvent(long event);              // slot 0 (+0x00)
    virtual void m01();
    virtual void AddObserver(CObserver* observer);   // slot 2 (+0x08)
    virtual void m03(); virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07(); virtual void m08();
    virtual void m09();
    virtual void ReleaseObserver(CObserver* observer);   // slot 10 (+0x28)
    static CManager* __fastcall GetInstance();       // 0x0041E5F2
};
}

class CThing;

#include "engine/CVectorMap.h"

class CTCBase;
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CPlayer {
public:
    CThing* GetPPlayerCharacterThing() const;      // 0x00487B70
};
class CPlayerManager {
public:
    CPlayer* GetMainPlayer() const;                  // 0x00449960
};
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char _pad_0x00[0x1c];
    CPlayerManager* PlayerManager;                   // +0x1c
};

// 0x0040DCE0 (COMDAT-folded IS_THING_ALIVE<T*> instantiations): the header template, visible here and out of line
// as in retail, so VC7.1 knows the hero is only read through the reference.
template<class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return thing != 0 && thing->IsDead() == 0;
}

enum EHeroTrainableStatType { HERO_TRAINABLE_STAT_NONE };

class CTCInventoryExperience {                       // interface type 0x16
public:
    void UndoBuyStat(EHeroTrainableStatType stat, bool confirmed);   // 0x00576ED1
};

extern EHeroTrainableStatType  g_UndoStat;           // 0x013B8AFC (the stat parked by UndoBuyStat)
extern NUISystem::CComponent*  g_pUndoYesNoDialog;   // 0x013B8B04
extern NUISystem::CComponent*  g_pUndoMousePointer;  // 0x013B8B00

void __fastcall EnsureUndoStat()
{
    CThing* hero = CMainGameComponent::Get()->PlayerManager->GetMainPlayer()->GetPPlayerCharacterThing();
    if (IS_THING_ALIVE(hero)) {
        CTCInventoryExperience* inventory;
        hero->GetTC(TCI_INVENTORY_EXPERIENCE, &inventory);
        inventory->UndoBuyStat(g_UndoStat, true);
        NUISystem::CManager::GetInstance()->ReleaseObserver(g_pUndoYesNoDialog);
        NUISystem::CManager::GetInstance()->ReleaseObserver(g_pUndoMousePointer);
        NUISystem::CManager::GetInstance()->AddObserver(g_pUndoMousePointer);
        g_pUndoMousePointer->ObserveEvent(0x19);
    }
    NUISystem::CComponent* yesButton = g_pUndoYesNoDialog->FindChild(CCharString("PC_YESNO_GAME_YESBUTTON"));
    if (yesButton)
        NUISystem::CManager::GetInstance()->ReleaseObserver(yesButton);
    NUISystem::CComponent* noButton = g_pUndoYesNoDialog->FindChild(CCharString("PC_YESNO_GAME_NOBUTTON"));
    if (noButton)
        NUISystem::CManager::GetInstance()->ReleaseObserver(noButton);
    NUISystem::CManager::GetInstance()->SendEvent(2);
}