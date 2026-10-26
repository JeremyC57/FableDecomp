// Behaviour test for 00a331b0: std::swap(std::pair<long,C3DMeshVolume>&, std::pair<long,C3DMeshVolume>&) -- the two
// entries trade keys, names and counted volumes; the temporary's add-ref and release leave every reference count as it
// was, nothing is deleted, and every string constructed is destroyed.
#include <cstdio>
#include <cstring>

static int g_live = 0;
static int g_deleted = 0;
class CCharString {
public:
    const char* PStringData;
    CCharString(const char* s) : PStringData(s) { ++g_live; }
    CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_live; }
    ~CCharString() { --g_live; }
    CCharString& operator=(const CCharString& other) { PStringData = other.PStringData; return *this; }
};
class CCPPointerInfo { public:
    unsigned long RefCount;
    void (*DeleteFunc)(void* data);
    void* Data;
};
static void count_delete(void*) { ++g_deleted; }
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(T* data, CCPPointerInfo* info) : Data(data), Info(info) { if (Info) ++Info->RefCount; }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    void ReleaseInfo()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                Info->Data = 0;                 // retail: delete Info (operator delete @ 0x00BFE9BC)
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        if (other.Info)
            ++other.Info->RefCount;
        ReleaseInfo();
        Data = other.Data;
        Info = other.Info;
        return *this;
    }
    ~CCountedPointer() { ReleaseInfo(); Data = 0; Info = 0; }
};
class CContainmentVolume { public: int id; };
class C3DMeshVolume { public:
    CCharString                         Name;
    CCountedPointer<CContainmentVolume> ConvexVolume;
    C3DMeshVolume(const char* name, CContainmentVolume* v, CCPPointerInfo* info) : Name(name), ConvexVolume(v, info) { }
    C3DMeshVolume& operator=(const C3DMeshVolume& other)   // retail: out of line @ 0x00A32960
    {
        Name = other.Name;
        ConvexVolume = other.ConvexVolume;
        return *this;
    }
};
template <class T1, class T2> struct SPair {
    T1 first;
    T2 second;
    SPair(const T1& a, const T2& b) : first(a), second(b) { }
    SPair(const SPair& other) : first(other.first), second(other.second) { }
};
template <class T> inline void swap_model(T& a, T& b)
{
    T tmp = a;
    a = b;
    b = tmp;
}

int main()
{
    {
        CContainmentVolume va = { 1 }, vb = { 2 };
        CCPPointerInfo ia = { 0, count_delete, &va };
        CCPPointerInfo ib = { 0, count_delete, &vb };
        SPair<long, C3DMeshVolume> a(11L, C3DMeshVolume("doorA", &va, &ia));
        SPair<long, C3DMeshVolume> b(-22L, C3DMeshVolume("doorB", &vb, &ib));
        unsigned long refA = ia.RefCount, refB = ib.RefCount;
        swap_model(a, b);
        if (!(a.first == -22) || !(b.first == 11) || std::strcmp(a.second.Name.PStringData, "doorB") != 0 ||
            std::strcmp(b.second.Name.PStringData, "doorA") != 0 || a.second.ConvexVolume.Data != &vb ||
            b.second.ConvexVolume.Info != &ia || ia.RefCount != refA || ib.RefCount != refB || g_deleted != 0) {
            std::printf("00a331b0_TEST FAIL\n");
            return 1;
        }
    }
    if (g_live != 0 || g_deleted != 2) { std::printf("00a331b0_TEST FAIL\n"); return 1; }
    std::printf("00a331b0_TEST PASS\n");
    return 0;
}