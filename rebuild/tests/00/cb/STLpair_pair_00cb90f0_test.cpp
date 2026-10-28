// Behaviour test for 0x00CB90F0 pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo> copy constructor.
#include <cstdio>
#include <cstring>

class CCharString {
public:
    const char* PData;
    static int s_copies;
    static int s_assigns;
    CCharString(const char* s) : PData(s) {}
    CCharString(const CCharString& o) : PData(o.PData) { ++s_copies; }
    const CCharString& operator=(const CCharString& o) { PData = o.PData; ++s_assigns; return *this; }
};
int CCharString::s_copies = 0;
int CCharString::s_assigns = 0;
static bool Same(const CCharString& s, const char* t) { return std::strcmp(s.PData, t) == 0; }
class CPersistInfo {
public:
    unsigned long UIDLow;                 // the 64-bit UID as two words (plain C++98)
    unsigned long UIDHigh;
    CCharString   PersistScript;
    CPersistInfo(unsigned long lo, unsigned long hi, const char* s) : UIDLow(lo), UIDHigh(hi), PersistScript(s) {}
};
struct CUID { unsigned long Low, High; };
namespace stlp {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& a, const _T2& b) : first(a), second(b) {}
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& p) : first(p.first), second(p.second) {}
    pair(const pair& p) : first(p.first), second(p.second) {}
};
}

int main()
{
    CUID uid; uid.Low = 0x89ABCDEFUL; uid.High = 0x01234567UL;
    stlp::pair<CUID, CPersistInfo> a(uid, CPersistInfo(uid.Low, uid.High, "PersistGuard"));
    CCharString::s_copies = 0;
    stlp::pair<CUID, CPersistInfo> b(a);
    if (!(CCharString::s_copies == 1 && b.first.Low == 0x89ABCDEFUL && b.first.High == 0x01234567UL && b.second.UIDLow == 0x89ABCDEFUL && b.second.UIDHigh == 0x01234567UL && Same(b.second.PersistScript, "PersistGuard"))) {
        std::printf("00cb90f0_TEST FAIL\n");
        return 1;
    }
    std::printf("00cb90f0_TEST PASS\n");
    return 0;
}