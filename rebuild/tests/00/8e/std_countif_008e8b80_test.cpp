#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
namespace std {
template <class Arg, class Result> struct unary_function {
    typedef Arg argument_type;
    typedef Result result_type;
};
template <class Predicate> class unary_negate : public unary_function<typename Predicate::argument_type, bool> {
protected:
    Predicate m_pred;
public:
    explicit unary_negate(const Predicate& x) : m_pred(x) {}
    bool operator()(const typename Predicate::argument_type& x) const { return !m_pred(x); }
};
template <class InputIter, class Predicate>
int __fastcall count_if(InputIter first, InputIter last, Predicate pred)
{
    int n = 0;
    for ( ; first != last; ++first)
        if (pred(*first))
            ++n;
    return n;
}
}
class CThing { public: bool m_inMode; };
template <class T> class CIntelligentPointer {
public:
    void* m_vtable;
    T* m_pItem;
};
class CThingFilter_IsNotInMode : public std::unary_function<CIntelligentPointer<CThing>, bool> {
public:
    CThingFilter_IsNotInMode() {}
    bool operator()(const CIntelligentPointer<CThing>& thing) const { return !thing.m_pItem || !thing.m_pItem->m_inMode; }
};
int main()
{
    CThing t[3]; t[0].m_inMode = true; t[1].m_inMode = false; t[2].m_inMode = true;
    CIntelligentPointer<CThing> p[4];
    for (int i = 0; i < 3; ++i) { p[i].m_vtable = 0; p[i].m_pItem = &t[i]; }
    p[3].m_vtable = 0; p[3].m_pItem = 0;
    int n = std::count_if((const CIntelligentPointer<CThing>*)p, (const CIntelligentPointer<CThing>*)p + 4, std::unary_negate<CThingFilter_IsNotInMode>(CThingFilter_IsNotInMode()));
    if (n != 2) { std::printf("008e8b80_TEST FAIL\n"); return 1; }
    n = std::count_if((const CIntelligentPointer<CThing>*)p, (const CIntelligentPointer<CThing>*)p + 2, std::unary_negate<CThingFilter_IsNotInMode>(CThingFilter_IsNotInMode()));
    if (n != 1) { std::printf("008e8b80_TEST FAIL\n"); return 1; }
    std::printf("008e8b80_TEST PASS\n");
    return 0;
}