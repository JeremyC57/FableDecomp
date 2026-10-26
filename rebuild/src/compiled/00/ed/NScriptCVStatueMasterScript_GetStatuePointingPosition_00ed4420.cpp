// CV_StatueMasterScript::GetStatuePointingPosition() -- which of the statue's facing angles matches the current time of day: the day fraction's fractional part is compared with AnglesToFaceList[1..3] within a tolerance; returns the index, or 0
// Leaf: ?GetStatuePointingPosition@CV_StatueMasterScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ED4420, length 157 bytes (padded to 160).
// Callees: CGameScriptInterface::GetTimeOfDay (slot 647, +0xa1c) of the interface at +0x40 (HHMM long), __ftol2 @ 0x00BFEA70.
// Float pool: 1/60 @ 0x0125BB08, 1/24 @ 0x0122DC88, the angle tolerance @ 0x0125B25C.
// Shape: the sibling CV_StatueMasterScript::GetTimeOfDayFraction (landed 0x00ED43D0) is inlined here (defined in the class),
// so the 80-bit fraction is copied (`fld st(0)`) for the `(long)` truncation and `f - (float)(long)f` spills to one frame float.
// Layout (retail = PDB - 0x14): Game interface +0x40, AnglesToFaceList float[4] +0x48 (the loop starts at element 1, +0x4c).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
#define GSI_V10(p) virtual void v##p##0(); virtual void v##p##1(); virtual void v##p##2(); virtual void v##p##3(); virtual void v##p##4(); virtual void v##p##5(); virtual void v##p##6(); virtual void v##p##7(); virtual void v##p##8(); virtual void v##p##9();
#define GSI_V100(p) GSI_V10(p##0) GSI_V10(p##1) GSI_V10(p##2) GSI_V10(p##3) GSI_V10(p##4) GSI_V10(p##5) GSI_V10(p##6) GSI_V10(p##7) GSI_V10(p##8) GSI_V10(p##9)
struct CGameScriptInterface {                      // retail vtable @ 0x01260F0C
    GSI_V100(0) GSI_V100(1) GSI_V100(2) GSI_V100(3) GSI_V100(4) GSI_V100(5)
    GSI_V10(60) GSI_V10(61) GSI_V10(62) GSI_V10(63)
    virtual void v640(); virtual void v641(); virtual void v642(); virtual void v643(); virtual void v644(); virtual void v645(); virtual void v646();
    virtual long GetTimeOfDay() const;             // slot 647 (+0xa1c)
};
// the angle tolerance 0.04f is a pool constant @ 0x0125B25C
struct CScriptBase {
    virtual void v00();
    unsigned char _body[0x3c];
};
struct CQuestScriptObject : CScriptBase {
    CGameScriptInterface* Game;                    // +0x40
    void*                 Data;                    // +0x44
};
namespace NScript {
class CV_StatueMasterScript : public CQuestScriptObject {
public:
    float AnglesToFaceList[4];                     // +0x48
    float GetTimeOfDayFraction()                   // 0x00ED43D0
    {
        long time = Game->GetTimeOfDay();
        long hours = time / 100;
        long minutes = time - hours * 100;
        return ((1.0f / 60.0f) * (float)minutes + (float)hours) * (1.0f / 24.0f);
    }
    long GetStatuePointingPosition();
};
} // namespace NScript
using NScript::CV_StatueMasterScript;
long NScript::CV_StatueMasterScript::GetStatuePointingPosition()
{
    float day = GetTimeOfDayFraction();
    float frac = day - (float)(long)day;
    for (long i = 1; i < 4; ++i) {
        if (AnglesToFaceList[i] - 0.04f < frac && AnglesToFaceList[i] + 0.04f > frac)
            return i;
    }
    return 0;
}