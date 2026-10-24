// Behaviour test for _STL::__uninitialized_fill_n<CActiveSoundInfo*> @ 0x00AACB50
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

template <class ForwardIter, class Size, class T>
ForwardIter uninitialized_fill_n_model(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for ( ; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}

union SBuffer { double align; unsigned char bytes[sizeof(CActiveSoundInfo) * 5]; };

int main()
{
    int a = 1, b = 2, c = 3;
    CActiveSoundInfo proto;
    proto.m_pSound = &a; proto.m_pOwner = &b; proto.m_pInfo = &c;
    SBuffer buffer;
    CActiveSoundInfo* dst = (CActiveSoundInfo*)buffer.bytes;
    CActiveSoundInfo* end = uninitialized_fill_n_model(dst, 4u, proto, __false_type());
    int ok = (end == dst + 4) && g_copies == 4;
    for (int i = 0; i < 4 && ok; ++i)
        if (dst[i].m_pSound != &a || dst[i].m_pOwner != &b || dst[i].m_pInfo != &c) ok = 0;
    if (uninitialized_fill_n_model(dst, 0u, proto, __false_type()) != dst || g_copies != 4) ok = 0;
    for (int i = 0; i < 4; ++i) dst[i].~CActiveSoundInfo();
    if (!ok) { std::printf("00aacb50_TEST FAIL\n"); return 1; }
    std::printf("00aacb50_TEST PASS\n");
    return 0;
}