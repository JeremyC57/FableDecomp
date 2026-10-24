// Behaviour test for 0x0074AC80 CTCShop::AddShopper(CThingCreatureBase*)
#include <cstdio>
#include <cstdlib>
#include <new>

class CThingCreatureBase { public: int id; };

class CBaseIntelligentPointer {
public:
    const void* item;
    CBaseIntelligentPointer() : item(0) {}
    virtual ~CBaseIntelligentPointer() {}
    const void* GetPItem() const { return item; }
    void SetPItem(const void* p) { item = p; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* p) { SetPItem(p); }
    CIntelligentPointer(const CIntelligentPointer& o) : CBaseIntelligentPointer() { SetPItem(o.GetPItem()); }
    bool operator==(const CIntelligentPointer& o) const { return GetPItem() == o.GetPItem(); }
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };
static int g_finds = 0;
template <class RandomAccessIter, class T>
RandomAccessIter __find(RandomAccessIter first, RandomAccessIter last, const T& val, const random_access_iterator_tag&)
{
    ++g_finds;
    for (; first != last; ++first)
        if (*first == val)
            return first;
    return last;
}
template <class InputIter, class T>
inline InputIter find(InputIter first, InputIter last, const T& val)
{
    return __find(first, last, val, random_access_iterator_tag());
}

template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    void push_back(const T& x) { new (_M_finish) T(x); ++_M_finish; }   // stand-in (capacity preallocated)
};

class CTCShop {
public:
    vector<CIntelligentPointer<CThingCreatureBase> > Shoppers;
    void AddShopper(CThingCreatureBase* shopper)
    {
        CIntelligentPointer<CThingCreatureBase> pShopper(shopper);
        CIntelligentPointer<CThingCreatureBase>* it = find(Shoppers.begin(), Shoppers.end(), pShopper);
        if (it == Shoppers.end())
            Shoppers.push_back(pShopper);
    }
};

int main()
{
    CThingCreatureBase a, b, c;
    CTCShop shop;
    void* raw = std::malloc(sizeof(CIntelligentPointer<CThingCreatureBase>) * 8);
    shop.Shoppers._M_start = shop.Shoppers._M_finish = (CIntelligentPointer<CThingCreatureBase>*)raw;
    shop.Shoppers._M_end_of_storage = shop.Shoppers._M_start + 8;
    int ok = 1;
    shop.AddShopper(&a);
    shop.AddShopper(&b);
    shop.AddShopper(&a);                                  // already shopping: not added again
    shop.AddShopper(&c);
    shop.AddShopper(&b);
    if (shop.Shoppers._M_finish - shop.Shoppers._M_start != 3) ok = 0;
    if (shop.Shoppers._M_start[0].GetPItem() != &a || shop.Shoppers._M_start[1].GetPItem() != &b
        || shop.Shoppers._M_start[2].GetPItem() != &c) ok = 0;
    if (g_finds != 5) ok = 0;
    for (CIntelligentPointer<CThingCreatureBase>* p = shop.Shoppers._M_start; p != shop.Shoppers._M_finish; ++p)
        p->~CIntelligentPointer<CThingCreatureBase>();
    std::free(raw);
    if (!ok) { std::printf("0074ac80_TEST FAIL\n"); return 1; }
    std::printf("0074ac80_TEST PASS\n");
    return 0;
}