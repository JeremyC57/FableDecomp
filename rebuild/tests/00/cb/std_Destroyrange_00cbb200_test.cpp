// behaviour test for 0x00cbb200 (_Destroy_range over SNamedCountedPair records)
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
static int g_destroyed = 0, g_stringsFreed = 0, g_basesDestroyed = 0;
static void __fastcall OnDestroy(void*) { ++g_destroyed; }
struct SCountBlock {
    long m_references;                          // +0x00
    void (__fastcall* m_pfnDestroy)(void*);     // +0x04
    void* m_pObject;                            // +0x08
};

template <class T>
struct CCountedPointer {
    T* Data;                                    // +0x00
    SCountBlock* Info;                          // +0x04
    ~CCountedPointer()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

struct CCharString {
    void* m_pStringData;                        // +0x00
    ~CCharString() { if (m_pStringData) ++g_stringsFreed; m_pStringData = 0; }
};

class CCountedObject;
struct SNamedCountedPair {                      // std::pair<CCharString, CCountedPointer<..> >, 12 bytes
    CCharString                     first;      // +0x00
    CCountedPointer<CCountedObject> second;     // +0x04
};

namespace std {
template <class T> struct allocator { allocator() {} };
template <class T>
void __fastcall _Destroy_range(T* first, T* last, const allocator<T>&)
{
    for (; first != last; ++first)
        first->~T();
}
}

int main()
{
    static char dummy;
    // raw storage: the elements are built field by field and destroyed only through _Destroy_range
    static double storage[64];
    SNamedCountedPair* e = (SNamedCountedPair*)storage;
    for (int i = 0; i < 3; ++i) {
        e[i].first.m_pStringData = &dummy;
        e[i].second.Data = 0;
        e[i].second.Info = new SCountBlock; e[i].second.Info->m_references = (i == 1) ? 2 : 1;
        e[i].second.Info->m_pfnDestroy = &OnDestroy; e[i].second.Info->m_pObject = &e[i];
    }
    SCountBlock* kept = e[1].second.Info;
    std::allocator<SNamedCountedPair> al;
    std::_Destroy_range(e, e, al);                // empty range: nothing happens
    if (g_destroyed != 0 || g_stringsFreed != 0) { printf("00cbb200_TEST FAIL\n"); return 1; }
    std::_Destroy_range(e, e + 3, al);
    int ok = g_destroyed == 2 && g_stringsFreed == 3 && e[0].second.Info == 0 && e[1].second.Info == 0 && e[2].second.Info == 0 && e[0].second.Data == 0 && kept->m_references == 1;
    delete kept;
    if (!ok) { printf("00cbb200_TEST FAIL\n"); return 1; }
    printf("00cbb200_TEST PASS\n");
    return 0;
}