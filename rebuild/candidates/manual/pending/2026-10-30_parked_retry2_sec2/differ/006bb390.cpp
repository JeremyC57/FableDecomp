// CGameTimeManager::IsTimeBetween(long start, long end) const -- whether the time of day lies in [start, end) given as HHMM clock values, the range wrapping through midnight when start >= end
// Grade: DIFFER(259v265) (retry wave 2 sec2: separate named day-fraction locals dayM / dayH stop the (m+h)*1/24 factoring; left: (1) the start<end arm cross-jumps one fmul more than retail -- retail jumps to the shared tail after the hours fmul, (2) the else arm's end conversion is hours-first in retail (fild quotient ; fmul 1/24 ; fild remainder ; fmul 1/60 ; fmul 1/24). Tried: hours declared first, dayH+dayM, the private float overload / 0<=Time spelling for the last test. Next: a second helper for the wrap-around end (hours term written first). Earlier note: VC7.1 folds the clock conversion: retail multiplies each term by 1/24 separately (minutes*1/60*1/24 + hours*1/24, the last range test hours-first); ours folds 1/60*1/24 into one constant (one-expression form) or factors (m+h)*1/24 (split form). 4 grades: private float overload, direct member reads, /60.0f/24.0f, split minutes statement
// Leaf: ?IsTimeBetween@CGameTimeManager@@QBE_NJJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BB390, 263 bytes (+ int3 padding to 265), thiscall, `ret 8`.
// Identity: the catalog's "Game_IsTimeOfDayInRange" is a descriptive auto label. FableWin.pdb names
//   ?IsTimeBetween@CGameTimeManager@@QBE_NJJ@Z (public, long clocks) next to the private float overload
//   ?IsTimeBetween@CGameTimeManager@@ABE_NMMM@Z and ?GetTimeFrom24HourClock@CGameTimeManager@@QBEMJ@Z, both inlined
//   here. Callers include CAIStateGroupLightHomeCandle::ShouldBeLit (0x00946690, landed: `IsTimeOfDayInRange(LightOnTime,
//   LightOffTime)`) and the script interface's IsTimeOfDayBetween. CGameTimeManager::Time (float, a fraction of the day)
//   is at +8 (Ego_r.pdb; no engine header).
// Constants: 1/60 @ 0x0125BB08, 1/24 @ 0x0122DC88, 1.0f @ 0x0122DED8, 0.0f @ 0x0122DEDC.
// Shape: `start < end` on the clocks picks one float range test, else the wrapped pair [start, 1) || [0, end); each clock
//   becomes `minutes * (1/60) * (1/24) + hours * (1/24)` through one `idiv` by 100.
class CWorld;
class CMainGameComponent;

class CGameTimeManager {
public:
    CWorld*             World;                          // +0x00
    CMainGameComponent* GameComponent;                  // +0x04
    float               Time;                           // +0x08
    float GetTimeFrom24HourClock(long clock) const
    {
        float minutes = (float)(clock % 100) * (1.0f / 60.0f);
        float hours = (float)(clock / 100);
        float dayM = minutes * (1.0f / 24.0f);
        float dayH = hours * (1.0f / 24.0f);
        return dayM + dayH;
    }
    bool IsTimeBetween(long start, long end) const;
private:
    bool IsTimeBetween(float time, float start, float end) const { return time >= start && time < end; }
};

bool CGameTimeManager::IsTimeBetween(long start, long end) const
{
    if (start < end) {
        if (Time >= GetTimeFrom24HourClock(start) && Time < GetTimeFrom24HourClock(end))
            return true;
    }
    else if ((Time >= GetTimeFrom24HourClock(start) && Time < 1.0f) || (Time >= 0.0f && Time < GetTimeFrom24HourClock(end)))
        return true;
    return false;
}
