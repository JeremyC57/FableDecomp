// CV_TempleOfLightScript::EndMission() -- wait until the quest is complete, then until the hero has left the Witchwood3 region, then hide the quest info and mark the quest finished
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EDC640, length 210 bytes (224 with padding).
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (landed), CCharString::CCharString(const char*, long)
// @ 0x0099EBF0, ~CCharString @ 0x0099EAE0, and through the quest's game script interface (+0x40): NewScriptFrame
// (slot 7, +0x1c), IsRegionLoaded(const CCharString&) (slot 12, +0x30), DisplayQuestInfo(bool) (slot 321, +0x504).
// Identity: the catalog label CV_ChapelOfEvilScript::EndMission is propagated from the byte twin 0x00E6A9E0. The only
// caller is 0x00EDC589 inside the DoMission at 0x00EDC450, whose address is stored by CV_TempleOfLightScript::Main
// (0x00EDB190, at 0x00EDB586); the flags fit Ego_r.pdb CV_TempleOfLightScript - 0x14 (QuestFinished +0x70,
// QuestComplete +0x71; the Temple adds DonationActive and AllAcolytesEscaped, so its block sits 4 bytes higher).
// Shape: two script waits written as the usual `while (!cond) { NewScriptFrame(); if (terminating) return; }`
// loops, each followed by a terminating check; the region-name temporary is built and destroyed inside every loop
// test through an inline helper holding a NAMED CCharString local and `? true : false` (same source as the
// ChapelOfEvil twin 0x00E6A9E0); the bool survives the destructor in bl.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
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

namespace NScript {
class CV_TempleOfLightScript : public CQuestScriptBase {
public:
    unsigned char _pad48[0x28];                             // +0x48 (donation / morality members)
    bool QuestFinished;                                     // +0x70
    bool QuestComplete;                                     // +0x71
    bool IsInWitchwood3() { CCharString region("Witchwood3", -1); return GSI->IsRegionLoaded(region) ? true : false; }
    void EndMission();
};
} // namespace NScript
using NScript::CV_TempleOfLightScript;

void NScript::CV_TempleOfLightScript::EndMission()
{
    while (!QuestComplete) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (IsInWitchwood3()) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    GSI->DisplayQuestInfo(false);
    QuestFinished = true;
}