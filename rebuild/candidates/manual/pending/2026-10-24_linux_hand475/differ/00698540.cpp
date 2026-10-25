// CGameCameraManager::UpdateEarthquake() -- sum the live earthquakes' decaying sine shake (Axis * sin(2*pi*(t*Frequency + Phase/4)) * exp(-3t/Duration) * Amplitude * distance falloff), erase expired quakes, clamp the offset length to 1 and add it to the camera position
// Grade: DIFFER(597v603) -- x87/frame allocation: retail copies g_framesPerSecond into a temp before the loop and converts it once in the preheader (fdiv [fpsF]); ours re-reads it with fidiv [g] (a named long local reproduces the hoist but takes its own slot, +4 frame); retail keeps falloff on the x87 stack across the !Global call, ours spills it; the final position add loads cameraPos.y/z first; the cameraPos copy goes through a copy of the &Camera CSE register
// Leaf: ?UpdateEarthquake@
// 0x00698540, 608-byte row (603 without padding), __thiscall, private. Callees: GFGetCurrentFrame 0x0049D870 (__fastcall),
// the quake distance falloff 0x007F1840 (__fastcall(const C3DVector&, const C3DVector&): exp(k * squared distance); descriptive
// name), inline sin/exp/sqrt (fsin, f2xm1/fscale, fsqrt). Globals: g_framesPerSecond 0x01375550, 1.0f 0x0122DED8, 0.25f 0x0125C1E4,
// 2*pi double 0x0124F2B8 (2.0 * (float)pi), 3.0f 0x0122DED4, 1e-4f 0x0129BA3C.
// Layout (retail): Camera.WorldPosition +0x04, EarthQuakes (STLport vector<CQuake>, 48-byte elements) +0xEC; CQuake per Ego_r.pdb.
// Shape: STLport erase (non-trivial assignment: __copy loop of rep movsd), loop rotated with it/it+1 induction pointers.
extern "C" double __cdecl sin(double);
extern "C" double __cdecl exp(double);
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sin, exp, sqrt)

long __fastcall GFGetCurrentFrame();                   // 0x0049D870
extern long g_framesPerSecond;                              // 0x01375550 (.data, 15)

#pragma pack(push, 1)
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector operator/(float f) const
    {
        float inv = 1.0f / f;
        return C3DVector(X * inv, Y * inv, Z * inv);
    }
    C3DVector& operator+=(const C3DVector& v) { X += v.X; Y += v.Y; Z += v.Z; return *this; }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};
#pragma pack(pop)

extern "C" float __fastcall GetEarthquakeFalloff(const C3DVector& quakePos, const C3DVector& pos);   // 0x007F1840

static const float PI = 3.14159274f;

namespace std {
struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
inline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                          _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _Tp> inline void _Destroy(_Tp*) { }

template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef __false_type _TrivialAss;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    iterator erase(iterator __position)
    {
        if (__position + 1 != end())
            __copy_ptrs(__position + 1, this->_M_finish, __position, _TrivialAss());
        --this->_M_finish;
        _Destroy(this->_M_finish);
        return __position;
    }
};
}

class CCamera {
public:
    C3DVector WorldPosition;                           // +0x00
    unsigned char _rest[0x38 - 0x0C];
};

class CGameCameraManager {
public:
    class CQuake {
    public:
        C3DVector Position;                            // +0x00
        C3DVector Axis;                                // +0x0C
        long      Frame;                               // +0x18
        float     Amplitude;                           // +0x1C
        float     Duration;                            // +0x20
        float     Frequency;                           // +0x24
        float     Phase;                               // +0x28
        bool      Global;                              // +0x2C
    };
    void*   __vftable;                                 // +0x00
    CCamera Camera;                                    // +0x04
    unsigned char _pad3c[0xEC - 0x3C];
    std::vector<CQuake> EarthQuakes;                   // +0xEC
private:
    void UpdateEarthquake();
};

void CGameCameraManager::UpdateEarthquake()
{
    C3DVector cameraPos = Camera.WorldPosition;
    long frame = GFGetCurrentFrame();
    C3DVector offset(0.0f, 0.0f, 0.0f);
    std::vector<CQuake>::iterator it = EarthQuakes.begin();
    while (it != EarthQuakes.end()) {
        CQuake& quake = *it;
        float falloff = 1.0f;
        float t = (float)(frame - quake.Frame) / g_framesPerSecond;
        if (t > quake.Duration) {
            it = EarthQuakes.erase(it);
        } else {
            ++it;
            if (!quake.Global)
                falloff = GetEarthquakeFalloff(quake.Position, cameraPos);
            float wave = (float)sin(2.0 * PI * (t * quake.Frequency + quake.Phase * 0.25f));
            float decay = (float)exp(-(t * (3.0f / quake.Duration)));
            float strength = wave * decay * (falloff * quake.Amplitude);
            offset += quake.Axis * strength;
        }
    }
    float length = offset.GetLength();
    if (length > 0.0001f) {
        float clamped = length < 1.0f ? length : 1.0f;
        Camera.WorldPosition = offset * clamped / length + cameraPos;
    }
}
