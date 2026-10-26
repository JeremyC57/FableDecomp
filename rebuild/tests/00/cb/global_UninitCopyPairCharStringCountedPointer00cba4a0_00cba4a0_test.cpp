// Behaviour test for 00cba4a0: placement-copy 12-byte pair<CCharString, CCountedPointer<...>> entries (name copy ctor, counted pointer add-ref'd)
#include <cstdio>
#include <new>

static int g_stringCopies = 0;

class CCharString {
public:
    CCharString() : PData(0) { }
    CCharString(const CCharString& other) : PData(other.PData) { ++g_stringCopies; }   // retail 0x0099EC30
    void* PData;
};
struct CCountBlock { unsigned int RefCount; };
template <class T> class CCountedPointer { public:
    T* Data;
    CCountBlock* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};
struct CCountedObject { int Id; };
template <class T1, class T2>
struct Pair {
    T1 first;
    T2 second;
    Pair() { }
    Pair(const Pair<T1, T2>& o) : first(o.first), second(o.second) { }
};
typedef Pair<CCharString, CCountedPointer<CCountedObject> > PairCharStringCountedPointer;

struct FalseType { FalseType() { } };

template <class T1, class T2>
inline void Construct(T1* p, const T2& value) { new (p) T1(value); }

PairCharStringCountedPointer* UninitCopy(const PairCharStringCountedPointer* first, const PairCharStringCountedPointer* last, PairCharStringCountedPointer* result, const FalseType&)
{
    PairCharStringCountedPointer* cur = result;
    for (; first != last; ++first, ++cur)
        Construct(cur, *first);
    return cur;
}

int main()
{
    static char text[8];
    CCountedObject obj = { 2 };
    CCountBlock count = { 0 };
    PairCharStringCountedPointer src[3];
    for (int i = 0; i < 3; ++i) {
        src[i].first.PData = text + i;
        src[i].second.Data = &obj;
        src[i].second.Info = (i == 0) ? 0 : &count;
    }
    void* raw = ::operator new(3 * sizeof(PairCharStringCountedPointer));
    PairCharStringCountedPointer* dst = static_cast<PairCharStringCountedPointer*>(raw);
    PairCharStringCountedPointer* end = UninitCopy(src, src + 3, dst, FalseType());
    bool ok = end == dst + 3 && count.RefCount == 2 && g_stringCopies == 3;
    for (int i = 0; i < 3; ++i)
        ok = ok && dst[i].first.PData == text + i && dst[i].second.Data == &obj && dst[i].second.Info == src[i].second.Info;
    ::operator delete(raw);
    if (!ok) { std::printf("00cba4a0_TEST FAIL\n"); return 1; }
    std::printf("00cba4a0_TEST PASS\n");
    return 0;
}