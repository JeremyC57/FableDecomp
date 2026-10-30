// CV_TempleOfLightScript::WatchForWeaponUnsheathed() -- track the hero drawing a weapon inside the Temple; the first time (after the intro) the sound altar warns him with a one-line conversation
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EDC010, 447 bytes (448 with padding), plain `ret`.
// Twin of the Chapel's 0x00E6A340 (catalog labels the two classes swapped).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptThing::IsAlive 0x004AB130 (out of
//   line here), ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC, ~CScriptThing 0x004AA840 (out of line),
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940; through the interface (+0x40) NewScriptFrame (slot 7), GetHero
//   (slot 70), GetThingWithScriptName (slot 72, hidden result), IsPlayerWieldingWeapon (slot 195), AddNewConversation
//   (slot 364), AddPersonToConversation (slot 365), AddLineToConversation (slot 366).
// Strings: "M_SoundAltar" 0x012E8324, "TEXT_QST_064_WEAPON_01" 0x012F3E7C.
// Shape: an `if (!terminating) { do { ... } while (!terminating); }` watch loop; the altar handle's inline destructor is expanded once at the loop exit
//   (zero kept in ebx), the out-of-line ~CScriptThing called at the in-loop returns.
// Layout (retail = Ego_r.pdb - 0x14): WeaponWarningComplete +0x57, WeaponUnsheathed +0x58, TempleIntroComplete +0x59,
//   InsideTemple +0x5a; interface pointer +0x40.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                     // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);              // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CScriptThingImpl;
class CScriptThing : public CBaseClass { public:                          // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThingImpl> PImp;                 // +0x04 Data, +0x08 Info
    CScriptThing(const CScriptThing& other);                // 0x004ABE90
    ~CScriptThing() {}                                      // out of line 0x004AA840
public:
    virtual bool IsAlive() const;   // 0x004AB130
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;                             // +0x40
    void*                 Data;                             // +0x44
};
namespace NScript {
class CV_TempleOfLightScript : public CQuestScriptBase {
public:
    long DonationTotal;                                     // +0x48
    long DonationThisVisit;                                 // +0x4c
    long DonationCounter;                                   // +0x50
    bool DonationInvalid;                                   // +0x54
    bool DonationActive;                                    // +0x55
    bool DisplayQuestInfo;                                  // +0x56
    bool WeaponWarningComplete;                             // +0x57
    bool WeaponUnsheathed;                                  // +0x58
    bool TempleIntroComplete;                               // +0x59
    bool InsideTemple;                                      // +0x5a
    void WatchForWeaponUnsheathed();
};
} // namespace NScript
using NScript::CV_TempleOfLightScript;

void NScript::CV_TempleOfLightScript::WatchForWeaponUnsheathed()
{
    CScriptThing altar = Game->GetThingWithScriptName("M_SoundAltar");
    if (!IsActiveThreadTerminating()) {
        do {
            if (InsideTemple && Game->IsPlayerWieldingWeapon() && !WeaponUnsheathed) {
                if (IsActiveThreadTerminating())
                    return;
                WeaponUnsheathed = true;
                if (!WeaponWarningComplete && TempleIntroComplete) {
                    if (IsActiveThreadTerminating())
                        return;
                    if (altar.IsAlive()) {
                        if (IsActiveThreadTerminating())
                            return;
                        long conversation = Game->AddNewConversation(altar, false, false);
                        Game->AddPersonToConversation(conversation, Game->GetHero());
                        Game->AddLineToConversation(conversation, "TEXT_QST_064_WEAPON_01", false, altar, Game->GetHero());
                    }
                    WeaponWarningComplete = true;
                }
            } else if (!Game->IsPlayerWieldingWeapon() && WeaponUnsheathed) {
                if (IsActiveThreadTerminating())
                    return;
                WeaponUnsheathed = false;
            }
            Game->NewScriptFrame();
        } while (!IsActiveThreadTerminating());
    }
}