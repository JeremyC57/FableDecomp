// _STL::vector<CAbilityButtonGuiDef>::size() const -- element count of the 92-byte ability-button definitions
// Leaf: ?size@?$vector@VCAbilityButtonGuiDef@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0043CAC0, 29 bytes, thiscall, plain `ret`.
// Identity: the only caller, CDrawAbilityButtonsPC::DrawExpressionButton (call at 0x006580E6), passes
//   `PeekGuiDef() + 0x7FC`: the CPlayerGuiDef vector AbilityButtons (Ego_r.pdb +2116; the 18 STLport vectors in front of
//   it -- 17 members plus one in the def base -- are 12 bytes in retail instead of 16, 2116 - 18*4 = 0x7FC). The
//   divide is by 92 (0xB21642C9 with the `add edx,esi` correction and `sar edx,6`): Ego_r CAbilityButtonGuiDef is 96
//   bytes, its def base holding the same 4-byte-shorter vector. Catalog "vector<CScriptThing>::size" is propagated.
// Shape: STLport 4.6 `size() const { return size_type(_M_finish - _M_start); }`: start into edx first, the finish into
//   the pushed esi, the pointer difference divided by the element size (signed reciprocal multiply).
class CAbilityButtonGuiDef {                  // retail 92 bytes (Ego_r layout less 4: the base's STLport vector)
public:
    unsigned char _defBase[0x3C];             // CGuiDef-style base (vtable, def data, one 12-byte vector)
    long ControllerType;                      // +0x3C  EControllerType
    long ControllerButton;                    // +0x40  EXboxControllerButton
    long MouseButton;                         // +0x44  EMouseButtonControl
    long ButtonGraphic;                       // +0x48
    float PositionX, PositionY;               // +0x4C  C2DVector Position
    float OffsetX, OffsetY;                   // +0x54  C2DVector Offset
};

namespace _STL {
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
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
};

template vector<CAbilityButtonGuiDef>::size_type vector<CAbilityButtonGuiDef>::size() const;
}