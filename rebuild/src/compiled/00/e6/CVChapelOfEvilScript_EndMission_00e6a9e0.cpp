// CV_ChapelOfEvilScript::EndMission() -- wait until the quest is complete, then until the hero has left the DarkwoodChapel region, then hide the quest info and mark the quest finished
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E6A9E0, length 210 bytes (224 with padding).
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (landed), CCharString::CCharString(const char*, long)
// @ 0x0099EBF0, ~CCharString @ 0x0099EAE0, and through the quest's game script interface (+0x40): NewScriptFrame
// (slot 7, +0x1c), IsRegionLoaded(const CCharString&) (slot 12, +0x30), DisplayQuestInfo(bool) (slot 321, +0x504).
// Identity: the only caller is 0x00E6A929 inside the ChapelOfEvil DoMission; the flags fit Ego_r.pdb
// CV_ChapelOfEvilScript - 0x14 (QuestFinished +0x6c, QuestComplete +0x6d).
// Shape: two script waits written as the usual `while (!cond) { NewScriptFrame(); if (terminating) return; }`
// loops, each followed by a terminating check; the region-name temporary is built and destroyed inside every loop
// test through an inline helper holding a NAMED CCharString local (a bare temporary argument lets VC fetch the
// interface vtable into edi before the string is built) and `? true : false` (the plain bool return gives
// `mov bl,al` where retail re-tests with `test al,al ; setne bl`); the bool survives the destructor in bl.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CScriptBase { public:                                        // retail 0x40 bytes (PDB 0x54: debug containers)
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};

struct CQuestScriptBase : CScriptBase {                     // retail 0x48 bytes
    CGameScriptInterface* GSI;                              // +0x40
    void*                 Owner;                            // +0x44
};

struct CV_ChapelOfEvilScript : CQuestScriptBase {
    unsigned char _pad48[0x24];                             // +0x48 (donation / morality members)
    bool QuestFinished;                                     // +0x6c
    bool QuestComplete;                                     // +0x6d
    bool IsInDarkwoodChapel() { CCharString region("DarkwoodChapel", -1); return GSI->IsRegionLoaded(region) ? true : false; }
    void EndMission();
};

void CV_ChapelOfEvilScript::EndMission()
{
    while (!QuestComplete) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (IsInDarkwoodChapel()) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    GSI->DisplayQuestInfo(false);
    QuestFinished = true;
}