// Behaviour test for 00bedb60: placement-copy 16-byte pair<long, CCompositeSurfaceInfo> entries (key word, the info's vptr, its counted cache add-ref'd)
#include <cstdio>
#include <new>

struct CCountBlock { unsigned int RefCount; };
template <class T> struct CCountedPointer {
    T* Data;
    CCountBlock* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};
class CBase {
public:
    virtual ~CBase() { }
};
struct CCompositeSurfaceCache { int Id; };
class CCompositeSurfaceInfo : public CBase {
public:
    CCompositeSurfaceInfo() { }
    virtual ~CCompositeSurfaceInfo() { }
    virtual int Kind() const { return 0x154; }
    CCountedPointer<CCompositeSurfaceCache> Cache;
};
template <class T1, class T2>
struct Pair {
    T1 first;
    T2 second;
    Pair() { }
    Pair(const Pair<T1, T2>& o) : first(o.first), second(o.second) { }
};
typedef Pair<long, CCompositeSurfaceInfo> PairLongCompositeSurfaceInfo;

struct FalseType { FalseType() { } };

template <class T1, class T2>
inline void Construct(T1* p, const T2& value) { new (p) T1(value); }

PairLongCompositeSurfaceInfo* UninitCopy(const PairLongCompositeSurfaceInfo* first, const PairLongCompositeSurfaceInfo* last, PairLongCompositeSurfaceInfo* result, const FalseType&)
{
    PairLongCompositeSurfaceInfo* cur = result;
    for (; first != last; ++first, ++cur)
        Construct(cur, *first);
    return cur;
}

int main()
{
    CCompositeSurfaceCache cache = { 1 };
    CCountBlock count = { 10 };
    PairLongCompositeSurfaceInfo src[3];
    for (int i = 0; i < 3; ++i) {
        src[i].first = 1000 + i;
        src[i].second.Cache.Data = &cache;
        src[i].second.Cache.Info = (i == 1) ? 0 : &count;
    }
    void* raw = ::operator new(3 * sizeof(PairLongCompositeSurfaceInfo));
    PairLongCompositeSurfaceInfo* dst = static_cast<PairLongCompositeSurfaceInfo*>(raw);
    PairLongCompositeSurfaceInfo* end = UninitCopy(src, src + 3, dst, FalseType());
    bool ok = end == dst + 3 && count.RefCount == 12;
    for (int i = 0; i < 3; ++i)
        ok = ok && dst[i].first == 1000 + i && dst[i].second.Kind() == 0x154 && dst[i].second.Cache.Data == &cache
                && dst[i].second.Cache.Info == src[i].second.Cache.Info;
    for (int i = 0; i < 3; ++i)
        dst[i].~PairLongCompositeSurfaceInfo();
    ::operator delete(raw);
    if (!ok) { std::printf("00bedb60_TEST FAIL\n"); return 1; }
    std::printf("00bedb60_TEST PASS\n");
    return 0;
}