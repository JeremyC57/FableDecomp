// CAnimSet::AddAnim(short anim, float alpha) -- append an animation index and its transition blend alpha pair (current = last = alpha) to the set and stamp the alpha update frame
// Leaf: AddAnim
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070D2F0, length 153 bytes (+ int3 padding). __fastcall, `ret 8`.
//         Ego_r.pdb: `?AddAnim@CAnimSet@@QAEXFM@Z`.
// Callees: vector<short>::_M_insert_overflow(pos, x, __true_type, 1, true) @ 0x00710980, vector<CTransitionBlendAlpha>::
//          _M_insert_overflow(pos, x, __true_type, 1, true) @ 0x00710F70, CWorld::GetFrame @ 0x0049D870 (the frame counter
//          the landed SetInternalAlpha 0x0070C280 / UpdateInternalAlphas 0x0070C2C0 use).
// Identity: the manifest label "CThingMovement_SetAnimFrame" is propagated. Retail CAnimSet (Ego_r.pdb minus 4 per 16-byte
//   vector): Anims +0x04 (vector<short>), InternalAlphas +0x10 (vector<CTransitionBlendAlpha>, 8-byte {Current, Last}),
//   FrameInternalAlphasUpdated +0x28 -- the members the landed SetInternalAlpha 0x0070C280 models.
// Shape: two inline STLport push_backs: the short binds straight to the `anim` argument slot (which the empty tag of the
//   overflow call shares), the alpha pair is a CTransitionBlendAlpha(alpha, alpha) temporary in the frame whose two
//   floats are copied as dwords; VC7.1 duplicates the frame-stamp tail into the second overflow branch. The overflow
//   bodies are same-TU stand-ins (below), which lets both tags share the dead argument slots as in retail.
class CWorld { public: static long __fastcall GetFrame(); };          // 0x0049D870
inline void* operator new(unsigned int, void* where) { return where; }
struct __true_type { __true_type() {} };

class CTransitionBlendAlpha { public:
    float Current;
    float Last;
    CTransitionBlendAlpha(float current, float last) : Current(current), Last(last) {}
};

template <class _Tp> struct vector {                   // STLport vector (12 bytes)
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool __atend);
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) _Tp(__x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __true_type(), 1UL, true);
    }
};

extern "C" void* __cdecl malloc(unsigned int size);
// Same-TU stand-in body for both overflow instances (0x00710980 / 0x00710F70 sit later in this TU): VC7.1 uses what it
// learns from it -- with declarations only, the empty tag temporaries get their own frame byte (`sub esp,0Ch`).
#pragma auto_inline(off)
template <class _Tp>
__declspec(noinline) void vector<_Tp>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool __atend)
{
    unsigned int __old_size = (unsigned int)(_M_finish - _M_start);
    unsigned int __len = __old_size + (__old_size < __fill_len ? __fill_len : __old_size);
    _Tp* __new_start = (_Tp*)malloc(__len * sizeof(_Tp));
    _Tp* __new_finish = __new_start;
    for (_Tp* __p = _M_start; __p != __position; ++__p, ++__new_finish)
        *__new_finish = *__p;
    for (; __fill_len > 0; --__fill_len, ++__new_finish)
        *__new_finish = __x;
    if (!__atend)
        for (_Tp* __q = __position; __q != _M_finish; ++__q, ++__new_finish)
            *__new_finish = *__q;
    _M_start = __new_start;
    _M_finish = __new_finish;
    _M_end_of_storage = __new_start + __len;
}
#pragma auto_inline(on)

class CAnimSet {
public:
    long UpdateMode;                                     // +0x00
    vector<short> Anims;                                 // +0x04
    vector<CTransitionBlendAlpha> InternalAlphas;        // +0x10
    float CurrentPercentageComplete;                     // +0x1C
    float LastPercentageComplete;                        // +0x20
    float Speed;                                         // +0x24
    unsigned long FrameInternalAlphasUpdated;            // +0x28
    void AddAnim(short anim, float alpha);
};

void CAnimSet::AddAnim(short anim, float alpha)
{
    Anims.push_back(anim);
    InternalAlphas.push_back(CTransitionBlendAlpha(alpha, alpha));
    FrameInternalAlphasUpdated = CWorld::GetFrame();
}