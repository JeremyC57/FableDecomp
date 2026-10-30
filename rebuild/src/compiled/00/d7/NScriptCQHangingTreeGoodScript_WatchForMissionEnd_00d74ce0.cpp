// CQ_HangingTreeGoodScript::WatchForMissionEnd() -- every script frame, fail the mission with the "guards killed" reason once no guards are left
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D74CE0, length 75 bytes.
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (landed), CCharString::operator=(const char*)
// @ 0x0099EFE0, CGameScriptInterface slot 7 NewScriptFrame (+0x1c) through the interface pointer at +0x40.
// String: 0x012D2A70 = "TEXT_QST_030_FAILURE_GUARDS_KILLED".
// Layout (Ego_r.pdb CQ_HangingTreeGoodScript - 0x14): MissionFailed +0x4b, CurrentGuards +0x64,
// ReasonForFailing (CCharString) +0x9c.
// Shape: `while (!terminating) { if (CurrentGuards == 0) { if (terminating) return; ReasonForFailing = "...";
// MissionFailed = true; } NewScriptFrame(); }`.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
struct CGameScriptInterface_Calls {                      // the quest's game script interface (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame();   // slot 7 (+0x1c)
};

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"

struct CQuestScriptBase : CScriptBase {                     // retail 0x48 bytes
    CGameScriptInterface_Calls* GSI;                              // +0x40
    unsigned char _pad44[4];                                // +0x44
};

class CCharString { public:
    char* m_pData;
    const CCharString& operator=(const char* text);               // 0x0099EFE0
};

namespace NScript {
class CQ_HangingTreeGoodScript : public CQuestScriptBase {
public:
    unsigned char _pad48[3];                                // +0x48
    bool MissionFailed;                                     // +0x4b
    unsigned char _pad4c[0x18];                             // +0x4c
    long CurrentGuards;                                     // +0x64
    unsigned char _pad68[0x34];                             // +0x68
    CCharString ReasonForFailing;                           // +0x9c
    void WatchForMissionEnd();
};
} // namespace NScript
using NScript::CQ_HangingTreeGoodScript;

void NScript::CQ_HangingTreeGoodScript::WatchForMissionEnd()
{
    while (!IsActiveThreadTerminating()) {
        if (CurrentGuards == 0) {
            if (IsActiveThreadTerminating())
                return;
            ReasonForFailing = "GUARDS_KILLED";
            MissionFailed = true;
        }
        GSI->NewScriptFrame();
    }
}