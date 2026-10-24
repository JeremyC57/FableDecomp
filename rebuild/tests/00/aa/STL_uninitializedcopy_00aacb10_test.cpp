// Behaviour test for _STL::__uninitialized_copy<CActiveSoundInfo*> @ 0x00AACB10
#include <cstdio>
#include <new>

static int g_copies = 0;

class CBaseClass {
public:
    CBaseClass() { }
    virtual ~CBaseClass() { }
};

class CActiveSoundInfo : public CBaseClass {
public:
    void* m_pSound;
    void* m_pOwner;
    void* m_pInfo;
    CActiveSoundInfo() : m_pSound(0), m_pOwner(0), m_pInfo(0) { }
    CActiveSoundInfo(const CActiveSoundInfo& o) : CBaseClass(), m_pSound(o.m_pSound), m_pOwner(o.m_pOwner), m_pInfo(o.m_pInfo) { ++g_copies; }
    virtual ~CActiveSoundInfo() { }
};

struct __false_type { __false_type() { } };
template <class T1, class T2> inline void _Construct(T1* p, const T2& v) { new (p) T1(v); }

template <class InputIter, class ForwardIter>
ForwardIter uninitialized_copy_model(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for ( ; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}

union SBuffer { double align; unsigned char bytes[sizeof(CActiveSoundInfo) * 4]; };

int main()
{
    int a = 1, b = 2, c = 3;
    CActiveSoundInfo src[3];
    for (int i = 0; i < 3; ++i) { src[i].m_pSound = &a; src[i].m_pOwner = &b; src[i].m_pInfo = &c; }
    src[1].m_pInfo = &a;
    SBuffer buffer;
    CActiveSoundInfo* dst = (CActiveSoundInfo*)buffer.bytes;
    CActiveSoundInfo* end = uninitialized_copy_model(src, src + 3, dst, __false_type());
    int ok = (end == dst + 3) && g_copies == 3;
    for (int i = 0; i < 3 && ok; ++i)
        if (dst[i].m_pSound != src[i].m_pSound || dst[i].m_pOwner != src[i].m_pOwner || dst[i].m_pInfo != src[i].m_pInfo) ok = 0;
    CActiveSoundInfo* none = uninitialized_copy_model(src, src, dst, __false_type());
    if (none != dst || g_copies != 3) ok = 0;
    for (int i = 0; i < 3; ++i) dst[i].~CActiveSoundInfo();
    if (!ok) { std::printf("00aacb10_TEST FAIL\n"); return 1; }
    std::printf("00aacb10_TEST PASS\n");
    return 0;
}