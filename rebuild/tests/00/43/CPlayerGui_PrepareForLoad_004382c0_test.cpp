// behaviour test for 0x004382C0
#include <stdio.h>
namespace NPlayerGui {
class CDrawBase {
public:
    int Prepared;
    CDrawBase() : Prepared(0) {}
    virtual ~CDrawBase() {}
    virtual void PrepareForLoad() { ++Prepared; }
};
}
namespace std {
template <class Arg, class Result> struct unary_function { typedef Arg argument_type; typedef Result result_type; };
template <class R, class T> class mem_fun_t : public unary_function<T*, R> {
    typedef R (T::*fun_type)();
    fun_type f;
public:
    explicit mem_fun_t(fun_type pf) : f(pf) {}
    R operator()(T* p) const { return (p->*f)(); }
};
template <class R, class T> inline mem_fun_t<R, T> mem_fun(R (T::*f)()) { return mem_fun_t<R, T>(f); }
template <class InputIter, class Function>
Function __fastcall for_each(InputIter first, InputIter last, Function f) { for (; first != last; ++first) f(*first); return f; }
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
};
}
class CPlayerGui {
public:
    unsigned char                         _pad00[0x2c0];
    std::vector<NPlayerGui::CDrawBase*>   Illustrators;  // +0x2c0
    void PrepareForLoad();
};
void CPlayerGui::PrepareForLoad()
{
    std::for_each(Illustrators.begin(), Illustrators.end(), std::mem_fun(&NPlayerGui::CDrawBase::PrepareForLoad));
}

struct CDrawDerived : NPlayerGui::CDrawBase { virtual void PrepareForLoad() { Prepared += 10; } };
int main()
{
    NPlayerGui::CDrawBase a; CDrawDerived b; NPlayerGui::CDrawBase* arr[2] = { &a, &b };
    static CPlayerGui g; g.Illustrators._M_start = arr; g.Illustrators._M_finish = arr + 2;
    g.PrepareForLoad();
    int ok = a.Prepared == 1 && b.Prepared == 10;
    printf(ok ? "004382c0_TEST PASS\n" : "004382c0_TEST FAIL\n");
    return ok ? 0 : 1;
}