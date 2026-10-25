// Behaviour test for 0x00748AA0 std::__uninitialized_fill_n over CTCStealth danger-thing pairs
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

template <class ForwardIter, class Size, class T>
ForwardIter __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}

int main()
{
    CThing thing;
    thing.id = 7;
    TDangerPair x;
    Set(x, &thing, 2, 55);
    static unsigned char raw[sizeof(TDangerPair) * 4];
    TDangerPair* dst = (TDangerPair*)raw;
    g_baseCtors = 0;
    TDangerPair* end = __uninitialized_fill_n(dst, 3u, x, __false_type());
    int ok = 1;
    if (end != dst + 3 || g_baseCtors != 3) ok = 0;
    for (int i = 0; i < 3; ++i)
        if (dst[i].first.Get() != &thing || dst[i].second.DangerStatus != 2 || dst[i].second.Timer != 55) ok = 0;
    if (__uninitialized_fill_n(dst + 3, 0u, x, __false_type()) != dst + 3 || g_baseCtors != 3) ok = 0;   // n == 0
    for (int i = 0; i < 3; ++i) dst[i].~TDangerPair();
    if (!ok) { std::printf("00748aa0_TEST FAIL\n"); return 1; }
    std::printf("00748aa0_TEST PASS\n");
    return 0;
}