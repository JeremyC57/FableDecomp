// CCreatureAction_AlertAllNearbyToThing::AlertAllNearby(const CThingSearchTools&, const CThing& perceived, CThingCreatureBase& alerter, float radius) -- every perceiver in the alerter's square neighbourhood that is perceiving the thing (other than the alerter) becomes indirectly aware of it
// Grade: DIFFER(188v190) -- box corner scheduling: retail computes TopLeft (x-r, y-r) first and stores it early, then grows x/y in place for BottomRight stored after the pushes; ours does the opposite (named 4-long box: all four early)
// Leaf: AlertAllNearby
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084A720, 188 bytes (`ret 8`: static `__fastcall`, tools in ecx, the perceived
// thing in edx, the alerter and the radius on the stack). Ego_r.pdb `?AlertAllNearby@CCreatureAction_AlertAllNearbyToThing@@
// SIXABVCThingSearchTools@@ABVCThing@@AAVCThingCreatureBase@@M@Z`; its only caller is PerformAction 0x0084A7E0 (the next
// function). The catalog's "CTCTrapBase::UpdateTriggerThings" is a propagated label.
// Callees: CThing::GetPosition() const 0x004C73D0; __ftol2 0x00BFEA70 (x, y, radius); CThingSearchTools::
//          GetAllTCsInRoughArea<CTCPerceiveThing, CFilter_Bind2<CThingFilter_IsPerceivingThing, CThingFilter_IsNot>,
//          vector<CTCPerceiveThing*> >(const C2DBoxI&, const CFilter_Bind2&, vector&) const 0x0084BA50 (the manifest
//          names the list instantiation); std::for_each<CTCPerceiveThing**, mem_fun_t<void, CTCPerceiveThing> >
//          0x0084AD60 (`__fastcall`, the functor returned by hidden pointer; manifest "_For_each<..., CAddDefNamesTo
//          ComboBox>" is propagated) with CTCPerceiveThing::SetAsIndirectlyAwareOfPerceivedThing 0x007449E0; free
//          0x00BFEA14 (the vector storage).
// Layout (Ego_r.pdb): CThingFilter_IsPerceivingThing {PerceivedThing}, CThingFilter_IsNot {Thing}, CFilter_Bind2 = two
//         filter pointers; C2DBoxI = two C2DCoordI corners.
// Stand-in: the GetAllTCsInRoughArea body above is a stand-in (auto_inline off) for the same-TU instantiation; only
//         AlertAllNearby is graded.
// Shape: the perceiver vector is the first local; the search box is the truncated alerter position grown by the
//        truncated radius; the two one-pointer filters are temporaries of the search call that VC homes in the dead
//        argument slots (the perceived pointer over the alerter's slot, the alerter over the radius slot), the binder
//        holds their addresses; mem_fun's member pointer is pushed as an immediate; the storage is freed by the inline
//        vector destructor.
extern "C" void __cdecl free(void* block);      // 0x00BFEA14

struct C3DVector { float X, Y, Z; };
struct CThing {
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CThingCreatureBase : CThing { };
struct CTCPerceiveThing {
    void SetAsIndirectlyAwareOfPerceivedThing();   // 0x007449E0
};

struct C2DCoordI {
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) {}
};
struct C2DBoxI {
    C2DCoordI TopLeft;
    C2DCoordI BottomRight;
    C2DBoxI(const C2DCoordI& topLeft, const C2DCoordI& bottomRight) : TopLeft(topLeft), BottomRight(bottomRight) {}
};

template <class T> struct CRetailVector {       // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CRetailVector() { if (_M_start) free(_M_start); }
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
};

struct CThingFilter_IsPerceivingThing {
    const CThing* PerceivedThing;
    CThingFilter_IsPerceivingThing(const CThing* thing) : PerceivedThing(thing) {}
};
struct CThingFilter_IsNot {
    const CThing* Thing;
    CThingFilter_IsNot(const CThing* thing) : Thing(thing) {}
};
template <class F1, class F2> struct CFilter_Bind2 {
    const F1* F1_;
    const F2* F2_;
    CFilter_Bind2(const F1& a, const F2& b) : F1_(&a), F2_(&b) {}
};
typedef CFilter_Bind2<CThingFilter_IsPerceivingThing, CThingFilter_IsNot> CAlertFilter;

extern "C" void* __cdecl malloc(unsigned int size);     // stand-in only
struct CThingSearchTools {
    CThing* FirstThingInArea(const C2DBoxI& area) const;           // stand-in callees of the stand-in body below
    CThing* NextThingInArea(const C2DBoxI& area, CThing* previous) const;
    template <class TFilter> bool Accepts(const TFilter& filter, CThing* thing) const;
    template <class T, class TFilter, class TVector>
    void GetAllTCsInRoughArea(const C2DBoxI& area, const TFilter& filter, TVector& tcs) const;   // 0x0084BA50
};
// Stand-in body (the real one is the 0x0084BA50 instantiation of this TU; not graded): it touches the vector only inline, so
// VC7.1 sees that the vector's address does not escape the call and keeps its storage pointer in esi across for_each.
#pragma auto_inline(off)
template <class T, class TFilter, class TVector>
void CThingSearchTools::GetAllTCsInRoughArea(const C2DBoxI& area, const TFilter& filter, TVector& tcs) const
{
    for (CThing* thing = FirstThingInArea(area); thing; thing = NextThingInArea(area, thing)) {
        if (!Accepts(filter, thing))
            continue;
        if (tcs._M_finish == tcs._M_end_of_storage) {
            unsigned long size = tcs._M_finish - tcs._M_start;
            unsigned long len = size ? 2 * size : 8;
            T** start = (T**)malloc(len * sizeof(T*));
            for (unsigned long i = 0; i < size; ++i)
                start[i] = tcs._M_start[i];
            if (tcs._M_start)
                free(tcs._M_start);
            tcs._M_start = start;
            tcs._M_finish = start + size;
            tcs._M_end_of_storage = start + len;
        }
        *tcs._M_finish++ = (T*)thing;
    }
}
#pragma auto_inline(on)

namespace std {
template <class R, class T> struct mem_fun_t {
    R (T::*m_f)();
    explicit mem_fun_t(R (T::*f)()) : m_f(f) {}
    R operator()(T* p) const { return (p->*m_f)(); }
};
template <class R, class T> inline mem_fun_t<R, T> mem_fun(R (T::*f)()) { return mem_fun_t<R, T>(f); }
template <class I, class F> F __fastcall for_each(I first, I last, F f);   // 0x0084AD60 (this instantiation)
}

struct CCreatureAction_AlertAllNearbyToThing {
    static void __fastcall AlertAllNearby(const CThingSearchTools& tools, const CThing& perceived, CThingCreatureBase& alerter,
                                          float radius);
};

void __fastcall CCreatureAction_AlertAllNearbyToThing::AlertAllNearby(const CThingSearchTools& tools, const CThing& perceived,
                                                                       CThingCreatureBase& alerter, float radius)
{
    CRetailVector<CTCPerceiveThing*> perceivers;
    const C3DVector& position = alerter.GetPosition();
    long x = (long)position.X;
    long y = (long)position.Y;
    long r = (long)radius;
    C2DBoxI area(C2DCoordI(x - r, y - r), C2DCoordI(x + r, y + r));
    tools.GetAllTCsInRoughArea<CTCPerceiveThing>(area,
        CAlertFilter(CThingFilter_IsPerceivingThing(&perceived), CThingFilter_IsNot(&alerter)), perceivers);
    std::for_each(perceivers.begin(), perceivers.end(), std::mem_fun(&CTCPerceiveThing::SetAsIndirectlyAwareOfPerceivedThing));
}
