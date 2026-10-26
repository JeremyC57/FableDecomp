#include <cstdio>
#include <new>

static int g_stringCopies = 0;
class CCharString { public:
    const char* m_pData;
    explicit CCharString(const char* text) : m_pData(text) { }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_stringCopies; }   // models 0x0099EC30
};
class CCPPointerInfo { public:
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

class CCompositeSurfaceInfo { public:   // vtable 0x012A8154
    virtual int Kind() const { return 0x8154; }
    CCountedPointer Cache;
    CCompositeSurfaceInfo(void* d, CCPPointerInfo* i) : Cache(d, i) { }
};
struct T {   // SPairLongCCompositeSurfaceInfo {unsigned long, CCompositeSurfaceInfo}
    unsigned long first;
    CCompositeSurfaceInfo second;
    T(unsigned long k, void* d, CCPPointerInfo* i) : first(k), second(d, i) { }
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
    std::printf("00bedbb0_TEST FAIL\n");
    return 1;
}

int main()
{
    CCPPointerInfo info = { 1 };
    int payload = 42;
    T* buf = (T*)g_storage;
    T value(5, &payload, &info);
    T* end = UninitFillN(buf, 3, value, CRetailAllocator());
    if (end != buf + 3)
        return Fail();
    for (int i = 0; i < 3; ++i) {
        if (buf[i].first != 5 || buf[i].second.Cache.Data != &payload || buf[i].second.Cache.Info != &info ||
            buf[i].second.Kind() != 0x8154)
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
        T nullValue(6, &payload, 0);
        T* e2 = UninitFillN(buf + 8, 2, nullValue, CRetailAllocator());
        if (e2 != buf + 10 || buf[8].second.Cache.Info != 0 || buf[9].second.Cache.Data != &payload || info.RefCount != 4)
            return Fail();
    }
    std::printf("00bedbb0_TEST PASS\n");
    return 0;
}