// behaviour test for 0x00AF2C00 std::__uninitialized_fill_n<pair<CCharString,CRegisteredPSCInfo>*,unsigned int,...>,
// linked against the built function: n copies constructed (both strings copied through the copy constructor, the
// allocator function and component type copied), first + n returned, n == 0 constructs nothing
#include <cstdio>
#include <cstring>
#include <new>
static int g_stringCopies = 0;
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);
};
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_stringCopies; }
namespace NParticleEngine {
enum EPSCType { PSC_TYPE_FIRST = 0 };
class CPSCBase;
class CRegisteredPSCInfo {
public:
    CPSCBase* (__fastcall* AllocFunc)();
    EPSCType    ComponentType;
    CCharString GUIName;
    CRegisteredPSCInfo(const CRegisteredPSCInfo& o) : AllocFunc(o.AllocFunc), ComponentType(o.ComponentType), GUIName(o.GUIName) { }
};
}
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> struct pair {
    _T1 first; _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&);
}
static NParticleEngine::CPSCBase* __fastcall MakeNothing() { return 0; }
typedef std::pair<CCharString, NParticleEngine::CRegisteredPSCInfo> P;
int main()
{
    if (sizeof(P) != 0x10) { std::printf("00af2c00_TEST FAIL size\n"); return 1; }
    unsigned char xmem[sizeof(P)];
    P* x = (P*)xmem;
    x->first.PStringData = (CCharStringData*)"name";
    x->second.AllocFunc = &MakeNothing;
    x->second.ComponentType = (NParticleEngine::EPSCType)9;
    x->second.GUIName.PStringData = (CCharStringData*)"gui";
    unsigned char buffer[sizeof(P) * 6];
    std::memset(buffer, 0xCD, sizeof(buffer));
    P* first = (P*)buffer;
    P* end = std::__uninitialized_fill_n<P*, unsigned int, P>(first, 5u, *x, std::__false_type());
    bool ok = end == first + 5 && g_stringCopies == 10;
    for (int i = 0; i < 5; ++i)
        ok = ok && first[i].first.PStringData == x->first.PStringData && first[i].second.AllocFunc == &MakeNothing
            && first[i].second.ComponentType == 9 && first[i].second.GUIName.PStringData == x->second.GUIName.PStringData;
    ok = ok && buffer[sizeof(P) * 5] == 0xCD;
    P* none = std::__uninitialized_fill_n<P*, unsigned int, P>(first, 0u, *x, std::__false_type());
    ok = ok && none == first && g_stringCopies == 10;
    if (!ok) { std::printf("00af2c00_TEST FAIL\n"); return 1; }
    std::printf("00af2c00_TEST PASS\n");
    return 0;
}
