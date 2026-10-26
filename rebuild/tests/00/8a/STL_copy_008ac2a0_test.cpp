#include <cstdio>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif
// Test model of CScriptThing (polymorphic root + CCountedPointer PImp) with the counted pointer's copy/assignment.
class CCPPointerInfo { public:
    unsigned int RefCount;
    void (__fastcall* DeleteFunc)(void*);
    void* Data;
};
static int g_deleted = 0;
static void __fastcall CountDelete(void*) { ++g_deleted; }
template <class T>
class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { ShareData(0, 0); }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        ShareData(other.Data, other.Info);
        return *this;
    }
    void ShareData(T* data, CCPPointerInfo* info)
    {
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
            if (info)
                ++info->RefCount;
        }
    }
};
class CBaseClass {
public:
    virtual ~CBaseClass() { }
};
class CScriptThing : public CBaseClass {
public:
    CCountedPointer<CScriptThing> PImp;
    CScriptThing() { }
    CScriptThing(const CScriptThing& other) : CBaseClass(), PImp(other.PImp) { }
    CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
    virtual ~CScriptThing() { }
};
static CCPPointerInfo* NewInfo(unsigned int refs)
{
    CCPPointerInfo* info = new CCPPointerInfo;
    info->RefCount = refs;
    info->DeleteFunc = CountDelete;
    info->Data = 0;
    return info;
}
// std::__copy<CScriptThing*, CScriptThing*, int> @ 0x008AC2A0: counted forward assignment loop.
struct random_access_iterator_tag { };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall Copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                            const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = (_Distance)(__last - __first); __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
int main()
{
    bool ok = true;
    CCPPointerInfo* a = NewInfo(1);
    CCPPointerInfo* b = NewInfo(1);
    CCPPointerInfo* old = NewInfo(1);
    CScriptThing src[2];
    CScriptThing dst[3];
    src[0].PImp.Info = a;
    src[1].PImp.Info = b;
    dst[0].PImp.Info = old;                             // its only reference: freed by the assignment
    dst[1].PImp.Info = b;
    ++b->RefCount;                                      // dst[1] already shares b: no change
    CScriptThing* end = Copy(src, src + 2, dst, random_access_iterator_tag(), (int*)0);
    if (end != dst + 2) ok = false;
    if (dst[0].PImp.Info != a || dst[1].PImp.Info != b || dst[2].PImp.Info != 0) ok = false;
    if (a->RefCount != 2 || b->RefCount != 2 || g_deleted != 1) ok = false;
    if (Copy(src, src, dst, random_access_iterator_tag(), (int*)0) != dst) ok = false;
    if (!ok) { std::printf("008ac2a0_TEST FAIL\n"); return 1; }
    std::printf("008ac2a0_TEST PASS\n");
    return 0;
}