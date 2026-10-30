// CV_ChapelOfEvilScript::WatchForWeaponUnsheathed() -- track the hero drawing a weapon inside the Chapel; the first time (after the intro) the sound altar warns him with a one-line conversation
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E6A340, 447 bytes (448 with padding), plain `ret`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptThing::IsAlive 0x004AB130 (out of
//   line here), ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC, ~CScriptThing 0x004AA840 (out of line),
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940; through the interface (+0x40) NewScriptFrame (slot 7), GetHero
//   (slot 70), GetThingWithScriptName (slot 72, hidden result), IsPlayerWieldingWeapon (slot 195), AddNewConversation
//   (slot 364), AddPersonToConversation (slot 365), AddLineToConversation (slot 366).
// Strings: "M_SoundAltar" 0x012E8324, "TEXT_QST_019_WEAPON_01" 0x012E8450.
// Shape: an `if (!terminating) { do { ... } while (!terminating); }` watch loop; the altar handle's inline destructor is expanded once at the loop exit
//   (zero kept in ebx), the out-of-line ~CScriptThing called at the in-loop returns.
// Layout (retail = Ego_r.pdb - 0x14): WeaponWarningComplete +0x56, WeaponUnsheathed +0x57, ChapelIntroComplete +0x58,
//   InsideChapel +0x59; interface pointer +0x40.
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
class CScriptBase { public:                                        // retail 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;                             // +0x40
    void*                 Data;                             // +0x44
};
struct CV_ChapelOfEvilScript : CQuestScriptBase {
    long DonationTotal;                                     // +0x48
    long DonationThisVisit;                                 // +0x4c
    long DonationCounter;                                   // +0x50
    bool DonationInvalid;                                   // +0x54
    bool DisplayQuestInfo;                                  // +0x55
    bool WeaponWarningComplete;                             // +0x56
    bool WeaponUnsheathed;                                  // +0x57
    bool ChapelIntroComplete;                               // +0x58
    bool InsideChapel;                                      // +0x59
    void WatchForWeaponUnsheathed();
};

void CV_ChapelOfEvilScript::WatchForWeaponUnsheathed()
{
    CScriptThing altar = Game->GetThingWithScriptName("M_SoundAltar");
    if (!IsActiveThreadTerminating()) {
        do {
            if (InsideChapel && Game->IsPlayerWieldingWeapon() && !WeaponUnsheathed) {
                if (IsActiveThreadTerminating())
                    return;
                WeaponUnsheathed = true;
                if (!WeaponWarningComplete && ChapelIntroComplete) {
                    if (IsActiveThreadTerminating())
                        return;
                    if (altar.IsAlive()) {
                        if (IsActiveThreadTerminating())
                            return;
                        long conversation = Game->AddNewConversation(altar, false, false);
                        Game->AddPersonToConversation(conversation, Game->GetHero());
                        Game->AddLineToConversation(conversation, "TEXT_QST_019_WEAPON_01", false, altar, Game->GetHero());
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