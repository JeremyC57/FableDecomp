// Leaf: ?GetTimeOfDay@CGameScriptInterface@@UBEJXZ
#include <cstddef>

class TimeOfDay
{
public:
};

class TimeManager
{
public:
    unsigned char pad_0[0x1C];
    TimeOfDay* pTimeOfDay;
};

class CGameScriptInterface
{
public:
    
    TimeManager* pTimeManager;

    virtual long GetTimeOfDay() const;
};

// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x006BB300 ?GetTimeAs24HourClock@CGameTimeManager@@QBEKXZ (was ?GetRoundedHour@TimeOfDay@@QAEJXZ)
class CGameTimeManager { public: unsigned long GetTimeAs24HourClock() const; };
long CGameScriptInterface::GetTimeOfDay() const
{
    return reinterpret_cast<const CGameTimeManager*>(pTimeManager->pTimeOfDay)->GetTimeAs24HourClock();
}