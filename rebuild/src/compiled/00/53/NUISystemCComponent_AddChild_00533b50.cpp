// NUISystem::CComponent::AddChild(CCountedPointer<NUISystem::CComponent>& child) -- append the child's counted pointer to Children (no parent / initialise step: that is AddChildDynamically)
// Leaf: ?AddChild@CComponent@NUISystem@@UAEXAAV?$CCountedPointer@VCComponent@NUISystem@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00533B50, 78 bytes (+ int3 padding), virtual thiscall, `ret 4`.
// Identity (proven): slot 59 of CComponent's vtable 0x0124608C, inherited unchanged by all 40 NUISystem component vtables; CChangingStateComponent / CList / CSpellContainer override slot 60 (0x00533CC0: push_back + SetParent + Initialise), which Ego_r.pdb names AddChildDynamically (overridden by CChangingStateComponent and CList there too), while Ego_r.pdb's CComponent::AddChild is the 11-byte, 2-instruction `add ecx ; jmp push_back` forwarder; slot 53 (GetChildren) returns this+0xB0 = Children, and slot 61 (0x00533BA0, AddChildFront) inserts at its begin.
// Label: the manifest's "Container_PushElement" is propagated / behavioural (family J093: inline STLport push_back adders; retail has no
//   identical-code folding, so each address is its own class's virtual).
// Callees: std::vector<CCountedPointer<NUISystem::CComponent> >::_M_insert_overflow(pos, x, const __false_type&, 1, true) 0x005359D0
//   (protected; agent950's landed spelling ?_M_insert_overflow@?$vector@V?$CCountedPointer@VCComponent@NUISystem@@@@V?$allocator@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@@std@@IAEXPAV?$CCountedPointer@VCComponent@NUISystem@@@@ABV3@ABU__false_type@2@I_N@Z).
// Shape: STLport 4.6 push_back inline: room left -> _Construct (placement copy behind its null test, the copy constructor bumping
//   Info->RefCount when Info is set), ++finish; full -> _M_insert_overflow with the __false_type temporary homed in the
//   argument slot. Layout: Children is a retail 12-byte STLport vector at +0xB0 (Ego_r.pdb +0xB8, Dinkumware 16-byte vectors).
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
namespace NUISystem { class CComponent; }

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
                            bool __atend = false);   // 0x005359D0
};
} // namespace std

namespace NUISystem {
class CComponent {                                         // partial view (retail RTTI: CUIInterface @0, CObserver @4, CActionParam @0x24
public:                                                    // flattened into the leading bytes; 134-slot vtable 0x0124608C)
    virtual void AddChild(CCountedPointer<CComponent>& child);   // slot 59
    unsigned char _unmodelled04[0xB0 - 4];
    std::vector<CCountedPointer<CComponent> > Children;    // +0xB0 (Ego_r.pdb +0xB8)
};
}

void NUISystem::CComponent::AddChild(CCountedPointer<CComponent>& child)
{
    Children.push_back(child);
}