// Behaviour test for 00a32f40: placement-copy 16-byte pair<unsigned int, C3DMeshVolume> entries (the volume name through the CCharString copy ctor, the counted convex volume add-ref'd)
#include <cstdio>
#include <new>

static int g_stringCopies = 0;

class CCharString {
public:
    CCharString() : PData(0) { }
    CCharString(const CCharString& other) : PData(other.PData) { ++g_stringCopies; }   // retail 0x0099EC30
    void* PData;
};
class CCPPointerInfo { public: unsigned int RefCount; };
template <class T> class CCountedPointer { public:
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};
class CContainmentVolume { public: int Id; };
class C3DMeshVolume { public:
    CCharString Name;
    CCountedPointer<CContainmentVolume> ConvexVolume;
};
template <class T1, class T2>
struct Pair {
    T1 first;
    T2 second;
    Pair() { }
    Pair(const Pair<T1, T2>& o) : first(o.first), second(o.second) { }
};
typedef Pair<unsigned int, C3DMeshVolume> PairEntry;

struct FalseType { FalseType() { } };

template <class T1, class T2>
inline void Construct(T1* p, const T2& value) { new (p) T1(value); }

PairEntry* UninitCopy(const PairEntry* first, const PairEntry* last, PairEntry* result, const FalseType&)
{
    PairEntry* cur = result;
    for (; first != last; ++first, ++cur)
        Construct(cur, *first);
    return cur;
}

static void SetKey(unsigned int& key, int i, char*) { key = 0x40u + i; }
static void SetKey(CCharString& key, int i, char* text) { key.PData = text + i; }
static bool KeyOk(const unsigned int& key, int i, char*) { return key == 0x40u + i; }
static bool KeyOk(const CCharString& key, int i, char* text) { return key.PData == text + i; }

int main()
{
    static char text[16];
    CContainmentVolume vol = { 9 };
    CCPPointerInfo info = { 2 };
    PairEntry src[3];
    for (int i = 0; i < 3; ++i) {
        SetKey(src[i].first, i, text);
        src[i].second.Name.PData = text + 8 + i;
        src[i].second.ConvexVolume.Data = &vol;
        src[i].second.ConvexVolume.Info = (i == 0) ? 0 : &info;
    }
    void* raw = ::operator new(3 * sizeof(PairEntry));
    PairEntry* dst = static_cast<PairEntry*>(raw);
    int before = g_stringCopies;
    PairEntry* end = UninitCopy(src, src + 3, dst, FalseType());
    int perEntry = (g_stringCopies - before) / 3;
    bool ok = end == dst + 3 && info.RefCount == 4 && (g_stringCopies - before) % 3 == 0 && perEntry >= 1;
    for (int i = 0; i < 3; ++i)
        ok = ok && KeyOk(dst[i].first, i, text) && dst[i].second.Name.PData == text + 8 + i
                && dst[i].second.ConvexVolume.Data == &vol && dst[i].second.ConvexVolume.Info == src[i].second.ConvexVolume.Info;
    ::operator delete(raw);
    if (!ok) { std::printf("00a32f40_TEST FAIL\n"); return 1; }
    std::printf("00a32f40_TEST PASS\n");
    return 0;
}