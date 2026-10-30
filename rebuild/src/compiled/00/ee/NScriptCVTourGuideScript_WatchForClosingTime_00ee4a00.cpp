// CV_TourGuideScript::WatchForClosingTime() -- script thread: each frame compare the time of day with the script def's tour-guide opening/closing times against the TourFinished flag; on a change of state run the terminate test, then wait a frame
// Leaf: ?WatchForClosingTime@CV_TourGuideScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EE4A00, length 108 bytes (padded to 112).
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (x3), CGameScriptInterface::GetTimeOfDay (slot 647, +0xa1c)
// and NewScriptFrame (slot 7, +0x1c) through Game at +0x40.
// Data: the script def pGlobalScriptDef @ 0x0143E90C: TourGuideOpeningTime +0x8e8, TourGuideClosingTime +0x8ec (PDB 0x96c/0x970,
// retail -0x84). Layout: CV_TourGuideScript TourFinished +0x123 (PDB 311 - 0x14).
// Shape: `while (!IsActiveThreadTerminating())` rotated (entry test + bottom test); the inner state test's body is only the
// script macro `if (IsActiveThreadTerminating()) return;`.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
#define GSI_V10(p) virtual void v##p##0(); virtual void v##p##1(); virtual void v##p##2(); virtual void v##p##3(); virtual void v##p##4(); virtual void v##p##5(); virtual void v##p##6(); virtual void v##p##7(); virtual void v##p##8(); virtual void v##p##9();
#define GSI_V100(p) GSI_V10(p##0) GSI_V10(p##1) GSI_V10(p##2) GSI_V10(p##3) GSI_V10(p##4) GSI_V10(p##5) GSI_V10(p##6) GSI_V10(p##7) GSI_V10(p##8) GSI_V10(p##9)
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptDef { public:
    unsigned char _pad[0x8e8];
    long TourGuideOpeningTime;                     // +0x8e8
    long TourGuideClosingTime;                     // +0x8ec
};
extern CScriptDef* pGlobalScriptDef;                   // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
class CScriptBase { public:
    virtual void v00();
    unsigned char _body[0x3c];
    bool IsActiveThreadTerminating();              // 0x00CB7940
};
struct CQuestScriptObject : CScriptBase {
    CGameScriptInterface* Game;                    // +0x40
    void*                 Data;                    // +0x44
};
namespace NScript {
class CV_TourGuideScript : public CQuestScriptObject {
public:
    unsigned char _pad48[0x123 - 0x48];            // WaypointInfo +0x48, TourGuideKilled .. QuestFinished +0x120..+0x122
    bool          TourFinished;                    // +0x123
    void WatchForClosingTime();
};
} // namespace NScript
using NScript::CV_TourGuideScript;
void NScript::CV_TourGuideScript::WatchForClosingTime()
{
    while (!IsActiveThreadTerminating()) {
        long time = Game->GetTimeOfDay();
        if ((time >= pGlobalScriptDef->TourGuideClosingTime && !TourFinished) ||
            (time <= pGlobalScriptDef->TourGuideClosingTime && time > pGlobalScriptDef->TourGuideOpeningTime && TourFinished)) {
            if (IsActiveThreadTerminating())
                return;
        }
        Game->NewScriptFrame();
    }
}