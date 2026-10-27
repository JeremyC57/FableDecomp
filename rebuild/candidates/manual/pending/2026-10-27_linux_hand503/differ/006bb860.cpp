// CGameTimeManager::ConsoleSetTimeOfDay(CGameTimeManager&, const vector<CGenericVar>&) -- console "SetTimeOfDay": hours / 24 becomes the time of day, clamped to the day (0..1), to village daytime (9:00..17:00) or pushed out of village daytime to 19:30
// Grade: DIFFER(173v181) -- under /Op ours forwards the parameter slot [esp] into the member clamps (fld [esp]) where retail reloads Time (fld [ecx+8]) and copies the value through edx -> eax; without /Op the value stays on the x87 stack (153v181)
// Flags: /Op
// Leaf: ?ConsoleSetTimeOfDay@CGameTimeManager@@CIXAAV1@ABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z
// Retail: Fable.exe @ 0x006BB860, 181 bytes, static __fastcall (ecx = manager, edx = args), plain `ret`.
// Identity (proven): Ego_r.pdb `?ConsoleSetTimeOfDay@CGameTimeManager@@CIXAAV1@ABV?$vector@VCGenericVar@@...@Z`; the
//   landed CGameTimeManager::CGameTimeManager 0x006BBC30 stores this address as the "SetTimeOfDay" CConsoleCommand's
//   callback (absolute reference at 0x006BBD14, reloc binding of that name), beside ConsoleSetDaySpeed 0x006BB940.
// Body: the inline CGameTimeManager::SetTimeOfDay(float) (Ego_r.pdb, QAEXM@Z): Time = PrevTime = t, then without
//   village time clamp to [0, 1]; on a village day clamp to [9/24, 17/24]; on a village night move any time between
//   5:30 and 19:30 to 19:30 (that test reads the parameter, the clamps read the member).
// Layout (Ego_r = retail): Time +0x08, PrevTime +0x0C, UsingVillageTime +0x18, VillageDay +0x20; CGenericVar ValFloat +0x18.
// Constants: 1/24 @ 0x0122DC88, 0.0 @ 0x0122DEDC, 1.0 @ 0x0122DED8, 0.375 @ 0x0125D630, 17/24 @ 0x0125D628,
//   5.5/24 @ 0x0125D63C, 19.5/24 @ 0x0125D638.
class CGenericVar {                                // 0x1C bytes (struct_layouts_egor)
public:
    unsigned char _pad00[0x18];                    // +0x00 (vptr, Type, ValString, ValSLONG, ValULONG, ValBool)
    float ValFloat;                                // +0x18
};

namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start; _Tp* _M_finish; _Tp* _M_end_of_storage;
    const _Tp& operator[](unsigned int __n) const { return *(_M_start + __n); }
};
}

class CWorld;
class CMainGameComponent;
class CGameTimeManager { public:                   // Ego_r.pdb 0x40
    CWorld*             World;                     // +0x00
    CMainGameComponent* GameComponent;             // +0x04
    float               Time;                      // +0x08
    float               PrevTime;                  // +0x0c
    float               TimeStep;                  // +0x10
    long                DayCount;                  // +0x14
    bool                UsingVillageTime;          // +0x18
    float               VillageTime;               // +0x1c
    bool                VillageDay;                // +0x20

    __forceinline void SetTimeOfDay(float time)
    {
        PrevTime = Time = time;
        if (!UsingVillageTime) {
            if (Time < 0.0f)
                Time = 0.0f;
            else if (Time > 1.0f)
                Time = 1.0f;
        } else if (VillageDay) {
            if (Time < 0.375f)
                Time = 0.375f;
            else if (Time > 0.70833331f)
                Time = 0.70833331f;
        } else if (time > 0.22916667f && time < 0.8125f)
            Time = 0.8125f;
    }
private:
    static void __fastcall ConsoleSetTimeOfDay(CGameTimeManager& manager, const std::vector<CGenericVar>& args);
};

void __fastcall CGameTimeManager::ConsoleSetTimeOfDay(CGameTimeManager& manager, const std::vector<CGenericVar>& args)
{
    manager.SetTimeOfDay(0.041666668f * args[0].ValFloat);
}
