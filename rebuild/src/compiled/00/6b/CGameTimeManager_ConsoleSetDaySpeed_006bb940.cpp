// CGameTimeManager::ConsoleSetDaySpeed(CGameTimeManager&, const vector<CGenericVar>&) -- console "SetDaySpeed": time step = 1 / speed (0 when the speed is not above 0.0001)
// Leaf: ?ConsoleSetDaySpeed@CGameTimeManager@@CIXAAV1@ABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z
// Retail: Fable.exe @ 0x006BB940, 39 bytes, static __fastcall (ecx = manager, edx = args).
// Identity (proven): Ego_r.pdb `?ConsoleSetDaySpeed@CGameTimeManager@@CIXAAV1@ABV?$vector@VCGenericVar@@...@Z`; the
// landed CGameTimeManager::CGameTimeManager 0x006BBC30 stores this address as the "SetDaySpeed" CConsoleCommand's
// callback (`mov [edi+18h], 6BB940h`, reloc binding of that name), beside ConsoleSetTimeOfDay 0x006BB860.
// Layout (Ego_r = retail): CGameTimeManager::TimeStep +0x10; CGenericVar (0x1C) ValFloat +0x18.
// Constants: 0.0001f @ 0x0129BA3C, 1.0f @ 0x0122DED8.
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
private:
    static void __fastcall ConsoleSetDaySpeed(CGameTimeManager& manager, const std::vector<CGenericVar>& args);
};

void __fastcall CGameTimeManager::ConsoleSetDaySpeed(CGameTimeManager& manager, const std::vector<CGenericVar>& args)
{
    if (args[0].ValFloat > 0.0001f)
        manager.TimeStep = 1.0f / args[0].ValFloat;
    else
        manager.TimeStep = 0.0f;
}