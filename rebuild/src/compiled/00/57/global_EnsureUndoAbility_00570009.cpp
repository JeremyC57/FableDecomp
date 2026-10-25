// EnsureUndoAbility() -- the "yes" answer of the experience screen's undo-ability prompt: if the main player's character is alive, undo the pending ability purchase and hand the mouse pointer back from the yes/no dialog, then drop the dialog's yes/no buttons from the UI manager and send the close event
// Leaf: ?EnsureUndoAbility@@YIXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00570009, 344 bytes (0x158, no padding: /Os TU), `void __fastcall EnsureUndoAbility()`
//   (Ego_r.pdb ?EnsureUndoAbility@@YIXXZ, a free yes/no callback). CTCInventoryExperience::UndoBuyAbility (0x00576C4F)
//   installs it (`push 0x00570009` into the dialog's slot 142) after parking the ability in 0x013B8B08. Frame-pointer
//   size TU: `#pragma optimize("y", off)` + `("s", on)`, as the landed neighbour YesCloseInventory 0x0056FBED.
// Callees: CPlayerManager::GetMainPlayer() const 0x00449960 (the main game component 0x013B86A0 +0x1C),
//   CPlayer::GetPlayerCharacter 0x00487B70, IS_THING_ALIVE<CThing*> 0x0040DCE0 (__fastcall, by reference),
//   std::bitset::test 0x00410DE0 + CVectorMap::LowerBound 0x0040F020 (the inline CThing::GetInterface for interface
//   0x16, the experience inventory), CTCInventoryExperience::UndoBuyAbility(EHeroAbility, bool) 0x00576C4F,
//   NUISystem::CManager::GetInstance 0x0041E5F2 (six times) with its ReleaseObserver (slot 10), AddObserver (slot 2)
//   and SendEvent (slot 0), the observer interface's ObserveEvent (observer slot 3), the dialog's FindChild (slot 3);
//   CCharString(const char*, long) 0x0099EBF0 ("PC_YESNO_GAME_YESBUTTON" 0x01236734, "PC_YESNO_GAME_NOBUTTON"
//   0x0123671C, built in the by-value argument slot). Globals: the parked ability 0x013B8B08, the yes/no dialog
//   0x013B8B04 and the mouse pointer 0x013B8B00 (UndoBuyAbility stores the experience screen's +0x1C4 / +0x258 there).
// Shape: the YesCloseInventory prologue (the uninitialised out local, the hero and the interface key share [ebp-4]);
//   the CComponent* -> CObserver* (+4) upcasts of the globals are null-tested (`neg/sbb/and`), the found buttons are
//   not (already tested), and ObserveEvent through the component pointer adjusts `this` without a test.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    CCharString(const CCharString& other);            // (declared: by-value arguments are built in place)
    ~CCharString();                                   // 0x0099EAE0
};

namespace NUISystem {
struct CObserver {                                   // the observer interface at +4 of every component
    virtual void o00(); virtual void o01(); virtual void o02();
    virtual void ObserveEvent(long eventType);       // slot 3 (+0x0C)
};
struct CComponentBase {                              // the primary vtable
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual struct CComponent* FindChild(CCharString name);   // slot 3 (+0x0C)
};
struct CComponent : CComponentBase, CObserver {
};
struct CManager {                                    // NUISystem::CManager (CObservable vtable part)
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

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    __declspec(noinline) SEntry* LowerBound(const long* type)   // 0x0040F020 (same-TU read-only stand-in)
    {
        SEntry* lo = first;
        int len = last - first;
        while (len > 0) {
            int half = len >> 1;
            SEntry* middle = lo + half;
            if (middle->type < *type) {
                lo = middle + 1;
                len = len - half - 1;
            }
            else
                len = half;
        }
        return lo;
    }
};
class CThing {
public:
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    unsigned char _pad_0x4c[0x91 - 0x4c];
    unsigned char DeadFlags;                         // +0x91 (bit 0: dead; retail)
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* found = map->LowerBound(&type);
            CInterfaceMap::SEntry* it;
            if (found != map->last && !(wanted < found->type))
                it = found;
            else
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CPlayer {
public:
    CThing* GetPlayerCharacter();                    // 0x00487B70
};
class CPlayerManager {
public:
    CPlayer* GetMainPlayer() const;                  // 0x00449960
};
struct CMainGameComponent {
    unsigned char _pad_0x00[0x1c];
    CPlayerManager* PlayerManager;                   // +0x1c
};
extern CMainGameComponent* g_pMainGameComponent;     // 0x013B86A0

// 0x0040DCE0 (COMDAT-folded IS_THING_ALIVE<T*> instantiations): the header template, visible here and out of line
// as in retail, so VC7.1 knows the hero is only read through the reference.
template<class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return thing != 0 && (thing->DeadFlags & 1) == 0;
}

enum EHeroAbility { HERO_ABILITY_NONE };

class CTCInventoryExperience {                       // interface type 0x16
public:
    void UndoBuyAbility(EHeroAbility ability, bool confirmed);   // 0x00576C4F
};

extern EHeroAbility            g_UndoAbility;        // 0x013B8B08 (the ability parked by UndoBuyAbility)
extern NUISystem::CComponent*  g_pUndoYesNoDialog;   // 0x013B8B04
extern NUISystem::CComponent*  g_pUndoMousePointer;  // 0x013B8B00

void __fastcall EnsureUndoAbility()
{
    CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetPlayerCharacter();
    if (IS_THING_ALIVE(hero)) {
        CTCInventoryExperience* inventory;
        hero->GetInterface(&inventory, 0x16);
        inventory->UndoBuyAbility(g_UndoAbility, true);
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