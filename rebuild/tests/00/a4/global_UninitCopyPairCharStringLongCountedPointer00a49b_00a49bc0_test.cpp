// Behaviour test for 00a49bc0: placement-copy 16-byte {CCharString, long, CCountedPointer} entries (name copy ctor, value word, counted object add-ref'd)
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
struct CCountedObject { int Id; };
struct SNamedSurfaceEntry {
    CCharString Name;
    long Value;
    CCountedPointer<CCountedObject> Object;
    SNamedSurfaceEntry() : Value(0) { }
};

struct FalseType { FalseType() { } };

template <class T1, class T2>
inline void Construct(T1* p, const T2& value) { new (p) T1(value); }

SNamedSurfaceEntry* UninitCopy(const SNamedSurfaceEntry* first, const SNamedSurfaceEntry* last, SNamedSurfaceEntry* result, const FalseType&)
{
    SNamedSurfaceEntry* cur = result;
    for (; first != last; ++first, ++cur)
        Construct(cur, *first);
    return cur;
}

int main()
{
    static char text[8];
    CCountedObject obj = { 4 };
    CCountBlock count = { 1 };
    SNamedSurfaceEntry src[3];
    for (int i = 0; i < 3; ++i) {
        src[i].Name.PData = text + i;
        src[i].Value = -5 + i;
        src[i].Object.Data = &obj;
        src[i].Object.Info = (i == 2) ? 0 : &count;
    }
    void* raw = ::operator new(3 * sizeof(SNamedSurfaceEntry));
    SNamedSurfaceEntry* dst = static_cast<SNamedSurfaceEntry*>(raw);
    SNamedSurfaceEntry* end = UninitCopy(src, src + 3, dst, FalseType());
    bool ok = end == dst + 3 && count.RefCount == 3 && g_stringCopies == 3;
    for (int i = 0; i < 3; ++i)
        ok = ok && dst[i].Name.PData == text + i && dst[i].Value == -5 + i && dst[i].Object.Data == &obj && dst[i].Object.Info == src[i].Object.Info;
    ::operator delete(raw);
    if (!ok) { std::printf("00a49bc0_TEST FAIL\n"); return 1; }
    std::printf("00a49bc0_TEST PASS\n");
    return 0;
}