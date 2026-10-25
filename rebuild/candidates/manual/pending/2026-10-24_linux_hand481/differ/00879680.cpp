// NCameraModes::CProjectileWeapon::GetPhiForPosition(const C3DVector& position) const -- the camera orbit angle (phi, in turns) that frames a world position: project the offset from the observed thing onto the camera's X/Y axes, solve the orbit equation numerically on [0,1) (tan form when |x| > |y|, cot form otherwise, only when the projection lies outside the unit circle), and of the two roots (lifted to the current turn) return the one nearest the current Phi; without two roots the current Phi
// Grade: DIFFER(535v573) -- frame layout: ours 0x54 v retail 0x48 (retail reuses the position copy's slot for the roots vector and parks x in a third slot), so most frame offsets and the store schedule differ; the logic and call sequence match
// Leaf: ?GetPhiForPosition@CProjectileWeapon@NCameraModes@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00879680, length 573 bytes (__thiscall, ret 4, float on the x87). Ego_r.pdb
// `?GetPhiForPosition@CProjectileWeapon@NCameraModes@@ABEMABVC3DVector@@@Z` (private const); the landed GetPhiForTargetedThing
// @ 0x008798C0 calls it. The catalog label "ProjectileWeapon_CalcPhiAngle" is descriptive only.
// Callees: NCameraModes::CBase::GetObservedThing() const 0x0084F1B0, CThing::GetPosition() const 0x004C73D0,
//          NMaths::GetRootsNumerically<CTanMethod>(float, float, const CTanMethod&, long, long) 0x0087AA10 and
//          <CCotMethod> 0x0087AFF0 (__fastcall, vector<float> by hidden pointer in ecx, the function object in edx; the
//          two anonymous-namespace function objects are named from the Ego_r.pdb instantiations of the same template, in
//          retail TU order), std::vector<float>::operator= 0x0045EF2D, free 0x00BFEA14 (the vectors' deallocate), __ftol2
//          0x00BFEA70.
// Pools: 1/360 0x01238E00, 0.25f 0x0123078C, 1.0f 0x0122DED8, 0.0001f 0x0129BA3C, 0.0f 0x0122DEDC.
// Layout (retail CBase 0xA4 bytes, byte-packed as the landed GetLookDirectionXYFromParams): CBase PCameraModeDef +0x44;
//         CProjectileWeapon XAxis +0xA9, YAxis +0xB5, ZAxis +0xC1, Phi +0xD0; CCameraModeDef
//         ProjectileCameraInitialPhiOffsetForCamera +0xA0.
// Shape: the dot products are `Z*o.Z + Y*o.Y + X*o.X`; both function objects are built (x, y, 1.0f, -(offset/360 - 0.25))
// before the |x| > |y| test; the turn base is (long)Phi, one less when Phi <= 0, as a float; the nearer root wins.
// Tried (diagnostic compiles): the offset in a block scope, a by-value helper and a by-value operator- (frame unchanged).
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14
extern "C" double __cdecl fabs(double x);
#pragma intrinsic(fabs)
inline float fabs(float x) { return (float)fabs((double)x); }

namespace std {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    typedef unsigned int size_type;
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _Tp* _M_end_of_storage;                                 // +0x08
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { _Alloc().deallocate(_M_start, _M_end_of_storage - _M_start); }
    size_type size() const { return size_type(_M_finish - _M_start); }
    _Tp& operator[](size_type __n) { return *(_M_start + __n); }
    vector& operator=(const vector& __x);                   // 0x0045EF2D (vector<float>)
};
}

#pragma pack(push, 1)
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& r) const { return C3DVector(X - r.X, Y - r.Y, Z - r.Z); }
    float DotProduct(const C3DVector& o) const { return Z * o.Z + Y * o.Y + X * o.X; }
};
#pragma pack(pop)
class CThing {
public:
    const C3DVector& GetPosition() const;                   // 0x004C73D0
};
struct CCameraModeDef {
    unsigned char _pad00[0xa0];
    float         ProjectileCameraInitialPhiOffsetForCamera;    // +0xa0
};
template <class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};

namespace {
class CTanMethod {
public:
    float X, Y, Scale, PhiOffset;
    CTanMethod(float x, float y, float scale, float phiOffset) : X(x), Y(y), Scale(scale), PhiOffset(phiOffset) { }
};
class CCotMethod {
public:
    float X, Y, Scale, PhiOffset;
    CCotMethod(float x, float y, float scale, float phiOffset) : X(x), Y(y), Scale(scale), PhiOffset(phiOffset) { }
};
}
namespace NMaths {
template <class T> std::vector<float> __fastcall GetRootsNumerically(float low, float high, const T& function, long samples, long iterations);
}

static const float ONE_OVER_360 = 1.0f / 360.0f;            // 0x01238E00

namespace NCameraModes {
#pragma pack(push, 1)
class CBase {
public:
    virtual void v00();
    unsigned char _pad04[0x44 - 0x04];
    CDefPointer<const CCameraModeDef> PCameraModeDef;       // +0x44
    unsigned char _pad48[0xa4 - 0x48];
    const CThing* GetObservedThing() const;                 // 0x0084F1B0
};
class CProjectileWeapon : public CBase {
public:
    long      ShotFiredTimer;                               // +0xa4
    bool      Finished;                                     // +0xa8
    C3DVector XAxis;                                        // +0xa9
    C3DVector YAxis;                                        // +0xb5
    C3DVector ZAxis;                                        // +0xc1
    unsigned char _padcd[3];
    float     Phi;                                          // +0xd0
    float     HeightParam;                                  // +0xd4
    float GetPhiForPosition(const C3DVector& position) const;
};
#pragma pack(pop)
}

float NCameraModes::CProjectileWeapon::GetPhiForPosition(const C3DVector& position) const
{
    float phi = Phi;
    C3DVector target = position;
    C3DVector thing = GetObservedThing()->GetPosition();
    C3DVector offset = target - thing;
    float x = offset.DotProduct(XAxis);
    float y = offset.DotProduct(YAxis);
    std::vector<float> roots;
    float phiOffset = -(PCameraModeDef->ProjectileCameraInitialPhiOffsetForCamera * ONE_OVER_360 - 0.25f);
    CTanMethod tanMethod(x, y, 1.0f, phiOffset);
    CCotMethod cotMethod(x, y, 1.0f, phiOffset);
    if (fabs(x) > fabs(y)) {
        if (fabs(x) > 1.0f + 0.0001f)
            roots = NMaths::GetRootsNumerically(0.0f, 1.0f, tanMethod, 2, 2);
    } else {
        if (fabs(y) > 1.0f + 0.0001f)
            roots = NMaths::GetRootsNumerically(0.0f, 1.0f, cotMethod, 2, 2);
    }
    if (roots.size() == 2) {
        long turns = (long)Phi;
        float base = Phi > 0.0f ? (float)turns : (float)(turns - 1);
        float first = base + roots[0];
        float second = base + roots[1];
        phi = fabs(Phi - first) < fabs(Phi - second) ? first : second;
    }
    return phi;
}
