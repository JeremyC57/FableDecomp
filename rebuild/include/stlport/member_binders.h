#ifndef FABLE_STLPORT_MEMBER_BINDERS_H
#define FABLE_STLPORT_MEMBER_BINDERS_H
// STLport4.6.2 original _function_adaptors.h483-491,765-767 and _function.h157-186.
// Configuration macro __BINARY_ARG resolved to its normal dependent-type spelling.
#include "stlport/fable_containers.h"
namespace std {
template <class _Ret, class _Tp, class _Arg>
class const_mem_fun1_ref_t : public binary_function<_Tp,_Arg,_Ret> {
  typedef _Ret (_Tp::*__fun_type)(_Arg) const;
public:
  explicit const_mem_fun1_ref_t(__fun_type __pf) : _M_f(__pf) {}
  _Ret operator()(const _Tp& __r, _Arg __x) const { return (__r.*_M_f)(__x); }
private:
  __fun_type _M_f;
};
template <class _Result, class _Tp, class _Arg>
inline const_mem_fun1_ref_t<_Result,_Tp,_Arg>
mem_fun_ref(_Result (_Tp::*__f)(_Arg) const) { return const_mem_fun1_ref_t<_Result,_Tp,_Arg>(__f); }
template <class _Operation> 
class binder2nd
  : public unary_function<typename _Operation::first_argument_type,
                          typename _Operation::result_type> {
protected:
  _Operation _M_op;
  typename _Operation::second_argument_type value;
public:
  binder2nd(const _Operation& __x,
            const typename _Operation::second_argument_type& __y) 
      : _M_op(__x), value(__y) {}

  typename _Operation::result_type
  operator()(const typename _Operation::first_argument_type& __x) const {
    return _M_op(__x, value); 
  }

  typename _Operation::result_type
  operator()(typename _Operation::first_argument_type& __x) const {
    return _M_op(__x, value); 
  }
};

template <class _Operation, class _Tp>
inline binder2nd<_Operation> 
bind2nd(const _Operation& __fn, const _Tp& __x) 
{
  typedef typename _Operation::second_argument_type _Arg2_type;
  return binder2nd<_Operation>(__fn, _Arg2_type(__x));
}
}
#endif
