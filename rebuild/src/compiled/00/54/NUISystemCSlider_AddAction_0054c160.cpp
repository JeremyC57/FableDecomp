// NUISystem::CSlider::AddAction(CCountedPointer<NUISystem::CAction>& action) -- append the action's counted pointer to Actions
// Leaf: ?AddAction@CSlider@NUISystem@@UAEXAAV?$CCountedPointer@VCAction@NUISystem@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0054C160, 78 bytes (+ int3 padding), virtual thiscall, `ret 4`.
// Identity (proven): slot 66 of CSlider's vtable 0x01248D04 (CComponent's is the empty `ret 4` 0x0052EFF0); the vector is Ego_r.pdb CSlider::Actions +0x1F0 rebased -0x30, the class's only vector<CCountedPointer<CAction> >; slots 67 / 68 fill the lazy PActionsOnSelect / PActionsOnBack lists (+0x1D0 / +0x1CC); FableWin.pdb holds ?AddAction@CSlider@NUISystem@@.
// Label: the manifest's "NUISystem::CSlider::vfunc_66" is propagated / behavioural (family J093: inline STLport push_back adders; retail has no
//   identical-code folding, so each address is its own class's virtual).
// Callees: std::vector<CCountedPointer<NUISystem::CAction> >::_M_insert_overflow(pos, x, const __false_type&, 1, true) 0x0054A2A0
//   (protected; agent950's landed spelling ?_M_insert_overflow@?$vector@V?$CCountedPointer@VCAction@NUISystem@@@@V?$allocator@V?$CCountedPointer@VCAction@NUISystem@@@@@std@@@std@@IAEXPAV?$CCountedPointer@VCAction@NUISystem@@@@ABV3@ABU__false_type@2@I_N@Z).
// Shape: STLport 4.6 push_back inline: room left -> _Construct (placement copy behind its null test, the copy constructor bumping
//   Info->RefCount when Info is set), ++finish; full -> _M_insert_overflow with the __false_type temporary homed in the
//   argument slot. Layout: Actions is a retail 12-byte STLport vector at +0x1C0 (Ego_r.pdb +0x1F0, Dinkumware 16-byte vectors).
inline void* operator new(unsigned int, void* p) { return p; }

class CCPPointerInfo {                                     // 12 bytes (Ego_r.pdb)
public:
    unsigned long RefCount;                                // +0x00
    void (__fastcall* DeleteFunc)(void* data);             // +0x04
    void* Data;                                            // +0x08
};
template <class T> class CCountedPointer {                 // 8 bytes (Ego_r.pdb)
public:
    T* Data;                                               // +0x00
    CCPPointerInfo* Info;                                  // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer();
};
namespace NUISystem { class CAction; }

namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> class allocator { };
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
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
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);   // 0x0054A2A0
};
} // namespace std

namespace NUISystem {
class CSlider {                                            // partial view (retail RTTI: CSprite -> CChangingStateComponent -> CComponent
public:                                                    // bases flattened into the leading bytes)
    virtual void AddAction(CCountedPointer<CAction>& action);   // slot 66
    unsigned char _unmodelled04[0x1C0 - 4];
    std::vector<CCountedPointer<CAction> > Actions;        // +0x1C0 (Ego_r.pdb +0x1F0)
};
}

void NUISystem::CSlider::AddAction(CCountedPointer<CAction>& action)
{
    Actions.push_back(action);
}