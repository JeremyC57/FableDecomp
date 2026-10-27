// Behaviour test for 0x00A49E00 std::__copy<CLipSyncPhonemeSample*, CLipSyncPhonemeSample*, int>: the source body verbatim (comments and CRT declarations dropped), mocks for its callees.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <new>
#define std tstd
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __forceinline inline
#endif

class CCPPointerInfo { public:
    unsigned long RefCount;                            // +0x00
    void (__fastcall* DeleteFunc)(void* data);         // +0x04
    void*         Data;                                // +0x08
};

template <class T> class CCountedPointer {
public:
    T*              Data;                              // +0x00
    CCPPointerInfo* Info;                              // +0x04
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            if (Info && --Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};

class CCharString {
public:
    void* m_pData;                                     // +0x00
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
};

class CSpeechFrame;

class CLipSyncPhonemeSample {                          // Ego_r.pdb, 16 bytes
public:
    CCharString                   Symbol;              // +0x00
    unsigned long                 ID;                  // +0x04
    CCountedPointer<CSpeechFrame> SampleFrame;         // +0x08
};

namespace std {
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                   const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template CLipSyncPhonemeSample* __copy(CLipSyncPhonemeSample*, CLipSyncPhonemeSample*, CLipSyncPhonemeSample*,
                                       const random_access_iterator_tag&, int*);
}

static int g_assigns = 0, g_deleted = 0;
CCharString& CCharString::operator=(const CCharString& other) { m_pData = other.m_pData; ++g_assigns; return *this; }
static void __fastcall DeleteFrame(void* data) { ++g_deleted; (void)data; }
int main()
{
    CCPPointerInfo* a = new CCPPointerInfo; a->RefCount = 1; a->DeleteFunc = DeleteFrame; a->Data = 0;   // owned by dst[0] only
    CCPPointerInfo* b = new CCPPointerInfo; b->RefCount = 2; b->DeleteFunc = DeleteFrame; b->Data = 0;   // src[0] and dst[1]
    static char t0[] = "s0", t1[] = "s1", t2[] = "d";
    CLipSyncPhonemeSample src[2], dst[3];
    memset(src, 0, sizeof(src)); memset(dst, 0, sizeof(dst));
    src[0].Symbol.m_pData = t0; src[0].ID = 10; src[0].SampleFrame.Info = b;
    src[1].Symbol.m_pData = t1; src[1].ID = 11; src[1].SampleFrame.Info = 0;
    dst[0].Symbol.m_pData = t2; dst[0].SampleFrame.Info = a;
    dst[1].SampleFrame.Info = b; dst[2].ID = 77;
    tstd::random_access_iterator_tag tag;
    CLipSyncPhonemeSample* end = tstd::__copy(src, src + 2, dst, tag, (int*)0);
    int ok = end == dst + 2;
    ok &= dst[0].ID == 10 && dst[0].Symbol.m_pData == t0 && dst[0].SampleFrame.Info == b;
    ok &= dst[1].ID == 11 && dst[1].Symbol.m_pData == t1 && dst[1].SampleFrame.Info == 0;
    ok &= dst[2].ID == 77 && g_assigns == 2 && g_deleted == 1 && b->RefCount == 2;   // a released and freed; b: +1 -1
    end = tstd::__copy(src, src, dst, tag, (int*)0);
    ok &= end == dst && g_assigns == 2;
    if (ok) printf("00a49e00_TEST PASS\n");
    return ok ? 0 : 1;
}