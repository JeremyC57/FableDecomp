// Behaviour test for 0x0080E2F0 std::remove_if over pair<CIntelligentPointer<CThing>, long> with CIsFirstElementEqualTo:
// entries whose thing is the predicate's thing are dropped, the rest keep their order, the new end is returned.
#include <stdio.h>
#include <stdlib.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __declspec(x)
#endif
class CBaseObject;
class CThing;
class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                          // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
    CBaseObject* PData;                                  // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    virtual void v00();                                  // this instantiation's vtable 0x01238C6C
    CIntelligentPointer(const CIntelligentPointer<T>& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    explicit CIntelligentPointer(CBaseObject* p) : CBaseIntelligentPointer() { SetPItem(p); }   // test-only
};
namespace std {
template <class T1, class T2> struct pair {
    T1 first;                                            // +0x00
    T2 second;                                           // +0x08
    pair(const T1& a, const T2& b) : first(a), second(b) {}
};
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };
template <class T> inline random_access_iterator_tag iterator_category(const T*) { return random_access_iterator_tag(); }
}
namespace {
class CIsFirstElementEqualTo {                           // 8 bytes (Ego_r.pdb)
public:
    CIntelligentPointer<CThing> Value;                   // +0x00
    bool operator()(const std::pair<CIntelligentPointer<CThing>, long>& entry) const;
};
}
namespace std {
template <class RandomAccessIter, class Predicate>
__declspec(noinline) RandomAccessIter __fastcall __find_if(RandomAccessIter first, RandomAccessIter last, Predicate pred,
                                                          const random_access_iterator_tag&)
{
    int trip_count = (last - first) >> 2;
    for (; trip_count > 0; --trip_count) {
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
template <class InputIter, class OutputIter, class Predicate>
OutputIter __fastcall remove_copy_if(InputIter first, InputIter last, OutputIter result, Predicate pred);           // 0x0080DF90

template <class InputIter, class Predicate>
inline InputIter find_if(InputIter first, InputIter last, Predicate pred)
{
    return __find_if(first, last, pred, random_access_iterator_tag());
}

template <class ForwardIter, class Predicate>
ForwardIter __fastcall remove_if(ForwardIter first, ForwardIter last, Predicate pred)
{
    first = find_if(first, last, pred);
    if (first != last) {
        ForwardIter next = first;
        first = remove_copy_if(++next, last, first, pred);
    }
    return first;
}

}


static int g_live = 0;
void CBaseIntelligentPointer::v00() { }
CBaseIntelligentPointer::CBaseIntelligentPointer() : PData(0) { ++g_live; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { --g_live; }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return PData; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { PData = item; }
template <> void CIntelligentPointer<CThing>::v00() { }
namespace {
bool CIsFirstElementEqualTo::operator()(const std::pair<CIntelligentPointer<CThing>, long>& entry) const
{
    return entry.first.GetPItem() == Value.GetPItem();
}
}
namespace std {
template <class InputIter, class OutputIter, class Predicate>
OutputIter __fastcall remove_copy_if(InputIter first, InputIter last, OutputIter result, Predicate pred)
{
    for (; first != last; ++first)
        if (!pred(*first)) { *result = *first; ++result; }
    return result;
}
}

int main()
{
    typedef std::pair<CIntelligentPointer<CThing>, long> E;
    int things[4];
    int pick[7] = { 0, 1, 0, 2, 0, 3, 0 };
    E* a = (E*)malloc(7 * sizeof(E));
    for (int k = 0; k < 7; ++k)
        new (&a[k]) E(CIntelligentPointer<CThing>((CBaseObject*)&things[pick[k]]), 10 + k);
    int live0 = g_live;
    CIsFirstElementEqualTo* pred = (CIsFirstElementEqualTo*)malloc(sizeof(CIsFirstElementEqualTo));
    new (&pred->Value) CIntelligentPointer<CThing>((CBaseObject*)&things[0]);
    int ok = 1;
    E* end = std::remove_if(a, a + 7, *pred);
    if (end != a + 3) ok = 0;
    long wantId[3] = { 11, 13, 15 };
    for (int k = 0; k < 3 && ok; ++k)
        if (a[k].first.GetPItem() != (CBaseObject*)&things[k + 1] || a[k].second != wantId[k]) ok = 0;
    if (g_live != live0 + 1) ok = 0;                        // every predicate copy was destroyed again
    CIsFirstElementEqualTo* none = (CIsFirstElementEqualTo*)malloc(sizeof(CIsFirstElementEqualTo));
    new (&none->Value) CIntelligentPointer<CThing>((CBaseObject*)0);
    if (std::remove_if(a, a + 3, *none) != a + 3) ok = 0;    // nothing matches: last is returned
    printf(ok ? "0080e2f0_TEST PASS\n" : "0080e2f0_TEST FAIL\n");
    return ok ? 0 : 1;
}