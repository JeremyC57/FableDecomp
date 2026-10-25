// Behaviour test for 0x00748A40 std::__uninitialized_copy over CTCStealth danger-thing pairs
#include <cstdio>
#include <new>
class CThing { public: int id; };

static int g_baseCtors = 0, g_sets = 0;
class CBaseIntelligentPointer {                         // stand-in for the 0x00A01Bxx helpers
public:
    const void* PItem;
    CBaseIntelligentPointer() : PItem(0) { ++g_baseCtors; }
    virtual ~CBaseIntelligentPointer() {}
    const void* GetPItem() const { return PItem; }
    void SetPItem(const void* item) { ++g_sets; PItem = item; }
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() {}
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    T* Get() const { return (T*)GetPItem(); }
};

namespace NStealth {
enum EDangerStatus { DANGER_STATUS_NONE = 0, DANGER_STATUS_SUSPICIOUS = 1, DANGER_STATUS_DANGER = 2 };
class CDangerStatus {
public:
    EDangerStatus DangerStatus;
    int           Timer;
};
}

template <class A, class B> struct pair {
    A first;
    B second;
    pair() {}
    pair(const pair& other) : first(other.first), second(other.second) {}
};

typedef pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus> TDangerPair;

static void Set(TDangerPair& e, const CThing* thing, int status, int timer)
{
    e.first.SetPItem(thing);
    e.second.DangerStatus = (NStealth::EDangerStatus)status;
    e.second.Timer = timer;
}

struct __false_type { __false_type() {} };

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

template <class InputIter, class ForwardIter>
ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}

int main()
{
    CThing things[5];
    TDangerPair src[5];
    for (int i = 0; i < 5; ++i) { things[i].id = i; Set(src[i], &things[i], i % 3, 100 + i); }
    static unsigned char raw[sizeof(TDangerPair) * 6];
    TDangerPair* dst = (TDangerPair*)raw;
    g_baseCtors = 0; g_sets = 0;
    TDangerPair* end = __uninitialized_copy(src, src + 5, dst, __false_type());
    int ok = 1;
    if (end != dst + 5) ok = 0;
    if (g_baseCtors != 5 || g_sets != 5) ok = 0;            // one intelligent-pointer construction per entry
    for (int i = 0; i < 5; ++i) {
        if (dst[i].first.Get() != &things[i]) ok = 0;
        if (dst[i].second.DangerStatus != i % 3 || dst[i].second.Timer != 100 + i) ok = 0;
    }
    if (__uninitialized_copy(src, src, dst + 5, __false_type()) != dst + 5) ok = 0;   // empty range
    if (g_baseCtors != 5) ok = 0;
    for (int i = 0; i < 5; ++i) dst[i].~TDangerPair();
    if (!ok) { std::printf("00748a40_TEST FAIL\n"); return 1; }
    std::printf("00748a40_TEST PASS\n");
    return 0;
}