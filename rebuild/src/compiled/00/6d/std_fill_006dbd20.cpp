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