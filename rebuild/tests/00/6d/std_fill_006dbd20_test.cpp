#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
// std::fill(first, last, value) -- Assign a value to each element in a range.
// Leaf: ??$fill@PAVCAttackHistoryCombo@@V1@@std@@YIXPAVCAttackHistoryCombo@@0ABV1@@Z
// Flags: /Gr
namespace std {
template<class A, class B> struct pair { A first; B second; };
template<class Iterator, class T>
void __fastcall fill(Iterator first, Iterator last, const T& value)
{
    for (; first != last; ++first) *first = value;
}
}
enum EAttackHistoryType { AttackHistoryDefault };
enum EDamageAttribute { DamageAttributeDefault };
namespace std {
template<class T> class allocator { public: allocator() {} };
template<class V, class A> class _STLP_alloc_proxy : public A { public: V _M_data; };
template<class T, class A> class _Vector_base { public: T* _M_start; T* _M_finish; _STLP_alloc_proxy<T*, A> _M_end_of_storage; };
template<class T, class A = allocator<T> > class vector : public _Vector_base<T,A> {
public: vector& operator=(const vector&);
};
}
class CAttackHistoryCombo {
public:
    std::vector<std::pair<EAttackHistoryType, EDamageAttribute> > AttackCombo;
    float Multiplier;
};

template void __fastcall std::fill<CAttackHistoryCombo*, CAttackHistoryCombo >(CAttackHistoryCombo*, CAttackHistoryCombo*, const CAttackHistoryCombo&);
static int assignments;
namespace std {
template<class T,class A> vector<T,A>& vector<T,A>::operator=(const vector<T,A>& rhs) {
    ++assignments;
    this->_M_start=rhs._M_start;
    this->_M_finish=rhs._M_finish;
    this->_M_end_of_storage._M_data=rhs._M_end_of_storage._M_data;
    return *this;
}
}
int main() {
    CAttackHistoryCombo a[5], value;
    std::pair<EAttackHistoryType, EDamageAttribute> items[2];
    value.AttackCombo._M_start=items;value.AttackCombo._M_finish=items+2;value.AttackCombo._M_end_of_storage._M_data=items+2;value.Multiplier=2.0f;
    for(int i=0;i<5;++i){ a[i].AttackCombo._M_start=0;a[i].AttackCombo._M_finish=0;a[i].AttackCombo._M_end_of_storage._M_data=0;a[i].Multiplier=-1.0f; }
    std::fill(a+1,a+4,value);
    int ok=assignments==3 && a[0].Multiplier==-1.0f && a[4].Multiplier==-1.0f;
    for(int j=1;j<4;++j)ok=ok && a[j].Multiplier==2.0f && a[j].AttackCombo._M_start==items && a[j].AttackCombo._M_finish==items+2;
    std::fill(a+2,a+2,value);ok=ok && assignments==3;
    std::fill(a+1,a+4,a[2]);ok=ok && assignments==6 && a[3].Multiplier==2.0f;
    puts(ok ? "006dbd20_TEST PASS" : "006dbd20_TEST FAIL");return ok ? 0 : 1;
}