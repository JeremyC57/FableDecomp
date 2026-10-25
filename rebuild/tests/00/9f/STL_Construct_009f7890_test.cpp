// behaviour test for 0x009F7890 std::_Construct(CVsyncCallbackTimer::CPendingQuery*, const CPendingQuery&): placement copy of the four words
#include <cstdio>
#include <new>

struct IDirect3DQuery9 { int m_dummy; };

class CVsyncCallbackTimer {
public:
    struct CPendingQuery {
        IDirect3DQuery9* Query;
        long             Frame;
        long             Data0;
        long             Data1;
    };
};

namespace std {
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val)
{
    new (__p) _T1(__val);
}
}

int main()
{
    IDirect3DQuery9 query;
    CVsyncCallbackTimer::CPendingQuery source;
    source.Query = &query;
    source.Frame = 1234;
    source.Data0 = -5;
    source.Data1 = 77;
    CVsyncCallbackTimer::CPendingQuery slot[2];
    slot[1].Query = 0; slot[1].Frame = 99; slot[1].Data0 = 98; slot[1].Data1 = 97;
    std::_Construct(&slot[0], source);
    if (slot[0].Query != &query || slot[0].Frame != 1234 || slot[0].Data0 != -5 || slot[0].Data1 != 77) { std::printf("009f7890_TEST FAIL\n"); return 1; }
    if (slot[1].Frame != 99 || slot[1].Data1 != 97) { std::printf("009f7890_TEST FAIL\n"); return 1; }
    std::printf("009f7890_TEST PASS\n");
    return 0;
}