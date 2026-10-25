// behaviour test for 0x00AF6980 std::__uninitialized_fill_n<pair<EPSCType,CComponentInstance>*,unsigned int,...>, linked
// against the built function: n copies constructed (vtable, counted-pointer addref, flag and name copied), the end
// pointer returned, n == 0 constructs nothing
#include <cstdio>
#include <cstring>
#include <new>
namespace NParticleEngine { enum EPSCType { PSC_TYPE_FIRST = 0 }; class CPSCBase; }
class CCPPointerInfo { public: unsigned long RefCount; };
template <class T> class CCountedPointer {
public:
    T* Data; CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
namespace NParticleEngine {
class CComponentInstance {
public:
    virtual ~CComponentInstance();
    CCountedPointer<CPSCBase> Component; bool Enabled; unsigned long InstanceName;
};
CComponentInstance::~CComponentInstance() { }
}
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&);
}
typedef std::pair<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance> P;
static unsigned char g_buf[sizeof(P) * 4];
int main()
{
    if (sizeof(P) != 0x18) { std::printf("00af6980_TEST FAIL size\n"); return 1; }
    CCPPointerInfo info = { 5 };
    unsigned char srcmem[sizeof(P)];
    std::memset(srcmem, 0, sizeof(srcmem));
    P* src = (P*)srcmem;
    src->first = (NParticleEngine::EPSCType)7;
    src->second.Component.Data = (NParticleEngine::CPSCBase*)0x1234;
    src->second.Component.Info = &info;
    src->second.Enabled = true;
    src->second.InstanceName = 0xBEEF;
    std::memset(g_buf, 0xCD, sizeof(g_buf));
    P* first = (P*)g_buf;
    P* end = std::__uninitialized_fill_n<P*, unsigned int, P>(first, 3u, *src, std::__false_type());
    bool ok = end == first + 3 && info.RefCount == 8;
    for (int i = 0; i < 3; ++i)
        ok = ok && first[i].first == 7 && first[i].second.Component.Data == (NParticleEngine::CPSCBase*)0x1234
            && first[i].second.Component.Info == &info && first[i].second.Enabled && first[i].second.InstanceName == 0xBEEF
            && *(unsigned char**)&first[i].second != (unsigned char*)0xCDCDCDCD;
    ok = ok && g_buf[sizeof(P) * 3] == 0xCD;
    P* e0 = std::__uninitialized_fill_n<P*, unsigned int, P>(first + 3, 0u, *src, std::__false_type());
    ok = ok && e0 == first + 3 && info.RefCount == 8 && g_buf[sizeof(P) * 3] == 0xCD;
    if (!ok) { std::printf("00af6980_TEST FAIL\n"); return 1; }
    std::printf("00af6980_TEST PASS\n");
    return 0;
}
