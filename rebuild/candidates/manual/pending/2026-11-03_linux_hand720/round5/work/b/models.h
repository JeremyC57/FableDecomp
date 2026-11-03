// ---- TU-local models (no engine header) ----
class CThingCompare_Nearest {                            // Ego_r.pdb CThingCompare_Nearest { C3DVector Pos; } (12 bytes)
public:
    CThingCompare_Nearest(const C3DVector& pos) : Pos(pos) {}   // ??0CThingCompare_Nearest@@QAE@ABVC3DVector@@@Z (inline)
    C3DVector Pos;
};
namespace std {                                          // STLport 4.6 <stl/_function.h>
template <class _Tp> struct logical_or : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x || __y; }
};
template <class _Operation1, class _Operation2, class _Operation3>
class binary_compose : public unary_function<typename _Operation2::argument_type, typename _Operation1::result_type> {
protected:
    _Operation1 _M_op1;
    _Operation2 _M_op2;
    _Operation3 _M_op3;
public:
    binary_compose(const _Operation1& __x, const _Operation2& __y, const _Operation3& __z) : _M_op1(__x), _M_op2(__y), _M_op3(__z) { }
};
template <class _Operation1, class _Operation2, class _Operation3>
inline binary_compose<_Operation1, _Operation2, _Operation3> __fastcall
compose2(const _Operation1& __op1, const _Operation2& __op2, const _Operation3& __op3)
{
    return binary_compose<_Operation1, _Operation2, _Operation3>(__op1, __op2, __op3);
}
}
struct HasGuardProperty {                                // FableWin.pdb UHasGuardProperty@@ (a struct); operator() 0x0065F152
    bool operator()(CIntelligentPointer<CThingAICreature> thing) const;   // ??RHasGuardProperty@@QBE_NV?$CIntelligentPointer@VCThingAICreature@@@@@Z
};
namespace std {
// The out-of-line algorithm COMDATs the function calls (STLport 4.6 _algo.h; __fastcall as FableWin.pdb's YI spellings):
// count_if 0x0065F6A6 and find_if 0x0066000A over list<CIntelligentPointer<CThingAICreature> >::iterator.
template <class _InputIter, class _Predicate> long __fastcall count_if(_InputIter __first, _InputIter __last, _Predicate __pred);
template <class _InputIter, class _Predicate> _InputIter __fastcall find_if(_InputIter __first, _InputIter __last, _Predicate __pred);
}
