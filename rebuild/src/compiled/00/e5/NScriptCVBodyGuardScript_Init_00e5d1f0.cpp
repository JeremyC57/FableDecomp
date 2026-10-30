// CV_BodyGuardScript::Init() -- reset the bodyguard script state: wages, flags, conversation index, the owner's word at +8, the commentary timer (to 0) and the four henchman status words
// Retail: Fable.exe @ 0x00E5D1F0, 70 bytes (+ int3 padding). Reached only through a vtable word (0x012E6E58).
// Callees: CGameScriptInterface::SetTimer (slot 89, +0x164) through the interface global @ 0x0143E8F8.
// Layout: Ego_r.pdb CV_BodyGuardScript - 0x14 (and - 4 more after the 12-byte retail HenchmanList vector): TotalWages
// +0x48, HeroCannotAffordHelp +0x4c, HeroWieldingWeapon +0x4d, CommentaryTimer +0x5c, ConversationIndex +0x70,
// HenchmanGlobalStatus[4] +0x84. +0x44 is the script's owner pointer (the constructor's first argument); the word it
// clears at +8 is unnamed.
// Shape: statement order = store order around the SetTimer call (all zeros from ebx); the four status words are
// written out (retail has no loop).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8

struct CScriptOwner {
    unsigned char _pad[8];
    long m_value08;                                      // +0x08
};

// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"

namespace NScript {
class CV_BodyGuardScript {
public:
    unsigned char _base[0x40];                           // CScriptBase
    void* Game;                                          // +0x40
    CScriptOwner* Owner;                                 // +0x44
    long TotalWages;                                     // +0x48
    bool HeroCannotAffordHelp;                           // +0x4c
    bool HeroWieldingWeapon;                             // +0x4d
    long HeroStoodStillCounter;                          // +0x50 (CCounter)
    CTimer HenchmanLastIdleChatTimer;                    // +0x54
    CTimer HenchmanLastBoredChatTimer;                   // +0x58
    CTimer CommentaryTimer;                              // +0x5c
    CTimer HenchmanFreakedOutThePlayerTimer;             // +0x60
    void* HenchmanList[3];                               // +0x64 (vector<CScriptThing>)
    long ConversationIndex;                              // +0x70
    CTimer HenchmanDeadRespawnTimer[4];                  // +0x74
    long HenchmanGlobalStatus[4];                        // +0x84
    void Init();
};
} // namespace NScript
using NScript::CV_BodyGuardScript;

void NScript::CV_BodyGuardScript::Init()
{
    ConversationIndex = 0;
    TotalWages = 0;
    HeroCannotAffordHelp = false;
    Owner->m_value08 = 0;
    g_pGameScriptInterface->SetTimer(CommentaryTimer.TimerIndex, 0);
    HeroWieldingWeapon = false;
    HenchmanGlobalStatus[0] = 0;
    HenchmanGlobalStatus[1] = 0;
    HenchmanGlobalStatus[2] = 0;
    HenchmanGlobalStatus[3] = 0;
}