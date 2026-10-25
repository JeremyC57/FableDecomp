// std::vector::push_back(const C3DAnimationStateAlpha& x) -- STLport vector<C3DAnimationStateAlpha>::push_back: copy-construct at _M_finish, or grow through _M_insert_overflow
// Leaf: ?push_back@
// Retail: Fable.exe @ 0x00A3D380, 90 bytes (+ int3 padding), thiscall `ret 4`.
// Caller: 0x00A3C323 in C3DAnimationBlendState::CSharedData's AddCombinationBlend path (the combination-blend vector).
// Callee: vector<C3DAnimationStateAlpha>::_M_insert_overflow @ 0x00A3D060 -- given a same-TU noinline STAND-IN body here
//   (not the real grow code; only its call matters) so the empty __false_type temporary shares x's argument slot as in
//   retail (`lea edx,[esp+0Ch]` = the x slot, pushed as the tag reference).
// Label: catalog "Vector_AddCombinationEntry" is descriptive; this is the STLport member.
// Shape: STLport 4.6 `if (_M_finish != _M_end_of_storage) { _Construct(_M_finish, x); ++_M_finish; } else
//   _M_insert_overflow(_M_finish, x, _IsPODType(), 1UL, true);` -- the implicit copy constructor inline (three vtable
//   stores), `add dword [ecx+4],14h`.

inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

// Local polymorphic model (engine/C3DAnimationStateAlpha.h flattens the vptr into a data member, which cannot produce
// the per-level vtable stores of the implicit copy constructor). Layout per Ego_r.pdb: CIVCountedPointeeBase is
// {vptr, IVRefCount} (8 bytes), C3DAnimationState adds AnimationIndex +0x08 / AnimationTime +0x0C, and
// C3DAnimationStateAlpha adds AlphaWeight +0x10 (0x14 bytes). Copy constructor and assignment are implicit: the copy
// stores the three vtables 0x012354D0 / 0x01249F50 / 0x01265128 in base-to-derived order, the assignment skips them.
class CIVCountedPointeeBase {                               // vtable 0x012354D0
public:
    virtual ~CIVCountedPointeeBase();                       // slot 0
    unsigned long IVRefCount;                               // +0x04
};

class C3DAnimationState : public CIVCountedPointeeBase {    // vtable 0x01249F50
public:
    virtual ~C3DAnimationState();
    virtual long GetType() const;
    virtual void Validate();
    long  AnimationIndex;                                   // +0x08
    float AnimationTime;                                    // +0x0C
};

class C3DAnimationStateAlpha : public C3DAnimationState {   // vtable 0x01265128, sizeof 0x14
public:
    virtual long GetType() const;                           // 0x0070FE20
    virtual void Validate();                                // 0x00A3BE20
    float AlphaWeight;                                      // +0x10
};

extern "C" void* __cdecl malloc(unsigned int size);         // MSVCR71 import thunk 0x00BFEA0E

namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _Tp* _M_end_of_storage;                                 // +0x08
#pragma auto_inline(off)
    // 0x00A3D060 (separate function; same-TU stand-in so the empty tag can share x's slot)
    __declspec(noinline) void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend)
    {
        if (__atend && __fill_len == 1 && __position == _M_finish)
            _Construct(_M_start, __x);
    }
#pragma auto_inline(on)
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, __x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);
    }
};
}

template void std::vector<C3DAnimationStateAlpha>::push_back(const C3DAnimationStateAlpha&);