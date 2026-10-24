// Behaviour test for 00674ce0: placement-copy 16-byte CVectorMap records (the entry vector through its out-of-line copy ctor, the Dirty byte copied)
#include <cstdio>
#include <new>

static int g_vectorCopies = 0;

struct SEntry12 { int a, b, c; };
struct CEntryVector {
    SEntry12* Start;
    SEntry12* Finish;
    SEntry12* EndOfStorage;
    CEntryVector() : Start(0), Finish(0), EndOfStorage(0) { }
    CEntryVector(const CEntryVector& other)              // retail 0x00673E10 (stub: share the block)
        : Start(other.Start), Finish(other.Finish), EndOfStorage(other.EndOfStorage) { ++g_vectorCopies; }
};
struct CKeyPairCompareLess { };
struct CVectorMap12 {
    CEntryVector Entries;
    CKeyPairCompareLess Compare;
    bool Dirty;
    CVectorMap12() : Dirty(false) { }
};

struct FalseType { FalseType() { } };

template <class T1, class T2>
inline void Construct(T1* p, const T2& value) { new (p) T1(value); }

CVectorMap12* UninitCopy(const CVectorMap12* first, const CVectorMap12* last, CVectorMap12* result, const FalseType&)
{
    CVectorMap12* cur = result;
    for (; first != last; ++first, ++cur)
        Construct(cur, *first);
    return cur;
}

int main()
{
    SEntry12 block[4];
    CVectorMap12 src[3];
    for (int i = 0; i < 3; ++i) {
        src[i].Entries.Start = block;
        src[i].Entries.Finish = block + i;
        src[i].Entries.EndOfStorage = block + 4;
        src[i].Dirty = (i & 1) != 0;
    }
    void* raw = ::operator new(3 * sizeof(CVectorMap12));
    CVectorMap12* dst = static_cast<CVectorMap12*>(raw);
    CVectorMap12* end = UninitCopy(src, src + 3, dst, FalseType());
    bool ok = end == dst + 3 && g_vectorCopies == 3;
    for (int i = 0; i < 3; ++i)
        ok = ok && dst[i].Entries.Finish == block + i && dst[i].Dirty == ((i & 1) != 0);
    ::operator delete(raw);
    if (!ok) { std::printf("00674ce0_TEST FAIL\n"); return 1; }
    std::printf("00674ce0_TEST PASS\n");
    return 0;
}