#include <cstdio>
#include <cstring>
#include <new>
// Behaviour model of _STL::pair<unsigned long, C3DMeshVolume>::pair() @ 0x00A327E0: zero key, the volume's name
// default-constructed (CCharString() stand-in), null counted convex volume.
static char gEmptyString[1] = { 0 };
static int gNameCtorCalls = 0;
class CCharString {
public:
    void* PStringData;
    CCharString() : PStringData(gEmptyString) { ++gNameCtorCalls; }   // stand-in for 0x0099E4B0
};
struct CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
};
class CContainmentVolume;
class C3DMeshVolume {
public:
    CCharString                         Name;
    CCountedPointer<CContainmentVolume> ConvexVolume;
};
namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
};
}
int main()
{
    typedef _STL::pair<unsigned long, C3DMeshVolume> Entry;
    union { double align; unsigned char bytes[sizeof(Entry)]; } buf;
    std::memset(buf.bytes, 0xCD, sizeof(buf.bytes));
    Entry* e = new (buf.bytes) Entry;
    bool ok = e->first == 0 && e->second.Name.PStringData == gEmptyString && e->second.ConvexVolume.Data == 0 &&
              e->second.ConvexVolume.Info == 0 && gNameCtorCalls == 1;
    Entry entries[3];
    for (int i = 0; i < 3; ++i)
        if (entries[i].first != 0 || entries[i].second.Name.PStringData != gEmptyString || entries[i].second.ConvexVolume.Data != 0)
            ok = false;
    if (gNameCtorCalls != 4)
        ok = false;
    if (!ok) { std::printf("00a327e0_TEST FAIL\n"); return 1; }
    std::printf("00a327e0_TEST PASS\n");
    return 0;
}