// std::vector::push_back(const C3DVector& __x) -- (vector<C3DVector >) append a copy, growing through _M_insert_overflow when full
// Leaf: ?push_back@?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@QAEXABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066E630, 64 bytes, thiscall, `ret 4`.
// Identity (proven): callers push C3DVectors onto vector<C3DVector> members and locals: CCombatActionBase::ProcessPossibleCreatureStrikeEvent 0x00858130, CTCMeleeTrap::ProcessPossibleStrike 0x00788617, CTCPhysicsRigidBody::CheckForCollisions 0x0075AAF0 (Ego_r.pdb CTCPhysicsRigidBody::CollisionNormal is vector<C3DVector>), CAIStateGroupJackDragon_FlyOutOfGround 0x00951B20; its overflow 0x0066E370 is bound ?_M_insert_overflow@?$vector@VC3DVector@@...ABU__false_type@2@I_N@Z (it copies through the __false_type __uninitialized_copy<C3DVector*> 0x0066DDF0).
// Callees: vector<C3DVector>::_M_insert_overflow(pointer, const value_type&, const __false_type&, size_type, bool) 0x0066E370.
// Shape (family M185, representative 0x0066E630): STLport `if (_M_finish != _M_end_of_storage._M_data) { _Construct(_M_finish,
//   __x); ++_M_finish; } else _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);` -- the implicit 12-byte copy
//   (three dword moves through esi) under placement new's null test; the empty tag homed in the dead __x slot. Speed TU.
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
class C3DVector {                                         // Ego_r.pdb, 12 bytes
public:
    float X, Y, Z;
};
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend = false);   // 0x0066E370
};
}
namespace std {
template void vector<C3DVector >::push_back(const C3DVector& __x);
}