// (anonymous namespace)::CIsUsableIciclePoint::operator()(const C3DVector& point) const -- a point is usable for a new icicle when no existing icicle lies within IciclesLimit of it in the ground (X/Y) plane
// Leaf: ??RCIsUsableIciclePoint@?A0x
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E2E40, 89 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb ??RCIsUsableIciclePoint@?A0xcee9a82c@@QBE_NABVC3DVector@@@Z (0x00825710), the predicate of the
//   STLport partition right after it (Ego_r 0x00825790 _Partition<C3DVector*, CIsUsableIciclePoint>); its only callers are
//   that partition's two calls (0x008E3449 / 0x008E3463); the label agrees.
// Layout: Ego_r.pdb CIsUsableIciclePoint {Icicles vector<CIcicleData>* +0, IciclesLimit float +4}; CIcicleData (36 bytes)
//   Position +0x18 (the landed copy constructor 0x008E2FC0).
// Shape: the loop-invariant `IciclesLimit * IciclesLimit` is hoisted after the empty test into the dead `point` argument
//   slot (the point pointer lives in edi); the offset is a C3DVector difference whose 2D square is written Y*Y + X*X
//   (VC7.1 then squares X first, as retail); `if (sq < limit*limit) return false;` (`test ah,5 ; jnp`).
//   IsUsableIciclePoint_Use is a same-TU external caller (no retail address) that keeps the anonymous-namespace body emitted.
namespace std {
template <class _Tp> class vector {                     // STLport 4.6 layout (12 bytes)
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    const_iterator begin() const { return _M_start; }
    const_iterator end() const { return _M_finish; }
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
}
class CThing;
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
private:
    void* PData;                                        // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& other) const { return C3DVector(X - other.X, Y - other.Y, Z - other.Z); }
};
namespace NCreatureMode {
class CModeIceTrollGroundSmashIcicleAssault {
public:
    class CIcicleData {                                 // Ego_r.pdb: 36 bytes
    public:
        CIntelligentPointer<CThing> PEffect;            // +0x00
        CIntelligentPointer<CThing> PTrap;              // +0x08
        long EffectTimer;                               // +0x10
        long TrapTimer;                                 // +0x14
        C3DVector Position;                             // +0x18
    };
};
}
namespace {
class CIsUsableIciclePoint {                            // Ego_r.pdb: Icicles +0, IciclesLimit +4
public:
    std::vector<NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::CIcicleData>* Icicles;
    float IciclesLimit;
    bool operator()(const C3DVector& point) const;
};
#pragma auto_inline(off)
bool CIsUsableIciclePoint::operator()(const C3DVector& point) const
{
    for (std::vector<NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::CIcicleData>::const_iterator it = Icicles->begin();
            it != Icicles->end(); ++it) {
        C3DVector offset = it->Position - point;
        if (offset.Y * offset.Y + offset.X * offset.X < IciclesLimit * IciclesLimit)
            return false;
    }
    return true;
}
#pragma auto_inline(on)
}
// External caller so the anonymous-namespace functor body stays emitted (the retail callers are the STLport partition 0x008E3449/0x008E3463).
bool IsUsableIciclePoint_Use(const CIsUsableIciclePoint& filter, const C3DVector& point) { return filter(point); }