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
class CGameScriptInterface { public:                      // retail vtable @ 0x01260F0C
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;           // slot 7 (+0x1c)
    virtual void v008(); virtual void v009();
    GSI_V10(01) GSI_V10(02) GSI_V10(03) GSI_V10(04) GSI_V10(05) GSI_V10(06) GSI_V10(07) GSI_V10(08) GSI_V10(09)
    GSI_V100(1) GSI_V100(2) GSI_V100(3) GSI_V100(4) GSI_V100(5)
    GSI_V10(60) GSI_V10(61) GSI_V10(62) GSI_V10(63)
    virtual void v640(); virtual void v641(); virtual void v642(); virtual void v643(); virtual void v644(); virtual void v645(); virtual void v646();
    virtual long GetTimeOfDay() const;             // slot 647 (+0xa1c)
};
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