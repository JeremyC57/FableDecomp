// Behaviour test for 0x007AF940 std::__uninitialized_fill_n over CIntelligentPointer<CThingAICreature>:
// n placement copies of x, each copy built by the base ctor then SetPItem(other.GetPItem()); returns first + n.
#include <cstdio>
#include <new>
class CBaseObject { public: int id; };
class CBaseIntelligentPointer {
public:
    CBaseObject* PData;
    CBaseIntelligentPointer() : PData(0) { }
    virtual ~CBaseIntelligentPointer() { }
    CBaseObject* GetPItem() const { return PData; }
    void SetPItem(CBaseObject* item) { PData = item; }
};
class CThingAICreature;
template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) { CBaseObject* item = other.GetPItem(); SetPItem(item); }
};
struct false_type_tag { };
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter uninitialized_fill_n_model(_ForwardIter __first, _Size __n, const _Tp& __x, const false_type_tag&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        new ((void*)&*__cur) _Tp(__x);
    return __cur;
}
typedef CIntelligentPointer<CThingAICreature> P;
int main()
{
    CBaseObject obj; obj.id = 7;
    P x; x.SetPItem(&obj);
    static unsigned char raw[sizeof(P) * 6];
    P* first = reinterpret_cast<P*>(raw);
    P* end = uninitialized_fill_n_model(first, 5u, x, false_type_tag());
    if (end != first + 5) { std::printf("007af940_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i)
        if (first[i].GetPItem() != &obj) { std::printf("007af940_TEST FAIL\n"); return 1; }
    P* end0 = uninitialized_fill_n_model(first + 5, 0u, x, false_type_tag());
    if (end0 != first + 5) { std::printf("007af940_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i) first[i].~P();
    std::printf("007af940_TEST PASS\n");
    return 0;
}