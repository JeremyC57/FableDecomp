// std::binary_compose<logical_or<bool>,CThingFilter_HasTC,CThingFilter_HasTC>::operator()(const CThing&) -- a thing passes when it has either of two TCs
// Leaf: ??R?$binary_compose@U?$logical_or@_N@std@@VCThingFilter_HasTC@@V3@@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AA6C0, 56 bytes (+8 int3), `__fastcall`, `ret 4`, bool in eax.
// Identity: FableWin.pdb has compose2<logical_or<bool>, CThingFilter_HasTC, CThingFilter_HasTC> and this
//   binary_compose's constructor next to CThingSearchTools::GetPBestThing<CFilter_Bind2<binary_compose<...>,
//   CThingFilter_In2DRange>>; the manifest's engine_api row names this operator() with the same arguments. Both
//   callees are CThingFilter_HasTC::operator()(const CThing&) const @ 0x006A80A0 (landed, chunk 335: tests the
//   thing's TC bit). Its one caller is 0x0065F70E.
// Layout: STLport binary_compose {_M_op1 logical_or<bool> +0 (empty), _M_op2 HasTC +4, _M_op3 HasTC +8}; HasTC is
//   {long Type} (the landed model).
// Shape: `return _M_op1(_M_op2(__x), _M_op3(__x));` -- VC7.1 evaluates the arguments right to left, so _M_op3
//   (+8) runs first and its result is kept in bl; the inline logical_or then tests _M_op2's result first and
//   returns 0/1 in the whole of eax.
namespace std {
template <class _Arg, class _Result> struct unary_function {
    typedef _Arg    argument_type;
    typedef _Result result_type;
};
template <class _Arg1, class _Arg2, class _Result> struct binary_function {
    typedef _Arg1   first_argument_type;
    typedef _Arg2   second_argument_type;
    typedef _Result result_type;
};
template <class _Tp> struct logical_or : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x || __y; }
};
template <class _Operation1, class _Operation2, class _Operation3>
class binary_compose
    : public unary_function<typename _Operation2::argument_type, typename _Operation1::result_type> {
protected:
    _Operation1 _M_op1;                                  // +0x00
    _Operation2 _M_op2;                                  // +0x04
    _Operation3 _M_op3;                                  // +0x08
public:
    binary_compose(const _Operation1& __x, const _Operation2& __y, const _Operation3& __z)
        : _M_op1(__x), _M_op2(__y), _M_op3(__z) { }
    typename _Operation1::result_type operator()(const typename _Operation2::argument_type& __x) const
    {
        return _M_op1(_M_op2(__x), _M_op3(__x));
    }
};
}

class CThing;

class CThingFilter_HasTC : public std::unary_function<CThing, bool> {
public:
    long Type;                                           // +0x00 ETCInterfaceType
    bool operator()(const CThing& thing) const;          // 0x006A80A0
};

template class std::binary_compose<std::logical_or<bool>, CThingFilter_HasTC, CThingFilter_HasTC>;