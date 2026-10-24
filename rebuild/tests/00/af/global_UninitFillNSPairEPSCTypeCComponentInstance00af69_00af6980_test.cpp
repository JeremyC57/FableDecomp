#include <cstdio>
#include <new>

static int g_stringCopies = 0;
struct CCharString {
    const char* m_pData;
    explicit CCharString(const char* text) : m_pData(text) { }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_stringCopies; }   // models 0x0099EC30
};
struct CCPPointerInfo {
    unsigned long RefCount;
};
struct CCountedPointer {
    void* Data;
    CCPPointerInfo* Info;
    CCountedPointer(void* data, CCPPointerInfo* info) : Data(data), Info(info) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};
struct CRetailAllocator { };
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif

namespace NParticleEngine {
enum EPSCType { PSC_TYPE_FIRST = 0, PSC_TYPE_SECOND = 1, PSC_TYPE_THIRD = 2 };
struct CComponentInstance {   // vtable 0x0129F478
    virtual int Kind() const { return 0xF478; }
    CCountedPointer Component;
    bool Enabled;
    unsigned long InstanceName;
    CComponentInstance(void* d, CCPPointerInfo* i, bool e, unsigned long n) : Component(d, i), Enabled(e), InstanceName(n) { }
};
}
struct T {   // SPairEPSCTypeCComponentInstance {EPSCType, CComponentInstance}
    NParticleEngine::EPSCType first;
    NParticleEngine::CComponentInstance second;
    T(NParticleEngine::EPSCType t, void* d, CCPPointerInfo* i, bool e, unsigned long n) : first(t), second(d, i, e, n) { }
};

static T* UninitFillN(T* first, unsigned long count, const T& value, CRetailAllocator)
{
    for (; 0 < count; --count, ++first)
        new (first) T(value);
    return first;
}

static double g_storage[256];

static int Fail()
{
    std::printf("00af6980_TEST FAIL\n");
    return 1;
}

int main()
{
    CCPPointerInfo info = { 1 };
    int payload = 42;
    T* buf = (T*)g_storage;
    T value(NParticleEngine::PSC_TYPE_THIRD, &payload, &info, true, 0xBEEF);
    T* end = UninitFillN(buf, 3, value, CRetailAllocator());
    if (end != buf + 3)
        return Fail();
    for (int i = 0; i < 3; ++i) {
        if (buf[i].first != NParticleEngine::PSC_TYPE_THIRD || buf[i].second.Component.Data != &payload ||
            buf[i].second.Component.Info != &info || !buf[i].second.Enabled || buf[i].second.InstanceName != 0xBEEF ||
            buf[i].second.Kind() != 0xF478)
            return Fail();
    }
    if (info.RefCount != 4)
        return Fail();
    T* same = UninitFillN(buf + 3, 0, value, CRetailAllocator());
    if (same != buf + 3)
        return Fail();
    if (info.RefCount != 4)
        return Fail();
    {
        // a null counted pointer copies without touching any count
        T nullValue(NParticleEngine::PSC_TYPE_FIRST, &payload, 0, false, 1);
        T* e2 = UninitFillN(buf + 8, 2, nullValue, CRetailAllocator());
        if (e2 != buf + 10 || buf[8].second.Component.Info != 0 || buf[9].second.Component.Data != &payload || info.RefCount != 4)
            return Fail();
    }
    std::printf("00af6980_TEST PASS\n");
    return 0;
}