// Behaviour test for std::vector<CActiveSoundInfo>::push_back(const CActiveSoundInfo&) @ 0x00AACEF0
#include <cstdio>
#include <new>

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
    virtual ~CActiveSoundInfo() { }
};

struct __false_type { __false_type() { } };

static int g_overflows = 0;
static unsigned int g_fill = 0;
static bool g_atEnd = false;

struct CSoundVector {
    CActiveSoundInfo* _M_start;
    CActiveSoundInfo* _M_finish;
    CActiveSoundInfo* _M_end_of_storage;
    void _M_insert_overflow(CActiveSoundInfo* position, const CActiveSoundInfo&, const __false_type&, unsigned int fillLen, bool atEnd)
    {
        ++g_overflows; g_fill = fillLen; g_atEnd = atEnd;
        if (position != _M_finish) g_overflows = 100;
    }
    void push_back(const CActiveSoundInfo& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CActiveSoundInfo(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};

union SBuffer { double align; unsigned char bytes[sizeof(CActiveSoundInfo) * 2]; };

int main()
{
    int a = 7, b = 8;
    SBuffer buffer;
    CSoundVector v;
    v._M_start = v._M_finish = (CActiveSoundInfo*)buffer.bytes;
    v._M_end_of_storage = v._M_start + 2;
    CActiveSoundInfo x;
    x.m_pSound = &a; x.m_pOwner = &b; x.m_pInfo = &a;
    v.push_back(x);
    v.push_back(x);
    int ok = (v._M_finish == v._M_start + 2) && g_overflows == 0;
    if (v._M_start[1].m_pSound != &a || v._M_start[1].m_pOwner != &b || v._M_start[0].m_pInfo != &a) ok = 0;
    v.push_back(x);
    if (g_overflows != 1 || g_fill != 1 || !g_atEnd || v._M_finish != v._M_start + 2) ok = 0;
    v._M_start[0].~CActiveSoundInfo();
    v._M_start[1].~CActiveSoundInfo();
    if (!ok) { std::printf("00aacef0_TEST FAIL\n"); return 1; }
    std::printf("00aacef0_TEST PASS\n");
    return 0;
}