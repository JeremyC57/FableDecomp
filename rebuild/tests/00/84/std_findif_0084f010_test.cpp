// Behaviour test for the STLport __find_if<CCountedPointer<CSubComponentBase>*, CIsOfType> (0x0084F010): the first
// element whose type matches, else last, for every remainder of the 4x unrolled loop.
#include <cstdio>
struct random_access_iterator_tag { };
struct CSubComponentBase {
    int Type;
    virtual ~CSubComponentBase() { }
    virtual int GetType() const { return Type; }
};
struct CCountedPointer {
    CSubComponentBase* Data;
    void* Info;
    CSubComponentBase* operator->() const { return Data; }
};
struct CIsOfType {
    int Type;
    bool operator()(const CCountedPointer& p) const { return p->GetType() == Type; }
};

static CCountedPointer* find_if_ra(CCountedPointer* first, CCountedPointer* last, CIsOfType pred, const random_access_iterator_tag&)
{
    int trip = (last - first) >> 2;
    for ( ; trip > 0; --trip) {
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (pred(*first)) return first;
        ++first;
    case 2:
        if (pred(*first)) return first;
        ++first;
    case 1:
        if (pred(*first)) return first;
    case 0:
    default:
        return last;
    }
}

int main()
{
    CSubComponentBase objs[11];
    CCountedPointer v[11];
    for (int i = 0; i < 11; ++i) { objs[i].Type = 100 + i; v[i].Data = &objs[i]; v[i].Info = 0; }
    random_access_iterator_tag tag;
    for (int n = 0; n <= 11; ++n) {
        for (int want = 0; want < 12; ++want) {
            CIsOfType pred;
            pred.Type = 100 + want;
            CCountedPointer* r = find_if_ra(v, v + n, pred, tag);
            CCountedPointer* expect = want < n ? v + want : v + n;
            if (r != expect) { std::printf("0084f010_TEST FAIL\n"); return 1; }
        }
    }
    std::printf("0084f010_TEST PASS\n");
    return 0;
}