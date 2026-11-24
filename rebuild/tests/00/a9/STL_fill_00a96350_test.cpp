// NF382 name fix: the row is fill<CCountedPointer<C3DClothPrimitive>*> (was the C3DGroup2 instance).
#include <cstdio>
#include <new>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
typedef unsigned long long u64;
#else
typedef unsigned __int64 u64;
#endif

static int g_fail = 0;
static void Check(bool ok) { if (!ok) g_fail = 1; }

static int g_deleted = 0;
class CCPPointerInfo { public:                                 // the shared count block
    long  RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    explicit CCountedPointer(T* data) : Data(0), Info(0) { Reset(data); }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { Release(); }
    static void __fastcall DeleteData(void* data) { ++g_deleted; delete (T*)data; }
    void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    void Reset(T* data)
    {
        Release();
        if (data) {
            Info = new CCPPointerInfo;
            Info->RefCount = 1;
            Info->DeleteFunc = &CCountedPointer<T>::DeleteData;
            Info->Data = data;
            Data = data;
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (Info)
                ++Info->RefCount;
        }
        return *this;
    }
    long Count() const { return Info ? Info->RefCount : 0; }
    T* operator->() const { return Data; }
};

class C3DClothPrimitive { public: int id; };
typedef CCountedPointer<C3DClothPrimitive> TPrimitivePtr;

// the row: std::fill over CCountedPointer<C3DClothPrimitive> elements (assignment releases each old group)
static void __fastcall FillGroups(TPrimitivePtr* first, TPrimitivePtr* last, const TPrimitivePtr& val)
{
    for (; first != last; ++first)
        *first = val;
}

int main()
{
    {
        TPrimitivePtr a[4];
        for (int k = 0; k < 4; ++k)
            a[k].Reset(new C3DClothPrimitive);
        TPrimitivePtr shared(a[2]);
        TPrimitivePtr val(new C3DClothPrimitive);
        FillGroups(a, a + 4, val);
        Check(val.Count() == 5);
        Check(g_deleted == 3);
        Check(shared.Count() == 1);
        for (int j = 0; j < 4; ++j)
            Check(a[j].Data == val.Data && a[j].Info == val.Info);
        FillGroups(a + 1, a + 3, val);
        Check(val.Count() == 5 && g_deleted == 3);
        FillGroups(a, a, shared);
        Check(shared.Count() == 1);
        TPrimitivePtr none;
        FillGroups(a, a + 4, none);
        Check(val.Count() == 1 && a[0].Data == 0 && a[3].Info == 0);
    }
    Check(g_deleted == 5);

    if (g_fail) {
        std::printf("00a96350_TEST FAIL\n");
        return 1;
    }
    std::printf("00a96350_TEST PASS\n");
    return 0;
}