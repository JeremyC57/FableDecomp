// CFrameRateSmoother::Init(long, long) -- initialise the frame-time ring: base Initialise, Times sized to maxNoTimes, smoothed time 0.1, counters reset
// Leaf: ?Init@CFrameRateSmoother@@QAE_NJJ@Z
// Retail: Fable.exe @ 0x0049BA70, 85 bytes (16-aligned /O2 entry), thiscall `ret 8`, returns bool.
// Identity: Ego_r.pdb `?Init@CFrameRateSmoother@@QAE_NJJ@Z`: the body starts with CInitBaseClass::Initialise() 0x0099A350
// (RTTI: CFrameRateSmoother -> CInitBaseClass -> CBaseClass), resizes Times (+0x08), stores SmoothedTime (+0x28),
// MaxNoTimes (+0x14) = arg 1, NoFramesToChangeFrameRateOver (+0x1C) = arg 2 and clears NoTimes / FirstTime / LastTime --
// the same members CalculateSmoothedTime 0x0049B9A0 and the landed AddFrameTime 0x0049B9E0 use; caller 0x00418901 (the game
// component set-up). The catalog label CEditInputProcessMapPlacement::AddToInterface is propagated.
// Callees: CInitBaseClass::Initialise 0x0099A350 (protected, PDB `?Initialise@CInitBaseClass@@IAE_NXZ`);
// std::vector<float>::resize(size_type, const float&) 0x00442770 (STLport; reached through the inline resize(n), whose
// `_Tp()` temporary is the zeroed frame word).
// Shape: the success path is the if body (an early `return false` moves the failure block up); 0.1f widened at compile time (the double pool constant 0x01238B48 = 0x3FB99999A0000000).
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector { public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    _Tp& operator[](unsigned int __n) { return *(_M_start + __n); }
    void resize(unsigned int __new_size, const _Tp& __x);                 // 0x00442770 (vector<float>)
    void resize(unsigned int __new_size) { resize(__new_size, _Tp()); }
};
}
template <class T> class CArray : public std::vector<T> { };
class CBaseClass { public:
    virtual ~CBaseClass();
};
class CInitBaseClass : public CBaseClass { public:
    bool Valid;                                      // +0x04
protected:
    bool Initialise();                               // 0x0099A350
};
class CFrameRateSmoother : public CInitBaseClass { public:
    CArray<float> Times;                             // +0x08
    long          MaxNoTimes;                        // +0x14
    long          NoTimes;                           // +0x18
    long          NoFramesToChangeFrameRateOver;     // +0x1C
    long          FirstTime;                         // +0x20
    long          LastTime;                          // +0x24
    double        SmoothedTime;                      // +0x28
    bool Init(long maxNoTimes, long noFramesToChangeFrameRateOver);
};
bool CFrameRateSmoother::Init(long maxNoTimes, long noFramesToChangeFrameRateOver)
{
    if (Initialise()) {
        Times.resize(maxNoTimes);
        SmoothedTime = 0.1f;
        MaxNoTimes = maxNoTimes;
        FirstTime = 0;
        LastTime = 0;
        NoTimes = 0;
        NoFramesToChangeFrameRateOver = noFramesToChangeFrameRateOver;
        return true;
    }
    return false;
}