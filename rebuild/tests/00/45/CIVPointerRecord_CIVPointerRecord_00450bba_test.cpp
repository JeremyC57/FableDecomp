// behaviour test for 0x00450BBA
#include <stdio.h>
static int g_deleted = 0;
class CIVCountedPointeeBase { public:
    virtual ~CIVCountedPointeeBase() {}
    virtual void DeleteThis() { ++g_deleted; }
    unsigned long IVRefCount;
    __forceinline void Release() { if (--IVRefCount == 0) DeleteThis(); }
};
template <class T> class CIVCountedPointer { public:
    T* Object;
    __forceinline ~CIVCountedPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
};
struct CIVPointerRecord {
    long Data[3];
    CIVCountedPointer<CIVCountedPointeeBase> Pointer;
    ~CIVPointerRecord();
};
CIVPointerRecord::~CIVPointerRecord() {}

int main()
{
    CIVCountedPointeeBase obj; obj.IVRefCount = 2;
    int ok = 1;
    { CIVPointerRecord* r = new CIVPointerRecord; r->Pointer.Object = &obj; r->~CIVPointerRecord(); ok &= r->Pointer.Object == 0; ::operator delete(r); }
    ok &= obj.IVRefCount == 1 && g_deleted == 0;
    { CIVPointerRecord* r = new CIVPointerRecord; r->Pointer.Object = &obj; r->~CIVPointerRecord(); ::operator delete(r); }
    ok &= obj.IVRefCount == 0 && g_deleted == 1;
    { CIVPointerRecord* r = new CIVPointerRecord; r->Pointer.Object = 0; r->~CIVPointerRecord(); ok &= r->Pointer.Object == 0; ::operator delete(r); }
    ok &= g_deleted == 1;
    printf(ok ? "00450bba_TEST PASS\n" : "00450bba_TEST FAIL\n");
    return ok ? 0 : 1;
}