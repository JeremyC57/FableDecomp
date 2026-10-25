// Behaviour test for 0x004AA6C0 _STL::binary_compose<logical_or<bool>,CThingFilter_HasTC,CThingFilter_HasTC>::operator():
// a thing passes when either TC filter accepts it (both filters always run, then their results are or-ed)
#include <stdio.h>
static int gFilterCalls = 0;
namespace _STL {
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
    _Operation1 _M_op1;
    _Operation2 _M_op2;
    _Operation3 _M_op3;
public:
    binary_compose(const _Operation1& __x, const _Operation2& __y, const _Operation3& __z)
        : _M_op1(__x), _M_op2(__y), _M_op3(__z) { }
    typename _Operation1::result_type operator()(const typename _Operation2::argument_type& __x) const
    {
        return _M_op1(_M_op2(__x), _M_op3(__x));
    }
};
}
class CThing {
public:
    unsigned long TCMask;
};
class CThingFilter_HasTC : public _STL::unary_function<CThing, bool> {
public:
    long Type;
    bool operator()(const CThing& thing) const;
};
bool CThingFilter_HasTC::operator()(const CThing& thing) const   // stand-in: the thing has TC number Type
{
    ++gFilterCalls;
    return (thing.TCMask & (1ul << Type)) != 0;
}
typedef _STL::binary_compose<_STL::logical_or<bool>, CThingFilter_HasTC, CThingFilter_HasTC> CEitherTC;
int main()
{
    CThingFilter_HasTC f1; f1.Type = 3;
    CThingFilter_HasTC f2; f2.Type = 5;
    CEitherTC either(_STL::logical_or<bool>(), f1, f2);
    CThing none; none.TCMask = 0;
    CThing first; first.TCMask = 1ul << 3;
    CThing second; second.TCMask = 1ul << 5;
    CThing both; both.TCMask = (1ul << 3) | (1ul << 5);
    bool ok = true;
    ok = ok && !either(none);
    ok = ok && either(first);
    ok = ok && either(second);
    ok = ok && either(both);
    ok = ok && gFilterCalls == 8;              // two filter calls per evaluation, no short circuit
    if (ok) { printf("004aa6c0_TEST PASS\n"); return 0; }
    printf("004aa6c0_TEST FAIL\n");
    return 1;
}