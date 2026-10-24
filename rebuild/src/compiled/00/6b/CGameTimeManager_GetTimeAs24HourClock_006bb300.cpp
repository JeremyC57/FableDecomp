// CGameTimeManager::GetTimeAs24HourClock() const -- the time of day as an HHMM clock value: whole hours * 100 + the minutes of the current hour
// Leaf: ?GetTimeAs24HourClock@CGameTimeManager@@QBEKXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BB300, 76 bytes (+ int3 padding), thiscall, plain `ret` via the tail
//   jump into __ftol2.
// Callers: one site, the tail jump at 0x0088FE36 (a script-interface wrapper).
// Callees: floor @ 0x00BFEB8A (CRT, double), __ftol2 @ 0x00BFEA70 (the two float-to-integer conversions).
// Constants: 24.0f @ 0x0123822C, 60.0f @ 0x0124F2FC, 100.0f @ 0x01230A08.
// Identity: Ego_r.pdb ?GetTimeAs24HourClock@CGameTimeManager@@QBEKXZ; CGameTimeManager::Time (float, a fraction of the
//   day) is at +8 (Ego_r.pdb: World +0, GameComponent +4, Time +8; no engine header).
// Shape: `float hours = Time * 24.0f;` (kept as a float in the frame by `fst`), `long minutes = (hours - floor(hours)) *
//   60.0f` (`fsubr dword` against the float local, then __ftol2), and `floor(hours) * 100.0f + minutes` (`fiadd` of the
//   minutes word) converted by a tail-jumped __ftol2. `floor(hours)` must resolve to the VC7.1 <math.h> C++ float
//   overload (`(float)floor((double)x)`): the float-typed difference and products give retail's `fsubr dword` /
//   `fmul dword` and the 4-byte frame float; the CRT double floor alone keeps hours as a frame double (80v76).
// Label correction: the catalog `TimeOfDay_GetRoundedHour` is descriptive.
extern "C" double __cdecl floor(double x);              // 0x00BFEB8A
inline float floor(float x) { return (float)floor((double)x); }    // the VC7.1 <math.h> C++ float overload (floorf)
extern const float g_hoursPerDay;                       // 0x0123822C (24.0f)
extern const float g_minutesPerHour;                    // 0x0124F2FC (60.0f)
extern const float g_clockHourScale;                    // 0x01230A08 (100.0f)

class CWorld;
class CMainGameComponent;

class CGameTimeManager {
public:
    CWorld*             World;                          // +0x00
    CMainGameComponent* GameComponent;                  // +0x04
    float               Time;                           // +0x08
    unsigned long GetTimeAs24HourClock() const;
};

unsigned long CGameTimeManager::GetTimeAs24HourClock() const
{
    float hours = Time * 24.0f;
    long minutes = (long)((hours - floor(hours)) * 60.0f);
    return (unsigned long)(floor(hours) * 100.0f + minutes);
}