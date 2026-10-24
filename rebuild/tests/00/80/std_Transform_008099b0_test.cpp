// Behaviour test for 008099b0: std::_Transform over CThing** into a back_insert_iterator of vector<CIntelligentPointer<CThing>>: wrap each raw pointer, push it (in place while capacity lasts, else through _M_insert_overflow)
#include <cstdio>
#include <new>

class CThing { public: int Id; };
static int g_overflows = 0;

class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer() : PData(0) { }                          // retail 0x00A01B10
    virtual ~CBaseIntelligentPointer() { }                            // retail 0x00A01C10
    void* GetPItem() const { return PData; }                          // retail 0x00A01B50
    void SetPItem(void* item) { PData = item; }                       // retail 0x00A01B90
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
template <class T> class Vector {
public:
    typedef T value_type;
    T* Start;
    T* Finish;
    T* EndOfStorage;
    T* Spill;                                   // stub storage for the overflow path
    void InsertOverflow(T* position, const T& x)   // retail 0x004AC940 (stub: continue in the spill block)
    {
        (void)position;
        ++g_overflows;
        Start = Spill; Finish = Spill; EndOfStorage = Spill + 8;
        new (Finish) T(x);
        ++Finish;
    }
    void push_back(const T& x)
    {
        if (Finish != EndOfStorage) {
            new (Finish) T(x);
            ++Finish;
        }
        else
            InsertOverflow(Finish, x);
    }
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
struct CToIntelligentPointer {
    CIntelligentPointer<CThing> operator()(CThing* thing) const { return CIntelligentPointer<CThing>(thing); }
};

int main()
{
    CThing things[3] = { { 1 }, { 2 }, { 3 } };
    CThing* src[3] = { &things[0], &things[1], &things[2] };
    void* rawA = ::operator new(2 * sizeof(CIntelligentPointer<CThing>));
    void* rawB = ::operator new(8 * sizeof(CIntelligentPointer<CThing>));
    Vector<CIntelligentPointer<CThing> > v;
    v.Start = v.Finish = static_cast<CIntelligentPointer<CThing>*>(rawA);
    v.EndOfStorage = v.Start + 2;
    v.Spill = static_cast<CIntelligentPointer<CThing>*>(rawB);
    BackInsertIterator<Vector<CIntelligentPointer<CThing> > > it =
        Transform(src, src + 3, BackInsertIterator<Vector<CIntelligentPointer<CThing> > >(v), CToIntelligentPointer());
    CIntelligentPointer<CThing>* a = static_cast<CIntelligentPointer<CThing>*>(rawA);
    bool ok = it.container == &v && g_overflows == 1;
    ok = ok && a[0].GetPItem() == &things[0] && a[1].GetPItem() == &things[1];
    ok = ok && v.Start == v.Spill && v.Finish == v.Spill + 1 && v.Spill[0].GetPItem() == &things[2];
    ::operator delete(rawA);
    ::operator delete(rawB);
    if (!ok) { std::printf("008099b0_TEST FAIL\n"); return 1; }
    std::printf("008099b0_TEST PASS\n");
    return 0;
}