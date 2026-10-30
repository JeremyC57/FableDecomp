// CV_BodyGuardScript::CheckForHenchmanFiredOverride() -- script thread: every frame, when the quest data reports the fire-override state (2) it waits frame by frame until the henchman list is empty, then clears the state; ends when the script thread terminates
// Leaf: CheckForHenchmanFiredOverride
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E5D900, length 155 bytes (padded to 160), `ret`.
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (x5), CGameScriptInterface::NewScriptFrame (slot 7,
// +0x1c, through Game at +0x40, x2).
// Layout (retail = Ego_r.pdb CV_BodyGuardScript - 0x14): Game +0x40, Data +0x44 (the shared quest data; the override
// state word at +0xc), HenchmanList (vector<CScriptThing>, 12-byte elements) +0x64.
// Shape: the outer `while (!terminating)` loop is rotated (test at the bottom), the henchman wait is a `while (size())`
// loop with the size recomputed by the signed divide-by-12.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptThing { public:                              // 12 bytes (vtable 0x01238C8C)
    void* __vftable;
    long  m_data[2];
};
struct CScriptThingVector {                        // STLport vector<CScriptThing> (12 bytes in retail)
    CScriptThing* _M_start;
    CScriptThing* _M_finish;
    CScriptThing* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
};
struct CBodyGuardQuestData {
    unsigned char _pad00[0xc];
    long HenchmanFiredState;                       // +0x0c (2 = override pending)
};
class CScriptBase { public:
    unsigned char _body[0x40];
public:
    bool IsActiveThreadTerminating();   // 0x00CB7940
};
namespace NScript {
class CV_BodyGuardScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                    // +0x40
    CBodyGuardQuestData*  Data;                    // +0x44
    long  TotalWages;                              // +0x48
    bool  HeroCannotAffordHelp;                    // +0x4c
    bool  HeroWieldingWeapon;                      // +0x4d
    long  HeroStoodStillCounter;                   // +0x50 (CCounter)
    long  HenchmanLastIdleChatTimer;               // +0x54 (CTimer)
    long  HenchmanLastBoredChatTimer;              // +0x58
    long  CommentaryTimer;                         // +0x5c
    long  HenchmanFreakedOutThePlayerTimer;        // +0x60
    CScriptThingVector HenchmanList;               // +0x64
    void CheckForHenchmanFiredOverride();
};
} // namespace NScript
using NScript::CV_BodyGuardScript;
void NScript::CV_BodyGuardScript::CheckForHenchmanFiredOverride()
{
    while (!IsActiveThreadTerminating())
    {
        if (Data->HenchmanFiredState == 2)
        {
            if (IsActiveThreadTerminating())
                return;
            while (HenchmanList.size() != 0)
            {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Data->HenchmanFiredState = 0;
        }
        Game->NewScriptFrame();
    }
}