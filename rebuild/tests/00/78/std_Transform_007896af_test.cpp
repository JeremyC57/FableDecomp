// Behaviour test for 007896af: std::_Transform over pair<CIntelligentPointer<CThing>, long> entries (12-byte stride) into a back_insert_iterator of vector<const CThing*>, pushing each handle's thing
#include <cstdio>

class CThing { public: int Id; };
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer() : PData(0) { }
    virtual ~CBaseIntelligentPointer() { }
    void* GetPItem() const { return PData; }   // retail 0x00A01B50 (stub: the handle holds the thing directly)
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
template <class T1, class T2> struct Pair {
    T1 first;
    T2 second;
};
struct CThingPtrVector {
    typedef const CThing* value_type;
    const CThing* Items[8];
    int Count;
    CThingPtrVector() : Count(0) { }
    void push_back(const CThing* const& value) { Items[Count++] = value; }   // retail 0x0049C450
};
template <class C> class BackInsertIterator {
public:
    C* container;
    explicit BackInsertIterator(C& cont) : container(&cont) { }
    BackInsertIterator& operator=(const typename C::value_type& value)
    {
        container->push_back(value);
        return *this;
    }
    BackInsertIterator& operator*() { return *this; }
    BackInsertIterator& operator++() { return *this; }
};
template <class InIt, class OutIt, class Fn1>
OutIt Transform(InIt first, InIt last, OutIt dest, Fn1 func)
{
    for (; first != last; ++first, ++dest)
        *dest = func(*first);
    return dest;
}
struct CGetThingFromIntelligentPointer {
    const CThing* operator()(const Pair<CIntelligentPointer<CThing>, long>& entry) const { return entry.first.GetPItem(); }
    const CThing* operator()(const CIntelligentPointer<CThing>& pointer) const { return pointer.GetPItem(); }
};

int main()
{
    CThing things[4] = { { 1 }, { 2 }, { 3 }, { 4 } };
    Pair<CIntelligentPointer<CThing>, long> src[4];
    for (int i = 0; i < 4; ++i) { src[i].first.PData = &things[i]; src[i].second = i; }
    CThingPtrVector out;
    BackInsertIterator<CThingPtrVector> it = Transform(src, src + 4, BackInsertIterator<CThingPtrVector>(out), CGetThingFromIntelligentPointer());
    bool ok = it.container == &out && out.Count == 4;
    for (int i = 0; i < 4; ++i)
        ok = ok && out.Items[i] == &things[i];
    Transform(src, src, BackInsertIterator<CThingPtrVector>(out), CGetThingFromIntelligentPointer());
    ok = ok && out.Count == 4;
    if (!ok) { std::printf("007896af_TEST FAIL\n"); return 1; }
    std::printf("007896af_TEST PASS\n");
    return 0;
}